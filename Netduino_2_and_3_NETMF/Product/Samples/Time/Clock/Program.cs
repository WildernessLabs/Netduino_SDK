////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Reflection;
using Microsoft.SPOT;
using Microsoft.SPOT.Input;
using Microsoft.SPOT.Presentation;
using Microsoft.SPOT.Presentation.Shapes;
using Microsoft.SPOT.Presentation.Controls;
using Microsoft.SPOT.Presentation.Media;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using Microsoft.SPOT.IO;
using Microsoft.SPOT.Time;
using Microsoft.SPOT.Net.NetworkInformation;

namespace ClockSample
{

    // Standard application class declaration
    public class MyClock : Microsoft.SPOT.Application
    {
        // In our sample we will use an arbitrary time server with IP address 192.43.244.18 
        // one should change this IP Address to a less arbitrary value like a public time server
        // listed here: http://tf.nist.gov/tf-cgi/servers.cgi 
        // (accessing a public time server will require adequate proxy settings though)
        public static byte[] TimeServerIPAddress = new byte[] { 192, 43, 244, 18 };

        //--//

        // Our main window class based on the standard window
        public class MyWindow : Window
        { 
            Text time = new Text();
            Text dayofWeek = new Text();
            Text date = new Text();
            Text ip = new Text();
            Text help = new Text();
            Panel panel = new Panel();

            string[] days = new string[] { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

            private DispatcherTimer clockTimer;

            // Constructor
            public MyWindow()
            {
                time.Font = Resources.GetFont(Resources.FontResources.nina48);
                time.TextContent = DateTime.Now.ToString("hh:mm:ss");
                time.HorizontalAlignment = HorizontalAlignment.Center;
                time.VerticalAlignment = VerticalAlignment.Center;

                dayofWeek.Font = Resources.GetFont(Resources.FontResources.nina14);
                dayofWeek.TextContent = days[(int)DateTime.Now.DayOfWeek];
                dayofWeek.HorizontalAlignment = HorizontalAlignment.Left;
                dayofWeek.VerticalAlignment = VerticalAlignment.Bottom;
                dayofWeek.ForeColor = Colors.Red;

                date.Font = Resources.GetFont(Resources.FontResources.nina14);
                date.TextContent = DateTime.Now.ToString("MM/dd/yyyy");
                date.HorizontalAlignment = HorizontalAlignment.Right;
                date.VerticalAlignment = VerticalAlignment.Bottom;
                date.ForeColor = Colors.Red;

                ip.Font = Resources.GetFont(Resources.FontResources.nina14);
                ip.TextContent = str;
                ip.HorizontalAlignment = HorizontalAlignment.Center;
                ip.VerticalAlignment = VerticalAlignment.Bottom;

                help.Font = Resources.GetFont(Resources.FontResources.nina14);
                help.TextContent = "Buttons: UP resets, SEL Syncs, DOWN schedules";
                help.HorizontalAlignment = HorizontalAlignment.Center;
                help.VerticalAlignment = VerticalAlignment.Top;

                clockTimer = new DispatcherTimer(this.Dispatcher);
                clockTimer.Interval = new TimeSpan(0, 0, 0, 1, 0);
                clockTimer.Tick += new EventHandler(ClockTimer_Tick);

                // Add the time control to the window.
                this.Child = panel;
                panel.Children.Add(time);
                panel.Children.Add(dayofWeek);
                panel.Children.Add(date);
                panel.Children.Add(ip);
                panel.Children.Add(help);

                clockTimer.Start();
            }

            void ClockTimer_Tick(object sender, EventArgs e)
            {
                ip.TextContent = str;
                DateTime now = DateTime.Now;
                time.TextContent = now.ToString("HH:mm:ss");
                dayofWeek.TextContent = days[(int)now.DayOfWeek];
                date.TextContent = now.ToString("MM/dd/yyyy");
                time.Invalidate();
            }            

            // Handle the touch down event
            protected override void OnTouchDown(TouchEventArgs e)
            {
                base.OnTouchDown(e);                
            }

            // Handle the touch up event
            protected override void OnTouchUp(TouchEventArgs e)
            {
                base.OnTouchUp(e);                
            }

            // Handle the touch move event
            protected override void OnTouchMove(TouchEventArgs e)
            {
                base.OnTouchMove(e);                
            }

            // Handle the render event
            public override void OnRender(DrawingContext dc)
            {
                base.OnRender(dc);                
            }
        }

        static MyClock myApplication;
        static string str;

        
        public static void Main()
        {
            myApplication = new MyClock();            

            NetworkInterface[] nis = NetworkInterface.GetAllNetworkInterfaces();

            for (int i = 0; i < nis.Length; i++)
            {
                NetworkInterface networkInterface = nis[i];

                if (networkInterface.NetworkInterfaceType == NetworkInterfaceType.Ethernet)
                {
                    string m_macAddress = networkInterface.PhysicalAddress[0].ToString();
                    for (int j = 1; j < networkInterface.PhysicalAddress.Length; ++j)
                    {
                        m_macAddress += "-" + networkInterface.PhysicalAddress[j].ToString();
                    }

                    string[] m_staticDnsAddresses = new string[] { "0.0.0.0", "0.0.0.0" };
                    for (int k = 0; k < networkInterface.DnsAddresses.Length; ++k)
                    {
                        m_staticDnsAddresses[k] = networkInterface.DnsAddresses[k];
                    }

                    str = networkInterface.IPAddress;
                    break;
                }
            }

            Microsoft.SPOT.Touch.Touch.Initialize(myApplication);

            Window mainWindow = myApplication.CreateWindow();

            // Create the object that configures the GPIO pins to buttons.
            GPIOButtonInputProvider inputProvider =
                new GPIOButtonInputProvider(null);

            // Start the application
            myApplication.Run(mainWindow);
        }

        private MyWindow mainWindow;

        public Window CreateWindow()
        {
            // Create a window object and set its size to the
            // size of the display.
            mainWindow = new MyWindow();
            mainWindow.Height = SystemMetrics.ScreenHeight;
            mainWindow.Width = SystemMetrics.ScreenWidth;

            mainWindow.AddHandler(Buttons.ButtonUpEvent,
                new RoutedEventHandler(OnButtonUp), false);

            // Set the window visibility to visible.
            mainWindow.Visibility = Visibility.Visible;

            // Attach the button focus to the window.
            Buttons.Focus(mainWindow);

            TimeService.SystemTimeChanged += new SystemTimeChangedEventHandler(TimeService_SystemTimeChanged);
            TimeService.TimeSyncFailed += new TimeSyncFailedEventHandler(TimeService_TimeSyncFailed);

            mainWindow.Dispatcher.Invoke(new TimeSpan(0, 0, 5),
                                        new DispatcherOperationCallback(CleanWindow), null);

            return mainWindow;
        }

        void TimeService_TimeSyncFailed(object sender, TimeSyncFailedEventArgs e)
        {
            string errorMsg = "Time Sync Failed with errorCode: " + e.ErrorCode.ToString();
            Debug.Print(errorMsg);
        }

        void TimeService_SystemTimeChanged(object sender, SystemTimeChangedEventArgs e)
        {
            Debug.Print("System Time Changed.");
        }

        protected object CleanWindow(object arg)
        {
            return null;
        }

        private void OnButtonUp(object sender, RoutedEventArgs evt)
        {
            ButtonEventArgs e = (ButtonEventArgs)evt;
            
            if (e.Button == Microsoft.SPOT.Hardware.Button.VK_UP)
            {
                // reset the time to an arbitrary value
                TimeService.SetUtcTime(128752416000000000);
                TimeService.SetTimeZoneOffset(-480); // time origin
            }
            else if (e.Button == Microsoft.SPOT.Hardware.Button.VK_SELECT)
            {
                // Perform a one time sync with the time server
                TimeServiceStatus status = TimeService.UpdateNow(TimeServerIPAddress, 10);
                TimeService.SetTimeZoneOffset(-480); // time origin
            }
            else if (e.Button == Microsoft.SPOT.Hardware.Button.VK_DOWN)
            {
                // start a scheduled periodic sync
                TimeServiceSettings settings = new TimeServiceSettings();

                settings.PrimaryServer = TimeServerIPAddress;
                settings.RefreshTime = 10; // sync every 10 seconds

                TimeService.Settings = settings;

                TimeService.Start(); 
                TimeService.SetTimeZoneOffset(-480); // time origin
            }
        }
    }
}
