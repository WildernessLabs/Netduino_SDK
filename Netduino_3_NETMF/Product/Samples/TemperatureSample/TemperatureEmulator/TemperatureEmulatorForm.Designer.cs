////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Drawing;
using System.Windows.Forms;

namespace Microsoft.SPOT.Emulator.Temperature {
    partial class TemperatureEmulatorForm {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.panel1 = new System.Windows.Forms.Panel();
            this.vScrollBar1 = new System.Windows.Forms.VScrollBar();
            this.textBoxTemperature = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioButtonFahrenheit = new System.Windows.Forms.RadioButton();
            this.radioButtonCelsius = new System.Windows.Forms.RadioButton();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonCollection = new Microsoft.SPOT.Emulator.Temperature.ButtonCollection();
            this.buttonDown = new Microsoft.SPOT.Emulator.Temperature.Button();
            this.buttonSelect = new Microsoft.SPOT.Emulator.Temperature.Button();
            this.buttonRight = new Microsoft.SPOT.Emulator.Temperature.Button();
            this.buttonUp = new Microsoft.SPOT.Emulator.Temperature.Button();
            this.buttonLeft = new Microsoft.SPOT.Emulator.Temperature.Button();
            this.lcdDisplay = new Microsoft.SPOT.Emulator.Temperature.LcdControl();
            this.panel1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.buttonCollection.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LightGray;
            this.panel1.Controls.Add(this.vScrollBar1);
            this.panel1.Controls.Add(this.textBoxTemperature);
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel1.Location = new System.Drawing.Point(501, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(207, 614);
            this.panel1.TabIndex = 3;
            // 
            // vScrollBar1
            // 
            this.vScrollBar1.Location = new System.Drawing.Point(76, 144);
            this.vScrollBar1.Maximum = 465;
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.Size = new System.Drawing.Size(49, 461);
            this.vScrollBar1.TabIndex = 19;
            this.vScrollBar1.Value = 188;
            this.vScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vScrollBar1_Scroll);
            // 
            // textBoxTemperature
            // 
            this.textBoxTemperature.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBoxTemperature.Location = new System.Drawing.Point(76, 115);
            this.textBoxTemperature.Name = "textBoxTemperature";
            this.textBoxTemperature.Size = new System.Drawing.Size(49, 26);
            this.textBoxTemperature.TabIndex = 16;
            this.textBoxTemperature.Text = "72";
            this.textBoxTemperature.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.Color.Silver;
            this.groupBox1.Controls.Add(this.radioButtonFahrenheit);
            this.groupBox1.Controls.Add(this.radioButtonCelsius);
            this.groupBox1.Location = new System.Drawing.Point(53, 44);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(95, 65);
            this.groupBox1.TabIndex = 17;
            this.groupBox1.TabStop = false;
            // 
            // radioButtonFahrenheit
            // 
            this.radioButtonFahrenheit.AutoSize = true;
            this.radioButtonFahrenheit.Location = new System.Drawing.Point(14, 36);
            this.radioButtonFahrenheit.Name = "radioButtonFahrenheit";
            this.radioButtonFahrenheit.Size = new System.Drawing.Size(75, 17);
            this.radioButtonFahrenheit.TabIndex = 14;
            this.radioButtonFahrenheit.TabStop = true;
            this.radioButtonFahrenheit.Text = "Fahrenheit";
            this.radioButtonFahrenheit.UseVisualStyleBackColor = true;
            // 
            // radioButtonCelsius
            // 
            this.radioButtonCelsius.AutoSize = true;
            this.radioButtonCelsius.Location = new System.Drawing.Point(14, 12);
            this.radioButtonCelsius.Name = "radioButtonCelsius";
            this.radioButtonCelsius.Size = new System.Drawing.Size(58, 17);
            this.radioButtonCelsius.TabIndex = 13;
            this.radioButtonCelsius.TabStop = true;
            this.radioButtonCelsius.Text = "Celsius";
            this.radioButtonCelsius.UseVisualStyleBackColor = true;
            this.radioButtonCelsius.CheckedChanged += new System.EventHandler(this.radioButtonCelsius_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(3, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(202, 24);
            this.label1.TabIndex = 0;
            this.label1.Text = "Temperature Sensor";
            // 
            // buttonCollection
            // 
            this.buttonCollection.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.buttonCollection.Controls.Add(this.buttonDown);
            this.buttonCollection.Controls.Add(this.buttonSelect);
            this.buttonCollection.Controls.Add(this.buttonRight);
            this.buttonCollection.Controls.Add(this.buttonUp);
            this.buttonCollection.Controls.Add(this.buttonLeft);
            this.buttonCollection.Location = new System.Drawing.Point(35, 330);
            this.buttonCollection.Name = "buttonCollection";
            this.buttonCollection.Size = new System.Drawing.Size(320, 244);
            this.buttonCollection.TabIndex = 2;
            this.buttonCollection.Text = "buttonCollection1";
            // 
            // buttonDown
            // 
            this.buttonDown.Key = System.Windows.Forms.Keys.None;
            this.buttonDown.Location = new System.Drawing.Point(133, 173);
            this.buttonDown.Name = "buttonDown";
            this.buttonDown.Port = null;
            this.buttonDown.Size = new System.Drawing.Size(56, 56);
            this.buttonDown.TabIndex = 1;
            this.buttonDown.TabStop = false;
            this.buttonDown.Text = "buttonDown";
            // 
            // buttonSelect
            // 
            this.buttonSelect.Key = System.Windows.Forms.Keys.None;
            this.buttonSelect.Location = new System.Drawing.Point(133, 96);
            this.buttonSelect.Name = "buttonSelect";
            this.buttonSelect.Port = null;
            this.buttonSelect.Size = new System.Drawing.Size(56, 56);
            this.buttonSelect.TabIndex = 1;
            this.buttonSelect.TabStop = false;
            this.buttonSelect.Text = "buttonEnter";
            // 
            // buttonRight
            // 
            this.buttonRight.Key = System.Windows.Forms.Keys.None;
            this.buttonRight.Location = new System.Drawing.Point(209, 96);
            this.buttonRight.Name = "buttonRight";
            this.buttonRight.Port = null;
            this.buttonRight.Size = new System.Drawing.Size(56, 56);
            this.buttonRight.TabIndex = 1;
            this.buttonRight.TabStop = false;
            this.buttonRight.Text = "buttonRight";
            // 
            // buttonUp
            // 
            this.buttonUp.Key = System.Windows.Forms.Keys.None;
            this.buttonUp.Location = new System.Drawing.Point(133, 19);
            this.buttonUp.Name = "buttonUp";
            this.buttonUp.Port = null;
            this.buttonUp.Size = new System.Drawing.Size(56, 56);
            this.buttonUp.TabIndex = 1;
            this.buttonUp.TabStop = false;
            this.buttonUp.Text = "buttonUp";
            // 
            // buttonLeft
            // 
            this.buttonLeft.Key = System.Windows.Forms.Keys.None;
            this.buttonLeft.Location = new System.Drawing.Point(56, 96);
            this.buttonLeft.Name = "buttonLeft";
            this.buttonLeft.Port = null;
            this.buttonLeft.Size = new System.Drawing.Size(56, 56);
            this.buttonLeft.TabIndex = 1;
            this.buttonLeft.TabStop = false;
            this.buttonLeft.Text = "buttonLeft";
            // 
            // lcdDisplay
            // 
            this.lcdDisplay.BackColor = System.Drawing.Color.White;
            this.lcdDisplay.LcdDisplay = null;
            this.lcdDisplay.Location = new System.Drawing.Point(35, 71);
            this.lcdDisplay.Name = "lcdDisplay";
            this.lcdDisplay.Size = new System.Drawing.Size(320, 240);
            this.lcdDisplay.TabIndex = 0;
            this.lcdDisplay.Text = "lcdControl1";
            // 
            // TemperatureEmulatorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.BackgroundImage = global::Microsoft.SPOT.Emulator.Temperature.Properties.Resources.EmulatorSkin;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ClientSize = new System.Drawing.Size(708, 614);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.buttonCollection);
            this.Controls.Add(this.lcdDisplay);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "TemperatureEmulatorForm";
            this.Text = "Temperature Emulator";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.buttonCollection.ResumeLayout(false);
            this.ResumeLayout(false);
        }

        #endregion

        private Microsoft.SPOT.Emulator.Temperature.LcdControl lcdDisplay;
        private Microsoft.SPOT.Emulator.Temperature.Button buttonLeft;
        private Microsoft.SPOT.Emulator.Temperature.Button buttonUp;
        private Microsoft.SPOT.Emulator.Temperature.Button buttonRight;
        private Microsoft.SPOT.Emulator.Temperature.Button buttonDown;
        private Microsoft.SPOT.Emulator.Temperature.Button buttonSelect;
        private Microsoft.SPOT.Emulator.Temperature.ButtonCollection buttonCollection;
        private Panel panel1;
        private Label label1;
        private VScrollBar vScrollBar1;
        private TextBox textBoxTemperature;
        private GroupBox groupBox1;
        private RadioButton radioButtonFahrenheit;
        private RadioButton radioButtonCelsius;
    }
}

