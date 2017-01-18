using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Input;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Presentation;

namespace MP3TestApp
{
    // This class dispatches input events from emulated GPIO pins (0-4) to Input.Button 
    // events. It is specific to the SDK's sample emulator; if you use this code,
    // please update this class to reflect the design of your hardware.
    public sealed class GPIOButtonInputProvider
    {
        public readonly Dispatcher Dispatcher;

        private ButtonPad[] buttons;
        private ReportInputCallback callback;
        private InputProviderSite site;
        private PresentationSource source;

        private delegate bool ReportInputCallback(InputReport inputReport);

        // This class maps GPIOs to Buttons processable by Microsoft.SPOT.Presentation
        public GPIOButtonInputProvider(PresentationSource source)
        {
            // Set the input source.
            this.source = source;
            // Register our object as an input source with the input manager and get back an
            // InputProviderSite object which forwards the input report to the input manager,
            // which then places the input in the staging area.
            site = InputManager.CurrentInputManager.RegisterInputProvider(this);
            // Create a delegate that refers to the InputProviderSite object's ReportInput method
            callback = new ReportInputCallback(site.ReportInput);
            Dispatcher = Dispatcher.CurrentDispatcher;

            // Allocate button pads and assign the (emulated) hardware pins as input 
            // from specific buttons.
            ButtonPad[] buttons = new ButtonPad[]
            {
                // Associate the buttons to the pins as setup in the emulator/hardware
                new ButtonPad(this, Button.VK_LEFT  , (Cpu.Pin)86),
                new ButtonPad(this, Button.VK_RIGHT , (Cpu.Pin)87),
                new ButtonPad(this, Button.VK_UP    , (Cpu.Pin)88),
                new ButtonPad(this, Button.VK_SELECT, (Cpu.Pin)89),
                new ButtonPad(this, Button.VK_DOWN  , (Cpu.Pin)90),

                // For a hardware board that has predefined buttons,  can use the 
                // Microsoft.SPOT.Hardware.GetButtonPin to retrieve the GPIO pin number associate 
                // with button name
                //      new ButtonPad(this, Button.Left  , hwProvider.GetButtonPins(Button.Left)),
                //      new ButtonPad(this, Button.Right , hwProvider.GetButtonPins(Button.Right)),
                //      new ButtonPad(this, Button.Up    , hwProvider.GetButtonPins(Button.Up)),
                //      new ButtonPad(this, Button.Select, hwProvider.GetButtonPins(Button.Select)),
                //      new ButtonPad(this, Button.Down  , hwProvider.GetButtonPins(Button.Down)),
                
                
            };

            this.buttons = buttons;
        }

        // The emulated device provides a button pad containing five buttons 
        // for user input. This class represents the button pad.
        internal class ButtonPad
        {
            private Button button;
            private InterruptPort port;
            private GPIOButtonInputProvider sink;

            // Construct the object. Set this class to handle the emulated 
            // hardware's button interrupts.
            public ButtonPad(GPIOButtonInputProvider sink, Button button, Cpu.Pin pin)
            {
                this.sink = sink;
                this.button = button;

                // When this GPIO pin is true, call the Interrupt method.
                port = new InterruptPort(pin, true, Port.ResistorMode.PullUp, Port.InterruptMode.InterruptEdgeBoth);
                port.OnInterrupt += new NativeEventHandler(this.Interrupt);
            }

            void Interrupt(uint data1, uint data2, TimeSpan time)
            {
                RawButtonActions action = (data2 != 0) ? RawButtonActions.ButtonUp : RawButtonActions.ButtonDown;

                RawButtonInputReport report = new RawButtonInputReport(sink.source, time, button, action);

                // Queue the button press to the input provider site.
                sink.Dispatcher.BeginInvoke(sink.callback, report);
            }
        }
    }
}


