////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using Microsoft.SPOT;
using Socket = System.Net.Sockets.Socket;

namespace SocketClientSample
{
    /// <summary>
    /// Demonstrates a simple network client.  This example makes an HTTP request to 
    /// a Web server, but it is not inherently HTTP-specific.
    /// </summary>
    public static class MySocketClient
    {
        // The Main method.  Retrieves the root document from www.msn.com.
        public static void Main()
        {
            string url = "http://www.msn.com/default.aspx";

            // If the device must go through a proxy server, then set proxy to the 
            // fqdn or ip of the proxy server.
            string proxy = "itgproxy.dns.microsoft.com";
            try
            {
                String html = GetWebPage(proxy, url, 80);
                Debug.Print(html);
            }
            catch (SocketException se)
            {
                Debug.Print("SocketException when connecting to " + url + ".");
                Debug.Print(
                    "If your network uses IPSec, you may need enable the port manually");
                Debug.Print("Socket Error Code: " + se.ErrorCode.ToString());

                Debug.Print(se.ToString());
            }
        }

        /// <summary>
        /// Issues a request for the root document on the specified server.
        /// </summary>
        /// <param name="Proxy"></param>
        /// <param name="URL"></param>
        /// <param name="port"></param>
        /// <returns></returns>
        private static String GetWebPage(String Proxy, String URL, Int32 port)
        {
            const Int32 c_microsecondsPerSecond = 1000000;
            string host = GetHostFromURL(URL);
            string server = Proxy.Length > 0 ? Proxy : host;

            // Create a socket connection to the specified server and port.
            using (Socket serverSocket = ConnectSocket(server, port))
            {
                // Send request to the server.
                String request = "GET " + URL + " HTTP/1.1\r\nHost: " + host +
                    "\r\nConnection: Close\r\n\r\n";
                Byte[] bytesToSend = Encoding.UTF8.GetBytes(request);
                serverSocket.Send(bytesToSend, bytesToSend.Length, 0);

                // Reusable buffer for receiving chunks of the document.
                Byte[] buffer = new Byte[1024];

                // Accumulates the received page as it is built from the buffer.
                String page = String.Empty;

                // Wait up to 30 seconds for initial data to be available.  Throws 
                // an exception if the connection is closed with no data sent.
                DateTime timeoutAt = DateTime.Now.AddSeconds(30);
                while (serverSocket.Available == 0 && DateTime.Now < timeoutAt)
                {
                    System.Threading.Thread.Sleep(100);
                }

                // Poll for data until 30-second timeout.  Returns true for data and 
                // connection closed.
                while (serverSocket.Poll(30 * c_microsecondsPerSecond,
                    SelectMode.SelectRead))
                {
                    // If there are 0 bytes in the buffer, then the connection is 
                    // closed, or we have timed out.
                    if (serverSocket.Available == 0)
                        break;

                    // Zero all bytes in the re-usable buffer.
                    Array.Clear(buffer, 0, buffer.Length);

                    // Read a buffer-sized HTML chunk.
                    Int32 bytesRead = serverSocket.Receive(buffer);

                    // Append the chunk to the string.
                    page = page + new String(Encoding.UTF8.GetChars(buffer));
                }

                // Return the complete string.
                return page;
            }
        }

        /// <summary>
        /// Creates a socket and uses the socket to connect to the server's IP 
        /// address and port.
        /// </summary>
        /// <param name="server"></param>
        /// <param name="port"></param>
        /// <returns></returns>
        private static Socket ConnectSocket(String server, Int32 port)
        {
            // Get server's IP address.
            IPHostEntry hostEntry = Dns.GetHostEntry(server);

            // Create socket and connect to the server's IP address and port
            Socket socket = new Socket(AddressFamily.InterNetwork,
                SocketType.Stream, ProtocolType.Tcp);
            socket.Connect(new IPEndPoint(hostEntry.AddressList[0], port));
            return socket;
        }

        /// <summary>
        /// Extracts the host string from the URL.
        /// </summary>
        /// <param name="URL">The complete URL to parse.</param>
        /// <returns>The host string.  For example: example.com/example</returns>
        private static String GetHostFromURL(string URL)
        {
            // Figure out host
            int start = URL.IndexOf("://");
            int end = start >= 0 ? URL.IndexOf('/', start + 3) : URL.IndexOf('/');
            if (start >= 0)
            {
                // move start after ://
                start += 3;

                if (end >= 0)
                    // http://example.com/example
                    return URL.Substring(start, end - start);

                else
                    // http://example.com
                    return URL.Substring(start);
            }
            if (end >= 0)
                // example.com/example
                return URL.Substring(0, end + 1);
            return URL;
        }
    }
}
