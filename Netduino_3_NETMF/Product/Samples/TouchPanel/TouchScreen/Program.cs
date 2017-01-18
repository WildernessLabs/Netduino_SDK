////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Touch;

namespace TouchScreenSample
{
    /// <summary>
    /// Simple touch input example for a touch screen.
    /// </summary>
    public class MyTouchScreen : TouchScreen
    {
        internal MyTouchScreen(TouchScreen.ActiveRectangle[] ars, Bitmap bmp) : base(ars)
        {
            _systemBmp = bmp;

            // the layout is a screen with 4 squares at the corners in the form of four 50x50 bitmaps
            // we assume that the screen width and height can contain the four bitmaps plus a 5 pixels
            // border around them
            // only the bitmaps will be 'clickable'

            int centerX = bmp.Width / 2;
            int centerY = bmp.Height / 2;
            _ar = new TouchScreen.ActiveRectangle[4];
            // north/east
            _ar[0] = new TouchScreen.ActiveRectangle(centerX - 55, centerY - 55, 50, 50, null);
            // north/west
            _ar[1] = new TouchScreen.ActiveRectangle(centerX + 5, centerY - 55, 50, 50, null);
            // south/west
            _ar[2] = new TouchScreen.ActiveRectangle(centerX + 5, centerY + 5, 50, 50, null); 
            // south/east
            _ar[3] = new TouchScreen.ActiveRectangle(centerX - 55, centerY + 5, 50, 50, null);
        }

        /// <summary>
        /// Execution entry point.
        /// </summary>
        public static void Main()
        {
            int width, height, bpp, orientation;
            Microsoft.SPOT.Hardware.HardwareProvider hwProvider = Microsoft.SPOT.Hardware.HardwareProvider.HwProvider;
            hwProvider.GetLCDMetrics(out width, out height, out bpp, out orientation);


            // allocate one bitmap to be used as drawing canvas
            MyTouchScreen myTouchScreen = new MyTouchScreen(new TouchScreen.ActiveRectangle[] { new TouchScreen.ActiveRectangle(10, 20, 100, 100, null) }, new Bitmap(width, height));

            // Touch notifications are not turned on by default.  You need to 
            // explicitly inform the Touch engine that you want touch events to 
            // be pumped to your direction, and you want to work with the rest 
            // of the architecture.
            Microsoft.SPOT.Touch.Touch.Initialize(myTouchScreen);

            // Start the application.
            myTouchScreen.Run(myTouchScreen);
        }

        /// <summary>
        /// Handler for the touch up event
        /// </summary>
        /// <param name="sender">The sender for this touch event</param>
        /// <param name="e">The event arguments</param>
        private void TouchUpHandler(object sender, TouchScreenEventArgs e)
        {
            TouchInput ti = e.Touches[0];

            string fire = "Fire was pressed";
            string snowflake = "Snowflake was pressed";
            string text;

            switch(_currentScreenId)
            {
                case ScreenId.NorthEast:
                    text = _ar[0].Contains(ti) ? fire : snowflake;
                    break;
                case ScreenId.NorthWest:
                    text = _ar[1].Contains(ti) ? fire : snowflake;
                    break;
                case ScreenId.SouthWest:
                    text = _ar[2].Contains(ti) ? fire : snowflake;
                    break;
                case ScreenId.SouthEast:
                    text = _ar[3].Contains(ti) ? fire : snowflake;
                    break;
                default:
                    throw new InvalidOperationException("This is not expected!");
            }
            Debug.Print(text);
        }

        /// <summary>
        /// The handler for the gesture changed event
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GestureChangedHandler(object sender, TouchGestureEventArgs e)
        {
            int id = (int)_currentScreenId;  id = (id + 1) % (int)ScreenId.Last;

            switch(e.Gesture)
            {
                case TouchGesture.Right:
                    Show((ScreenId)id, false);
                    break;
                case TouchGesture.Left:
                    Show((ScreenId)id, false);
                    break;
                default:
                    break;
            }
        }

        /// <summary>
        /// Enumeration to identify the image to display
        /// </summary>
        enum ScreenId
        {
            NorthEast = 0,
            NorthWest,
            SouthWest,
            SouthEast,
            Last,
        }

        /// <summary>
        /// Execute the sample code.
        /// </summary>
        private void Run(TouchScreen touchScreen)
        {
            touchScreen.OnTouchUp += new TouchScreenEventHandler(TouchUpHandler);

            touchScreen.OnGestureChanged += new TouchGestureEventHandler(GestureChangedHandler);

            Show(ScreenId.NorthEast, false);

            Thread.Sleep(Timeout.Infinite);
        }

        /// <summary>
        /// Displays the selected image
        /// </summary>
        /// <param name="sid">Identifier for the image combination to display</param>
        /// <param name="slide">Boolean parameter to suggest that the transition should be animated</param>
        private void Show(ScreenId sid, bool slide) 
        {
            Bitmap bmp = PrepareScreen(sid);
            if (slide)
            {
            }
            else
            {
                bmp.Flush();
            }

            _currentScreenId = sid;
        }

        /// <summary>
        /// Prepares the selected image combination by collating several bitmaps
        /// </summary>
        /// <param name="sid">Identifier for the image combination to display</param>
        /// <returns>The image combination to display</returns>
        private Bitmap PrepareScreen(ScreenId sid)
        {
            _systemBmp.Clear();

            switch(sid)
            {
                case ScreenId.NorthEast:
                {
                    // put fire in the north/east corner and snowflake in the south/west 
                    _systemBmp.DrawImage(_ar[0].X, _ar[0].Y, Resources.GetBitmap(Resources.BitmapResources.fire)     , 0, 0, 50, 50);
                    _systemBmp.DrawImage(_ar[2].X, _ar[2].Y, Resources.GetBitmap(Resources.BitmapResources.snowflake), 0, 0, 50, 50);
                    this.ActiveRegions = new TouchScreen.ActiveRectangle[] { _ar[0], _ar[2] };
                }
                break;
                case ScreenId.NorthWest:
                {
                    // put bit fire in the north/west and snowflake south/east corners
                    _systemBmp.DrawImage(_ar[1].X, _ar[1].Y, Resources.GetBitmap(Resources.BitmapResources.fire)      , 0, 0, 50, 50);
                    _systemBmp.DrawImage(_ar[3].X, _ar[3].Y, Resources.GetBitmap(Resources.BitmapResources.snowflake), 0, 0, 50, 50);
                    this.ActiveRegions = new TouchScreen.ActiveRectangle[] { _ar[1], _ar[3] };
                }
                break;
                case ScreenId.SouthWest:
                {
                    // put bit snowflake in the north/east and fire south/west corners
                    _systemBmp.DrawImage(_ar[0].X, _ar[0].Y, Resources.GetBitmap(Resources.BitmapResources.snowflake), 0, 0, 50, 50);
                    _systemBmp.DrawImage(_ar[2].X, _ar[2].Y, Resources.GetBitmap(Resources.BitmapResources.fire)      , 0, 0, 50, 50);                    
                    this.ActiveRegions = new TouchScreen.ActiveRectangle[] { _ar[0], _ar[2] };
                }
                break;
                case ScreenId.SouthEast:
                {
                    // put bit snowflake in the north/west and fire south/east corners
                    _systemBmp.DrawImage(_ar[1].X, _ar[1].Y, Resources.GetBitmap(Resources.BitmapResources.snowflake), 0, 0, 50, 50);
                    _systemBmp.DrawImage(_ar[3].X, _ar[3].Y, Resources.GetBitmap(Resources.BitmapResources.fire), 0, 0, 50, 50);
                    this.ActiveRegions = new TouchScreen.ActiveRectangle[] { _ar[1], _ar[3] };
                }
                break;
                default:
                    throw new ArgumentException();
            }

            return _systemBmp;
        }

        //--//

        Bitmap _systemBmp;
        ScreenId _currentScreenId;
        TouchScreen.ActiveRectangle[] _ar;
    }
}
