namespace c_UART_v2_50
{
    partial class CyUartControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.gbRXTX = new System.Windows.Forms.GroupBox();
            this.m_rbHALF = new System.Windows.Forms.RadioButton();
            this.m_rbRXTX = new System.Windows.Forms.RadioButton();
            this.m_rbTXOnly = new System.Windows.Forms.RadioButton();
            this.m_rbRXOnly = new System.Windows.Forms.RadioButton();
            this.gbMain = new System.Windows.Forms.GroupBox();
            this.parityGroupBox = new System.Windows.Forms.GroupBox();
            this.m_chbSoftware = new System.Windows.Forms.CheckBox();
            this.m_lblParity = new System.Windows.Forms.Label();
            this.m_cbParity = new System.Windows.Forms.ComboBox();
            this.m_lblExternalClockMessage = new System.Windows.Forms.Label();
            this.m_lblFlowControl = new System.Windows.Forms.Label();
            this.m_lblStopBits = new System.Windows.Forms.Label();
            this.m_cbFlowControl = new System.Windows.Forms.ComboBox();
            this.m_cbStopBits = new System.Windows.Forms.ComboBox();
            this.m_lblDataBits = new System.Windows.Forms.Label();
            this.m_lblBPS = new System.Windows.Forms.Label();
            this.m_cbDataBits = new System.Windows.Forms.ComboBox();
            this.m_cbBitsPerSecond = new System.Windows.Forms.ComboBox();
            this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
            this.gbRXTX.SuspendLayout();
            this.gbMain.SuspendLayout();
            this.parityGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
            this.SuspendLayout();
            // 
            // gbRXTX
            // 
            this.gbRXTX.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gbRXTX.Controls.Add(this.m_rbHALF);
            this.gbRXTX.Controls.Add(this.m_rbRXTX);
            this.gbRXTX.Controls.Add(this.m_rbTXOnly);
            this.gbRXTX.Controls.Add(this.m_rbRXOnly);
            this.gbRXTX.Location = new System.Drawing.Point(6, 2);
            this.gbRXTX.Margin = new System.Windows.Forms.Padding(2);
            this.gbRXTX.Name = "gbRXTX";
            this.gbRXTX.Padding = new System.Windows.Forms.Padding(2);
            this.gbRXTX.Size = new System.Drawing.Size(286, 57);
            this.gbRXTX.TabIndex = 0;
            this.gbRXTX.TabStop = false;
            this.gbRXTX.Text = "Mode";
            // 
            // m_rbHALF
            // 
            this.m_rbHALF.AutoSize = true;
            this.m_rbHALF.Location = new System.Drawing.Point(22, 33);
            this.m_rbHALF.Margin = new System.Windows.Forms.Padding(2);
            this.m_rbHALF.Name = "m_rbHALF";
            this.m_rbHALF.Size = new System.Drawing.Size(78, 17);
            this.m_rbHALF.TabIndex = 1;
            this.m_rbHALF.TabStop = true;
            this.m_rbHALF.Text = "Half duplex";
            this.m_rbHALF.UseVisualStyleBackColor = true;
            this.m_rbHALF.CheckedChanged += new System.EventHandler(this.ModeRB_CheckedChanged);
            // 
            // m_rbRXTX
            // 
            this.m_rbRXTX.AutoSize = true;
            this.m_rbRXTX.Location = new System.Drawing.Point(22, 14);
            this.m_rbRXTX.Margin = new System.Windows.Forms.Padding(2);
            this.m_rbRXTX.Name = "m_rbRXTX";
            this.m_rbRXTX.Size = new System.Drawing.Size(124, 17);
            this.m_rbRXTX.TabIndex = 0;
            this.m_rbRXTX.TabStop = true;
            this.m_rbRXTX.Text = "Full UART (TX + RX)";
            this.m_rbRXTX.UseVisualStyleBackColor = true;
            this.m_rbRXTX.CheckedChanged += new System.EventHandler(this.ModeRB_CheckedChanged);
            // 
            // m_rbTXOnly
            // 
            this.m_rbTXOnly.AutoSize = true;
            this.m_rbTXOnly.Location = new System.Drawing.Point(199, 34);
            this.m_rbTXOnly.Margin = new System.Windows.Forms.Padding(2);
            this.m_rbTXOnly.Name = "m_rbTXOnly";
            this.m_rbTXOnly.Size = new System.Drawing.Size(61, 17);
            this.m_rbTXOnly.TabIndex = 3;
            this.m_rbTXOnly.TabStop = true;
            this.m_rbTXOnly.Text = "TX only";
            this.m_rbTXOnly.UseVisualStyleBackColor = true;
            this.m_rbTXOnly.CheckedChanged += new System.EventHandler(this.ModeRB_CheckedChanged);
            // 
            // m_rbRXOnly
            // 
            this.m_rbRXOnly.AutoSize = true;
            this.m_rbRXOnly.Location = new System.Drawing.Point(199, 14);
            this.m_rbRXOnly.Margin = new System.Windows.Forms.Padding(2);
            this.m_rbRXOnly.Name = "m_rbRXOnly";
            this.m_rbRXOnly.Size = new System.Drawing.Size(62, 17);
            this.m_rbRXOnly.TabIndex = 2;
            this.m_rbRXOnly.TabStop = true;
            this.m_rbRXOnly.Text = "RX only";
            this.m_rbRXOnly.UseVisualStyleBackColor = true;
            this.m_rbRXOnly.CheckedChanged += new System.EventHandler(this.ModeRB_CheckedChanged);
            // 
            // gbMain
            // 
            this.gbMain.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gbMain.Controls.Add(this.parityGroupBox);
            this.gbMain.Controls.Add(this.m_lblExternalClockMessage);
            this.gbMain.Controls.Add(this.m_lblFlowControl);
            this.gbMain.Controls.Add(this.m_lblStopBits);
            this.gbMain.Controls.Add(this.m_cbFlowControl);
            this.gbMain.Controls.Add(this.m_cbStopBits);
            this.gbMain.Controls.Add(this.m_lblDataBits);
            this.gbMain.Controls.Add(this.m_lblBPS);
            this.gbMain.Controls.Add(this.m_cbDataBits);
            this.gbMain.Controls.Add(this.m_cbBitsPerSecond);
            this.gbMain.Location = new System.Drawing.Point(6, 59);
            this.gbMain.Margin = new System.Windows.Forms.Padding(2);
            this.gbMain.Name = "gbMain";
            this.gbMain.Padding = new System.Windows.Forms.Padding(2);
            this.gbMain.Size = new System.Drawing.Size(286, 174);
            this.gbMain.TabIndex = 1;
            this.gbMain.TabStop = false;
            // 
            // parityGroupBox
            // 
            this.parityGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.parityGroupBox.Controls.Add(this.m_chbSoftware);
            this.parityGroupBox.Controls.Add(this.m_lblParity);
            this.parityGroupBox.Controls.Add(this.m_cbParity);
            this.parityGroupBox.Location = new System.Drawing.Point(35, 58);
            this.parityGroupBox.Margin = new System.Windows.Forms.Padding(2);
            this.parityGroupBox.Name = "parityGroupBox";
            this.parityGroupBox.Padding = new System.Windows.Forms.Padding(2);
            this.parityGroupBox.Size = new System.Drawing.Size(245, 57);
            this.parityGroupBox.TabIndex = 2;
            this.parityGroupBox.TabStop = false;
            // 
            // m_chbSoftware
            // 
            this.m_chbSoftware.AutoSize = true;
            this.m_chbSoftware.Location = new System.Drawing.Point(101, 36);
            this.m_chbSoftware.Margin = new System.Windows.Forms.Padding(2);
            this.m_chbSoftware.Name = "m_chbSoftware";
            this.m_chbSoftware.Size = new System.Drawing.Size(119, 17);
            this.m_chbSoftware.TabIndex = 1;
            this.m_chbSoftware.Text = "API control enabled";
            this.m_chbSoftware.UseVisualStyleBackColor = true;
            this.m_chbSoftware.CheckedChanged += new System.EventHandler(this.m_chbSoftware_CheckedChanged);
            // 
            // m_lblParity
            // 
            this.m_lblParity.AutoSize = true;
            this.m_lblParity.Location = new System.Drawing.Point(17, 14);
            this.m_lblParity.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.m_lblParity.Name = "m_lblParity";
            this.m_lblParity.Size = new System.Drawing.Size(59, 13);
            this.m_lblParity.TabIndex = 143;
            this.m_lblParity.Text = "Parity type:";
            this.m_lblParity.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // m_cbParity
            // 
            this.m_cbParity.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_cbParity.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbParity.FormattingEnabled = true;
            this.m_cbParity.Location = new System.Drawing.Point(100, 11);
            this.m_cbParity.Margin = new System.Windows.Forms.Padding(2);
            this.m_cbParity.Name = "m_cbParity";
            this.m_cbParity.Size = new System.Drawing.Size(128, 21);
            this.m_cbParity.TabIndex = 0;
            this.m_cbParity.SelectedIndexChanged += new System.EventHandler(this.m_cbParity_SelectedIndexChanged);
            // 
            // m_lblExternalClockMessage
            // 
            this.m_lblExternalClockMessage.AutoSize = true;
            this.m_lblExternalClockMessage.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.m_lblExternalClockMessage.Location = new System.Drawing.Point(123, 13);
            this.m_lblExternalClockMessage.Name = "m_lblExternalClockMessage";
            this.m_lblExternalClockMessage.Size = new System.Drawing.Size(159, 13);
            this.m_lblExternalClockMessage.TabIndex = 140;
            this.m_lblExternalClockMessage.Text = "1/8 Input Clock Frequency";
            // 
            // m_lblFlowControl
            // 
            this.m_lblFlowControl.AutoSize = true;
            this.m_lblFlowControl.Location = new System.Drawing.Point(48, 149);
            this.m_lblFlowControl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.m_lblFlowControl.Name = "m_lblFlowControl";
            this.m_lblFlowControl.Size = new System.Drawing.Size(67, 13);
            this.m_lblFlowControl.TabIndex = 135;
            this.m_lblFlowControl.Text = "Flow control:";
            this.m_lblFlowControl.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // m_lblStopBits
            // 
            this.m_lblStopBits.AutoSize = true;
            this.m_lblStopBits.Location = new System.Drawing.Point(64, 123);
            this.m_lblStopBits.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.m_lblStopBits.Name = "m_lblStopBits";
            this.m_lblStopBits.Size = new System.Drawing.Size(51, 13);
            this.m_lblStopBits.TabIndex = 135;
            this.m_lblStopBits.Text = "Stop bits:";
            this.m_lblStopBits.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // m_cbFlowControl
            // 
            this.m_cbFlowControl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_cbFlowControl.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbFlowControl.FormattingEnabled = true;
            this.m_cbFlowControl.Location = new System.Drawing.Point(136, 147);
            this.m_cbFlowControl.Margin = new System.Windows.Forms.Padding(2);
            this.m_cbFlowControl.Name = "m_cbFlowControl";
            this.m_cbFlowControl.Size = new System.Drawing.Size(127, 21);
            this.m_cbFlowControl.TabIndex = 4;
            this.m_cbFlowControl.SelectedIndexChanged += new System.EventHandler(this.m_cbFlowControl_SelectedIndexChanged);
            // 
            // m_cbStopBits
            // 
            this.m_cbStopBits.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_cbStopBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbStopBits.FormattingEnabled = true;
            this.m_cbStopBits.Location = new System.Drawing.Point(136, 120);
            this.m_cbStopBits.Margin = new System.Windows.Forms.Padding(2);
            this.m_cbStopBits.Name = "m_cbStopBits";
            this.m_cbStopBits.Size = new System.Drawing.Size(127, 21);
            this.m_cbStopBits.TabIndex = 3;
            this.m_cbStopBits.SelectedIndexChanged += new System.EventHandler(this.m_cbStopBits_SelectedIndexChanged);
            // 
            // m_lblDataBits
            // 
            this.m_lblDataBits.AutoSize = true;
            this.m_lblDataBits.Location = new System.Drawing.Point(64, 40);
            this.m_lblDataBits.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.m_lblDataBits.Name = "m_lblDataBits";
            this.m_lblDataBits.Size = new System.Drawing.Size(52, 13);
            this.m_lblDataBits.TabIndex = 133;
            this.m_lblDataBits.Text = "Data bits:";
            this.m_lblDataBits.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // m_lblBPS
            // 
            this.m_lblBPS.AutoSize = true;
            this.m_lblBPS.Location = new System.Drawing.Point(32, 13);
            this.m_lblBPS.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.m_lblBPS.Name = "m_lblBPS";
            this.m_lblBPS.Size = new System.Drawing.Size(83, 13);
            this.m_lblBPS.TabIndex = 130;
            this.m_lblBPS.Text = "Bits per second:";
            this.m_lblBPS.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // m_cbDataBits
            // 
            this.m_cbDataBits.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_cbDataBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbDataBits.FormattingEnabled = true;
            this.m_cbDataBits.Location = new System.Drawing.Point(136, 37);
            this.m_cbDataBits.Margin = new System.Windows.Forms.Padding(2);
            this.m_cbDataBits.Name = "m_cbDataBits";
            this.m_cbDataBits.Size = new System.Drawing.Size(126, 21);
            this.m_cbDataBits.TabIndex = 1;
            this.m_cbDataBits.SelectedIndexChanged += new System.EventHandler(this.m_cbDataBits_SelectedIndexChanged);
            // 
            // m_cbBitsPerSecond
            // 
            this.m_cbBitsPerSecond.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.m_cbBitsPerSecond.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_cbBitsPerSecond.FormattingEnabled = true;
            this.m_cbBitsPerSecond.Items.AddRange(new object[] {
            "110",
            "300",
            "1200",
            "2400",
            "4800",
            "9600",
            "19200",
            "38400",
            "57600",
            "115200",
            "230400",
            "460800",
            "921600"});
            this.m_cbBitsPerSecond.Location = new System.Drawing.Point(136, 10);
            this.m_cbBitsPerSecond.Margin = new System.Windows.Forms.Padding(2);
            this.m_cbBitsPerSecond.Name = "m_cbBitsPerSecond";
            this.m_cbBitsPerSecond.Size = new System.Drawing.Size(126, 21);
            this.m_cbBitsPerSecond.TabIndex = 0;
            this.m_cbBitsPerSecond.SelectedIndexChanged += new System.EventHandler(this.m_cbBitsPerSecond_SelectedIndexChanged);
            // 
            // errorProvider1
            // 
            this.errorProvider1.ContainerControl = this;
            // 
            // CyUartControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.gbRXTX);
            this.Controls.Add(this.gbMain);
            this.Name = "CyUartControl";
            this.Size = new System.Drawing.Size(306, 236);
            this.Load += new System.EventHandler(this.CyUartControl_Load);
            this.gbRXTX.ResumeLayout(false);
            this.gbRXTX.PerformLayout();
            this.gbMain.ResumeLayout(false);
            this.gbMain.PerformLayout();
            this.parityGroupBox.ResumeLayout(false);
            this.parityGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox gbRXTX;
        private System.Windows.Forms.RadioButton m_rbRXTX;
        private System.Windows.Forms.RadioButton m_rbTXOnly;
        private System.Windows.Forms.RadioButton m_rbRXOnly;
        private System.Windows.Forms.GroupBox gbMain;
        private System.Windows.Forms.Label m_lblFlowControl;
        private System.Windows.Forms.Label m_lblStopBits;
        private System.Windows.Forms.ComboBox m_cbFlowControl;
        private System.Windows.Forms.ComboBox m_cbStopBits;
        private System.Windows.Forms.Label m_lblDataBits;
        private System.Windows.Forms.Label m_lblBPS;
        private System.Windows.Forms.ComboBox m_cbDataBits;
        private System.Windows.Forms.ComboBox m_cbBitsPerSecond;
        private System.Windows.Forms.RadioButton m_rbHALF;
        private System.Windows.Forms.Label m_lblExternalClockMessage;
        private System.Windows.Forms.GroupBox parityGroupBox;
        private System.Windows.Forms.CheckBox m_chbSoftware;
        private System.Windows.Forms.Label m_lblParity;
        private System.Windows.Forms.ComboBox m_cbParity;
        private System.Windows.Forms.ErrorProvider errorProvider1;
    }
}
