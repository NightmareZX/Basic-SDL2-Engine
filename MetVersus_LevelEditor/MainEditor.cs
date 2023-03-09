using MetVersus_LevelEditor.LevelTiles;
using MetVersus_LevelEditor.Properties;
using Microsoft.VisualBasic.Devices;
using System.Diagnostics.CodeAnalysis;
using System.Drawing;
using System.Linq.Expressions;
using System.Resources;

namespace MetVersus_LevelEditor
{
    public partial class MainEditor : Form
    {
        CollissionTile?[,] LevelCollissionData;
        CollissionType CurrentlySelectedType = CollissionType.Air;
        private Rectangle LastImagePosition;
        //Stores which collision tile was last 
        private CollissionTile? LastCollisionTile;

        UInt32 CollissionArray_RowLength { get; set; }
        UInt32 CollissionArray_ColumnLength { get; set; }

        System.ComponentModel.ComponentResourceManager EditorResources;

        const int TileSize = 32;

        public MainEditor()
        {
            InitializeComponent();

            EditorResources = new System.ComponentModel.ComponentResourceManager(typeof(MainEditor));
            //Initialise a basic collission array
            CollissionArray_RowLength = 16;
            CollissionArray_ColumnLength = 16;
            LevelCollissionData = new CollissionTile[CollissionArray_ColumnLength, CollissionArray_RowLength];
            for (int column = 0; column < CollissionArray_ColumnLength; column++)
            {
                for (int row = 0; row < CollissionArray_RowLength; row++)
                {
                    LevelCollissionData[row, column] = null;
                }
            }
        }
        private void Panel_Level_Paint(object sender, PaintEventArgs e)
        {
        }

        private void Panel_Level_MouseHover(object sender, EventArgs e)
        {

        }

        private void Panel_Level_MouseMove(object sender, MouseEventArgs e)
        {
            Panel panel = sender as Panel;

            // Get the mouse position relative to the Panel
            Point mousePosition = panel.PointToClient(Cursor.Position);

            Graphics graphics1 = Panel_Level.CreateGraphics();
            graphics1.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            Color bgColor = Panel_Level.BackColor;
            
            //convert normal mouse positions into a grid form
            decimal gridx = mousePosition.X / TileSize;
            decimal gridy = mousePosition.Y / TileSize;

            //Store the converted mouse position so we can remember the tile that was originaly there
            int sX = (int)Math.Ceiling(gridx);
            int sY = (int)Math.Ceiling(gridy);
            if (sX < 0) sX = 0;
            if (sY < 0) sY = 0;

            //Multiply the converted positions with the tile size so it can be used as the drawing position for the image
            int tileX = sX * TileSize;
            int tileY = sY * TileSize;

            Rectangle imagePosition = new Rectangle(tileX, tileY, TileSize, TileSize);

            if(LastImagePosition != imagePosition)
            {
                graphics1.FillRectangle(new SolidBrush(bgColor), LastImagePosition);
                if(LastCollisionTile != null)
                {
                    graphics1.DrawImage(LastCollisionTile.GetImage(), LastImagePosition);
                }
            }
            if (sX < CollissionArray_RowLength && sY < CollissionArray_ColumnLength)
            {
                Image? imageToDraw = GetImageFromType(CurrentlySelectedType);
                if (imageToDraw != null) graphics1.DrawImage(imageToDraw, imagePosition);
                LastImagePosition = imagePosition;
                if (sX < CollissionArray_RowLength && sY < CollissionArray_ColumnLength && e.Button == MouseButtons.Left)
                {
                    LevelCollissionData[sX, sY] = new CollissionTile(CurrentlySelectedType, EditorResources);
                }
                else if (e.Button == MouseButtons.Right)
                {
                    LevelCollissionData[sX, sY] = null;
                }
            }

            if (sX < CollissionArray_RowLength && sY < CollissionArray_ColumnLength)
            {
                if (LevelCollissionData[sX, sY] == null)
                {
                    LastCollisionTile = null;
                }
                else
                {
                    LastCollisionTile = LevelCollissionData[sX, sY];
                }
            }
            // fill the area with the background color
            graphics1.Dispose();
        }

        private void Panel_Level_MouseDown(object sender, MouseEventArgs e)
        {
            Graphics graphics = Panel_Level.CreateGraphics();
            decimal gridx = e.X / TileSize;
            decimal gridy = e.Y / TileSize;

            //Store the converted mouse position so we can remember the tile that was originaly there
            int sX = (int)Math.Ceiling(gridx);
            int sY = (int)Math.Ceiling(gridy);
            if (sX < 0) sX = 0;
            if (sY < 0) sY = 0;

            if (sX < CollissionArray_RowLength && sY < CollissionArray_ColumnLength && e.Button == MouseButtons.Left)
            {
                LevelCollissionData[sX, sY] = new CollissionTile(CurrentlySelectedType, EditorResources);
            }
            if (e.Button == MouseButtons.Right)
            {
                LevelCollissionData[sX, sY] = null;
            }
            graphics.Dispose();
        }
        private Image? GetImageFromType(CollissionType type)
        {
            switch (type) 
            {
                case CollissionType.Solid:
                    return (Image?)EditorResources.GetObject("TilePicture_Solid.Image");
                case CollissionType.Left_Slope:
                    return (Image?)EditorResources.GetObject("TilePicture_LSlope.Image");
                case CollissionType.Right_Slope:
                    return (Image?)EditorResources.GetObject("TilePicture_RSlope.Image");
                default:
                    return null;
            }
        }
        private void TilePicture_Solid_Click(object sender, EventArgs e)
        {
            CurrentlySelectedType = CollissionType.Solid;
        }

        private void TilePicture_LSlope_Click(object sender, EventArgs e)
        {
            CurrentlySelectedType = CollissionType.Left_Slope;
        }

        private void TilePicture_RSlope_Click(object sender, EventArgs e)
        {
            CurrentlySelectedType = CollissionType.Right_Slope;
        }

        private void ToolStripMenuItem_CompileLevel_Click(object sender, EventArgs e)
        {
            FileStream fileStream = new FileStream("level.lvl", FileMode.Create);
            BinaryWriter levelWriter = new BinaryWriter(fileStream);
            levelWriter.Write(CollissionArray_RowLength); //write rows
            levelWriter.Write(CollissionArray_ColumnLength); //write columns
            for (int column = 0; column < CollissionArray_ColumnLength; column++)
            {
                for (int row = 0; row < CollissionArray_RowLength; row++)
                {
                    if (LevelCollissionData[row, column] == null)
                    {
                        levelWriter.Write((Byte)0x00);//write air
                    }
                    else 
                    {
                        switch(LevelCollissionData[row, column].TileType)
                        {
                            case CollissionType.Solid:
                                levelWriter.Write((Byte)01);//write solid
                                break;
                            case CollissionType.Left_Slope:
                                levelWriter.Write((Byte)02);//write left slope
                                break;
                            case CollissionType.Right_Slope:
                                levelWriter.Write((Byte)03);//Write right slope
                                break;
                            default:
                                levelWriter.Write((Byte)0xFF);//write unknown or invalid
                                break;
                        }
                    }
                }
            }
            levelWriter.Close();
            fileStream.Close();
            MessageBox.Show("Level File compiled Successfully!");
        }
    }
}