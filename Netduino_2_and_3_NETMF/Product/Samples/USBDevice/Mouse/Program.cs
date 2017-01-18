////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Hardware.UsbClient;

namespace USBMouseSample
{
    /// <summary>
    /// Demonstrates how to use a .NET Micro Framework device (a mouse) as a USB 
    /// device for a desktop computer, and how to configure the USB controller 
    /// on the fly. 
    /// </summary>
    public class MyUSBMouse
    {
        public struct ButtonList
        {
            public InputPort Up;
            public InputPort Down;
            public InputPort Left;
            public InputPort Right;
            public InputPort LeftMouseButton;
            public InputPort RightMouseButton;
            public InputPort Toggle;
            public InputPort Done;
        };

        public static ButtonList buttons;

        /// <summary>
        /// Execution entry point.
        /// </summary>
        public static void Main()
        {
            // Gain access to all USB controllers.
            UsbController[] controllers = UsbController.GetControllers();

            HardwareProvider hwProvider = new HardwareProvider();

            // Set up all buttons to be monitored.
            buttons.Up = new InputPort(hwProvider.GetButtonPins(Button.VK_UP),
                true, Port.ResistorMode.Disabled);
            buttons.Down = new InputPort(hwProvider.GetButtonPins(Button.VK_DOWN),
                true, Port.ResistorMode.Disabled);
            buttons.Left = new InputPort(hwProvider.GetButtonPins(Button.VK_LEFT),
                true, Port.ResistorMode.Disabled);
            buttons.Right = new InputPort(hwProvider.GetButtonPins(Button.VK_RIGHT),
                true, Port.ResistorMode.Disabled);
            buttons.LeftMouseButton = new InputPort(hwProvider.GetButtonPins(Button.VK_BACK),
                true, Port.ResistorMode.Disabled);
            buttons.RightMouseButton = new InputPort(hwProvider.GetButtonPins(Button.VK_HOME),
                true, Port.ResistorMode.Disabled);
            buttons.Toggle = new InputPort(hwProvider.GetButtonPins(Button.VK_SELECT),
                true, Port.ResistorMode.Disabled);
            buttons.Done = new InputPort(hwProvider.GetButtonPins(Button.VK_MENU),
                true, Port.ResistorMode.Disabled);

            // Use the first available USB controller, if it exists.
            if (controllers.Length < 1)
            {
                Debug.Print("No USB controllers exist for this device - we're done.");
                return;
            }
            UsbController UsbPort = controllers[0];
            UsbStream mouseStream = null;

            if (UsbPort.Status == UsbController.PortState.Running)
            {
                Debug.Print(
                    "USB controller 0 is up and running - are you debugging with USB?");

                Debug.Print(
                    "Make sure your platform supports overriding the debug transport.");

                Thread.Sleep(500);
            }

            try
            {
                ConfigureUsbPort(UsbPort, true);

                mouseStream = UsbPort.CreateUsbStream(3, UsbStream.NullEndpoint);
            }
            catch (Exception e)
            {
                Debug.Print(
                    "Mouse stream could not be created due to exception " +
                    e.Message);
                Debug.Print(
                    "Perhaps your native configuration does not contain endpoint 3?");
                return;
            }

            // Be a mouse until the Done button is pressed.
            MouseLoop(UsbPort, mouseStream);

            mouseStream.Close();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="port"></param>
        /// <param name="mouse"></param>
        static void MouseLoop(UsbController port, UsbStream mouse)
        {
            // Mouse report storage.
            byte[] report = new byte[5];

            // Offsets into mouse report storage.
            const byte BUTTONS = 0;
            const byte X_MOVEMENT = 1;
            const byte Y_MOVEMENT = 2;

            // Size of mouse movement for each report (every 10 mS).
            const byte mouseStep = 3;
            bool fAddMouse = true;

            // While the Done button is not pressed...
            while (buttons.Done.Read())
            {
                // Perform this operation once every 10 milliseconds (actually a 
                // bit more than 10 milliseconds).  We've asked the host to 
                // query for mouse info at least every 10 milliseconds, but it 
                // actually queries every 8 milliseconds - it's OK not to 
                // respond to every query.
                Thread.Sleep(10);

                report[X_MOVEMENT] = 0;      // Zero X movement
                report[Y_MOVEMENT] = 0;      // Zero Y movement

                // Add X,Y movement to the mouse report.
                if (!buttons.Left.Read())
                    report[X_MOVEMENT] -= mouseStep;
                if (!buttons.Right.Read())
                    report[X_MOVEMENT] += mouseStep;
                if (!buttons.Up.Read())
                    report[Y_MOVEMENT] -= mouseStep;
                if (!buttons.Down.Read())
                    report[Y_MOVEMENT] += mouseStep;

                if (!buttons.Toggle.Read())
                {
                    if (mouse != null)
                    {
                        mouse.Dispose();
                        mouse = null;
                    }

                    fAddMouse = !fAddMouse;

                    ConfigureUsbPort(port, fAddMouse);

                    if (fAddMouse)
                    {
                        mouse = port.CreateUsbStream(3, UsbStream.NullEndpoint);
                    }
                }

                // Add the button state to the mouse report.
                report[BUTTONS] = (byte)((!buttons.LeftMouseButton.Read() ?
                    1 : 0) | (!buttons.RightMouseButton.Read() ? 2 : 0));

                if (mouse != null)
                {
                    // Send the mouse report to the host.
                    mouse.Write(report, 0, 3);
                }
            }

            // Wait for the Done button to be released.
            while (!buttons.Done.Read()) ;
        }

        /// <summary>
        /// Configures the USB port.
        /// </summary>
        /// <param name="port"></param>
        /// <param name="fAddMouse"></param>
        private static void ConfigureUsbPort(UsbController port, bool fAddMouse)
        {
            try
            {
                bool fMouseConfigExists = false;
                Configuration configuration = port.Configuration;
                Configuration.DeviceDescriptor device;
                Configuration.ConfigurationDescriptor cfgDesc = null;
                int cfgDescIndex = 0;
                int mouseInterfaceIndex = 0;

                if (configuration == null)
                {
                    configuration = new Configuration();

                    // Create the device descriptor.
                    device = new Configuration.DeviceDescriptor(0xBADA, 0x0026,
                        0x0100);
                    device.bcdUSB = 0x200;    // USB 2.00
                    device.bDeviceClass = 0;
                    device.bDeviceSubClass = 0;
                    device.bDeviceProtocol = 0;
                    device.bMaxPacketSize0 = 8;
                    device.iManufacturer = 1; // String #1 is the manufacturer name.
                    device.iProduct = 2;      // String #2 is the product name.
                    device.iSerialNumber = 0;
                }
                else
                {
                    for (int i = 0; i < configuration.descriptors.Length; i++)
                    {
                        if (configuration.descriptors[i] is Configuration.ConfigurationDescriptor)
                        {
                            Configuration.ConfigurationDescriptor cfg =
                                (Configuration.ConfigurationDescriptor)configuration.descriptors[i];

                            // We make the assumption here that the second 
                            // interface is always a mouse.
                            for (int j = 0; j < cfg.interfaces.Length; j++)
                            {
                                if (cfg.interfaces[j].bInterfaceClass == 3 &&
                                    cfg.interfaces[j].bInterfaceProtocol == 2)
                                {
                                    fMouseConfigExists = true;
                                    mouseInterfaceIndex = j;
                                }
                            }
                            cfgDesc =
                                (Configuration.ConfigurationDescriptor)configuration.descriptors[i];
                            break;
                        }
                        cfgDescIndex++;
                    }

                    device = configuration.descriptors[0] as
                        Configuration.DeviceDescriptor;
                }

                if (fAddMouse && !fMouseConfigExists)
                {
                    Configuration.Endpoint endpoint = new Configuration.Endpoint(
                        3, Configuration.Endpoint.ATTRIB_Interrupt |
                        Configuration.Endpoint.ATTRIB_Write);
                    endpoint.wMaxPacketSize = 8; // Mouse data requires only eight bytes.
                    endpoint.bInterval = 10;     // Host should request data from mouse every 10 mS.

                    Configuration.Endpoint[] mouseEndpoints =
                        new Configuration.Endpoint[1] { endpoint };

                    // Set up the mouse interface.
                    Configuration.UsbInterface usbInterface =
                        new Configuration.UsbInterface(1, mouseEndpoints);
                    usbInterface.bInterfaceClass = 3;    // HID interface.
                    usbInterface.bInterfaceSubClass = 1; // Boot device.
                    usbInterface.bInterfaceProtocol = 2; // Standard mouse protocol.
                    usbInterface.iInterface = 1;    // Interface index.

                    // Assemble the HID class descriptor.
                    byte[] HidPayload = new byte[]
                    {
                        0x01, 0x01,     // bcdHID = HID version 1.01.
                        0x00,           // bCountryCode (unimportant).
                        0x01,           // bNumDescriptors = number of descriptors available for this device.
                        0x22,           // bDescriptorType = Report descriptor.
                        0x32, 0x00      // Total size of Report descriptor (50).
                    };

                    usbInterface.classDescriptors =
                        new Configuration.ClassDescriptor[1] { 
                            new Configuration.ClassDescriptor(0x21, HidPayload) };

                    if (cfgDesc == null)
                    {
                        Configuration.UsbInterface[] UsbInterfaces =
                            new Configuration.UsbInterface[1] { usbInterface };
                        cfgDesc = new Configuration.ConfigurationDescriptor(
                            280, UsbInterfaces);
                        mouseInterfaceIndex = 0;
                    }
                    else
                    {
                        Configuration.UsbInterface[] UsbInterfaces =
                            new Configuration.UsbInterface[cfgDesc.interfaces.Length + 1];

                        Array.Copy(cfgDesc.interfaces, UsbInterfaces,
                            cfgDesc.interfaces.Length);

                        UsbInterfaces[UsbInterfaces.Length - 1] = usbInterface;

                        mouseInterfaceIndex = UsbInterfaces.Length - 1;

                        cfgDesc.interfaces = UsbInterfaces;
                    }

                    // Create the report descriptor as a Generic.  This data was 
                    // created using...
                    byte[] BootMouseReportPayload = null;

                    BootMouseReportPayload = new byte[]
                    {
                        0x05, 0x01,  // USAGE_PAGE (Generic Desktop)
                        0x09, 0x02,  // USAGE (Mouse)
                        0xa1, 0x01,  // COLLECTION (Application)
                        0x09, 0x01,  //   USAGE (Pointer)
                        0xa1, 0x00,  //   COLLECTION (Physical)
                        0x05, 0x09,  //     USAGE_PAGE (Button)
                        0x19, 0x01,  //     USAGE_MINIMUM (Button 1)
                        0x29, 0x03,  //     USAGE_MAXIMUM (Button 3)
                        0x15, 0x00,  //     LOGICAL_MINIMUM (0)
                        0x25, 0x01,  //     LOGICAL_MAXIMUM (1)
                        0x95, 0x03,  //     REPORT_COUNT (3)
                        0x75, 0x01,  //     REPORT_SIZE (1)
                        0x81, 0x02,  //     INPUT (Data,Var,Abs)
                        0x95, 0x01,  //     REPORT_COUNT (1)
                        0x75, 0x05,  //     REPORT_SIZE (5)
                        0x81, 0x01,  //     INPUT (Cnst,Ary,Abs)
                        0x05, 0x01,  //     USAGE_PAGE (Generic Desktop)
                        0x09, 0x30,  //     USAGE (X)
                        0x09, 0x31,  //     USAGE (Y)
                        0x15, 0x81,  //     LOGICAL_MINIMUM (-127)
                        0x25, 0x7f,  //     LOGICAL_MAXIMUM (127)
                        0x75, 0x08,  //     REPORT_SIZE (8)
                        0x95, 0x02,  //     REPORT_COUNT (2)
                        0x81, 0x06,  //     INPUT (Data,Var,Rel)
                        0xc0,        //   END_COLLECTION
                        0xc0         // END_COLLECTION
                    };

                    const byte DescriptorRequest = 0x81;
                    const byte ReportDescriptor = 0x22;
                    const byte GetDescriptor = 0x06;
                    const ushort Report_wValue = (ushort)ReportDescriptor << 8;
                    Configuration.GenericDescriptor mouseReportDescriptor =
                        new Configuration.GenericDescriptor(DescriptorRequest,
                            Report_wValue, BootMouseReportPayload);
                    mouseReportDescriptor.bRequest = GetDescriptor;
                    mouseReportDescriptor.wIndex =
                        (ushort)(mouseInterfaceIndex);  // The interface number.

                    Configuration.StringDescriptor manufacturerName;
                    Configuration.StringDescriptor productName;
                    Configuration.StringDescriptor displayName;
                    Configuration.StringDescriptor friendlyName;

                    // Create the standard strings for the USB device
                    manufacturerName = new Configuration.StringDescriptor(
                        1, "Sample Manufacturer");
                    productName = new Configuration.StringDescriptor(
                        2, "Micro Framework Mouse Sample");
                    displayName = new Configuration.StringDescriptor(
                        4, "Micro Framework Mouse");
                    friendlyName = new Configuration.StringDescriptor(
                        5, "NetMF_Mouse");

                    if (configuration.descriptors == null)
                    {
                        configuration.descriptors =
                            new Configuration.Descriptor[]
                        {
                            device,
                            cfgDesc,
                            manufacturerName,
                            productName,
                            displayName,
                            friendlyName,
                            mouseReportDescriptor
                        };
                    }
                    else
                    {
                        int descLength = configuration.descriptors.Length;

                        Configuration.Descriptor[] descriptors =
                            new Configuration.Descriptor[descLength + 1];

                        Array.Copy(configuration.descriptors, descriptors,
                            descLength);

                        descriptors[cfgDescIndex] = cfgDesc;
                        descriptors[descLength] = mouseReportDescriptor;

                        configuration.descriptors = descriptors;
                    }
                }
                else if (!fAddMouse && fMouseConfigExists)
                {
                    Configuration.UsbInterface[] UsbInterfaces =
                        new Configuration.UsbInterface[cfgDesc.interfaces.Length - 1];

                    Array.Copy(cfgDesc.interfaces, UsbInterfaces,
                        cfgDesc.interfaces.Length - 1);

                    // Make sure we are removing the correct interface.
                    if (mouseInterfaceIndex != (cfgDesc.interfaces.Length - 1))
                    {
                        cfgDesc.interfaces[mouseInterfaceIndex] =
                            cfgDesc.interfaces[cfgDesc.interfaces.Length - 1];
                    }

                    cfgDesc.interfaces = UsbInterfaces;

                    configuration.descriptors[cfgDescIndex] = cfgDesc;
                }

                port.Stop();

                // Give the PC some time to unload the driver.
                Thread.Sleep(500);

                port.Configuration = configuration;

                if (port.ConfigurationError !=
                    UsbController.ConfigError.ConfigOK)
                {
                    Debug.Print("Compound configuration reported an error " +
                        port.ConfigurationError.ToString());
                }

                // Kick the USB controller into action.
                if (!port.Start())
                {
                    Debug.Print("Compound USB could not be started.");
                }
            }
            catch (ArgumentException)
            {
                try
                {
                    // Try to recover from a bad USB configuration.
                    port.Configuration = null;
                    port.Start();
                }
                catch
                {
                }

                Debug.Print("Couldn't configure Compound USB due to error " +
                    port.ConfigurationError.ToString());
            }
        }
    }
}
