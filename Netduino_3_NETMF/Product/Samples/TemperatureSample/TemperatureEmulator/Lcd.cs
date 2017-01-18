////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using Microsoft.SPOT.Emulator;
using Microsoft.SPOT.Emulator.Lcd;

namespace Microsoft.SPOT.Emulator.Temperature
{
    /// <summary>
    /// A WinForm control to display the contents of an LCD of a MicroFramework 
    /// application.
    /// </summary>
    public class LcdControl : Control
    {
        // The MicroFramework LCD emulator component
        Lcd.LcdDisplay _lcd;

        // A bitmap to store the current lcd contents
        Bitmap _bitmap;

        /// <summary>
        /// Default constructor for an LcdControl.
        /// </summary>
        public LcdControl()
        {
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.Opaque, true);
        }

        /// <summary>
        /// Gets or sets the LCD emulator component.
        /// </summary>
        public Lcd.LcdDisplay LcdDisplay
        {
            get { return _lcd; }
            set
            {
                if (_lcd != null)
                {
                    _lcd.OnDevicePaint -=
                        new OnDevicePaintEventHandler(OnDevicePaint);
                }

                _lcd = value;

                if (_lcd != null)
                {
                    _lcd.OnDevicePaint +=
                        new OnDevicePaintEventHandler(OnDevicePaint);
                }
            }
        }

        /// <summary>
        /// A callback function that occurs when the MicroFramework application 
        /// flushes the LCD buffer to the screen.
        /// </summary>
        /// <param name="sender">The emulator component firing the event.
        /// </param>
        /// <param name="args">Specifies what is being redrawn.</param>
        private void OnDevicePaint(object sender, OnDevicePaintEventArgs args)
        {
            Bitmap bitmap = args.Bitmap;

            if (_bitmap == null)
            {
                // The first time the callback occurs, make a copy of the LCD 
                // contents.  A copy must be created, because when this callback 
                // returns, the MicroFramework is able to write into the frame 
                // buffer.
                _bitmap = (Bitmap)bitmap.Clone();
            }
            else
            {
                // Lock the _bitmap object.  This bitmap is accessed on the 
                // MicroFramework thread, during this callback.  This bitmap is 
                // also accessed on the UI thread, during paint.
                lock (_bitmap)
                {
                    Rectangle rectangle = new Rectangle(0, 0, _bitmap.Width,
                        _bitmap.Height);

                    // Copy bitmap data.
                    BitmapData bdSrc = bitmap.LockBits(rectangle,
                        System.Drawing.Imaging.ImageLockMode.ReadOnly,
                        bitmap.PixelFormat);
                    BitmapData bdDst = _bitmap.LockBits(rectangle,
                        System.Drawing.Imaging.ImageLockMode.WriteOnly,
                        bitmap.PixelFormat);

                    // Copying the entire frame buffer can be substantially 
                    // slower in safe code.  Copy the bitmap data, 4 bytes at a 
                    // time.
                    unsafe
                    {
                        int* src = (int*)bdSrc.Scan0.ToPointer();
                        int* dst = (int*)bdDst.Scan0.ToPointer();
                        int cInts = bdSrc.Stride / 4 * bitmap.Height;

                        Debug.Assert(bdSrc.Stride > 0);
                        Debug.Assert(bitmap.Width == _bitmap.Width);
                        Debug.Assert(bitmap.Height == _bitmap.Height);
                        Debug.Assert(bitmap.PixelFormat == _bitmap.PixelFormat);

                        for (int i = 0; i < cInts; i++)
                        {
                            *dst++ = *src++;
                        }
                    }

                    bitmap.UnlockBits(bdSrc);
                    _bitmap.UnlockBits(bdDst);
                }
            }

            this.Invalidate();
        }

        /// <summary>
        /// Handles painting.
        /// </summary>
        protected override void OnPaint(PaintEventArgs e)
        {
            if (_bitmap != null)
            {
                // Synchonize access to the bitmap with the MicroFramwork 
                // thread.
                lock (_bitmap)
                {
                    e.Graphics.DrawImage(_bitmap, 0, 0);
                }
            }
            else
            {
                base.OnPaintBackground(e);
            }

            if (this.DesignMode)
            {
                // In design time, paint a dotted outline of the control.
                OnPaintDesignMode(e);
            }

            base.OnPaint(e);
        }

        /// <summary>
        /// A simple design mode to allow this control to be used with the 
        /// WinForm designer.
        /// </summary>
        private void OnPaintDesignMode(PaintEventArgs e)
        {
            Rectangle rc = this.ClientRectangle;
            Color penColor;

            // Select a black or white pen, to complement the color of the 
            // control.
            if (this.BackColor.GetBrightness() < .5)
            {
                penColor = ControlPaint.Light(this.BackColor);
            }
            else
            {
                penColor = ControlPaint.Dark(this.BackColor); ;
            }

            using (Pen pen = new Pen(penColor))
            {
                pen.DashStyle = DashStyle.Dash;

                rc.Width--;
                rc.Height--;
                e.Graphics.DrawRectangle(pen, rc);
            }
        }
    }
}
