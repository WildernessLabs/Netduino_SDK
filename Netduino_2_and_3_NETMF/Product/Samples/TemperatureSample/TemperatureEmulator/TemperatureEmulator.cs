////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Threading;
using System.Windows.Forms;
using Microsoft.SPOT.Emulator;

namespace Microsoft.SPOT.Emulator.Temperature
{
    /// <summary>
    /// Emulates a temperature sensor.
    /// </summary>
    public class TemperatureEmulator : Emulator
    {
        // Private member to hold the main form.
        private TemperatureEmulatorForm _form;

        /// <summary>
        /// Initializes the application.  Called by the emulator after all 
        /// components are set up and registered.
        /// </summary>
        public override void InitializeComponent()
        {
            // Call the base class implementation
            base.InitializeComponent();

            // Create the main form, passing in the emulator object.
            _form = new TemperatureEmulatorForm(this.Emulator);

            // Initialize the main form's components.
            _form.OnInitializeComponent();

            // Launch the UI thread.
            new Thread(RunForm).Start();
        }

        /// <summary>
        /// Destroys the temperature emulator component.  Called by the emulator 
        /// after the program exits.
        /// </summary>
        public override void UninitializeComponent()
        {
            // Call the base class implementation.
            base.UninitializeComponent();

            // When the Micro Framework is shutting down, inform the the WinForm 
            // application to exit as well.
            Application.Exit();
        }

        /// <summary>
        /// Calls the .NET method that handles the UI processing.
        /// </summary>
        [STAThread]
        private void RunForm()
        {
            // Run the form.
            Application.Run(_form);

            // When this WinForm exits, inform the emulator to shut down.
            this.Emulator.Stop();
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        public static void Main()
        {
            // Start an instance of the temperature emulator class.
            (new TemperatureEmulator()).Start();
        }
    }
}
