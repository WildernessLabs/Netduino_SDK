////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Ink;
using Microsoft.SPOT.Input;
using Microsoft.SPOT.Presentation;
using Microsoft.SPOT.Presentation.Controls;
using Microsoft.SPOT.Presentation.Media;
using Microsoft.SPOT.Touch;

namespace InkCanvasSample
{
    /// <summary>
    /// Demonstrates an ink canvas for a touch panel.
    /// </summary>
    public class MyInkCanvas : Microsoft.SPOT.Application
    {
        /// <summary>
        /// Holds palette events.
        /// </summary>
        public class PaletteEventArg : EventArgs
        {
            /// <summary>
            /// Constructs a PaletteEventArg for the specified item.
            /// </summary>
            /// <param name="item"></param>
            public PaletteEventArg(int item)
            {
                Item = item;
            }

            public readonly int Item;
        }

        /// <summary>
        /// Delegate for palette control events.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public delegate void PaletteControlEventHandler(Object sender, 
            PaletteEventArg e);

        /// <summary>
        /// Defines a Palette control.
        /// </summary>
        public class PaletteControl : UIElement
        {
            /// <summary>
            /// Constructs a PaletteControl with the specified settings.
            /// </summary>
            /// <param name="row"></param>
            /// <param name="column"></param>
            /// <param name="width"></param>
            /// <param name="height"></param>
            public PaletteControl(int row, int column, int width, int height)
            {
                // Store the values.
                _width = width;
                _height = height;
                _itemCount = row * column;

                // Calculate the width and height of each item.
                _itemWidth = (width - 4) / column;
                _itemHeight = (height - 4) / row;

                // Store the row and column.
                _row = row;
                _column = column;
            }

            /// <summary>
            /// Handles the render item event.  This definition is here so we 
            /// can call it from the OnRender method, but it is implemented in 
            /// the derived class below.
            /// </summary>
            /// <param name="dc"></param>
            /// <param name="row"></param>
            /// <param name="column"></param>
            /// <param name="x"></param>
            /// <param name="y"></param>
            /// <param name="width"></param>
            /// <param name="height"></param>
            protected virtual void OnRenderItem(DrawingContext dc, int row, 
                int column, int x, int y, int width, int height)
            {
            }

            /// <summary>
            /// Overrides the OnRender to do our own drawing.
            /// </summary>
            /// <param name="dc"></param>
            public override void OnRender(DrawingContext dc)
            {
                base.OnRender(dc);

                // Start at row 0, column 0.
                int i = 0;
                int j = 0;

                // Loop through all the rows.
                for (i = 0; i < _row; i++)
                {
                    // Loop through all the columns.
                    for (j = 0; j < _column; j++)
                    {
                        int x = j * _itemWidth + 2;
                        int y = i * _itemHeight + 2;

                        // Render the item.
                        OnRenderItem(dc, i, j, x, y, _itemWidth, _itemHeight);
                    }
                }
            }

            /// <summary>
            /// 
            /// </summary>
            /// <param name="arrangeWidth"></param>
            /// <param name="arrangeHeight"></param>
            protected override void ArrangeOverride(int arrangeWidth,
                int arrangeHeight)
            {
            }

            /// <summary>
            /// 
            /// </summary>
            /// <param name="availableWidth"></param>
            /// <param name="availableHeight"></param>
            /// <param name="desiredWidth"></param>
            /// <param name="desiredHeight"></param>
            protected override void MeasureOverride(int availableWidth,
                int availableHeight, out int desiredWidth,
                out int desiredHeight)
            {
                desiredWidth = (availableWidth > _width) ?
                _width : availableWidth;
                desiredHeight = (availableHeight > _height) ?
                _height : availableHeight;
            }

            /// <summary>
            /// Handles clicking on an item.
            /// </summary>
            /// <param name="e"></param>
            protected virtual void OnItemClick(PaletteEventArg e)
            {
                if (ItemClick != null)
                {
                    ItemClick(this, e);
                }
            }

            /// <summary>
            /// Handles the touch up event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void  OnStylusUp(StylusEventArgs e)
            {
 	            base.OnStylusUp(e);

                Stylus.Capture(this, CaptureMode.None);
#else
            protected override void OnTouchUp(TouchEventArgs e)
            {
                base.OnTouchUp(e);

                TouchCapture.Capture(this, CaptureMode.None);
#endif
                int x;
                int y;

                // Get the position of the event.
#if MF_FRAMEWORK_VERSION_V3_0
                e.GetPosition(this, out x, out y);
#else
                e.GetPosition(this, 0, out x, out y);
#endif

                int r = 0;
                int c = 0;

                // Calculate the row and column.
                r = y / _itemHeight;
                c = x / _itemWidth;

                // Validate the row and column.
                if ((r >= 0) && (r < _row) && (c >= 0) && (c < _column))
                {
                    // Calculate the item.
                    int item = c + r * _column;

                    // Make sure the item is the same one that the touch down 
                    // event occurred on.
                    if (item == _pressedItem)
                    {
                        // Trigger a palette event.
                        PaletteEventArg arg = new PaletteEventArg(item);
                        OnItemClick(arg);
                    }
                }
            }

            /// <summary>
            /// Handles the touch down event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void  OnStylusDown(StylusEventArgs e)
            {
 	            base.OnStylusDown(e);

                Stylus.Capture(this);
#else
            protected override void OnTouchDown(TouchEventArgs e)
            {
                base.OnTouchDown(e);

                TouchCapture.Capture(this);
#endif

                int x;
                int y;

                // Get the position of the event.
#if MF_FRAMEWORK_VERSION_V3_0
                e.GetPosition(this, out x, out y);
#else
                e.GetPosition(this, 0, out x, out y);
#endif

                int r = 0;
                int c = 0;

                // Calculate the row and column.
                r = y / _itemHeight;
                c = x / _itemWidth;

                // Validate the row and column.
                if ((r >= 0) && (r < _row) && (c >= 0) && (c < _column))
                {
                    // Set the item pressed value.
                    _pressedItem = c + r * _column;
                }
                else
                {
                    // Reset the item pressed value.
                    _pressedItem = -1;
                }

                Invalidate();
            }

            protected int _width;
            protected int _height;
            protected int _itemWidth;
            protected int _itemHeight;
            protected int _itemCount;
            protected int _row;
            protected int _column;

            protected int _pressedItem = -1;

            public event PaletteControlEventHandler ItemClick;
        }

        /// <summary>
        /// Defines a Button.
        /// </summary>
        public class Button : UIElement
        {
            /// <summary>
            /// Constructs a button with the specified caption and font, and 
            /// default size.
            /// </summary>
            /// <param name="caption"></param>
            /// <param name="font"></param>
            public Button(string caption, Font font)
            {
                _caption = caption;
                _font = font;
                HorizontalAlignment = HorizontalAlignment.Left;
                VerticalAlignment = VerticalAlignment.Bottom;

                int textWidth;
                int textHeight;
                _font.ComputeExtent(_caption, out textWidth, out textHeight);

                _width = textWidth + _textMarginX * 2;
                _height = textHeight + _textMarginY * 2;
            }

            /// <summary>
            /// Constructs a button with the specified caption, font, and size.
            /// </summary>
            /// <param name="caption"></param>
            /// <param name="font"></param>
            /// <param name="width"></param>
            /// <param name="height"></param>
            public Button(string caption, Font font, int width, int height)
            {
                _width = width;
                _height = height;
                _caption = caption;
                _font = font;
            }

            /// <summary>
            /// Overrides OnRender to do our own drawing.
            /// </summary>
            /// <param name="dc"></param>
            public override void OnRender(DrawingContext dc)
            {
                int x;
                int y;

                SolidColorBrush brush;
                Pen pen;
                Color color;
                Pen shade1;
                Pen shade2;

                // Check the pressed state, and draw accordingly.
                if (_pressed)
                {
                    brush = _pressedBackgroundBrush;
                    pen = _pressedBorderPen;
                    color = _pressedForeColor;
                    shade1 = _darkShade;
                    shade2 = _lightShade;
                }
                else
                {
                    brush = _normalBackgroundBrush;
                    pen = _borderPen;
                    color = _foreColor;
                    shade1 = _lightShade;
                    shade2 = _darkShade;
                }

                GetLayoutOffset(out x, out y);

                // Draw the base rectangle of the button
                dc.DrawRectangle(brush, pen, 1, 1, _width - 1, _height - 1);

                // Draw the caption
                string caption = _caption;
                dc.DrawText(ref caption, _font, color, 0, _textMarginY, _width, 
                    _height, _alignment, _trimming);

                // Shade the outline of the rectangle for classic button look
                dc.DrawLine(shade1, 1, 1, _width - 1, 1);
                dc.DrawLine(shade1, 1, 1, 1, _height - 1);
                dc.DrawLine(shade2, _width - 1, 1, _width - 1, _height - 1);
                dc.DrawLine(shade2, 1, _height - 1, _width - 1, _height - 1);
            }

            public event EventHandler Click;

            /// <summary>
            /// Handles the touch down event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void  OnStylusDown(StylusEventArgs e)
#else
            protected override void OnTouchDown(TouchEventArgs e)
#endif
            {
                // Flag for the drawing state.
                _pressed = true;

#if MF_FRAMEWORK_VERSION_V3_0
                Stylus.Capture(this);
#else
                TouchCapture.Capture(this);
#endif

                // Trigger a redraw.
                Invalidate();
            }

            /// <summary>
            /// Handles the touch up event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void  OnStylusUp(StylusEventArgs e)
#else
            protected override void OnTouchUp(TouchEventArgs e)
#endif
            {
                // Flag for the drawing state.
                _pressed = false;

#if MF_FRAMEWORK_VERSION_V3_0
                Stylus.Capture(this, CaptureMode.None);
#else
                TouchCapture.Capture(this, CaptureMode.None);
#endif

                // Trigger a redraw.
                Invalidate();

                // Fire a click event.
                EventArgs args = new EventArgs();
                OnClick(args);
            }

            /// <summary>
            /// Handles the touch move.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void  OnStylusMove(StylusEventArgs e)
#else
            protected override void OnTouchMove(TouchEventArgs e)
#endif
            {
            }

            /// <summary>
            /// Handles a click event.
            /// </summary>
            /// <param name="e"></param>
            protected virtual void OnClick(EventArgs e)
            {
                if (Click != null)
                {
                    Click(this, e);
                }
            }

            /// <summary>
            /// 
            /// </summary>
            /// <param name="arrangeWidth"></param>
            /// <param name="arrangeHeight"></param>
            protected override void ArrangeOverride(int arrangeWidth, 
                int arrangeHeight)
            {
            }

            /// <summary>
            /// 
            /// </summary>
            /// <param name="availableWidth"></param>
            /// <param name="availableHeight"></param>
            /// <param name="desiredWidth"></param>
            /// <param name="desiredHeight"></param>
            protected override void MeasureOverride(int availableWidth, 
                int availableHeight, out int desiredWidth, 
                out int desiredHeight)
            {
                desiredWidth = (availableWidth > _width) ? 
                _width : availableWidth;
                desiredHeight = (availableHeight > _height) ? 
                _height : availableHeight;
            }

            private SolidColorBrush _normalBackgroundBrush = 
                new SolidColorBrush(ColorUtility.ColorFromRGB(192, 192, 192));
            private SolidColorBrush _pressedBackgroundBrush = 
                new SolidColorBrush(ColorUtility.ColorFromRGB(128, 128, 128));

            private Pen _borderPen = 
                new Pen(ColorUtility.ColorFromRGB(128, 128, 128));

            private Pen _pressedBorderPen = 
                new Pen(ColorUtility.ColorFromRGB(128, 128, 128));

            private Pen _lightShade = 
                new Pen(ColorUtility.ColorFromRGB(216, 216, 216));
            private Pen _darkShade = 
                new Pen(ColorUtility.ColorFromRGB(64, 64, 64));

            int _width;
            int _height;
            string _caption = "";
            Font _font = null;
            Color _foreColor = ColorUtility.ColorFromRGB(0, 0, 0);
            Color _pressedForeColor = ColorUtility.ColorFromRGB(255, 255, 255);
            private TextTrimming _trimming = TextTrimming.WordEllipsis;
            private TextAlignment _alignment = TextAlignment.Center;
            protected int _textMarginX = 16;
            protected int _textMarginY = 8;
            protected bool _pressed = false;
        }

        /// <summary>
        /// A palette class based on the generic palette class defined above.
        /// </summary>
        public class MyColorPalette : PaletteControl
        {
            /// <summary>
            /// Constructs a MyColorPalette with the specified settings.
            /// </summary>
            /// <param name="row"></param>
            /// <param name="column"></param>
            /// <param name="width"></param>
            /// <param name="height"></param>
            public MyColorPalette(int row, int column, int width, int height)
                : base(row, column, width, height)
            {
                color = new Color[16];

                // Create the colors we will display.
                color[0] = ColorUtility.ColorFromRGB(255, 0, 0);
                color[1] = ColorUtility.ColorFromRGB(128, 0, 0);
                color[2] = ColorUtility.ColorFromRGB(64, 0, 0);
                color[3] = ColorUtility.ColorFromRGB(0, 255, 0);
                color[4] = ColorUtility.ColorFromRGB(0, 128, 0);
                color[5] = ColorUtility.ColorFromRGB(0, 64, 0);
                color[6] = ColorUtility.ColorFromRGB(0, 0, 255);
                color[7] = ColorUtility.ColorFromRGB(0, 0, 128);
                color[8] = ColorUtility.ColorFromRGB(0, 0, 64);
                color[9] = ColorUtility.ColorFromRGB(255, 255, 0);
                color[10] = ColorUtility.ColorFromRGB(128, 128, 0);
                color[11] = ColorUtility.ColorFromRGB(0, 255, 255);
                color[12] = ColorUtility.ColorFromRGB(0, 128, 128);
                color[13] = ColorUtility.ColorFromRGB(255, 0, 255);
                color[14] = ColorUtility.ColorFromRGB(128, 0, 128);
                color[15] = ColorUtility.ColorFromRGB(0, 0, 0);

                int i = 0;
                brushes = new Brush[16];

                for (i = 0; i < 16; i++)
                {
                    brushes[i] = new SolidColorBrush(color[i]);
                }
            }

            /// <summary>
            /// Handle the render item event.
            /// </summary>
            /// <param name="dc"></param>
            /// <param name="row"></param>
            /// <param name="column"></param>
            /// <param name="x"></param>
            /// <param name="y"></param>
            /// <param name="width"></param>
            /// <param name="height"></param>
            protected override void OnRenderItem(DrawingContext dc, int row, 
                int column, int x, int y, int width, int height)
            {
                // The base class doesn't do anything, so don't bother calling 
                // it.

                // Calculate the item so we know what color to use.
                int i = column + row * _column;

                // Draw a rectangle in that color at the correct location.
                dc.DrawRectangle(brushes[i], _pressedItem == i ? selectBorderPen : borderPen, x, y, width, height);
            }

            /// <summary>
            /// Helper method to get the color based on an index.
            /// </summary>
            /// <param name="index"></param>
            /// <returns></returns>
            public Color GetColor(int index)
            {
                return color[index];
            }

            protected Brush emptyBrush = new SolidColorBrush(Color.White);
            protected Brush[] brushes = null;
            protected Color[] color = null;
            protected Pen borderPen = new Pen(Color.Black, 1);
            protected Pen selectBorderPen = new Pen(ColorUtility.ColorFromRGB(200, 0, 200), 2);
        }

        /// <summary>
        /// A canvas class based on the .NET Micro Framework InkCanvas class.
        /// </summary>
        public class MyCanvas : InkCanvas
        {
            /// <summary>
            /// Constructs a MyCanvas with the specified settings.
            /// </summary>
            /// <param name="width"></param>
            /// <param name="height"></param>
            public MyCanvas(int width, int height)
                : base(0, 0, width, height)
            {
#if MF_FRAMEWORK_VERSION_V3_0
                _drawingAttribs = new DrawingAttributes();
                _drawingAttribs.Color = ColorUtility.ColorFromRGB(0, 0, 255);
#endif
            }

#if !MF_FRAMEWORK_VERSION_V3_0
            /// <summary>
            /// Stores the original bitmap for use in gesture operations (like rotation, pan, and zoom).
            /// </summary>
            private Bitmap _bmpGesture = null;

            /// <summary>
            /// When the gesture starts, save the current bitmap so that gesture events will 
            /// be performed on the original (to avoid degradation of the image from multiple operations).
            /// </summary>
            /// <param name="e">Touch gesture event data</param>
            protected override void OnTouchGestureStarted(TouchGestureEventArgs e)
            {
                base.OnTouchGestureStarted(e);

                _bmpGesture = _bitmap;
            }

            /// <summary>
            /// Reset the gesture bitmap when the gesture is finished.
            /// </summary>
            /// <param name="e">Touch gesture event data</param>
            protected override void OnTouchGestureEnded(TouchGestureEventArgs e)
            {
                base.OnTouchGestureEnded(e);

                _bmpGesture = null;
            }

            /// <summary>
            /// Handles multi-touch gesture events.  Multitouch gestures are performed on the emulator by
            /// holding down CTRL+[Z|P|R] and inking (to change the angle/zoom level).
            /// </summary>
            /// <param name="e">Touch gesture event data</param>
            protected override void OnTouchGestureChanged(TouchGestureEventArgs e)
            {
                base.OnTouchGestureChanged(e);

                Bitmap b;
                switch (e.Gesture)
                {
                    /// 
                    /// Zoom gesture magnifies the original bitmap by the gesture value contained in e.Arguments
                    /// 
                    case TouchGesture.Zoom:
                        if (_bmpGesture != null)
                        {
                            b = new Bitmap(_bitmap.Width, _bitmap.Height);
                            b.DrawRectangle(Color.Black, 1, 0, 0, b.Width - 2, b.Height - 2, 0, 0, Color.White, 0, 0, Color.White, b.Width, b.Height, Bitmap.OpacityOpaque);
                            b.StretchImage(-(e.Arguments), -(e.Arguments), _bmpGesture, _bmpGesture.Width + 2 * (e.Arguments), _bmpGesture.Height + 2 * (e.Arguments), Bitmap.OpacityOpaque);
                            _bitmap = b;
                            Invalidate();
                        }
                        break;
                    /// 
                    /// Pan (zoom out) gesture shrinks the original bitmap by the gesture value contained in e.Arguments
                    /// 
                    case TouchGesture.Pan:
                        if (_bmpGesture != null)
                        {
                            b = new Bitmap(_bitmap.Width, _bitmap.Height);
                            b.DrawRectangle(Color.Black, 1, 0, 0, b.Width - 2, b.Height - 2, 0, 0, Color.White, 0, 0, Color.White, b.Width, b.Height, Bitmap.OpacityOpaque);
                            b.StretchImage((e.Arguments), (e.Arguments), _bmpGesture, _bmpGesture.Width - 2 * (e.Arguments), _bmpGesture.Height - 2 * (e.Arguments), Bitmap.OpacityOpaque);
                            _bitmap = b;
                            Invalidate();
                        }
                        break;
                    /// 
                    /// Rotate gesture spins the original bitmap by the gesture value contained in e.Angle (0-360).
                    /// 
                    case TouchGesture.Rotate:
                        if (_bmpGesture != null)
                        {
                            b = new Bitmap(_bitmap.Width, _bitmap.Height);
                            b.DrawRectangle(Color.Black, 1, 0, 0, b.Width, b.Height, 0, 0, Color.White, 0, 0, Color.White, b.Width, b.Height, Bitmap.OpacityOpaque);
                            b.RotateImage((int)e.Angle, 1, 1, _bmpGesture, 1, 1, _bmpGesture.Width - 2, _bmpGesture.Height - 2, Bitmap.OpacityOpaque);
                            _bitmap = b;
                            Invalidate();
                        }
                        break;
                }
            }

            /// <summary>
            /// Gets or sets the color.
            /// </summary>
            public Color Color
            {
                get
                {
                    return DefaultDrawingAttributes.Color;
                }

                set
                {
                    DefaultDrawingAttributes.Color = value;
                }
            }
#else
            private DrawingAttributes _drawingAttribs;

            public Color Color
            {
                get
                {
                    return _drawingAttribs.Color;
                }

                set
                {
                    _drawingAttribs.Color = value;
                    DefaultDrawingAttributes = _drawingAttribs;
                }
            }
#endif
        }

        /// <summary>
        /// The main window class, based on the standard Window.
        /// </summary>
        public class MyWindow : Window
        {
            Panel panel = new Panel();
            MyCanvas inkCanvas = null;
            Button button = null;
            MyColorPalette colorPalette = null;

            /// <summary>
            /// The default constructor.
            /// </summary>
            public MyWindow()
            {
                // Create the canvas to draw on.
                int screenWidth;
                int screenHeight;
                int bitsPerPixel;
                int orientation;
                HardwareProvider.HwProvider.GetLCDMetrics(out screenWidth, 
                    out screenHeight, out bitsPerPixel, out orientation);

                inkCanvas = new MyCanvas(screenWidth - 60, screenHeight - 30);

                // Create the Clear button.
                button = new Button("Clear", 
                    Resources.GetFont(Resources.FontResources.small));
                button.Click += new EventHandler(button_Click);

                // Create the palette for selecting colors.
                colorPalette = new MyColorPalette(8, 2, 54, 204);
                colorPalette.HorizontalAlignment = HorizontalAlignment.Right;
                colorPalette.VerticalAlignment = VerticalAlignment.Top;

                colorPalette.ItemClick += 
                    new PaletteControlEventHandler(colorPalette_ItemClick);

                // Add a panel to hold the other controls.
                this.Child = panel;

                // Add the canvas, clear button and palette to the panel.
                panel.Children.Add(inkCanvas);
                panel.Children.Add(button);
                panel.Children.Add(colorPalette);

                // Set the drawing attributes to a default set.
                DrawingAttributes da = new DrawingAttributes();
                inkCanvas.DefaultDrawingAttributes = da;
            }

            /// <summary>
            /// Handles the color palette item click event.
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
            void colorPalette_ItemClick(object sender, PaletteEventArg e)
            {
                inkCanvas.Color = colorPalette.GetColor(e.Item);
            }

            /// <summary>
            /// Handles the button click event.
            /// </summary>
            /// <param name="sender"></param>
            /// <param name="e"></param>
            void button_Click(object sender, EventArgs e)
            {
                inkCanvas.Clear();
                Invalidate();
            }
        }

        static MyInkCanvas myApplication;

        /// <summary>
        /// The executable entry point.
        /// </summary>
        public static void Main()
        {
            myApplication = new MyInkCanvas();

            // Touch notifications are not turned on by default.  You need to 
            // explicitly inform the Touch engine that you want touch events to 
            // be pumped to your direction, and you want to work with rest of 
            // the architecture.

            Microsoft.SPOT.Touch.Touch.Initialize(myApplication);

            TouchCollectorConfiguration.CollectionMethod = 
                CollectionMethod.Native;
            TouchCollectorConfiguration.CollectionMode = CollectionMode.InkAndGesture;
            
            Window mainWindow = myApplication.CreateWindow();

            // Start the application.
            myApplication.Run(mainWindow);
        }

        private MyWindow mainWindow;

        /// <summary>
        /// Creates the window.
        /// </summary>
        /// <returns></returns>
        public Window CreateWindow()
        {
            // Create a window object and set its size to the
            // size of the display.
            mainWindow = new MyWindow();
            mainWindow.Height = SystemMetrics.ScreenHeight;
            mainWindow.Width = SystemMetrics.ScreenWidth;

            // Set the window visibility to visible.
            mainWindow.Visibility = Visibility.Visible;

            // Attach the button focus to the window.
            Buttons.Focus(mainWindow);

            return mainWindow;
        }
    }
}
