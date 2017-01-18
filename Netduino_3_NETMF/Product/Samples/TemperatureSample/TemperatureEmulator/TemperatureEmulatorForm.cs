////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Microsoft.SPOT.Emulator;
using Microsoft.SPOT.Emulator.Temperature.SpiTemperatureDevice;

namespace Microsoft.SPOT.Emulator.Temperature
{
    /// <summary>
    /// Defines the temperature emulator window.
    /// </summary>
    public partial class TemperatureEmulatorForm : Form
    {
        // The emulator object is used to gain access to the 
        // SpiTemperatureComponent.
        private Emulator _emulator;
        SpiTemperatureComponent _temperatureDevice;

        /// <summary>
        /// Constructs a TemperatureEmulatorForm using the specified emulator.
        /// </summary>
        /// <param name="emulator"></param>
        public TemperatureEmulatorForm(Emulator emulator)
        {
            // Set the emulator object.
            _emulator = emulator;

            // Find the emulated SPI temperature component.
            _temperatureDevice =
                (SpiTemperatureComponent)_emulator.FindComponentById(
                "SpiTemperatureDevice");

            // Initialize the component on the UI form.
            InitializeComponent();

            // Initialize the radio buttons and temperature.
            radioButtonCelsius.Checked = false;
            radioButtonFahrenheit.Checked = true;
            SetTemperature(72);
        }

        /// <summary>
        /// Overrides the base class OnLoad method, to set up the buttons.
        /// </summary>
        /// <param name="e">EventArgs sent from the environment.</param>
        protected override void OnLoad(EventArgs e)
        {
            // Give the buttonCollection focus, to allow it to handle keyboard 
            // events.
            this.buttonCollection.Select();

            // Call the base class implementation.
            base.OnLoad(e);
        }

        /// <summary>
        /// Helper method to initialize buttons.
        /// </summary>
        /// <param name="button">The button to be initialized.</param>
        /// <param name="componentId">The string identifying the button, found 
        /// in the config file.</param>
        /// <param name="key">The key that this button should respond to.
        /// </param>
        private void InitializeButton(Button button, string componentId,
            Keys key)
        {
            button.Port =
                _emulator.FindComponentById(componentId) as Gpio.GpioPort;
            button.Key = key;
        }

        /// <summary>
        /// Helper method to initialize the component.
        /// </summary>
        public void OnInitializeComponent()
        {
            // Initialize the LCD control with the LCD emulator component.
            this.lcdDisplay.LcdDisplay = _emulator.LcdDisplay;

            // Grab the GPIO pins from the config file.  This can allow 
            // overriding the specific gpio pin numbers, for example, without 
            // having to change code.
            InitializeButton(this.buttonDown, "Pin_Down", Keys.Down);
            InitializeButton(this.buttonLeft, "Pin_Left", Keys.Left);
            InitializeButton(this.buttonRight, "Pin_Right", Keys.Right);
            InitializeButton(this.buttonUp, "Pin_Up", Keys.Up);
            InitializeButton(this.buttonSelect, "Pin_Select", Keys.Enter);
        }

        /// <summary>
        /// Helper method to set the current temperature value that also updates 
        /// the UI.
        /// </summary>
        /// <param name="value">The temperature to set.</param>
        private void SetTemperature(int value)
        {
            // Set the scroll bar.
            vScrollBar1.Value = scrollOffset - value;

            // Update the rest of the UI.
            UpdateTemperature(vScrollBar1.Value);
        }

        /// <summary>
        /// Private member to keep track of the scroll offset.  The scroll 
        /// offset is based on whether the UI is set to Fahrenheit or Celcius.
        /// </summary>
        private int scrollOffset = 260;

        /// <summary>
        /// Helper method to update the UI based on the temperature and type 
        /// settings.
        /// </summary>
        /// <param name="value">The temperature to set.</param>
        private void UpdateTemperature(int value)
        {
            // Calculate the temperature from the scroll position and offset.
            double temperature = scrollOffset - value;

            // Update the text box with the new value.
            textBoxTemperature.Text = (temperature).ToString();

            // The sensor is always in Celsius.  Determine whether the value in 
            // the sensor needs to be converted and then updated.
            if (!radioButtonCelsius.Checked)
            {
                temperature = ((temperature - 32) / 1.8);
            }

            // Invoke the SetTemperature Method on the Emulator Thread.
            if (_temperatureDevice != null)
            {
                _temperatureDevice.Temperature = temperature;
            }
        }

        /// <summary>
        /// Helper method to configure the UI based on temperature type.
        /// </summary>
        private void SetTemperatureType()
        {
            // Get the current value from the text box.
            int temperature = Convert.ToInt32(textBoxTemperature.Text);

            // Check for Celsius or Fahrenheit.
            if (radioButtonCelsius.Checked)
            {

                // Update the scroll bar max, min and position.
                vScrollBar1.Maximum = 263;
                vScrollBar1.Minimum = 0;
                scrollOffset = 127;

                // Because this will only get called when the temperature type 
                // is changing, we know we need to convert.
                temperature =
                    (int)System.Math.Round(((temperature - 32) / 1.8));
            }
            else
            {
                // Update the scroll bar max, min and position.
                vScrollBar1.Maximum = 465;
                vScrollBar1.Minimum = 0;
                scrollOffset = 260;

                // Because this will only get called when the temperature type 
                // is changing, we know we need to convert.
                temperature =
                    (int)System.Math.Round(((temperature * 1.8) + 32));
            }

            // Update the UI.
            SetTemperature(temperature);
        }

        /// <summary>
        /// Handles the radio button event.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="sender">The event arguments.</param>
        private void radioButtonCelsius_CheckedChanged(object sender,
            EventArgs e)
        {
            // Update the UI.
            SetTemperatureType();
        }

        /// <summary>
        /// Handles the scroll event.
        /// </summary>
        /// <param name="sender">The sender of the event.</param>
        /// <param name="sender">The event arguments.</param>
        private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            // Set a new temperature from the scroll value.
            UpdateTemperature(e.NewValue);
        }
    }
}