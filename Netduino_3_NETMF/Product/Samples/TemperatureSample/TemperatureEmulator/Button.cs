////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Reflection;
using System.Windows.Forms;
using Microsoft.SPOT.Emulator;
using Microsoft.SPOT.Hardware;

namespace Microsoft.SPOT.Emulator.Temperature
{
    /// <summary>
    /// Connects a button to a GPIO pin.  This class is a WinForm control.
    /// </summary>
    public class Button : Control
    {
        // Contains the GPIO pin that this button controls.
        Gpio.GpioPort _port;

        // Stores whether the button is pressed.
        bool _pressed;

        Image _image;
        Image _imagePressed;

        // A key that enables input via the keyboard.
        Keys _key;

        delegate void PortWriteDelegate(bool fState);

        /// <summary>
        /// The default constructor.
        /// </summary>
        public Button()
        {
            _image = Properties.Resources.DefaultButtonUp;
            _imagePressed = Properties.Resources.DefaultButtonDown;

            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);

            this.SetStyle(ControlStyles.Opaque, true);
        }

        /// <summary>
        /// Gets or sets the GPIO pin that this button is responsible for 
        /// toggling.
        /// </summary>
        public Gpio.GpioPort Port
        {
            get { return _port; }
            set { _port = value; }
        }

        /// <summary>
        /// Gets or sets the key that this button should respond to.  This is 
        /// mainly for use if this control is a child of a ButtonCollection 
        /// control with keyboard focus.
        /// </summary>
        public Keys Key
        {
            get { return _key; }
            set { _key = value; }
        }

        /// <summary>
        /// Sets the state of the button.
        /// </summary>
        /// <param name="pressed">Whether the button is depressed.</param>
        internal void OnButtonStateChanged(bool pressed)
        {
            if (_port != null)
            {
                if (_pressed != pressed)
                {
                    _pressed = pressed;
                    bool val = false;

                    switch (_port.Resistor)
                    {
                        case Microsoft.SPOT.Emulator.Gpio.GpioResistorMode.Disabled:
                        case Microsoft.SPOT.Emulator.Gpio.GpioResistorMode.PullUp:
                            val = pressed;
                            break;
                        case Microsoft.SPOT.Emulator.Gpio.GpioResistorMode.PullDown:
                            val = !pressed;
                            break;
                    }

                    // Marshal to the MicroFramework thread.  There's no need to 
                    // wait for a response; just post the message.
                    _port.BeginInvoke(
                        new PortWriteDelegate(_port.Write), !val
                        );

                    this.Invalidate();
                }
            }
        }

        /// <summary>
        /// Returns whether the specified key is the input key.
        /// </summary>
        /// <param name="keyData"></param>
        /// <returns>true if the specified key is the input key; otherwise, 
        /// false.</returns>
        protected override bool IsInputKey(Keys keyData)
        {
            return _key == keyData;
        }

        /// <summary>
        /// Paints the control.  Either Image or ImagePressed is drawn, 
        /// depending on the state of the button.
        /// </summary>
        protected override void OnPaint(PaintEventArgs e)
        {
            Image image = _pressed ? _imagePressed : _image;

            e.Graphics.DrawImage(image, 0, 0,
                new Rectangle(0, 0, image.Width, image.Height),
                GraphicsUnit.Pixel);

            base.OnPaint(e);
        }

        /// <summary>
        /// If this control has focus, any keypress triggers the GPIO port.  
        /// Normally, this control does not have focus.  Instead, it is a child 
        /// of a ButtonCollection.
        /// </summary>
        protected override void OnKeyDown(KeyEventArgs e)
        {
            OnButtonStateChanged(true);

            base.OnKeyDown(e);
        }

        /// <summary>
        /// If this control has focus, any keypress triggers the GPIO port.  
        /// Normally, this control does not have focus.  Instead, it is a child 
        /// of a ButtonCollection.
        /// </summary>
        protected override void OnKeyUp(KeyEventArgs e)
        {
            OnButtonStateChanged(false);

            base.OnKeyUp(e);
        }

        /// <summary>
        /// Responds to mouse down events by depressing the button.
        /// </summary>
        protected override void OnMouseDown(MouseEventArgs e)
        {
            OnButtonStateChanged(true);

            base.OnMouseDown(e);
        }

        /// <summary>
        /// Responds to mouse up events by releasing the button.
        /// </summary>
        protected override void OnMouseUp(MouseEventArgs e)
        {
            OnButtonStateChanged(false);

            base.OnMouseUp(e);
        }
    }
}
