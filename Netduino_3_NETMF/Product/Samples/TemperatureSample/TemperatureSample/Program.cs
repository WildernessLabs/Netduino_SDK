////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Input;
using Microsoft.SPOT.Presentation;
using Microsoft.SPOT.Presentation.Controls;
using Microsoft.SPOT.Presentation.Media;

namespace TemperatureSample
{
    /// <summary>
    /// The main application class.
    /// </summary>
    public class MyTemperature : Microsoft.SPOT.Application
    {
        /// <summary>
        /// The executable entry point.
        /// </summary>
        public static void Main()
        {
            // Create a new program object.
            MyTemperature myApplication = new MyTemperature();

            // Create the main window.
            Window mainWindow = myApplication.CreateWindow();

            // Create the object that configures the GPIO pins to buttons.
            GPIOButtonInputProvider inputProvider =
                new GPIOButtonInputProvider(null);

            // Start the application.
            myApplication.Run(mainWindow);
        }

        /// <summary>
        /// Defines values for the temperature format.
        /// </summary>
        private enum TempMode
        {
            Celcius,
            Fahrenheit,
        }

        // Private members:

        // The main window.
        private Window mainWindow;

        // Target Temperature text field.
        private Text _textTargetTemp;

        // Current Temperature text field.
        private Text _textCurrentTemp;

        // Status Indicator (heat/cool/off).
        private StatusIndicator _statusIndicator;

        // Target temperature.
        private double _targetTemp = 72;

        // Timer to check the temperature.
        private DispatcherTimer _timer;

        // Temperature mode.
        private TempMode _tempMode = TempMode.Fahrenheit;

        /// <summary>
        /// Creates the main window.
        /// </summary>
        /// <returns>The main window.</returns>
        public Window CreateWindow()
        {
            // Create a window object and set its size to the size of the 
            // display.
            mainWindow = new Window();
            mainWindow.Width = SystemMetrics.ScreenWidth;
            mainWindow.Height = SystemMetrics.ScreenHeight;

            // Detect landscape or portrait so the program can adjust its 
            // layout.
            bool portrait =
                SystemMetrics.ScreenWidth < SystemMetrics.ScreenHeight;

            // Create the main stack panel.
            StackPanel stack = new StackPanel(portrait ?
                Orientation.Vertical : Orientation.Horizontal);

            // Create stack panels for the current temperature and target 
            // temperature.
            StackPanel stack1 = new StackPanel(Orientation.Vertical);
            StackPanel stack2 = new StackPanel(Orientation.Vertical);

            // Create border panels for the current temperature and target 
            // temperature.
            BorderPanel panel1;
            BorderPanel panel2;

            // Detect portrait or landscape orientation.
            if (portrait)
            {

                // If in portrait mode, set the width to the screen width and 
                // height to 1/4 and 3/4.
                panel1 = new BorderPanel(SystemMetrics.ScreenWidth,
                    (int)(SystemMetrics.ScreenHeight * .25));
                panel2 = new BorderPanel(SystemMetrics.ScreenWidth,
                    (int)(SystemMetrics.ScreenHeight * .75));
            }
            else
            {
                // Otherwise, set the width and height to 1/2 and 1/2, 
                // respectively.
                panel1 = new BorderPanel(SystemMetrics.ScreenWidth / 2,
                    SystemMetrics.ScreenHeight);
                panel2 = new BorderPanel(SystemMetrics.ScreenWidth / 2,
                    SystemMetrics.ScreenHeight);
            }

            // Add the border panels to the panels containing the current 
            // temperature and target temperature.
            panel1.Children.Add(stack1);
            panel2.Children.Add(stack2);

            // Add the stack panels to the border panels.
            stack.Children.Add(panel1);
            stack.Children.Add(panel2);

            // Create text controls for the current temperature.
            _textCurrentTemp = new Text();
            _textCurrentTemp.Font =
                Resources.GetFont(Resources.FontResources.nina48);
            _textCurrentTemp.TextContent =
                Resources.GetString(Resources.StringResources.InitialTemp);
            _textCurrentTemp.HorizontalAlignment =
                Microsoft.SPOT.Presentation.HorizontalAlignment.Center;
            _textCurrentTemp.VerticalAlignment =
                Microsoft.SPOT.Presentation.VerticalAlignment.Center;

            Text currentLabel = new Text();
            currentLabel.Font =
                Resources.GetFont(Resources.FontResources.nina14);
            currentLabel.TextContent =
                Resources.GetString(Resources.StringResources.CurrentTemp);
            currentLabel.HorizontalAlignment =
                Microsoft.SPOT.Presentation.HorizontalAlignment.Center;
            currentLabel.VerticalAlignment =
                Microsoft.SPOT.Presentation.VerticalAlignment.Center;

            // Add the text controls to the current temperature stack panel.
            stack1.Children.Add(currentLabel);
            stack1.Children.Add(_textCurrentTemp);

            // Create controls for the target temperature.
            _textTargetTemp = new Text();
            _textTargetTemp.Font =
                Resources.GetFont(Resources.FontResources.nina48);
            _textTargetTemp.TextContent =
                Resources.GetString(Resources.StringResources.InitialTemp);
            _textTargetTemp.HorizontalAlignment =
                HorizontalAlignment.Center;
            _textTargetTemp.VerticalAlignment =
                VerticalAlignment.Center;

            Text targetLabel = new Text();
            targetLabel.Font =
                Resources.GetFont(Resources.FontResources.nina14);
            targetLabel.TextContent =
                Resources.GetString(Resources.StringResources.TargetTemp);
            targetLabel.HorizontalAlignment = HorizontalAlignment.Center;
            targetLabel.VerticalAlignment = VerticalAlignment.Center;

            // Create the air conditioner or heater status indicator.
            _statusIndicator = new StatusIndicator();

            // Add the controls to the target temperature stack panel.
            stack2.Children.Add(targetLabel);
            stack2.Children.Add(_textTargetTemp);
            stack2.Children.Add(_statusIndicator);

            // Add instructions to the target temperature stack panel.
            TextFlow instructions = new TextFlow();
            instructions.HorizontalAlignment = HorizontalAlignment.Center;
            instructions.VerticalAlignment = VerticalAlignment.Center;
            instructions.TextAlignment = TextAlignment.Center;
            instructions.TextRuns.Add(new TextRun("Up Button to Increase",
                Resources.GetFont(Resources.FontResources.nina14), Color.Black));
            instructions.TextRuns.Add(TextRun.EndOfLine);
            instructions.TextRuns.Add(new TextRun("Down Button to Decrease",
                Resources.GetFont(Resources.FontResources.nina14), Color.Black));
            instructions.TextRuns.Add(TextRun.EndOfLine);
            instructions.TextRuns.Add(new TextRun(
                "Select Button to Toggle Celsius / Fahrenheit",
                Resources.GetFont(Resources.FontResources.nina14), Color.Black));
            stack2.Children.Add(instructions);

            // Set the main window child to the main horizontal stack panel.
            mainWindow.Child = stack;

            // Connect the button handler to all of the buttons.
            mainWindow.AddHandler(Buttons.ButtonUpEvent,
                new RoutedEventHandler(OnButtonUp), false);

            // Set the window visibility to Visible.
            mainWindow.Visibility = Visibility.Visible;

            // Attach the button focus to the window.
            Buttons.Focus(mainWindow);

            // Create the timer that will check the current temperature.
            _timer = new DispatcherTimer(mainWindow.Dispatcher);
            _timer.Interval = new TimeSpan(0, 0, 0, 0, 50);
            _timer.Tick += new EventHandler(OnTimer);
            _timer.Start();

            return mainWindow;
        }

        /// <summary>
        /// Helper function for updating the user interface.
        /// </summary>
        private void UpdateUI()
        {

            // Read the temperature sensor.

            // Make sure the sensor object is valid.
            if (_sensor == null)
                _sensor = new SpiTemperatureSensor();

            // Read the value from the sensor.
            double temp = _sensor.ReadTemperature();
            if (_tempMode == TempMode.Fahrenheit)
                temp = (temp * 1.8) + 32;

            // Round off the value so it will match the emulator, because of 
            // rounding errors during Celsius conversion.
            temp = System.Math.Round(temp);

            // Set the current temperature text control value based on the 
            // temperature mode of celcius or fahrenheit
            if (_tempMode == TempMode.Celcius)
            {
                _textCurrentTemp.TextContent = temp.ToString() + " C";
                _textTargetTemp.TextContent = _targetTemp.ToString() + " C";
            }
            else
            {
                _textCurrentTemp.TextContent = temp.ToString() + " F";
                _textTargetTemp.TextContent = _targetTemp.ToString() + " F";
            }

            // See if we need to turn on the heater or cooler.
            if (temp > _targetTemp)
            {
                // Turn on the cooler.
                _statusIndicator.Status = StatusIndicator.StatusType.Cool;
            }
            else if (temp < _targetTemp)
            {
                // Turn on the heater.
                _statusIndicator.Status = StatusIndicator.StatusType.Heat;
            }
            else
            {
                // Turn off both.
                _statusIndicator.Status = StatusIndicator.StatusType.Off;
            }
        }

        /// <summary>
        /// Private member for the temperature sensor.
        /// </summary>
        private SpiTemperatureSensor _sensor = null;

        /// <summary>
        /// Handles the timer to update the user interface.
        /// </summary>
        /// <param name="o">Generic object.  Not used.</param>
        /// <param name="e">Generic event args.  Not used.</param>
        private void OnTimer(object o, EventArgs e)
        {
            // Stop the timer until this method is done.
            _timer.Stop();

            // Update the UI.
            UpdateUI();

            // Start the timer back up.
            _timer.Start();
        }

        /// <summary>
        /// Processes button input.
        /// </summary>
        /// <param name="sender">Generic sender.  Not used.</param>
        /// <param name="e">Button event argument that holds information about 
        /// the button that was pressed.</param>
        private void OnButtonUp(object sender, RoutedEventArgs evt)
        {
            ButtonEventArgs e = (ButtonEventArgs)evt;

            // Switch based on what button was pressed.
            switch (e.Button)
            {
                case Button.VK_UP:    // Process the up button.
                    _targetTemp++;    // Increment the target temperature.
                    UpdateUI();       // Update the user interface.
                    break;

                case Button.VK_DOWN:  // Process the down button.
                    _targetTemp--;    // Decrement the target temperature.
                    UpdateUI();       // Update the user interface.
                    break;

                case Button.VK_SELECT:  // Process the select button.

                    // Switch the temperature format between Celcius and 
                    // Fahrenheit.
                    if (_tempMode == TempMode.Celcius)
                    {
                        // Set the new mode
                        _tempMode = TempMode.Fahrenheit;

                        // Modify the current target temperature.
                        _targetTemp = System.Math.Round((_targetTemp * 1.8) + 32);
                    }
                    else
                    {
                        // Set the new mode.
                        _tempMode = TempMode.Celcius;

                        // Modify the current target temperature.
                        _targetTemp = System.Math.Round((_targetTemp - 32) / 1.8);
                    }

                    // Update the user interface.
                    UpdateUI();
                    break;
            }
        }
    }

    /// <summary>
    /// This helper class draws a border around the panel.
    /// </summary>
    internal sealed class BorderPanel : Panel
    {

        // Private members for width and height so they can be set manually.
        private int _width;
        private int _height;

        /// <summary>
        /// Constructs a BorderPanel with a specified width and a height.
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        public BorderPanel(int width, int height)
        {
            // Set the width and height.
            _width = width;
            _height = height;
        }

        /// <summary>
        /// Override the MeasureOverride to tell the GUI it should not set the 
        /// width and height according to the controls contained inside this 
        /// object (the latter is the default behavior).  Sets a width and 
        /// height such that when the border is drawn, it is uniform and of a 
        /// specific size.  
        /// <para>
        /// By setting the desiredWidth and desiredHeight parameters, the GUI 
        /// uses these values instead of ones it would have calculated based on 
        /// what controls are contained in this object.
        /// </para>
        /// </summary>
        /// <param name="availableWidth"></param>
        /// <param name="availableHeight"></param>
        /// <param name="desiredWidth"></param>
        /// <param name="desiredHeight"></param>
        protected override void MeasureOverride(int availableWidth,
            int availableHeight, out int desiredWidth, out int desiredHeight)
        {
            desiredWidth = _width;
            desiredHeight = _height;
        }

        /// <summary>
        /// Overrides the base class OnRender method, to handle drawing.
        /// </summary>
        /// <param name="dc"></param>
        public override void OnRender(DrawingContext dc)
        {
            // Call the base class.
            base.OnRender(dc);

            // Create a solid black brush.
            SolidColorBrush brush =
                new SolidColorBrush(ColorUtility.ColorFromRGB(0, 0, 0));

            // Make the brush transparent.  Display only the outside edge, which 
            // uses the pen.
            brush.Opacity = 0;

            // Draw the border around the outside.
            dc.DrawRectangle(brush, new Pen(Color.Black), 0, 0, dc.Width,
                dc.Height);
        }
    }

    /// <summary>
    /// This helper class displays a test value and draws a bitmap image for 
    /// status.
    /// </summary>
    internal sealed class StatusIndicator : Panel
    {

        // Defines values for the different states.
        public enum StatusType
        {
            Off,
            Heat,
            Cool,
        }

        // Private member for current status.
        private StatusType _status = StatusType.Off;

        // Public accessor for current status.
        public StatusType Status
        {

            // Return the status.
            get { return _status; }

            // Set the status and then redraw.
            set
            {
                _status = value;
                Invalidate();
            }
        }

        /// <summary>
        /// The default constructor.
        /// </summary>
        public StatusIndicator()
        {
        }

        /// <summary>
        /// Override the MeasureOverride method to tell the GUI it should not 
        /// set the width and height according to the controls contained inside 
        /// this object (the latter is the default behavior). Set a width and 
        /// height such that when the border is drawn, it is uniform and of a 
        /// specific size.
        /// <para>
        /// By setting the desiredWidth and desiredHeight parameters, the GUI 
        /// uses these values instead of ones it would have calculated based on 
        /// what controls are contained in this object.
        /// </para>
        /// </summary>
        /// <param name="availableWidth"></param>
        /// <param name="availableHeight"></param>
        /// <param name="desiredWidth"></param>
        /// <param name="desiredHeight"></param>
        protected override void MeasureOverride(int availableWidth,
            int availableHeight, out int desiredWidth, out int desiredHeight)
        {
            desiredWidth = 100;
            desiredHeight = 40;
        }

        /// <summary>
        /// Override OnRender to handle drawing.
        /// </summary>
        /// <param name="dc"></param>
        public override void OnRender(DrawingContext dc)
        {
            // Call the base class.
            base.OnRender(dc);

            // Set the status string to a default of Off.
            string status = "Off";

            // Get a bitmap depending on the current status.
            Bitmap bmp = null;
            if (_status == StatusType.Cool)
            {

                // Set the status text to Cool.
                status = "Cool";

                // Get the bitmap that represents a status of Cool.
                bmp = Resources.GetBitmap(Resources.BitmapResources.snowflake);
            }
            else if (_status == StatusType.Heat)
            {

                // Set the status text to Heat.
                status = "Heat";

                // Get the bitmap that represents a status of Heat.
                bmp = Resources.GetBitmap(Resources.BitmapResources.fire);
            }

            // Draw the status text.
            dc.DrawText("Status: " + status,
                Resources.GetFont(Resources.FontResources.small), Color.Black,
                10, 10);

            // If a bitmap was loaded, display it.
            if (bmp != null)
                dc.DrawImage(bmp, 100, 0);
        }
    }
}
