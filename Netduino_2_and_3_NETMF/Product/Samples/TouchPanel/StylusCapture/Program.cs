////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Input;
using Microsoft.SPOT.Presentation;
using Microsoft.SPOT.Presentation.Controls;
using Microsoft.SPOT.Presentation.Media;

namespace StylusCaptureSample
{
    /// <summary>
    /// Stylus capture demo for touch panel.
    /// </summary>
    public class MyStylusCapture : Microsoft.SPOT.Application
    {
        /// <summary>
        /// The main window class, based on the standard Window class.
        /// </summary>
        public class MyWindow : Window
        {
            SolidColorBrush brush = new SolidColorBrush(Color.Black);
            Pen pen2 = new Pen(ColorUtility.ColorFromRGB(0, 0, 255));
            Text text1 = new Text();
            Text text2 = new Text();
            Text text3 = new Text();
            Panel panel = new Panel();

            /// <summary>
            /// The default constructor.
            /// </summary>
            public MyWindow()
            {
                // Create 3 text objects for the user to tap on.

                text1.Font = Resources.GetFont(Resources.FontResources.small);
                text1.TextContent = "[Text #1] Tap To Capture";
                text1.HorizontalAlignment = HorizontalAlignment.Center;
                text1.VerticalAlignment = VerticalAlignment.Top;
#if MF_FRAMEWORK_VERSION_V3_0
                text1.StylusDown += new StylusEventHandler(Text_StylusDown);
                text1.StylusUp += new StylusEventHandler(Text_StylusUp);
#else
                text1.TouchDown += new TouchEventHandler(Text_TouchDown);
                text1.TouchUp += new TouchEventHandler(Text_TouchUp);
#endif

                text2.Font = Resources.GetFont(Resources.FontResources.small);
                text2.TextContent = "[Text #2] Tap To Capture";
                text2.HorizontalAlignment = HorizontalAlignment.Center;
                text2.VerticalAlignment = VerticalAlignment.Center;
#if MF_FRAMEWORK_VERSION_V3_0
                text2.StylusDown += new StylusEventHandler(Text_StylusDown);
                text2.StylusUp += new StylusEventHandler(Text_StylusUp);
#else
                text2.TouchDown += new TouchEventHandler(Text_TouchDown);
                text2.TouchUp += new TouchEventHandler(Text_TouchUp);
#endif

                text3.Font = Resources.GetFont(Resources.FontResources.small);
                text3.TextContent = "[Text #3] Tap To Capture";
                text3.HorizontalAlignment = HorizontalAlignment.Center;
                text3.VerticalAlignment = VerticalAlignment.Bottom;
#if MF_FRAMEWORK_VERSION_V3_0
                text3.StylusDown += new StylusEventHandler(Text_StylusDown);
                text3.StylusUp += new StylusEventHandler(Text_StylusUp);
#else
                text3.TouchDown += new TouchEventHandler(Text_TouchDown);
                text3.TouchUp += new TouchEventHandler(Text_TouchUp);
#endif

                // Add the text controls to the window.
                this.Child = panel;
                panel.Children.Add(text1);
                panel.Children.Add(text2);
                panel.Children.Add(text3);
            }

            /// <summary>
            /// Handles the touch down event.
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected void Text_StylusDown(object sender, StylusEventArgs e)
            {
                int x;
                int y;

                e.GetPosition((UIElement)sender, out x, out y);
#else
            void Text_TouchDown(object sender, TouchEventArgs e)
            {
                int x;
                int y;

                e.GetPosition((UIElement)sender, 0, out x, out y);
#endif
                Text text = (Text)sender;

                // If we have already captured the touch, show the point.
#if MF_FRAMEWORK_VERSION_V3_0
                if (sender == Stylus.Captured)
#else
                if (sender == TouchCapture.Captured)
#endif
                {
                    // If the user tapped inside the same control that has the 
                    // touch captured, release the capture.
                    if ((x >= 0) && (y >= 0) && (x <= text.ActualWidth) && (y <= text.ActualHeight))
                    {
#if MF_FRAMEWORK_VERSION_V3_0
                        Stylus.Capture(text, CaptureMode.None);
#else
                        TouchCapture.Capture(text, CaptureMode.None);
#endif
                        text.ForeColor = ColorUtility.ColorFromRGB(0, 0, 0);
                        text.TextContent = "Capture Released.";
                    }
                    // Else, show the point as captured.
                    else
                    {
                        text.TextContent = "Captured. Tap to toggle. Down at (" + x + "," + y + ")";
                    }
                }
                // Else, show the point as captured.
                else
                {
                    text.ForeColor = ColorUtility.ColorFromRGB(255, 0, 0);

                    text.TextContent = "Captured. Tap to toggle. Down at (" + x + "," + y + ")";
#if MF_FRAMEWORK_VERSION_V3_0
                    Stylus.Capture(text);
#else
                    TouchCapture.Capture(text);
#endif
                }
            }

            /// <summary>
            /// Handles the touch up event.
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected void Text_StylusUp(object sender, StylusEventArgs e)
            {
 	            int x;
                int y;

                e.GetPosition((UIElement)sender, out x, out y);
#else
            void Text_TouchUp(object sender, TouchEventArgs e)
            {
                int x;
                int y;

                e.GetPosition((UIElement)sender, 0, out x, out y);
#endif
                Text text = (Text)sender;

                // If we have already captured the touch, show the point.  The 
                // point will be in relation to the text object that captured 
                // the touch.
#if MF_FRAMEWORK_VERSION_V3_0
                if (sender == Stylus.Captured)
#else
                if (sender == TouchCapture.Captured)
#endif
                {
                    text.ForeColor = ColorUtility.ColorFromRGB(255, 0, 0);
                    text.TextContent = "Captured. Tap to toggle. Up at (" + x + "," + y + ")";
                }
                else
                {
                    text.TextContent = "NOT Captured. Up detected at (" + x + "," + y + ")";
                }
            }

            // Structure to hold a point.
            struct point
            {
                public ushort x;
                public ushort y;
            }

            // Create 1000 points.
            const int pointCount = 1000;
            point[] Points = new point[pointCount];
            int pointIndex = 0;

            int cx = 0;
            int cy = 0;
            int r = 6;
            bool drawCircle = false;

            /// <summary>
            /// Helper function to add points to the array.
            /// </summary>
            /// <param name="x"></param>
            /// <param name="y"></param>
            void AddPoint(int x, int y)
            {
                Points[pointIndex].x = (ushort)x;
                Points[pointIndex].y = (ushort)y;
                pointIndex = (pointIndex + 1) % pointCount;
            }

            /// <summary>
            /// Handles the touch down event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void OnStylusDown(StylusEventArgs e)
            {
 	            base.OnStylusDown(e);

                int x;
                int y;

                e.GetPosition((UIElement)this, out x, out y);
#else
            protected override void OnTouchDown(TouchEventArgs e)
            {
                base.OnTouchDown(e);

                int x;
                int y;

                e.GetPosition((UIElement)this, 0, out x, out y);
#endif
                // Start at the beginning of the array and set the flag to draw 
                // a circle.
                pointIndex = 0;
                drawCircle = true;

                cx = x;
                cy = y;

                r = 10;
                Invalidate();
            }

            /// <summary>
            /// Handles the touch up event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void OnStylusUp(StylusEventArgs e)
            {
 	            base.OnStylusUp(e);

                int x;
                int y;

                e.GetPosition((UIElement)this, out x, out y);
#else
            protected override void OnTouchUp(TouchEventArgs e)
            {
                base.OnTouchUp(e);

                int x;
                int y;

                e.GetPosition((UIElement)this, 0, out x, out y);
#endif
                // Set the flag to draw a circle.
                drawCircle = true;

                cx = x;
                cy = y;

                r = 6;
                Invalidate();
            }

            /// <summary>
            /// Handles the render event.
            /// </summary>
            /// <param name="dc"></param>
            public override void OnRender(DrawingContext dc)
            {
                base.OnRender(dc);

                // If the flag is set, draw a circle.
                if (drawCircle)
                {
                    dc.DrawEllipse(brush, pen2, cx, cy, r, r);
                }
            }
        }

        static MyStylusCapture myApplication;

        /// <summary>
        /// Execution entry point.
        /// </summary>
        public static void Main()
        {
            myApplication = new MyStylusCapture();

            // Touch notifications are not turned on by default.  You need to 
            // explicitly inform the Touch engine that you want touch events to 
            // be pumped to your direction, and you want to work with the rest 
            // of the architecture.

            Microsoft.SPOT.Touch.Touch.Initialize(myApplication);

            Window mainWindow = myApplication.CreateWindow();

            // Start the application.
            myApplication.Run(mainWindow);
        }

        private MyWindow mainWindow;

        /// <summary>
        /// Creates a window that has button focus.
        /// </summary>
        /// <returns></returns>
        public Window CreateWindow()
        {
            // Create a window object and set its size to the size of the 
            // display.
            mainWindow = new MyWindow();
            mainWindow.Height = SystemMetrics.ScreenHeight;
            mainWindow.Width = SystemMetrics.ScreenWidth;

            // Set the window visibility to Visible.
            mainWindow.Visibility = Visibility.Visible;

            // Attach the button focus to the window.
            Buttons.Focus(mainWindow);

            return mainWindow;
        }
    }
}
