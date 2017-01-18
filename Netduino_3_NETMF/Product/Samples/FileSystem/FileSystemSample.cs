////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Collections;
using System.IO;
using Microsoft.SPOT;
using Microsoft.SPOT.Input;
using Microsoft.SPOT.IO;
using Microsoft.SPOT.Presentation;
using Microsoft.SPOT.Presentation.Controls;
using Microsoft.SPOT.Presentation.Media;
using Microsoft.SPOT.Touch;

namespace FileSystemSample
{
    /// <summary>
    /// Demonstrates file system support.
    /// </summary>
    public class MyFileSystem : Microsoft.SPOT.Application
    {
        /// <summary>
        /// Holds a sub-item for a list view. 
        /// </summary>
        public class ListViewSubItem
        {
            public string Text;
        }

        /// <summary>
        /// Defines the list view item.
        /// </summary>
        public class ListViewItem
        {
            // Initialize the array of sub-items.
            public ArrayList SubItems = new ArrayList();

            /// <summary>
            /// Adds a sub-item to the list view item. 
            /// </summary>
            /// <param name="itemText">The text of the sub-item.</param>
            public void AddSubItem(string itemText)
            {
                ListViewSubItem listViewSubItem = new ListViewSubItem();
                listViewSubItem.Text = itemText;

                SubItems.Add(listViewSubItem);
            }
        }

        /// <summary>
        /// Holds information about a column in the list view.
        /// </summary>
        public class ListViewColumn
        {
            public string Name;
            public int Width;
        }

        /// <summary>
        /// Defines the list view object.
        /// </summary>
        public class ListView : UIElement
        {
            // Initialize the members.
            protected int _width;
            protected int _height;
            protected Brush _emptyBrush = new SolidColorBrush(Color.White);
            protected Brush _grayBrush =
                new SolidColorBrush(ColorUtility.ColorFromRGB(160, 160, 160));
            protected Brush _lightBrush =
                new SolidColorBrush(ColorUtility.ColorFromRGB(216, 216, 216));
            protected Brush _cyanBrush =
                new SolidColorBrush(ColorUtility.ColorFromRGB(192, 192, 255));
            protected Brush _lightCyanBrush =
                new SolidColorBrush(ColorUtility.ColorFromRGB(224, 224, 255));
            protected Pen _borderPen = new Pen(Color.Black, 1);
            protected Pen _cyanPen =
                new Pen(ColorUtility.ColorFromRGB(192, 192, 255), 1);
            protected Pen _darkCyanPen =
                new Pen(ColorUtility.ColorFromRGB(128, 128, 192), 1);
            protected Pen _emptyPen =
                new Pen(ColorUtility.ColorFromRGB(255, 255, 255), 1);
            protected Pen _grayPen =
                new Pen(ColorUtility.ColorFromRGB(192, 192, 192), 1);
            protected Font _font;
            protected int totalColumnWidth;
            protected int cy1;
            protected int cy2;
            protected int cy3;
            protected int cx1;
            protected int cx2;
            protected int cx3;
            protected int cx4;
            protected int cx5;

            protected ListViewItem _selectedItem = null;

            protected int _columnHeaderHeight = 40;
            protected int _itemHeight = 20;
            protected int _scrollbarDimension = 10;

            protected int _verticalSliderY = 0;
            protected int _verticalSliderHeight = 0;

            protected int _horizontalSliderX = 0;
            protected int _horizontalSliderWidth = 0;

            protected int _verticalScrollViewHeight = 0;
            protected int _horizontalScrollViewWidth = 0;

            protected int _totalViewHeight = 0;
            protected int _totalViewWidth = 0;

            protected int _sx = 0;
            protected int _sy = 0;

            public ArrayList Columns = new ArrayList();
            public ArrayList Items = new ArrayList();

            /// <summary>
            /// Defines a vertical scroll bar.
            /// </summary>
            protected class VerticalScrollBar
            {
                public int Width;
                public int Height;
            }

            /// <summary>
            /// Constructs a ListView, of the specified width and height.
            /// </summary>
            /// <param name="width">The width of the ListView.</param>
            /// <param name="height">The height of the ListView.</param>
            public ListView(int width, int height)
            {
                _width = width;
                _height = height;
                _font = Resources.GetFont(Resources.FontResources.small);
            }

            /// <summary>
            /// Renders the list view.
            /// </summary>
            /// <param name="dc">The drawing context of the list view.</param>
            public override void OnRender(DrawingContext dc)
            {
                int x;
                int y;

                // Get the x and y offsets.
                GetLayoutOffset(out x, out y);

                // Draw a border around the list view.
                dc.DrawRectangle(_emptyBrush, _borderPen, x, y, _width, _height);

                // Render the buttons.
                RenderButtons(dc, x, y);

                // Render the columns.
                cy1 = _columnHeaderHeight - 20;
                cy2 = _columnHeaderHeight;
                RenderColumns(dc, x, y + cy1, 20);

                // Render all the items.
                RenderItems(dc, x, y, _columnHeaderHeight, _itemHeight);

                // Render the vertical scroll bar.
                RenderVerticalScrollBar(dc, x, y + _columnHeaderHeight,
                    _scrollbarDimension, _itemHeight);

                // Calculate the positions of the different areas of the screen.
                cy3 = _height - _scrollbarDimension;
                cx1 = (_width - _scrollbarDimension) / 5;
                cx2 = cx1 * 2;
                cx3 = cx1 * 3;
                cx4 = cx1 * 4;
                cx5 = _width - _scrollbarDimension;

                // Render the horizontal scroll bar.
                RenderHorizontalScrollBar(dc, x, y, _scrollbarDimension);
            }

            /// <summary>
            /// Adds a column to the list view.
            /// </summary>
            /// <param name="name">The name of the column.</param>
            /// <param name="width">The width of the column.</param>
            public void AddColumn(string name, int width)
            {
                ListViewColumn column = new ListViewColumn();
                column.Name = name;
                column.Width = width;

                Columns.Add(column);
            }

            /// <summary>
            /// Adds an item to the list view, given a full list view item.
            /// </summary>
            /// <param name="listViewItem">The item to add to the list view.
            /// </param>
            public void AddItem(ListViewItem listViewItem)
            {
                Items.Add(listViewItem);
            }

            /// <summary>
            /// Adds an item to the list view, given text for the first column.
            /// </summary>
            /// <param name="itemText"></param>
            public void AddItem(string itemText)
            {
                ListViewItem listViewItem = new ListViewItem();
                listViewItem.AddSubItem(itemText);

                AddItem(listViewItem);
            }

            /// <summary>
            /// Adds an item to the list view, given text for the first and 
            /// second columns.
            /// </summary>
            /// <param name="text1"></param>
            /// <param name="text2"></param>
            public void AddItem(string text1, string text2)
            {
                ListViewItem listViewItem = new ListViewItem();
                listViewItem.AddSubItem(text1);
                listViewItem.AddSubItem(text2);

                AddItem(listViewItem);
            }

            /// <summary>
            /// Adds an item to the list view, given text for all 3 columns.
            /// </summary>
            /// <param name="text1">The text for the first column.</param>
            /// <param name="text2">The text for the second column.</param>
            /// <param name="text3">The text for the third column.</param>
            public void AddItem(string text1, string text2, string text3)
            {
                ListViewItem listViewItem = new ListViewItem();
                listViewItem.AddSubItem(text1);
                listViewItem.AddSubItem(text2);
                listViewItem.AddSubItem(text3);

                AddItem(listViewItem);
            }

            /// <summary>
            /// Renders the buttons.
            /// </summary>
            /// <param name="dc">The drawing context.</param>
            /// <param name="x">The width of a button.</param>
            /// <param name="y">The height of a button.</param>
            protected virtual void RenderButtons(DrawingContext dc, int x,
                int y)
            {

                // Calculate the width of a single button.
                int buttonWidth = _width / 5;

                // Render the New File button.
                dc.DrawRectangle(_cyanBrush, _darkCyanPen, x, y, buttonWidth,
                    20);
                string sNewFile = "New File";
                dc.DrawText(ref sNewFile, _font, Color.Black, x, y + 2,
                    buttonWidth, 16, TextAlignment.Center,
                    TextTrimming.None);

                // Render the New Directory button.
                dc.DrawRectangle(_cyanBrush, _darkCyanPen, x + buttonWidth, y,
                    buttonWidth, 20);
                string sNewDir = "New Dir";
                dc.DrawText(ref sNewDir, _font, Color.Black, x + buttonWidth,
                    y + 2, buttonWidth, 16,
                    TextAlignment.Center, TextTrimming.None);

                // Render the Move button.
                dc.DrawRectangle(_cyanBrush, _darkCyanPen, x + (buttonWidth * 2),
                    y, buttonWidth, 20);
                string sMove = "Move";
                dc.DrawText(ref sMove, _font, Color.Black, x + (buttonWidth * 2),
                    y + 2, buttonWidth, 16,
                    TextAlignment.Center, TextTrimming.None);

                // Render the Delete button.
                dc.DrawRectangle(_cyanBrush, _darkCyanPen, x + (buttonWidth * 3),
                    y, buttonWidth, 20);
                string sDelete = "Delete";
                dc.DrawText(ref sDelete, _font, Color.Black,
                    x + (buttonWidth * 3), y + 2, buttonWidth, 16,
                    TextAlignment.Center, TextTrimming.None);

                // Render the Format button.
                dc.DrawRectangle(_cyanBrush, _darkCyanPen, x + (buttonWidth * 4),
                    y, buttonWidth, 20);
                string sFormat = "Format";
                dc.DrawText(ref sFormat, _font,
                    Color.Black, x + (buttonWidth * 4), y + 2, buttonWidth, 16,
                    TextAlignment.Center, TextTrimming.None);
            }

            /// <summary>
            /// Renders the column titles of the list view.
            /// </summary>
            /// <param name="dc"></param>
            /// <param name="x"></param>
            /// <param name="y"></param>
            /// <param name="headerHeight"></param>
            protected virtual void RenderColumns(DrawingContext dc, int x,
                int y, int headerHeight)
            {

                // Draw the header background.
                dc.DrawRectangle(_cyanBrush, _darkCyanPen, x, y + 2, _width,
                    headerHeight);

                // Iterate through the columns.
                int cx = 5 - _sx;
                for (int i = 0; i < Columns.Count; i++)
                {
                    // Draw the column text.
                    dc.DrawText(((ListViewColumn)Columns[i]).Name, _font, Color.Black, cx, y + 2);

                    // Increment by the column width.
                    cx += ((ListViewColumn)Columns[i]).Width;
                }

                // Store the total column width.
                totalColumnWidth = cx;
            }

            /// <summary>
            /// Renders the items in the list view.
            /// </summary>
            /// <param name="dc"></param>
            /// <param name="x"></param>
            /// <param name="y"></param>
            /// <param name="headerHeight"></param>
            /// <param name="itemHeight"></param>
            protected virtual void RenderItems(DrawingContext dc, int x, int y,
                int headerHeight, int itemHeight)
            {

                // Create the brushes and pens for drawing the items.
                Brush[] brushes = new Brush[] { _emptyBrush, _lightCyanBrush, 
                    _grayBrush };
                Pen[] pens = new Pen[] { _emptyPen, _cyanPen };

                // Set the starting location for the first item.  Offset the 
                // location by _sx and _sy, which are the scroll positions.
                int cx = x + 6 - _sx;
                int cy = y + headerHeight + 2 - _sy;
                int j = 0;

                // Iterate through all the items.
                for (int ii = 0; ii < Items.Count; ii++)
                {
                    // Only draw the items that are visible.
                    if (cy >= _columnHeaderHeight)
                    {

                        // If the item is selected...
                        if (((ListViewItem)Items[ii]) == _selectedItem)
                        {
                            // Draw a rectangle alternately shaded and 
                            // transparent.
                            dc.DrawRectangle(brushes[2], pens[j & 1], x + 2, cy,
                            _width - 4, _itemHeight);
                        }
                        else
                        {
                            // Draw a rectangle alternately shaded and 
                            // transparent.
                            dc.DrawRectangle(brushes[j & 1], pens[j & 1], x + 2,
                                cy, _width - 4, _itemHeight);
                        }

                        // Set the starting location for the first column.
                        // Offset the location by _sx, which is the scroll 
                        // position.
                        cx = 4 - _sx;
                        int i = 0;
                        int newcx = 0;

                        // Iterate through the sub-items, which are columns.
                        for (int e = 0; e < ((ListViewItem)Items[ii]).SubItems.Count; e++)
                        {
                            // Calculate the x position of the next column.
                            newcx = cx + ((ListViewColumn)Columns[i]).Width;

                            // Draw only the items that will appear on the 
                            // screen.
                            if (newcx > 0)
                            {
                                dc.DrawText(((ListViewSubItem)((ListViewItem)Items[ii]).SubItems[e]).Text, _font, Color.Black,
                                    cx, cy + 2);
                            }

                            // Move to the next column.
                            cx = newcx;
                            i++;

                            // If the location is past the right side of the 
                            // screen, don't show any more columns.
                            if (cx > _width)
                                break;
                        }
                    }

                    // Increment to the next item.
                    cy += itemHeight;
                    j++;

                    // If the location is past the bottom of the screen, don't 
                    // show any more items.
                    if (cy > _height)
                        break;
                }
            }

            /// <summary>
            /// Renders the vertical scroll bar.
            /// </summary>
            /// <param name="dc"></param>
            /// <param name="x"></param>
            /// <param name="y"></param>
            /// <param name="scrollWidth"></param>
            /// <param name="itemHeight"></param>
            protected void RenderVerticalScrollBar(DrawingContext dc, int x,
                int y, int scrollWidth, int itemHeight)
            {
                try
                {
                    // Calculate and store the total view height.
                    _totalViewHeight = Items.Count * itemHeight;

                    // If there is no height, don't go any further.
                    if (_totalViewHeight == 0)
                        return;

                    // Calculate and store the vertical scroll information.
                    _verticalScrollViewHeight = _height - scrollWidth - y;
                    _verticalSliderHeight = (_verticalScrollViewHeight *
                        _verticalScrollViewHeight) / _totalViewHeight;
                    _verticalSliderY = (_sy * _verticalScrollViewHeight) /
                        _totalViewHeight;

                    // Draw the vertical scrollbar background.
                    int sx = x + _width - scrollWidth;
                    dc.DrawRectangle(_lightBrush, _grayPen, sx, y, scrollWidth,
                        _verticalScrollViewHeight);

                    // Draw the scrollbar thumb.
                    if (_verticalSliderHeight < _height)
                    {
                        dc.DrawRectangle(_grayBrush, _emptyPen, sx + 1, y + 1 +
                            _verticalSliderY, scrollWidth - 2,
                            _verticalSliderHeight);
                    }
                }
                catch (Exception ex)
                {
                    Debug.Print(ex.ToString());
                }
            }

            /// <summary>
            /// Renders the horizontal scroll bar. 
            /// </summary>
            /// <param name="dc">The drawing context.</param>
            /// <param name="x">The X coordinate.</param>
            /// <param name="y">The Y coordinate.</param>
            /// <param name="scrollHeight">The height of the scroll bar.</param>
            protected void RenderHorizontalScrollBar(DrawingContext dc, int x,
                int y, int scrollHeight)
            {

                // Calculate and store the total width of the view.
                _horizontalScrollViewWidth = _width;
                _totalViewWidth = totalColumnWidth;

                // If there is no width, don't go any further.
                if (_totalViewWidth == 0)
                    return;

                // Calculate and store the horizontal scroll information.
                _horizontalSliderWidth = (_horizontalScrollViewWidth *
                    _horizontalScrollViewWidth) / _totalViewWidth;
                if (_horizontalSliderWidth > _width)
                    _horizontalSliderWidth = _width;
                _horizontalSliderX = (_sx * _horizontalScrollViewWidth) /
                    _totalViewWidth;

                // Draw the horizontal scrollbar background.
                int sy = y + _height - scrollHeight;
                dc.DrawRectangle(_lightBrush, _grayPen, x, sy,
                    _horizontalScrollViewWidth, scrollHeight);

                // Draw the scrollbar thumb.
                dc.DrawRectangle(_grayBrush, _emptyPen, x + _horizontalSliderX,
                    sy + 1, _horizontalSliderWidth,
                    scrollHeight - 2);
            }

            /// <summary>
            /// Overrides the default measurements and sets them to the desired 
            /// width and height.
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
            /// Handles the OnTouchDown event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void  OnStylusDown(StylusEventArgs e)
#else
            protected override void OnTouchDown(TouchEventArgs e)
#endif
            {

                int x = 0;
                int y = 0;

#if MF_FRAMEWORK_VERSION_V3_0
                x = e.X;
                y = e.Y;
#else
                e.GetPosition(this, 0, out x, out y);
#endif

                try
                {
                    // Figure out which section of the screen was clicked.
                    if (y <= cy1)
                    {
                        // The button area was clicked.

                        if (x <= cx1)
                        {
                            // The New File button was clicked.

                            if (Directory.GetCurrentDirectory() == "\\")
                            {
                                Debug.Print("Cannot create a file at \\");
                                return;
                            }

                            // Get the next file name, by looping until the file 
                            // doesn't exist.
                            int index = 0;
                            string name = "File_0.txt";
                            while (File.Exists(name))
                                name = "File_" + (++index).ToString() + ".txt";

                            // Create the file using the standard .NET FileStream.
                            FileStream file = new FileStream(name, FileMode.Create);

                            // Write some dummy data to the file.
                            for (int i = 0; i < (index + 5) * 2; i++)
                                file.WriteByte((byte)i);

                            // Close the file.
                            file.Close();

                            // Refresh the list and invalidate.
                            myApplication.mainWindow.RefreshList();
                            Invalidate();
                        }
                        else if (x <= cx2)
                        {
                            // The New Directory button was clicked.

                            if (Directory.GetCurrentDirectory() == "\\")
                            {
                                Debug.Print("Cannot create a directory at \\");
                                return;
                            }

                            // Get the next directory name, by looping until the 
                            // directory doesn't exist.
                            int index = 0;
                            string name = "Directory_0";
                            while (Directory.Exists(name))
                                name = "Directory_" + (++index).ToString();

                            // Create the directory.
                            Directory.CreateDirectory(name);

                            // Refresh the list, then re-draw the list.
                            myApplication.mainWindow.RefreshList();
                            Invalidate();
                        }
                        else if (x <= cx3)
                        {
                            // The Move button was clicked.

                            if (Directory.GetCurrentDirectory() == "\\")
                            {
                                Debug.Print("Cannot move to or from \\");
                                return;
                            }

                            // If an item is selected, "move" it.
                            if (_selectedItem != null)
                            {

                                // Get the sub-item that has the name.
                                ListViewSubItem subItem =
                                    (ListViewSubItem)_selectedItem.SubItems[0];
                                if (subItem != null)
                                {

                                    // If the name starts with [ and ends with ] 
                                    // then it is a directory.  This is only because 
                                    // we put the [ and ] on our directory names.
                                    // There is nothing in the file system that 
                                    // requires the [ and ].
                                    if (subItem.Text[0] == '[' &&
                                        subItem.Text[subItem.Text.Length - 1] == ']')
                                    {
                                        // Remove the [ and ] characters.
                                        string name = subItem.Text.Substring(1,
                                            subItem.Text.Length - 2);

                                        // Make sure the directory exists.
                                        if (Directory.Exists(name))
                                        {

                                            // Move the directory to the same name + 
                                            // .moved.
                                            Directory.Move(name, name + ".moved");

                                            // Update the local name variable.
                                            name += ".moved";

                                            // Update the name text.
                                            ((ListViewSubItem)_selectedItem.SubItems[0]).Text =
                                                '[' + name + ']';

                                            // Get the index in Items of the 
                                            // selected list view item.
                                            int index = Items.IndexOf(_selectedItem);

                                            // Remove the item, and then add the 
                                            // item back in.
                                            Items.Remove(_selectedItem);
                                            Items.Insert(index, _selectedItem);

                                            // Re-draw the list.
                                            Invalidate();
                                        }
                                    }
                                    else if (File.Exists(subItem.Text))
                                    {
                                        // Without the [ and ] it is a file.

                                        // Move the file to the same name + .moved.
                                        File.Move(subItem.Text, subItem.Text +
                                            ".moved");

                                        // Update the subitem text.
                                        subItem.Text += ".moved";

                                        // Get the index in Items of the selected 
                                        // list view item.
                                        int index = Items.IndexOf(_selectedItem);

                                        // Remove the item, then add the item back 
                                        // in.
                                        Items.Remove(_selectedItem);
                                        Items.Insert(index, _selectedItem);

                                        // Re-draw the list.
                                        Invalidate();
                                    }
                                }
                            }
                        }
                        else if (x <= cx4)
                        {
                            // The Delete button was clicked.

                            if (Directory.GetCurrentDirectory() == "\\")
                            {
                                Debug.Print("Cannot delete from \\");
                                return;
                            }

                            // If an item is selected, delete the item.
                            if (_selectedItem != null)
                            {

                                // Get the sub-item that has the name.
                                ListViewSubItem subItem =
                                    (ListViewSubItem)_selectedItem.SubItems[0];
                                if (subItem != null)
                                {

                                    // If the name starts with [ and ends with ], 
                                    // then it is a directory.  This is only because 
                                    // we put the [ and ] on our directory names.
                                    // There is nothing in the file system that 
                                    // requires the [ and ].
                                    if (subItem.Text[0] == '[' &&
                                        subItem.Text[subItem.Text.Length - 1] == ']')
                                    {
                                        // Remove the [ and ].
                                        string name = subItem.Text.Substring(1,
                                            subItem.Text.Length - 2);

                                        // Make sure the directory exists.
                                        if (Directory.Exists(name))
                                        {

                                            // Delete the directory.
                                            Directory.Delete(name);

                                            // Remove it from the list view.
                                            Items.Remove(_selectedItem);

                                            // Reset the selected item member.
                                            _selectedItem = null;
                                        }
                                    }
                                    else if (File.Exists(subItem.Text))
                                    {
                                        // Without the [ and ], it is a file.

                                        // Delete the file.
                                        File.Delete(subItem.Text);

                                        // Remove it from the list view.
                                        Items.Remove(_selectedItem);

                                        // Reset the selected item member.
                                        _selectedItem = null;
                                    }

                                    // Re-draw the list view.
                                    Invalidate();
                                }
                            }
                        }
                        else if (x <= cx5)
                        {
                            // The Format button was clicked.

                            // Always go back to the root directory before 
                            // formatting.
                            if(_selectedItem != null)
                            {
                                Directory.SetCurrentDirectory( "\\" );

                                string volume = ( (ListViewSubItem)_selectedItem.SubItems[0] ).Text;

                                volume = volume.Trim( '[', ']', '\\' );

                                // Format the volume and call it ROOT.
                                Microsoft.SPOT.IO.VolumeInfo volInfo =
                                    new VolumeInfo( volume );

                                if(volInfo.FileSystem == null)
                                {
#if MF_FRAMEWORK_VERSION_V3_0 || MF_FRAMEWORK_VERSION_V4_0 || MF_FRAMEWORK_VERSION_V4_1
                                    volInfo.Format( "FAT", 0, true );
#else
                                    volInfo.Format( "FAT", 0, volume + "FS", true );
#endif
                                }
                                else
                                {
                                    volInfo.Format( 0 );
                                }

                                // Refresh the list, then re-draw the list.
                                myApplication.mainWindow.RefreshList();
                                Invalidate();
                            }
                        }
                    }
                    else if (y <= cy2)
                    {
                        // Column.
                    }
                    else if (y >= cy3)
                    {
                        // The horizontal scrollbar was clicked.
                        OnHorizontalScrollStylusDown(x);
                    }
                    else
                    {
                        if (x >= cx5)
                        {
                            // Vertical Scroll
                            OnVerticalScrollStylusDown(y);
                        }
                        else
                        {
                            // Main section.

                            // Calculate which item was clicked.
                            int itemNumber = ((y - _columnHeaderHeight) + _sy) /
                                _itemHeight;

                            // If an item was clicked...
                            if (itemNumber >= 0 && itemNumber < Items.Count)
                            {

                                // See if this item is already selected.
                                if (_selectedItem == (ListViewItem)Items[itemNumber])
                                {
                                    // See if this is a directory.
                                    if (_selectedItem.SubItems.Count > 0)
                                    {
                                        string directoryName =
                                            ((ListViewSubItem)_selectedItem.SubItems[0]).Text;
                                        directoryName = directoryName.Substring(1,
                                            directoryName.Length - 2);

                                        // Check for special ".." name
                                        if (directoryName == "..")
                                        {
                                            directoryName =
                                                Directory.GetCurrentDirectory();
                                            directoryName =
                                                Path.GetDirectoryName(directoryName);
                                            // directoryName.Substring(0, directoryName.LastIndexOf('\\'));
                                        }

                                        // If the directory exists...
                                        if (Directory.Exists(directoryName))
                                        {

                                            // Set the current directory.
                                            Directory.SetCurrentDirectory(
                                                directoryName);

                                            // Refresh the list.
                                            myApplication.mainWindow.RefreshList();
                                        }
                                    }
                                }
                                else
                                    // No item is selected, so select this one.
                                    _selectedItem = (ListViewItem)Items[itemNumber];
                            }
                            else
                                // No item is selected and we didn't click on one 
                                // either.
                                _selectedItem = null;

                            // Refresh the list view
                            Invalidate();
                        }
                    }
                }
                catch (IOException ex) { Debug.Print(ex.ToString()); }
            }

            /// <summary>
            /// Handles the OnTouchMove event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void  OnStylusMove(StylusEventArgs e)
#else
            protected override void OnTouchMove(TouchEventArgs e)
#endif
            {

                int x = 0;
                int y = 0;

#if MF_FRAMEWORK_VERSION_V3_0
                x = e.X;
                y = e.Y;
#else
                e.GetPosition(this, 0, out x, out y);
#endif

                // If a slider was tapped...
                if (_moveVerticalSlider)
                {
                    // The vertical slider was clicked.

                    // Calculate the difference between the old position and the 
                    // new position.
                    int diff = y - _verticalSliderMoveY;

                    // Calculate the view difference.
                    int realDiff = (diff * _totalViewHeight) /
                        _verticalScrollViewHeight;

                    // Set the new position of the slider.
                    _verticalSliderMoveY = y;

                    // Validate the new position and bound-align it.
                    if (_verticalSliderMoveY < _verticalSliderY + _columnHeaderHeight)
                        _verticalSliderMoveY = _verticalSliderY + _columnHeaderHeight;

                    // Update the view scroll position.
                    ScrollVertical(realDiff);
                }
                else if (_moveHorizontalSlider)
                {
                    // The horizontal slider was clicked.

                    // Calculate the difference between the old position and the 
                    // new position.
                    int diff = x - _horizontalSliderMoveX;

                    // Validate and bound-align the difference.
                    if (x >= _horizontalSliderMoveX &&
                        _horizontalSliderX + _horizontalSliderWidth + diff >=
                        _width + _scrollbarDimension)
                    {
                        diff -= (_width + _scrollbarDimension) -
                            (_horizontalSliderX + _horizontalSliderWidth + diff);
                    }

                    // Validate the new position.
                    if (_horizontalSliderX + _horizontalSliderWidth + diff <
                        _width + _scrollbarDimension ||
                        x < _horizontalSliderMoveX)
                    {
                        // Calculate the view difference.
                        int realDiff = (diff * _totalViewWidth) /
                            _horizontalScrollViewWidth;

                        // Set the new position of the slider.
                        _horizontalSliderMoveX = x;

                        // Update the view scroll position.
                        ScrollHorizontal(realDiff);
                    }
                }
            }

            /// <summary>
            /// The method handles the OnTouchUp event.
            /// </summary>
            /// <param name="e"></param>
#if MF_FRAMEWORK_VERSION_V3_0
            protected override void  OnStylusUp(StylusEventArgs e)
#else
            protected override void OnTouchUp(TouchEventArgs e)
#endif
            {

                // Reset the slider flags.
                _moveVerticalSlider = false;
                _moveHorizontalSlider = false;
            }

            // Declarations of the slider flags.
            protected bool _moveVerticalSlider = false;
            protected int _verticalSliderMoveY = 0;
            protected bool _moveHorizontalSlider = false;
            protected int _horizontalSliderMoveX = 0;

            /// <summary>
            /// Handles the OnVerticalScrollStylusDown event.
            /// </summary>
            /// <param name="y"></param>
            protected virtual void OnVerticalScrollStylusDown(int y)
            {
                if (y < _verticalSliderY)
                {
                    // Page up.
                    ScrollVertical(-_itemHeight * 5);
                }
                else if (y > (_verticalSliderY + _verticalSliderHeight))
                {
                    // Page down.
                    ScrollVertical(_itemHeight * 5);
                }
                else
                {
                    // On the slider.
                    _verticalSliderMoveY = y;
                    _moveVerticalSlider = true;
                }
            }

            /// <summary>
            /// Handles the OnHorizontalScrollStylusDown event.
            /// </summary>
            /// <param name="x"></param>
            protected virtual void OnHorizontalScrollStylusDown(int x)
            {
                if (x < _horizontalSliderX)
                {
                    // Page left.
                    ScrollHorizontal(-20);
                }
                else if (x > (_horizontalSliderX + _horizontalSliderWidth))
                {
                    // Page right.
                    ScrollHorizontal(20);
                }
                else
                {
                    // The thumb was clicked.
                    _horizontalSliderMoveX = x;
                    _moveHorizontalSlider = true;
                }
            }

            /// <summary>
            /// Updates the vertical value of the scroll view.
            /// </summary>
            /// <param name="delta"></param>
            protected virtual void ScrollVertical(int delta)
            {
                // Update the member
                _sy += delta;

                // Validate and bounds align
                if (_sy < 0)
                    _sy = 0;
                if (_sy > _totalViewHeight)
                    _sy = _totalViewHeight;

                // Refresh the view
                Invalidate();
            }

            /// <summary>
            /// Updates the horizontal value of the scroll view.
            /// </summary>
            /// <param name="delta"></param>
            protected virtual void ScrollHorizontal(int delta)
            {
                // Update the member.
                _sx += delta;

                // Validate and bounds-align.
                if (_sx < 0)
                    _sx = 0;
                if (_sx > _totalViewWidth)
                    _sx = _totalViewWidth;

                // Refresh the view.
                Invalidate();
            }
        }

        /// <summary>
        /// The main window class.
        /// </summary>
        public class MyWindow : Window
        {
            // Create a panel to hold the list view.
            StackPanel stackPanel = new StackPanel();

            // Create the list view.
            ListView _listView = new ListView(SystemMetrics.ScreenWidth, SystemMetrics.ScreenHeight);

            // The window constructor.
            public MyWindow()
            {

                // Set the alignment of the list view.
                _listView.HorizontalAlignment = HorizontalAlignment.Left;
                _listView.VerticalAlignment = VerticalAlignment.Top;

                // Add the columns to the list view.
                _listView.AddColumn("Name", 200);
                _listView.AddColumn("Size", 80);
                _listView.AddColumn("Created", 80);

                // Refresh the directory and file list
                RefreshList();

                // Set up the stack panel.
                stackPanel.Orientation = Orientation.Vertical;

                // Add the list view to the panel.
                stackPanel.Children.Add(_listView);

                // Set the main window child to the panel.
                this.Child = stackPanel;
            }

            /// <summary>
            /// Refreshes the list of directories and files.
            /// </summary>
            public void RefreshList()
            {
                // Clear the list.
                _listView.Items.Clear();

                try
                {
                    // Get the string list of directories for the current 
                    // directory.
                    string[] dirs = Directory.GetDirectories(
                        Directory.GetCurrentDirectory());

                    // If this is not the root directory of the volume, add a 
                    // ".." entry.  In this demo, the root volume is named 
                    // "\\ROOT".
                    if (Directory.GetCurrentDirectory() != "\\")
                    {
                        _listView.AddItem("[..]");
                    }

                    // Add each directory name to the list view.
                    for (int i = 0; i < dirs.Length;i ++ )
                    {
                        _listView.AddItem("[" + dirs[i] + "]");
                    }

                    // Get the string list of files for the current directory.
                    string[] files = Directory.GetFiles(
                        Directory.GetCurrentDirectory());

                    // Add each file name and its information to the list view.
                    for (int i = 0; i < files.Length;i ++ )
                    {
                        // Get information about the file.
                        FileInfo info = new FileInfo(files[i]);

                        // Add the name, length, and creation date.
                        _listView.AddItem(files[i], info.Length.ToString(),
                            info.CreationTime.ToString("d"));
                    }
                }
                catch (Exception ex)
                {
                    Debug.Print(ex.ToString());
                }
            }
        }

        // The main application object.
        static MyFileSystem myApplication;

        /// <summary>
        /// The main application entry point.
        /// </summary>
        public static void Main()
        {
            // Create the main application.
            myApplication = new MyFileSystem();

            // By default, touch notifications are not turned on.  You must 
            // explicitly inform the Touch engine that you want touch events to 
            // be pumped into the application, and you want the Touch engine to 
            // work with the rest of the architecture.
            Microsoft.SPOT.Touch.Touch.Initialize(myApplication);

            // Always start in the root directory.
            Directory.SetCurrentDirectory("\\");

            // Create the main window.
            Window mainWindow = myApplication.CreateWindow();

            RemovableMedia.Eject += new EjectEventHandler(Eject);
            RemovableMedia.Insert += new InsertEventHandler(Insert);
            // Start the application.
            myApplication.Run(mainWindow);
        }

        static void Insert(object sender, MediaEventArgs args)
        {
            Debug.Print("Media Inserted: " + args.Volume.Name);
        }

        static void Eject(object sender, MediaEventArgs args)
        {
            Debug.Print("Media Ejected: " + args.Volume.Name);
        }

        public MyWindow mainWindow;

        /// <summary>
        /// Creates the main window.
        /// </summary>
        /// <returns>The newly created main window.</returns>
        public Window CreateWindow()
        {
            // Create a window object and set its size to the size of the 
            // display.
            mainWindow = new MyWindow();
            mainWindow.Height = SystemMetrics.ScreenHeight;
            mainWindow.Width = SystemMetrics.ScreenWidth;

            // Set the window visibility to Visible.
            mainWindow.Visibility = Visibility.Visible;

            return mainWindow;
        }
    }
}
