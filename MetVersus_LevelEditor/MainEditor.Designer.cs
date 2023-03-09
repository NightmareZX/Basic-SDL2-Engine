using System.Drawing.Drawing2D;

namespace MetVersus_LevelEditor
{
    partial class MainEditor
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainEditor));
            this.MenuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItem_CompileLevel = new System.Windows.Forms.ToolStripMenuItem();
            this.Panel_Level = new System.Windows.Forms.Panel();
            this.TilePicture_Solid = new MetVersus_LevelEditor.TilePicture();
            this.TilePicture_LSlope = new MetVersus_LevelEditor.TilePicture();
            this.TilePicture_RSlope = new MetVersus_LevelEditor.TilePicture();
            this.MenuStrip.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TilePicture_Solid)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.TilePicture_LSlope)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.TilePicture_RSlope)).BeginInit();
            this.SuspendLayout();
            // 
            // MenuStrip
            // 
            this.MenuStrip.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.MenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.MenuStrip.Location = new System.Drawing.Point(0, 0);
            this.MenuStrip.Name = "MenuStrip";
            this.MenuStrip.Size = new System.Drawing.Size(1221, 28);
            this.MenuStrip.TabIndex = 0;
            this.MenuStrip.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItem_CompileLevel});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(46, 24);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // ToolStripMenuItem_CompileLevel
            // 
            this.ToolStripMenuItem_CompileLevel.Name = "ToolStripMenuItem_CompileLevel";
            this.ToolStripMenuItem_CompileLevel.Size = new System.Drawing.Size(224, 26);
            this.ToolStripMenuItem_CompileLevel.Text = "Compile Level";
            this.ToolStripMenuItem_CompileLevel.Click += new System.EventHandler(this.ToolStripMenuItem_CompileLevel_Click);
            // 
            // Panel_Level
            // 
            this.Panel_Level.BackColor = System.Drawing.Color.DimGray;
            this.Panel_Level.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Panel_Level.Location = new System.Drawing.Point(194, 31);
            this.Panel_Level.Name = "Panel_Level";
            this.Panel_Level.Size = new System.Drawing.Size(779, 651);
            this.Panel_Level.TabIndex = 1;
            this.Panel_Level.Paint += new System.Windows.Forms.PaintEventHandler(this.Panel_Level_Paint);
            this.Panel_Level.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Panel_Level_MouseDown);
            this.Panel_Level.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Panel_Level_MouseMove);
            // 
            // TilePicture_Solid
            // 
            this.TilePicture_Solid.Image = ((System.Drawing.Image)(resources.GetObject("TilePicture_Solid.Image")));
            this.TilePicture_Solid.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            this.TilePicture_Solid.Location = new System.Drawing.Point(979, 95);
            this.TilePicture_Solid.Name = "TilePicture_Solid";
            this.TilePicture_Solid.Size = new System.Drawing.Size(64, 64);
            this.TilePicture_Solid.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.TilePicture_Solid.TabIndex = 2;
            this.TilePicture_Solid.TabStop = false;
            this.TilePicture_Solid.Click += new System.EventHandler(this.TilePicture_Solid_Click);
            // 
            // TilePicture_LSlope
            // 
            this.TilePicture_LSlope.Image = ((System.Drawing.Image)(resources.GetObject("TilePicture_LSlope.Image")));
            this.TilePicture_LSlope.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            this.TilePicture_LSlope.Location = new System.Drawing.Point(1049, 95);
            this.TilePicture_LSlope.Name = "TilePicture_LSlope";
            this.TilePicture_LSlope.Size = new System.Drawing.Size(64, 64);
            this.TilePicture_LSlope.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.TilePicture_LSlope.TabIndex = 3;
            this.TilePicture_LSlope.TabStop = false;
            this.TilePicture_LSlope.Click += new System.EventHandler(this.TilePicture_LSlope_Click);
            // 
            // TilePicture_RSlope
            // 
            this.TilePicture_RSlope.Image = ((System.Drawing.Image)(resources.GetObject("TilePicture_RSlope.Image")));
            this.TilePicture_RSlope.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            this.TilePicture_RSlope.Location = new System.Drawing.Point(1119, 95);
            this.TilePicture_RSlope.Name = "TilePicture_RSlope";
            this.TilePicture_RSlope.Size = new System.Drawing.Size(64, 64);
            this.TilePicture_RSlope.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.TilePicture_RSlope.TabIndex = 4;
            this.TilePicture_RSlope.TabStop = false;
            this.TilePicture_RSlope.Click += new System.EventHandler(this.TilePicture_RSlope_Click);
            // 
            // MainEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(24)))), ((int)(((byte)(38)))), ((int)(((byte)(35)))));
            this.ClientSize = new System.Drawing.Size(1221, 685);
            this.Controls.Add(this.TilePicture_RSlope);
            this.Controls.Add(this.TilePicture_LSlope);
            this.Controls.Add(this.TilePicture_Solid);
            this.Controls.Add(this.Panel_Level);
            this.Controls.Add(this.MenuStrip);
            this.MainMenuStrip = this.MenuStrip;
            this.Name = "MainEditor";
            this.Text = "Level Editor";
            this.MenuStrip.ResumeLayout(false);
            this.MenuStrip.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.TilePicture_Solid)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.TilePicture_LSlope)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.TilePicture_RSlope)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private MenuStrip MenuStrip;
        private ToolStripMenuItem fileToolStripMenuItem;
        private Panel Panel_Level;
        private TilePicture TilePicture_Solid;
        private TilePicture TilePicture_LSlope;
        private TilePicture TilePicture_RSlope;
        private ToolStripMenuItem ToolStripMenuItem_CompileLevel;
    }
}