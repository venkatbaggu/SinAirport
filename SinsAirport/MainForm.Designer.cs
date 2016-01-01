namespace Sins.Airport.Mat
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
            this.components = new System.ComponentModel.Container();
            this.Close_Btn = new System.Windows.Forms.Button();
            this.MatVideo = new System.Windows.Forms.PictureBox();
            this.Setting_Btn = new System.Windows.Forms.Button();
            this.detect1 = new System.Windows.Forms.Timer(this.components);
            this.detect2 = new System.Windows.Forms.Timer(this.components);
            this.detect3 = new System.Windows.Forms.Timer(this.components);
            this.detect4 = new System.Windows.Forms.Timer(this.components);
            this.status_pan = new System.Windows.Forms.Panel();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.MatVideo)).BeginInit();
            this.status_pan.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // Close_Btn
            // 
            this.Close_Btn.Location = new System.Drawing.Point(266, -1);
            this.Close_Btn.Name = "Close_Btn";
            this.Close_Btn.Size = new System.Drawing.Size(20, 18);
            this.Close_Btn.TabIndex = 0;
            this.Close_Btn.Text = "X(&X)";
            this.Close_Btn.UseVisualStyleBackColor = true;
            // 
            // MatVideo
            // 
            this.MatVideo.Location = new System.Drawing.Point(0, 0);
            this.MatVideo.Name = "MatVideo";
            this.MatVideo.Size = new System.Drawing.Size(100, 50);
            this.MatVideo.TabIndex = 1;
            this.MatVideo.TabStop = false;
            // 
            // Setting_Btn
            // 
            this.Setting_Btn.Location = new System.Drawing.Point(252, 0);
            this.Setting_Btn.Name = "Setting_Btn";
            this.Setting_Btn.Size = new System.Drawing.Size(20, 18);
            this.Setting_Btn.TabIndex = 2;
            this.Setting_Btn.Text = "S(&S)";
            this.Setting_Btn.UseVisualStyleBackColor = true;
            // 
            // status_pan
            // 
            this.status_pan.BackColor = System.Drawing.Color.Transparent;
            this.status_pan.Controls.Add(this.pictureBox4);
            this.status_pan.Controls.Add(this.pictureBox3);
            this.status_pan.Controls.Add(this.pictureBox2);
            this.status_pan.Controls.Add(this.pictureBox1);
            this.status_pan.Location = new System.Drawing.Point(666, 191);
            this.status_pan.Name = "status_pan";
            this.status_pan.Size = new System.Drawing.Size(73, 23);
            this.status_pan.TabIndex = 4;
            // 
            // pictureBox4
            // 
            this.pictureBox4.BackColor = System.Drawing.Color.Red;
            this.pictureBox4.Location = new System.Drawing.Point(54, 4);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(16, 16);
            this.pictureBox4.TabIndex = 7;
            this.pictureBox4.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.BackColor = System.Drawing.Color.Red;
            this.pictureBox3.Location = new System.Drawing.Point(37, 4);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(16, 16);
            this.pictureBox3.TabIndex = 6;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.Color.Red;
            this.pictureBox2.Location = new System.Drawing.Point(20, 4);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(16, 16);
            this.pictureBox2.TabIndex = 5;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Red;
            this.pictureBox1.Location = new System.Drawing.Point(3, 4);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(16, 16);
            this.pictureBox1.TabIndex = 4;
            this.pictureBox1.TabStop = false;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.ClientSize = new System.Drawing.Size(965, 502);
            this.Controls.Add(this.status_pan);
            this.Controls.Add(this.Setting_Btn);
            this.Controls.Add(this.MatVideo);
            this.Controls.Add(this.Close_Btn);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "机场拼接预警系统";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.MatVideo)).EndInit();
            this.status_pan.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button Close_Btn;
        private System.Windows.Forms.PictureBox MatVideo;
        private System.Windows.Forms.Button Setting_Btn;
        private System.Windows.Forms.Timer detect1;
        private System.Windows.Forms.Timer detect2;
        private System.Windows.Forms.Timer detect3;
        private System.Windows.Forms.Timer detect4;
        private System.Windows.Forms.Panel status_pan;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}

