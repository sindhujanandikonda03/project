
using System;
using System.Collections.Generic;
using System.Text;

namespace CSColorMedian2D
{
    public class TImage
    {
        #region Local Variables
        internal byte[][] m_data = null;
        protected int m_width = 0;
        protected int m_height = 0;
        #endregion

        #region Ctors

        public TImage(int width, int height)
        {
            m_data = new byte[height][];
            for (int y = 0; y < height; y++)
                m_data[y] = new byte[width];
            m_width = width;
            m_height = height;
        }

        public TImage(TImage obj)
        {
            m_width = obj.Width;
            m_height = obj.Height;
            m_data = new byte[m_height][];
            for (int y = 0; y < Height; y++)
            {
                m_data[y] = new byte[m_width];
                for (int x = 0; x < obj.Width; x++)
                    m_data[y][x] = obj.m_data[y][x];
            }
        }

        #endregion

        #region Properties

        public int Width
        {
            get
            {
                return m_width;
            }
        }

        public int Height
        {
            get
            {
                return m_height;
            }
        }


        #endregion

        #region Methods

        public byte getValue(int row, int col)
        {
            if (row < 0)
                row = 0;
            else if (row >= Height)
                row = Height - 1;

            if (col < 0)
                col = 0;
            else if (col >= Width)
                col = Width - 1;
            return m_data[row][col];
        }

        public void setValue(int row, int col, byte value)
        {
            m_data[row][col] = value;
        }

        public void fill(byte value)
        {
            for (int row = 0; row < Height; row++)
                for (int col = 0; col < Width; col++)
                    m_data[row][col] = value;
        }

        public void addNoise(double p, byte a, byte b)
        {
            Random r = new Random(Convert.ToInt32(DateTime.Now.Ticks % 0x7FFFFFFF));
            for (int row = 0; row < Height; row++)
                for (int col = 0; col < Width; col++)
                {
                    if (r.NextDouble() < p)
                        m_data[row][col] = (byte)(a + r.Next(b - a));
                }
        }

        public override bool Equals(object obj)
        {
            if (obj != null && obj is TImage)
            {
                TImage o = (TImage)obj;
                if (this.Width == o.Width && this.Height == o.Height)
                {
                    for (int row = 0; row < Height; row++)
                    {
                        for (int col = 0; col < Width; col++)
                            if (m_data[row][col] != o.m_data[row][col])
                                return false;
                    }
                    return true;
                }
            }
            return false;
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public unsafe void LoadFromBitmap(System.Drawing.Bitmap bmp)
        {
            System.Drawing.Imaging.BitmapData bmd = bmp.LockBits(new System.Drawing.Rectangle(0, 0, bmp.Width - 1, bmp.Height - 1), System.Drawing.Imaging.ImageLockMode.ReadOnly, bmp.PixelFormat);
            for (int y = 0; y < bmd.Height; y++)
            {
                fixed (byte* rowPtr = &m_data[y][0])
                {
                    System.Runtime.InteropServices.Marshal.Copy(new IntPtr((int)bmd.Scan0 + bmd.Stride * y), m_data[y], 0, bmp.Width * sizeof(byte));

                }
            }
            bmp.UnlockBits(bmd);

        }

        public unsafe void SaveToBitmap(System.Drawing.Bitmap bmp)
        {
            System.Drawing.Imaging.BitmapData bmd = bmp.LockBits(new System.Drawing.Rectangle(0, 0, bmp.Width - 1, bmp.Height - 1), System.Drawing.Imaging.ImageLockMode.WriteOnly, bmp.PixelFormat);
            for (int y = 0; y < bmd.Height; y++)
            {
                fixed (byte* rowPtr = &m_data[y][0])
                {
                    System.Runtime.InteropServices.Marshal.Copy(m_data[y], 0, new IntPtr((int)bmd.Scan0 + bmd.Stride * y), bmp.Width);
                }
            }
            bmp.UnlockBits(bmd);
        }


        public void SaveTo(TImage image)
        {
            for (int row = 0; row < Height; row++)
            {
                for (int col = 0; col < Width; col++)
                    image.m_data[row][col] = m_data[row][col];
            }
        }

        #endregion
    }

}
