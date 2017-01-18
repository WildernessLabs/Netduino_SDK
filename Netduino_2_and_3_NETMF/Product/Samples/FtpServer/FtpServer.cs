using System;
using System.Collections;
using System.IO;
using System.Net;
using Microsoft.SPOT;
using Microsoft.SPOT.IO;
using Microsoft.SPOT.Net;
using Microsoft.SPOT.Net.Ftp;

namespace FtpServerSample
{
    public class MyFtpServer
    {
        // the standard emulator has two volumes: "ROOT" and "WINFS"
        static string virtualROOT = "/pub/config/root/";
        static string virtualWINFS = "/pub/config/winfs/";

        static string monitoredDirectory = @"\ROOT\special";
        //--//

        public static void Main()
        {
            SetupFiles();

            ProcessClientRequest();
        }

        private static void ProcessClientRequest()
        {
            Debug.Print("Welcome to the FTP Service.");

            FtpListener.AuthenticationEvent += (object sender, UserAuthenticatorArgs e) =>
            {
                if (e.User == "anonymous")
                {
                    Debug.Print("hi anonymous");
                    e.Result = UserAuthenticationResult.Approved;
                }
            };

            // a custom listener will be looking after the "special" directory in the ROOT volumes only 
            FtpListener listener = new FtpListener();
            listener.Prefixes.Add(virtualROOT + "special/");
            listener.Start();

            // all other directories will be handled by the standard listener, on both volumes
            FtpListener listener_root = new FtpFilesystemListener(virtualROOT, @"\ROOT\");
            listener_root.Start();
            FtpListener listener_winfs = new FtpFilesystemListener(virtualWINFS, @"\WINFS\");
            listener_winfs.Start();

            for (; ; )
            {
                FtpListenerContext context = listener.GetContext();
                Stream stream = context.Response.OutputStream;

                switch (context.Request.Method)
                {
                    case WebRequestMethodsEx.Ftp.ChangeDirectory:

                        string path = context.Request.QueryString.Substring(virtualROOT.Length);
                        int markerIdx = path.IndexOf('/');
                        if (markerIdx != -1)
                        {
                            path = path.Substring(0, markerIdx);
                            string dir = @"\ROOT\" + path;
                            if (Directory.Exists(dir) && monitoredDirectory == dir)
                            {
                                Debug.Print("Changed to the monitored directory");
                                context.Response.StatusCode = FtpStatusCode.FileActionOK;
                            }
                            else
                            {
                                context.Response.StatusCode = FtpStatusCode.ActionNotTakenFileUnavailable;
                            }
                        }
                        else
                        {
                            Directory.SetCurrentDirectory(@"\ROOT");
                            context.Response.StatusCode = FtpStatusCode.FileActionOK;
                        }
                        stream.Close();
                        break;
                    case WebRequestMethods.Ftp.ListDirectory:
                    case WebRequestMethods.Ftp.ListDirectoryDetails:
                        {
                            DirectoryInfo cd = new DirectoryInfo(monitoredDirectory);
                            foreach (FileInfo fi in cd.GetFiles())
                            {
                                (stream as FtpResponseStream).Write(fi);
                            }
                            foreach (DirectoryInfo di in cd.GetDirectories())
                            {
                                (stream as FtpResponseStream).Write(di);
                            }
                            context.Response.StatusCode = FtpStatusCode.ClosingData;
                            stream.Close();
                        }
                        break;
                    case WebRequestMethods.Ftp.DownloadFile:
                        DirectoryInfo info = new DirectoryInfo(monitoredDirectory);
                        string prefix = virtualROOT + info.Name + "/";
                        string download = context.Request.QueryString.Substring(prefix.Length);
                        string file = monitoredDirectory + @"\" + download;
                        if (!File.Exists(file))
                        {
                            throw new Exception("File does not exists!");
                        }

                        using (FileStream fs = new FileStream(file, FileMode.Open))
                        {
                            (stream as FtpResponseStream).Write(fs);
                            context.Response.StatusCode = FtpStatusCode.ClosingData;
                            stream.Close();
                        }
                        break;
                    default:
                        context.Response.StatusCode = FtpStatusCode.CommandNotImplemented;
                        stream.Close();
                        break;
                }
            }
        }

        // this method writes some files in the ROOT and WINFS volumes of the standard emulator
        // there will be a "special" directory that will be associated with the main program loop handler
        private static void SetupFiles()
        {
            // Format
            VolumeInfo[] vis = VolumeInfo.GetVolumes();

            if (vis.Length == 0)
            {
                throw new Exception("Cannot run this sample because this emulator does not support a file system");
            }

            for (int i = 0; i < vis.Length; i++)
            {
                if (vis[i].Name.ToLower() == "root")
                {
                    if (!vis[i].IsFormatted)
                    {
                        vis[i].Format(0);
                    }
                    break;
                }
                else if (vis[i].Name.ToLower() == "winfs")
                {
                    if (!vis[i].IsFormatted)
                    {
                        vis[i].Format(0);
                    }
                    break;
                }
            }

            // Create some files in each volume
            // we will prefix files with the volume name to make them recognizable
            Hashtable files = new Hashtable();
            files.Add("SampleReadme.txt", "In this directory you will find the very source of this program.");
            files.Add("SampleCode.cs", "Check http://netmf.codeplex.com for the actual (open) sources!");
            for(int volIdx = 0; volIdx < vis.Length; ++volIdx)
            {
                // all files go into the data directory, duplicated in each volume
                Directory.CreateDirectory(vis[volIdx].Name + @"\" + "data");
                // the same file will also be duplicated inside a special directory in each volume
                Directory.CreateDirectory(vis[volIdx].Name + @"\" + "special");

                foreach (string fileName in files.Keys)
                {
                    // 
                    string path = @"\" + vis[volIdx].Name + @"\special\" + vis[volIdx].Name + "_special_" + fileName;
                    using (StreamWriter sw = new StreamWriter(new FileStream(path, FileMode.Create)))
                    {
                        sw.Write((string)files[fileName]);
                    }
                }
                foreach (string fileName in files.Keys)
                {
                    string path = @"\" + vis[volIdx].Name + @"\data\" + vis[volIdx].Name + "_"+ fileName;
                    using (StreamWriter sw = new StreamWriter(new FileStream(path, FileMode.Create)))
                    {
                        sw.Write((string)files[fileName]);
                    }
                }
            }
        }
    }
}
