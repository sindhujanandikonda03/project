using System;
using System.Collections.Generic;
using System.Text;

namespace CSColorMedian2D
{
    public class TColorImage
    {
        #region Local Variables
        protected TImage m_red = null;
        protected TImage m_green = null;
        protected TImage m_blue = null;
        protected System.Drawing.Imaging.PixelFormat m_pixelFormat = System.Drawing.Imaging.PixelFormat.Undefined;
        #endregion

        #region Ctors

        public TColorImage(int width, int height)
        {
            m_red = new TImage(width, height);
            m_green = new TImage(width, height);
            m_blue = new TImage(width, height);
        }

        public TColorImage(int width, int height, System.Drawing.Imaging.PixelFormat pixelFormat)
        {
            m_red = new TImage(width, height);
            m_green = new TImage(width, height);
            m_blue = new TImage(width, height);
            m_pixelFormat = pixelFormat;
        }

        public TColorImage(TColorImage obj)
        {
            m_red = new TImage(obj.m_red);
            m_green = new TImage(obj.m_green);
            m_blue = new TImage(obj.m_blue);
            m_pixelFormat = obj.m_pixelFormat;
        }

        public TColorImage(TImage r, TImage g, TImage b)
        {
            m_red = r;
            m_green = g;
            m_blue = b;
        }

        #endregion

        #region Properties

        public int Width
        {
            get
            {
                return m_red.Width;
            }
        }

        public int Height
        {
            get
            {
                return m_red.Height;
            }
        }

        public System.Drawing.Imaging.PixelFormat PixelFormat
        {
            get
            {
                return m_pixelFormat;
            }
            set
            {
                m_pixelFormat = value;
            }
        }

        public TImage Red
        {
            get
            {
                return m_red;
            }
        }

        public TImage Green
        {
            get
            {
                return m_green;
            }
        }

        public TImage Blue
        {
            get
            {
                return m_blue;
            }
        }
        #endregion


        #region Methods


        public void setRed(TImage image)
        {
            if (image.Width != this.Width || image.Height != this.Height)
                throw new Exception("");
            image.SaveTo(m_red);
        }

        public void copyRed(TImage image)
        {
            if (image.Width != this.Width || image.Height != this.Height)
                throw new Exception("");
            m_red.SaveTo(image);
        }

        public void setGreen(TImage image)
        {
            if (image.Width != this.Width || image.Height != this.Height)
                throw new Exception("");
            image.SaveTo(m_green);
        }

        public void copyGreen(TImage image)
        {
            if (image.Width != this.Width || image.Height != this.Height)
                throw new Exception("");
            m_green.SaveTo(image);
        }

        public void setBlue(TImage image)
        {
            if (image.Width != this.Width || image.Height != this.Height)
                throw new Exception("");
            image.SaveTo(m_blue);
        }

        public void copyBlue(TImage image)
        {
            if (image.Width != this.Width || image.Height != this.Height)
                throw new Exception("");
            m_blue.SaveTo(image);
        }

        public override bool Equals(object obj)
        {
            if (obj != null && obj is TColorImage)
            {
                TColorImage o = (TColorImage)obj;
                if (this.Width == o.Width && this.Height == o.Height)
                {
                    return this.Red == o.Red && this.Green == o.Green && this.Blue == o.Blue;
                }
            }
            return false;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public void fill(byte red, byte green, byte blue)
        {
            for (int row = 0; row < Height; row++)
                for (int col = 0; col < Width; col++)
                {
                    m_red.m_data[row][col] = red;
                    m_green.m_data[row][col] = green;
                    m_blue.m_data[row][col] = blue;
                }
        }


        public void SaveTo(TColorImage image)
        {
            m_red.SaveTo(image.m_red);
            m_red.SaveTo(image.m_red);
            m_red.SaveTo(image.m_red);
        }

        // implemented only for Format24bppRgb !!!
        public void LoadFromBitmap(System.Drawing.Bitmap bmp)
        {
            switch (bmp.PixelFormat)
            {
                case System.Drawing.Imaging.PixelFormat.Format24bppRgb:
                    LoadFrom24bppBitmap(bmp);
                break;

                case System.Drawing.Imaging.PixelFormat.Format32bppRgb:
                    break;
            }
        }

        // implemented only for Format24bppRgb !!!
        public void SaveToBitmap(System.Drawing.Bitmap bitmap)
        {
            switch (bitmap.PixelFormat)
            {
                case System.Drawing.Imaging.PixelFormat.Format24bppRgb:
                    SaveTo24bppBitmap(bitmap);
                    break;

            }
        }

        public void LoadFrom24bppBitmap(System.Drawing.Bitmap bitmap)
        {
            System.Drawing.Imaging.BitmapData bmd = bitmap.LockBits(new System.Drawing.Rectangle(0, 0, bitmap.Width - 1, bitmap.Height - 1), System.Drawing.Imaging.ImageLockMode.ReadOnly, bitmap.PixelFormat);
            int offset = 0;
            for (int y = 0; y < bmd.Height; y++)
            {
                for (int x = 0; x < bmd.Width; x++)
                {
                    int pixel = System.Runtime.InteropServices.Marshal.ReadInt32(bmd.Scan0, offset + x * 3);
                    byte[] value = BitConverter.GetBytes(pixel);
                    m_red.m_data[y][x] = value[0];
                    m_green.m_data[y][x] = value[1];
                    m_blue.m_data[y][x] = value[2];
                }
                offset += bmd.Stride;
            }
            bitmap.UnlockBits(bmd);
            m_pixelFormat = System.Drawing.Imaging.PixelFormat.Format24bppRgb;
        }

        public void SaveTo24bppBitmap(System.Drawing.Bitmap bitmap)
        {
            System.Drawing.Imaging.BitmapData bmd = bitmap.LockBits(new System.Drawing.Rectangle(0, 0, bitmap.Width - 1, bitmap.Height - 1), System.Drawing.Imaging.ImageLockMode.ReadOnly, bitmap.PixelFormat);
            int offset = 0;
            byte[] value = new byte[4];
            for (int y = 0; y < bmd.Height; y++)
            {
                for (int x = 0; x < bmd.Width; x++)
                {
                    value[0] = this.m_red.m_data[y][x];
                    value[1] = this.m_green.m_data[y][x];
                    value[2] = this.m_blue.m_data[y][x];
                    int pixel = BitConverter.ToInt32(value, 0);
                    System.Runtime.InteropServices.Marshal.WriteInt32(bmd.Scan0, offset + x * 3, BitConverter.ToInt32(value,0));
                }
                offset += bmd.Stride;
            }
            bitmap.UnlockBits(bmd);
        }


        #endregion
    }
}
