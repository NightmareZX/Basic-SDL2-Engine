using MetVersus_LevelEditor.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MetVersus_LevelEditor.LevelTiles
{
    public enum CollissionType
    {
        Air,
        Solid,
        Left_Slope,
        Right_Slope,
    }
    public class Tile_16p
    {
        public TilePicture? tileTexture { get; set; }
        public Tile_16p() 
        {
            tileTexture = null;
        }

        public Image? GetImage()
        {
            if (tileTexture == null)
            {
                return null;
            }
            return tileTexture.Image;
        }
        
    }

    public class CollissionTile : Tile_16p
    {
        public CollissionType TileType { get; set; }
        public CollissionTile(CollissionType type, ComponentResourceManager resource) 
        {
            Image? tempImage = null;
            switch(type)
            {
                case CollissionType.Solid:
                    tempImage = (Image?)resource.GetObject("TilePicture_Solid.Image");
                    break;
                case CollissionType.Left_Slope:
                    tempImage = (Image?)resource.GetObject("TilePicture_LSlope.Image");
                    break;
                case CollissionType.Right_Slope:
                    tempImage = (Image?)resource.GetObject("TilePicture_RSlope.Image");
                    break;
                default:
                    break;
            }
            TileType = type;
            tileTexture = new TilePicture
            {
                InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor,
                Image = tempImage
            };
        }
    }
}
