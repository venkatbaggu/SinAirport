namespace Sins.Airport.Detect
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.runInfo = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.Flag = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // runInfo
            // 
            this.runInfo.Location = new System.Drawing.Point(7, 29);
            this.runInfo.Name = "runInfo";
            this.runInfo.Size = new System.Drawing.Size(445, 330);
            this.runInfo.TabIndex = 0;
            this.runInfo.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.Blue;
            this.label1.Location = new System.Drawing.Point(12, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "运行状态";
            // 
            // Flag
            // 
            this.Flag.AutoSize = true;
            this.Flag.ForeColor = System.Drawing.Color.Blue;
            this.Flag.Location = new System.Drawing.Point(383, 11);
            this.Flag.Name = "Flag";
            this.Flag.Size = new System.Drawing.Size(0, 12);
            this.Flag.TabIndex = 2;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(459, 361);
            this.Controls.Add(this.Flag);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.runInfo);
            this.ForeColor = System.Drawing.Color.Blue;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "检测客户端";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RichTextBox runInfo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label Flag;
    }
}

