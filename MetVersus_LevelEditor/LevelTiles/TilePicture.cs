using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Drawing.Drawing2D;
using System.Windows.Forms.Design;

namespace MetVersus_LevelEditor
{
    [Designer(typeof(TilePictureDesigner))]
    public class TilePicture : PictureBox
    {
        public TilePicture()
        {
            InterpolationMode = InterpolationMode.NearestNeighbor;
            Image = null;
        }
        [Category("Behavior")]
        [DefaultValue(InterpolationMode.Default)]
        public InterpolationMode InterpolationMode { get; set; }

        protected override void OnPaint(PaintEventArgs pe)
        {
            pe.Graphics.InterpolationMode = InterpolationMode;
            base.OnPaint(pe);
        }
    }

    public class TilePictureDesigner : ControlDesigner
    {
        // custom design-time code here
    }
}
