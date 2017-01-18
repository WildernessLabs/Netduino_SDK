////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using Microsoft.SPOT;
using Microsoft.SPOT.IO;
using System.Net;
using System.IO;
using System.Text;
using Microsoft.SPOT.Net.Security;
using System.Threading;
using System.Collections;
using System.Security.Cryptography.X509Certificates;
using Microsoft.SPOT.MFUpdate;
using Microsoft.SPOT.Hardware;
using System.Net.Sockets;

namespace HttpServerSample
{

    public static class MyHttpServer
    {
        const int BUFFER_SIZE = 1024;

        public static void Main()
        {
            // make sure the date is set on the device
            if (Microsoft.SPOT.Hardware.SystemInfo.SystemID.SKU != 3)
            {
                Microsoft.SPOT.Hardware.Utility.SetLocalTime(new DateTime(2012, 8, 24));
            }

            // Wait for DHCP (on LWIP devices)
            while (true)
            {
                IPAddress ip = IPAddress.GetDefaultLocalAddress();

                if (ip != IPAddress.Any) break;

                Thread.Sleep(1000);
            }


            // Starts http server in another thread.
            Thread httpThread = new Thread((new PrefixKeeper("http")).RunServerDelegate);
            httpThread.Start();
            
            /// Starts https server. The server uses certificate with host name Device.Microsoft.Com
            /// In order to connect from browser to https server running on device you need to do following steps:
            /// 1. On your PC install certificate for server Device.Microsoft.Com. Use Windows Explorer to view and install
            ///    certificate from file "Device.Microsoft.Com.cer" from folder "Certificates".
            ///    This certificate should be installed to "Trusted Root Certification Authorities".
            ///    If you view this certificates using Explorer, in Certification Path tab the status should be "OK"
            /// 2. Edit hosts file on your PC to map from host name Device.Microsoft.Com to IP address of your device.
            ///    On WIndows Vista this file is located in folder "C:\Windows\System32\drivers\etc" folder. 
            ///    THis is text file and it keeps entries for DNS lookup.
            /// 3. Validate that DMS properly setup using "ping device.dicrosoft.com" and check that device respond to ping request.
            /// 4. Start IE. Validate that proxy is not used and "Automatically detect settings" in Lan Connections is not checked.
            /// 5. Open site http://Device.Microsoft.Com.
            /// 6  Open site https://Device.Microsoft.Com
            ///
            /// Troubleshooting:
            ///    A. If page https://Device.Microsoft.Com does not load, try http://Device.Microsoft.Com 
            ///    Since device runs both http and https server you should see the same page.
            ///    B. If page http://Device.Microsoft.Com does not load - means either mapping of host name Device.Microsoft.Com does not work or server does not run.
            ///       First try to ping you device on IP address and then on Device.Microsoft.Com.
            ///       Try to type http:// followed by your device IP address. If this works, then check the mapping from Device.Microsoft.Com to your device IP address.
            ///    C. If http://Device.Microsoft.Com loads, but https://Device.Microsoft.Com does not load - means certificate "Device.Microsoft.Com.cer" was not 
            ///       into trusted "Trusted Root Certification Authorities". Check the status of certificate.
            
            try
            {
                RunServer("https");
            }
            catch (Exception e)
            {
                Debug.Print(e.Message);
            }
        }

        /// <summary>
        /// All this class does is keeps the prefix and provides 
        /// RunServerDelegate to run in separate thread.
        /// RunServerDelegate calls HttpServerApp.RunServer with saved prefix.
        /// </summary>
        class PrefixKeeper
        {
            /// <summary>
            /// Keeps the prefix to start server.
            /// </summary>
            private string m_prefix;

            /// <summary>
            /// Saves the prefix
            /// </summary>
            /// <param name="prefix">Prefix</param>
            internal PrefixKeeper(string prefix)
            {
                m_prefix = prefix;
            }
            
            /// <summary>
            /// Delegate to run server in separate thread.
            /// </summary>
            internal void RunServerDelegate()
            {
                MyHttpServer.RunServer(m_prefix);
            }
        }

        private static void HandleRequestThread()
        {
            HttpListenerContext context = null;

            try
            {
                lock (m_responseQueue)
                {
                    context = (HttpListenerContext)m_responseQueue.Dequeue();
                }

                if (context != null)
                {
                    HttpListenerRequest request = context.Request;
                    switch (request.HttpMethod.ToUpper())
                    {
                        case "GET": ProcessClientGetRequest(context); break;
                        case "POST": ProcessClientPostRequest(context); break;
                    }
                }
            }
            catch (SocketException)
            {
            }
            finally
            {
                if (context != null)
                {
                    context.Close();
                }
            }
        }

        static Queue m_responseQueue = new Queue();

        internal static void RunServer(string prefix)
        {
            HttpListener listener = new HttpListener(prefix, -1);
            // Loads certificate if it is https server.
            if (prefix == "https")
            {
                // 
                // SKU == 3 indicates the device is the emulator.
                //
                // The emulator and the device use different certificate types.  The emulator requires the use of a .PFX certficate, whereas the device simply
                // requires a CER certificate with appended private key.  In addtion, the .PFX certificate requires a password ("NetMF").
                //
                if (Microsoft.SPOT.Hardware.SystemInfo.SystemID.SKU == 3)
                {
                    byte[] serverCertAsArray = HttpServerSample.Resource1.GetBytes(HttpServerSample.Resource1.BinaryResources.NetMFDeviceCert);
                    listener.HttpsCert = new X509Certificate(serverCertAsArray, "NetMF");
                }
                else
                {
                    string serverCertAsString = HttpServerSample.Resource1.GetString(HttpServerSample.Resource1.StringResources.cert_device_microsoft_com);
                    byte[] serverCertAsArray = Encoding.UTF8.GetBytes(serverCertAsString);
                    listener.HttpsCert = new X509Certificate(serverCertAsArray, "NetMF");
                }
            }
            
            while (true)
            {
                try
                {
                    if (!listener.IsListening)
                    {
                        listener.Start();
                    }
                    
                    HttpListenerContext context = listener.GetContext();
                    lock (m_responseQueue)
                    {
                        m_responseQueue.Enqueue(context);
                    }

                    Thread th = new Thread(new ThreadStart(HandleRequestThread));
                    th.Start();
                }
                catch(InvalidOperationException)
                {
                    listener.Stop();
                    Thread.Sleep(200);
                }
                catch(ObjectDisposedException)
                {
                    listener.Start();
                }
                catch
                {
                    Thread.Sleep(200);
                }
            }
        }

        private static void ProcessClientPostRequest(HttpListenerContext context)
        {
            // Retrieves request and response.
            HttpListenerRequest request = context.Request;
            HttpListenerResponse response = context.Response;
            
            // Allocates buffer for reading of message body
            byte[] postdata = new byte[BUFFER_SIZE];

            // Now reads the posted data. The content length should be supplied. 
            // It is error not to have content length with post request.
            if (request.ContentLength64 > 0)
            {
                Debug.Print("Request Headers:");
                Debug.Print(request.Headers.ToString());

                bool fHasVolume = HasFileSystem();

                long totalBytesReceived = 0;
                long contLen = request.ContentLength64;
                // index of the end of the binary file (start of the end boundary)
                long fileEnd = contLen;

                int bytesToRead = contLen < BUFFER_SIZE ? (int)contLen : BUFFER_SIZE; // (int)(contLen - totalBytesReceived);

                // read file header 
                int headerRead = 0;
                do
                {
                    headerRead += request.InputStream.Read(postdata, headerRead, bytesToRead-headerRead);
                }
                while(headerRead < bytesToRead);
                
                MFFirmwareUpdate fwUpdate = null;
                int updatePktIdx = 0;
                FileStream fs = null;
                bool fAbort = false;
                bool fEndFile = false;
                string strUpdate = "";
                uint updateCRC = 0;

                if (headerRead != 0)
                {
                    string fileName, ext;
                    int fileStartOffset;

                    string boundary = GetFileBoundaryEnd(request);

                    if (ParseFileName(postdata, out fileName, out ext, out fileStartOffset) && ext.ToLower() == ".nmf")
                    {
                        try
                        {
                            fileEnd = contLen - UTF8Encoding.UTF8.GetBytes(boundary).Length;

                            byte []crcBuf = UTF8Encoding.UTF8.GetBytes(fileName);

                            // The .nmf extension indicates .Net Micro Framework firmware update files (compressed binary files with CompressedImage_Header).
                            fwUpdate = new MFFirmwareUpdate("HTTPSUpdate", Utility.ComputeCRC(crcBuf, 0, crcBuf.Length, (uint)(fileEnd - fileStartOffset)), new Version(4, 2), MFUpdateSubType.FirmwareUpdate_CLR, (int)(fileEnd - fileStartOffset), BUFFER_SIZE);

                            if (fwUpdate != null)
                            {
                                if (!fwUpdate.Open(null)) fwUpdate = null;
                            }

                            // do not store update on filesystem
                            fHasVolume = false;
                        }
                        catch
                        {
                            strUpdate = "Firmware update is not supported on this platform";
                        }
                    }
                    else
                    {
                        fileName = "Upload.txt";
                    }

                    totalBytesReceived += fileStartOffset; // the rest of initial read bytes will be added later

                    if (fHasVolume)
                    {
                        fs = CreateNewFile(fileName);

                        if (fs == null) fHasVolume = false;
                    }

                    int offset = fileStartOffset;

                    // copy the file portion of the bytes to a new packet array
                    if (offset > 0)
                    {
                        offset = headerRead - fileStartOffset;
                        Array.Copy(postdata, fileStartOffset, postdata, 0, offset);
                    }

                    while (totalBytesReceived < contLen)
                    {
                        // Limit to buffer size
                        if (BUFFER_SIZE > (contLen - totalBytesReceived))
                        {
                            bytesToRead = (int)(contLen - totalBytesReceived);
                        }

                        // read 1K chunks at a time so that we have a consistent package size for MFUpdates
                        while (offset < bytesToRead)
                        {
                            int dataRead = request.InputStream.Read(postdata, offset, bytesToRead-offset);

                            if (dataRead == 0)
                            {
                                // Definitely some error. Means file incomplete.
                                fAbort = true;
                                break;
                            }

                            offset += dataRead;
                        }

                        if (fAbort) break;

                        totalBytesReceived += offset;

                        // if we have reached the end of the file, then we need to trucate the 
                        // end boundary so that only the file portion is saved
                        if (totalBytesReceived > fileEnd)
                        {
                            // We only have end boundary bytes left
                            if ((offset - (totalBytesReceived - fileEnd)) <= 0)
                            {
                                fEndFile = true;
                            }
                            else
                            {   // Create a new smaller packet for the remaining file bytes.
                                int len = offset - (int)(totalBytesReceived - fileEnd);
                                byte[] tmp = new byte[len];
                                Array.Copy(postdata, 0, tmp, 0, tmp.Length);
                                postdata = tmp;
                            }
                        }

                        if (!fEndFile)
                        {
                            // Add MFUpdate packet
                            if (fwUpdate != null)
                            {
                                byte[] pktData = postdata;
                                if (offset < postdata.Length)
                                {
                                    byte[] tmp = new byte[bytesToRead];
                                    Array.Copy(postdata, tmp, offset);
                                    pktData = tmp;
                                }
                                // compute CRC for packet (to verify correctness)
                                uint value = Utility.ComputeCRC(pktData, 0, pktData.Length, 0);
                                updateCRC = Utility.ComputeCRC(pktData, 0, pktData.Length, updateCRC);
                                byte[] pktValid = new byte[] { (byte)(value >> 0), (byte)(value >> 8), (byte)(value >> 16), (byte)(value >> 24) };

                                if (0 == (updatePktIdx % 10))
                                {
                                    Debug.Print("Adding packet: " + updatePktIdx.ToString());
                                }

                                if (!fwUpdate.AddPacket(new MFUpdatePkt(updatePktIdx++, pktData, pktValid)))
                                {
                                    Debug.Print("AddPacket Failed");
                                    break;
                                }
                            }

                            if (fHasVolume)
                            {
                                fs.Write(postdata, 0, offset);
                            }
                        }

                        offset = 0;
                    }
                }

                byte[] updateValidation = new byte[] { (byte)(updateCRC >> 0), (byte)(updateCRC >> 8), (byte)(updateCRC >> 16), (byte)(updateCRC >> 24) };

                if (!fAbort && fwUpdate != null)
                {
                    // For the sample the image validatation is just a CRC of the entire image.
                    // In a real firmware download scenario, this value would likely be 
                    // a cryptographic signature or HMAC.
                    if (fwUpdate.ValidateUpdate(updateValidation))
                    {
                        strUpdate = "Update Succeeded (rebooting...)";
                    }
                    else
                    {
                        strUpdate = "Update FAILED: Validation incorrect!";
                    }
                }

                if (fHasVolume && fs != null)
                {
                    fs.Close();
                }

                // Sends response:
                string strResp = "<HTML><BODY>.Net Micro Framework Example HTTP Server<p>";
                
                // Print requested verb, URL and version.. Adds information from the request.
                strResp += "HTTP Method: " + request.HttpMethod + "<br> Requested URL: \"" + request.RawUrl +
                    "<br> HTTP Version: " + request.ProtocolVersion + "\"<p>";

                strResp += "Amount of data received in message body: " + totalBytesReceived + "<br>";
                if (strUpdate.Length > 0)
                {
                    strResp += strUpdate;
                }
                else
                {
                    strResp += "Data of message body is discarded (if there is no filesystem). Please review HTTP Server sample code to add processing of data";
                }
                strResp  += "</BODY></HTML>";
                response.StatusCode = (int)HttpStatusCode.OK;
                //response.RedirectLocation = "http://localhost/WINFS/DpwsDevice/";
                byte[] messageBody = Encoding.UTF8.GetBytes(strResp);
                response.ContentType = "text/html";
                response.OutputStream.Write(messageBody, 0, messageBody.Length);

                if (!fAbort && fwUpdate != null)
                {
                    // We want to send the response to the browser before installing because the install process requires a reboot
                    // which will exit this application.
                    response.Close();
                    if (!fwUpdate.InstallUpdate(updateValidation)) Debug.Print("Firmware Update Failed!!!!!");

                    // we only get here if the update failed.
                    fwUpdate.DeleteUpdate();
                }
            }
            else // Content length is missing, send error back
            {
                // Sends response:
                string strResp = "<HTML><BODY>Content length is missing in Post request</BODY></HTML>";
                byte[] messageBody = Encoding.UTF8.GetBytes(strResp);
                response.ContentType = "text/html";
                response.OutputStream.Write(messageBody, 0, messageBody.Length);
            }
        }

        /// <summary>
        /// Gets the end boundary for posted file.
        /// </summary>
        /// <param name="request"></param>
        /// <returns></returns>
        private static string GetFileBoundaryEnd(HttpListenerRequest request)
        {
            string boundary = "";
            string ct = request.ContentType;
            int idx = ct.IndexOf("boundary=");

            if (idx >= 0)
            {
                idx += 9;
                int end = ct.IndexOf(';', idx);

                if (end == -1)
                {
                    end = ct.Length;
                }

                if (end > idx)
                {
                    boundary = ct.Substring(idx, end - idx);
                    boundary = "\r\n\r\n--" + boundary + "\r\n" + "Content-Disposition: form-data; name=\"submitBtn\"\r\n\r\n" +
                               "Install Firmware Update\r\n" + boundary + "--\r\n";
                }
            }
            return boundary;
        }

        private static FileStream CreateNewFile(string fileName)
        {
            FileStream fs = null;
            string path = "\\ROOT\\" + fileName;
            try
            {
                if (File.Exists(path))
                {
                    File.Delete(path);
                }

                fs = new FileStream(path, FileMode.Create);
            }
            catch
            {
                path = "\\WINFS\\" + fileName;
                try
                {
                    if (File.Exists(path))
                    {
                        File.Delete(path);
                    }
                    fs = new FileStream(path, FileMode.Create);
                }
                catch
                {
                }
            }

            return fs;
        }

        /// <summary>
        /// Determine if the device has a valid file system.  This will determine if the file transfer
        /// from the desktop browser will be stored or not.
        /// </summary>
        /// <returns></returns>
        private static bool HasFileSystem()
        {
            bool fHasVolume = false;

            VolumeInfo[] vis = VolumeInfo.GetVolumes();

            if (vis.Length > 0)
            {
                for (int i = 0; i < vis.Length; i++)
                {
                    if (vis[i].TotalSize == 0) continue;

                    if (vis[i].Name.ToLower() == "root")
                    {
                        if (!vis[i].IsFormatted)
                        {
                            vis[i].Format(0);
                        }
                        fHasVolume = true;
                        break;
                    }
                    else if (vis[i].Name.ToLower() == "winfs")
                    {
                        if (!vis[i].IsFormatted)
                        {
                            vis[i].Format(0);
                        }
                        fHasVolume = true;
                        break;
                    }
                }
            }
            return fHasVolume;
        }

        /// <summary>
        /// Parses the request data for the file name being transfered and returns the file start offset
        /// </summary>
        /// <param name="postdata">Post request data bytes</param>
        /// <param name="fileName">Out parameter set to the file name of the file being transfered</param>
        /// <param name="ext">File extension of the file being transfered</param>
        /// <param name="fileStartOffset">Offset into postdata parameter where the actual file starts</param>
        /// <returns></returns>
        private static bool ParseFileName(byte[] postdata, out string fileName, out string ext, out int fileStartOffset)
        {
            fileName = null;
            ext = null;
            fileStartOffset = -1;

            try
            {
                string str = new string(UTF8Encoding.UTF8.GetChars(postdata));

                Debug.Print(str);

                fileStartOffset = str.Length;

                while (fileStartOffset < postdata.Length && postdata[fileStartOffset] != 0)
                {
                    fileStartOffset++;
                }

                int idx = str.IndexOf("filename=");

                if (idx >= 0)
                {
                    fileName = str.Substring(idx + 9);

                    fileName = fileName.Trim(' ', '\"');

                    idx = fileName.IndexOf('"');

                    if (idx > 0)
                    {
                        fileName = fileName.Substring(0, idx);

                        idx = fileName.LastIndexOf('\\');

                        if (idx >= 0)
                        {
                            fileName = fileName.Substring(idx + 1);
                        }

                        idx = fileName.LastIndexOf(".");

                        if (idx > 0)
                        {
                            ext = fileName.Substring(idx);
                        }
                    }
                }

                return fileName.Length > 0;
            }
            catch
            {
            }

            return false;
        }

        static private string GetPathFromURL(string URL)
        {
            char[] szFilePath = URL.ToCharArray();
            
            for( int i = 0, length = szFilePath.Length; i <  length; i++ )
            {
                if (szFilePath[i] == '/')
                {
                    szFilePath[i] = '\\'; 
                }
            }
            // Return updated string with \SD in front
            return new string( szFilePath );
        }

        private static Encoding encoder = Encoding.UTF8;
        private static char[] fwdSlashDelim = { '\\' };

        private static void ProcessClientGetRequest( HttpListenerContext context )
        {
            
            HttpListenerRequest request = context.Request;
            HttpListenerResponse response = context.Response;

            string strFilePath = GetPathFromURL(request.RawUrl);
            // There is one particular URL that we process differently
            if (strFilePath == "\\PasswordProtected")
            {
                ProcessPasswordProtectedArea( request, response);
                return;
            }
            
            response.StatusCode = (int)HttpStatusCode.OK;
            // Find if file is present. If file exists - sends its context
            if (File.Exists(strFilePath))
            {
                SendFileOverHTTP(response, strFilePath);
            }
            else // The path is directory or not present.
            {
                // Start HTML document
                string strResp = "<HTML><BODY>.Net Micro Framework Example HTTP Server<p>";

                // Print requested verb, URL and version.. Adds information from the request.
                strResp += "HTTP Method: " + request.HttpMethod + "<br> Requested URL: \"" + request.RawUrl +
                    "<br> HTTP Version: " + request.ProtocolVersion + "\"<p>";

                // Information about the path that we access.
                strResp += "File to access " + strFilePath + "<p>";

                // If directory present - iterate over it and 
                if (Directory.Exists(strFilePath))
                {
                    strResp += FillDirectoryContext(strFilePath, request);
                }
                else // Neither File or directory exists
                {
                    // Prints that directory is not found.
                    strResp += "Directory: \"" + strFilePath + "\" Does not exists";
                }

                // Closes HTML
                strResp += "</BODY></HTML>";

                // Sends it.
                byte[] messageBody = encoder.GetBytes(strResp);
                response.ContentType = "text/html";
                response.OutputStream.Write(messageBody, 0, messageBody.Length);
            }
        }

        private static void ProcessPasswordProtectedArea( HttpListenerRequest request, HttpListenerResponse response)
        {
            string strResp = "<HTML><BODY>.Net MF Example HTTP Server. Secure Area<p>";
            if (request.Credentials != null)
            { // Parse and Decode string.
                Debug.Print("User Name : " + request.Credentials.UserName);
                Debug.Print("Password : "  + request.Credentials.Password);
                if (request.Credentials.UserName != "Igor" || request.Credentials.Password != "MyPassword")
                {
                    strResp += "Authentication failed. <br>You supplied <p>User Name: " + request.Credentials.UserName +
                               "<br>Password: " + request.Credentials.Password;
                    strResp += "<p>Please look at root page for required credentials";
                    response.StatusCode = (int)HttpStatusCode.Unauthorized;
                    response.Headers.Add("WWW-Authenticate: Basic realm=\".Net MF Example of Secure Area\"");
                }
                else
                {
                    response.StatusCode = (int)HttpStatusCode.OK;
                    strResp += "Authentication Succeeded.";
                }
            }
            else
            {
                strResp += "<p> Authentication Required<p> Please look at root page for required credentials";

                response.StatusCode = (int)HttpStatusCode.Unauthorized;
                response.Headers.Add("WWW-Authenticate: Basic realm=\".Net MF Example of Secure Area\"");
            }

            // Closes HTML
            strResp += "</BODY></HTML>";
            byte[] messageBody = encoder.GetBytes(strResp);
            response.ContentType = "text/html";
            response.OutputStream.Write(messageBody, 0, messageBody.Length);
        }

        static string FillDirectoryContext(string strFilePath, HttpListenerRequest request )
        {
            string strRet = "Context of directory: \"" + strFilePath + "\"<p>";

            try
            {
                string[] dirs = Directory.GetDirectories(strFilePath);
                foreach (string strDirPath in dirs)
                {
                    // dirs keep full path. We need only relative path in directory.
                    // So we need to split by '\' and take the last directory,
                    string[] splitDir = strDirPath.Split(fwdSlashDelim);
                    string strDir = splitDir[splitDir.Length - 1];
                    string strUrl = request.RawUrl;
                    if (strUrl[strUrl.Length - 1] != '/')
                    {
                        strUrl += '/';
                    }
                    strRet += "Dir  - " +
                    "<A HREF=\"" + strUrl + strDir + "\">" + strDir + "</A><br>";
                }

                string[] files = Directory.GetFiles(strFilePath);
                foreach (string locFilePath in files)
                {
                    string[] splitFile = locFilePath.Split(fwdSlashDelim);
                    string strFile = splitFile[splitFile.Length - 1];
                    string strUrl = request.RawUrl;
                    if (strUrl[strUrl.Length - 1] != '/')
                    {
                        strUrl += '/';
                    }

                    strRet += "File - " +
                    "<A HREF=\"" + strUrl + strFile + "\">" + strFile + "</A><br>";
                }
            }
            catch
            {
            }
            
            // Following line adds file post capabilities
            strRet += "<p>Below is example of sending file data to HTTP server";
            strRet += HttpServerSample.Resource1.GetString(HttpServerSample.Resource1.StringResources.PostForm);
            strRet += HttpServerSample.Resource1.GetString(HttpServerSample.Resource1.StringResources.MFUpdateForm);
            // Next line shows link protected by password.
            strRet += "<br>Below is example of protected link.";
            strRet += "<br><A HREF=\"/PasswordProtected\">Password Protected Secure Area</A>";
            strRet += "<br>Use following credentials to access it:<br> User Name: Igor<br> Password: MyPassword<br>";

            return strRet;
        }

        static void SendFileOverHTTP(HttpListenerResponse response, string strFilePath )
        {
            if (strFilePath.IndexOf(".cs") != -1 ||
                 strFilePath.IndexOf(".txt") != -1 ||
                 strFilePath.IndexOf(".csproj") != -1
               )
            {
                response.ContentType = "text/plain";
            }

            if (strFilePath.IndexOf(".jpg") != -1 ||
                strFilePath.IndexOf(".bmp") != -1 ||
                strFilePath.IndexOf(".jpeg") != -1
              )
            {
                response.ContentType = "image";
            }

            if (strFilePath.IndexOf(".htm") != -1 ||
                strFilePath.IndexOf(".html") != -1
              )
            {
                response.ContentType = "text/html";
            }

            if (strFilePath.IndexOf(".mp3") != -1)
            {
                response.ContentType = "audio/mpeg";
            }

            FileStream fileToServe = null;
            try
            {
                fileToServe = new FileStream(strFilePath, FileMode.Open, FileAccess.Read);
                long fileLength = fileToServe.Length;
                // Once we know the file length, set the content length.
                response.ContentLength64 = fileLength;
                // Send HTTP headers. Content lenght is ser
                Debug.Print("File length " + fileLength);
                // Now loops sending all the data.

                byte[] buf = new byte[4096];
                for (long bytesSent = 0; bytesSent < fileLength; )
                {
                    // Determines amount of data left.
                    long bytesToRead = fileLength - bytesSent;
                    bytesToRead = bytesToRead < 4096 ? bytesToRead : 4096;
                    // Reads the data.
                    fileToServe.Read(buf, 0, (int)bytesToRead);
                    // Writes data to browser
                    response.OutputStream.Write(buf, 0, (int)bytesToRead);

                    System.Threading.Thread.Sleep(100);
                    // Updates bytes read.
                    bytesSent += bytesToRead;
                }
                fileToServe.Close();
            }
            catch (Exception e)
            {
                if (fileToServe != null)
                {
                    fileToServe.Close();
                }
                throw e;
            }

        }
    }
}

