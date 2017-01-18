////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.IO;
using System.Net;
using System.Text;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Net.Security;
using System.Security.Cryptography.X509Certificates;

/// This program demonstrates how to use the .NET Micro Framework HTTP classes 
/// to create a simple HTTP client that retrieves pages from several different 
/// websites, including secure sites.
namespace HttpClientSample
{
    public static class MyHttpClient
    {
        /// <summary>
        /// Retrieves a page from a Web server, using a simple GET request.
        /// </summary>
        public static void Main()
        {
            Microsoft.SPOT.Hardware.Utility.SetLocalTime(new DateTime(2012, 7, 24));

            // Wait for DHCP (on LWIP devices)
            while (true)
            {
                IPAddress ip = IPAddress.GetDefaultLocalAddress();

                if (ip != IPAddress.Any) break;

                Thread.Sleep(1000);
            }

            // Root CA Certificate needed to validate HTTPS servers.
            byte[] ca = Resource1.GetBytes(
                Resource1.BinaryResources.VerisignCA);

            X509Certificate[] caCerts = 
                new X509Certificate[] { new X509Certificate(ca) };

            // Initialize the default webproxy to be used by all 
            // HttpWebRequests.
            // Change the proxy address to fit your environment.
            HttpWebRequest.DefaultWebProxy =
                new WebProxy("itgproxy.dns.microsoft.com", true);

            // Print the HTTP data from each of the following pages.
            PrintHttpData("http://autos.msn.com/default.aspx", null);
            PrintHttpData("http://www.nytimes.com/", null);

            // Test SSL connection with no certificate verification
            PrintHttpData("https://www.google.com/accounts/ManageAccount/", null);

            // Read from secure webpages by using the Verisign Root CA 
            // certificate that is stored in the Resource1.resx file.
            PrintHttpData("https://www.google.com/accounts/ManageAccount/", 
                caCerts);
        }

        /// <summary>
        /// Prints the HTTP Web page from the given URL and status data while 
        /// receiving the page.
        /// </summary>
        /// <param name="url">The URL of the page to print.</param>
        /// <param name="caCerts">The root CA certificates that are required for 
        /// validating a secure website (HTTPS).</param>
        public static void PrintHttpData(string url, X509Certificate[] caCerts)
        {
            // Create an HTTP Web request.
            HttpWebRequest request = 
                HttpWebRequest.Create(url) as HttpWebRequest;

            // Assign the certificates. The value must not be null if the
            // connection is HTTPS.
            request.HttpsAuthentCerts = caCerts;

            // Set request.KeepAlive to use a persistent connection. 
            request.KeepAlive = true;

            // Get a response from the server.
            WebResponse resp = null;

            try
            {
                resp = request.GetResponse();
            }
            catch (Exception e)
            {
                Debug.Print("Exception in HttpWebRequest.GetResponse(): " + 
                    e.ToString());
            }

            // Get the network response stream to read the page data.
            if (resp != null)
            {
                Stream respStream = resp.GetResponseStream();
                string page = null;
                byte[] byteData = new byte[4096];
                char[] charData = new char[4096];
                int bytesRead = 0;
                Decoder UTF8decoder = System.Text.Encoding.UTF8.GetDecoder();
                int totalBytes = 0;

                // allow 5 seconds for reading the stream
                respStream.ReadTimeout = 5000; 

                // If we know the content length, read exactly that amount of 
                // data; otherwise, read until there is nothing left to read.
                if (resp.ContentLength != -1)
                {
                    for (int dataRem = (int)resp.ContentLength; dataRem > 0; )
                    {
                        Thread.Sleep(500);
                        bytesRead = 
                            respStream.Read(byteData, 0, byteData.Length);
                        if (bytesRead == 0)
                        {
                            Debug.Print("Error: Received " +
                                (resp.ContentLength - dataRem) + " Out of " +
                                resp.ContentLength);
                            break;
                        }
                        dataRem -= bytesRead;

                        // Convert from bytes to chars, and add to the page 
                        // string.
                        int byteUsed, charUsed;
                        bool completed = false;
                        totalBytes += bytesRead;
                        UTF8decoder.Convert(byteData, 0, bytesRead, charData, 0, 
                            bytesRead, true, out byteUsed, out charUsed, 
                            out completed);
                        page = page + new String(charData, 0, charUsed);

                        // Display the page download status.
                        Debug.Print("Bytes Read Now: " + bytesRead + 
                            " Total: " + totalBytes);
                    }

                    page = new String(
                        System.Text.Encoding.UTF8.GetChars(byteData));
                }
                else
                {
                    // Read until the end of the data is reached.
                    while (true)
                    {
                        // If the Read method times out, it throws an exception, 
                        // which is expected for Keep-Alive streams because the 
                        // connection isn't terminated.
                        try
                        {
                            Thread.Sleep(500);
                            bytesRead = 
                                respStream.Read(byteData, 0, byteData.Length);
                        }
                        catch (Exception)
                        {
                            bytesRead = 0;
                        }

                        // Zero bytes indicates the connection has been closed 
                        // by the server.
                        if (bytesRead == 0)
                        {
                            break;
                        }

                        int byteUsed, charUsed;
                        bool completed = false;
                        totalBytes += bytesRead;
                        UTF8decoder.Convert(byteData, 0, bytesRead, charData, 0, 
                            bytesRead, true, out byteUsed, out charUsed, 
                            out completed);
                        page = page + new String(charData, 0, charUsed);

                        // Display page download status.
                        Debug.Print("Bytes Read Now: " + bytesRead + 
                            " Total: " + totalBytes);
                    }

                    Debug.Print("Total bytes downloaded in message body : " 
                        + totalBytes);
                }

                // Display the page results.
                Debug.Print(page);

                // Close the response stream.  For Keep-Alive streams, the 
                // stream will remain open and will be pushed into the unused 
                // stream list.
                resp.Close();
            }
        }
    }
}
