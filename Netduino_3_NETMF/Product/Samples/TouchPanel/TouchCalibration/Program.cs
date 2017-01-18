////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Input;
using Microsoft.SPOT.Presentation;
using Microsoft.SPOT.Presentation.Controls;

namespace TouchCalibrationSample
{
    /// <summary>
    /// Touch calibration demo for touch panel.
    /// </summary>
    public class MyTouchCalibration : Microsoft.SPOT.Application
    {
        /// <summary>
        /// The main window class, based on the standard Window class.
        /// </summary>
        public class MyWindow : Window
        {
            Microsoft.SPOT.Presentation.Media.SolidColorBrush brush =
                new Microsoft.SPOT.Presentation.Media.SolidColorBrush(
                    Microsoft.SPOT.Presentation.Media.Color.Black);
            Microsoft.SPOT.Presentation.Media.Pen pen =
                new Microsoft.SPOT.Presentation.Media.Pen(
                    Microsoft.SPOT.Presentation.Media.ColorUtility.ColorFromRGB(
                    255, 0, 0));
            Text text = new Text();
            Panel panel = new Panel();

            // Structure to hold a point.
            struct point
            {
                public int x;
                public int y;
            }

            /// <summary>
            /// The default constructor.
            /// </summary>
            public MyWindow()
            {
                text.Font = Resources.GetFont(Resources.FontResources.small);
                text.TextContent = "Press [Select] button to start calibration";
                text.HorizontalAlignment =
                    Microsoft.SPOT.Presentation.HorizontalAlignment.Center;
                text.VerticalAlignment =
                    Microsoft.SPOT.Presentation.VerticalAlignment.Center;

                // Add the text control to the window.
                this.Child = panel;
                panel.Children.Add(text);
            }

            int centerx = 0;
            int centery = 0;
            int r = 6;

            /// <summary>
            /// Handles the touch down event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void  OnStylusDown(StylusEventArgs e)
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

                // If we are in calibrating mode...
                ++currentCalPoint;
                if (calibrating)
                {
                    // Cycle through all of the calibration points.
                    int index = currentCalPoint - 1;

                    cx[index] = (short)x;
                    cy[index] = (short)y;

                    text.TextContent = "{("                    +
                        sx[index].ToString() + "," +
                        sy[index].ToString() + ")=(" +
                        cx[index].ToString() + "," +
                        cy[index].ToString() + ")}";
                        
                    if (currentCalPoint == calPoints.Length)
                    {
                        // The last point has been reached , so set the 
                        // calibration.
                        Microsoft.SPOT.Touch.Touch.ActiveTouchPanel.SetCalibration(
                            calPoints.Length,
                            sx,
                            sy,
                            cx,
                            cy);

                        // Show the calibration points.
                        int i = 0;
                        string str = "";
                        for (i = 0; i < calPoints.Length; i++)
                        {
                            str += i.ToString() + "=" + cx[i].ToString() + "," + cy[i].ToString() + " ";
                        }
                        text.TextContent = str;

                        calibrating = false;
                    }
                }
                else
                {
                    // We are not calibrating, so just show the point.
                    r = 10;
                    centerx = x;
                    centery = y;

                    text.TextContent = x.ToString() + "," + y.ToString();
                }

                Invalidate();
            }

            /// <summary>
            /// Handles the touch up event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void  OnStylusUp(StylusEventArgs e)
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
                r = 6;

                text.TextContent = x.ToString() + "," + y.ToString();

                Invalidate();
            }

            /// <summary>
            /// Handles the touch move event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void OnStylusMove(StylusEventArgs e)
#else
            protected override void OnTouchMove(TouchEventArgs e)
#endif
            {
            }

            point[] calPoints = null;
            int currentCalPoint = 0;
            short[] sx = null;
            short[] sy = null;
            short[] cx = null;
            short[] cy = null;
            bool calibrating = false;

            /// <summary>
            /// Helper function to calibration the screen.
            /// </summary>
            public void CalibrateScreen()
            {
                // If we are already calibrating, don't start calibration again.
                if (!calibrating)
                {
                    text.TextContent = "                                                                     ";

                    calibrating = true;

                    // Ask the touch system how many points are needed to 
                    // calibrate.
                    int calibrationPointCount = 0;
                    Microsoft.SPOT.Touch.Touch.ActiveTouchPanel.GetCalibrationPointCount(
                        ref calibrationPointCount);

                    // Create the calibration point array.
                    calPoints = new point[calibrationPointCount];
                    sx = new short[calibrationPointCount];
                    sy = new short[calibrationPointCount];
                    cx = new short[calibrationPointCount];
                    cy = new short[calibrationPointCount];

                    int i = 0;
                    int x = 0;
                    int y = 0;

                    // Get the points for calibration.
                    for (i = 0; i < calibrationPointCount; i++)
                    {
                        Microsoft.SPOT.Touch.Touch.ActiveTouchPanel.GetCalibrationPoint(
                            i, ref x, ref y);
                        calPoints[i].x = x;
                        calPoints[i].y = y;

                        sx[i] = (short)x;
                        sy[i] = (short)y;
                    }

                    // Start the calibration process.
                    currentCalPoint = 0;
                    Microsoft.SPOT.Touch.Touch.ActiveTouchPanel.StartCalibration();

                    Invalidate();
                }
            }

            /// <summary>
            /// Handles the render event.
            /// </summary>
            /// <param name="dc"></param>
            public override void OnRender(
                Microsoft.SPOT.Presentation.Media.DrawingContext dc)
            {
                base.OnRender(dc);

                if (calibrating)
                {
                    DrawCrossHair(dc, calPoints[currentCalPoint].x,
                        calPoints[currentCalPoint].y);
                }
                else
                {
                    dc.DrawEllipse(brush, pen, centerx, centery, r, r);
                }
            }

            /// <summary>
            /// Helper function to draw a crosshair.
            /// </summary>
            /// <param name="dc"></param>
            /// <param name="x"></param>
            /// <param name="y"></param>
            private void DrawCrossHair(
                Microsoft.SPOT.Presentation.Media.DrawingContext dc, int x,
                int y)
            {
                dc.DrawLine(pen, x - 10, y, x - 2, y);
                dc.DrawLine(pen, x + 10, y, x + 2, y);

                dc.DrawLine(pen, x, y - 10, x, y - 2);
                dc.DrawLine(pen, x, y + 10, x, y + 2);
            }

        }

        static MyTouchCalibration myApplication;

        /// <summary>
        /// Execution entry point.
        /// </summary>
        public static void Main()
        {
            myApplication = new MyTouchCalibration();

            // Touch notifications are not turned on by default.  You need to 
            // explicitly inform the Touch engine that you want touch events to 
            // be pumped to your direction, and you want to work with the rest 
            // of the architecture.

            Microsoft.SPOT.Touch.Touch.Initialize(myApplication);

            Window mainWindow = myApplication.CreateWindow();

            // Create the object that configures the GPIO pins to buttons.
            GPIOButtonInputProvider inputProvider =
                new GPIOButtonInputProvider(null);

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

            // Connect the button handler to all of the buttons.
            mainWindow.AddHandler(Buttons.ButtonUpEvent,
                new RoutedEventHandler(OnButtonUp), false);

            // Set the window visibility to Visible.
            mainWindow.Visibility = Visibility.Visible;

            // Attach the button focus to the window.
            Buttons.Focus(mainWindow);

            return mainWindow;
        }

        /// <summary>
        /// Handles a button click.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnButtonUp(object sender, RoutedEventArgs evt)
        {
            ButtonEventArgs e = (ButtonEventArgs)evt;
            
            // Any button will start the calibration process.
            mainWindow.CalibrateScreen();

            // Print the button code to the Visual Studio output window.
            Debug.Print(e.Button.ToString());
        }
    }
}
