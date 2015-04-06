

using System;
using System.Collections.Generic;
using System.Text;

namespace CSColorMedian2D
{
    class TImagePool : TPool<TColorImage>
    {
        #region Methods

        public static TColorImage CreateImage(int width, int height)
        {
            TColorImage image = Dequeue();
            if (image != null)
                return image;
            else
                return new TColorImage(width, height);
        }

        #endregion

    }
}
