namespace Managed_Data_Request
{
    partial class Form1
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonConnect = new System.Windows.Forms.Button();
            this.buttonDisconnect = new System.Windows.Forms.Button();
            this.buttonRequestData = new System.Windows.Forms.Button();
            this.richResponse = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(12, 38);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(131, 23);
            this.buttonConnect.TabIndex = 0;
            this.buttonConnect.Text = "Connect to FSX";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // buttonDisconnect
            // 
            this.buttonDisconnect.Location = new System.Drawing.Point(12, 168);
            this.buttonDisconnect.Name = "buttonDisconnect";
            this.buttonDisconnect.Size = new System.Drawing.Size(131, 23);
            this.buttonDisconnect.TabIndex = 1;
            this.buttonDisconnect.Text = "Disconnect from FSX";
            this.buttonDisconnect.UseVisualStyleBackColor = true;
            this.buttonDisconnect.Click += new System.EventHandler(this.buttonDisconnect_Click);
            // 
            // buttonRequestData
            // 
            this.buttonRequestData.Location = new System.Drawing.Point(12, 96);
            this.buttonRequestData.Name = "buttonRequestData";
            this.buttonRequestData.Size = new System.Drawing.Size(131, 41);
            this.buttonRequestData.TabIndex = 2;
            this.buttonRequestData.Text = "Request User Aircraft Data";
            this.buttonRequestData.UseVisualStyleBackColor = true;
            this.buttonRequestData.Click += new System.EventHandler(this.buttonRequestData_Click);
            // 
            // richResponse
            // 
            this.richResponse.Location = new System.Drawing.Point(149, 40);
            this.richResponse.Name = "richResponse";
            this.richResponse.ReadOnly = true;
            this.richResponse.Size = new System.Drawing.Size(221, 151);
            this.richResponse.TabIndex = 3;
            this.richResponse.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(149, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Responses";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(393, 211);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.richResponse);
            this.Controls.Add(this.buttonRequestData);
            this.Controls.Add(this.buttonDisconnect);
            this.Controls.Add(this.buttonConnect);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Form1";
            this.Text = "  SimConnect Managed Data Request";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.Button buttonDisconnect;
        private System.Windows.Forms.Button buttonRequestData;
        private System.Windows.Forms.RichTextBox richResponse;
        private System.Windows.Forms.Label label1;
    }
}

