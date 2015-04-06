using System;
using System.Collections.Generic;
using System.Text;

namespace CSColorMedian2D
{
    class TBaseFilter2D
    {
        #region Local Variables
        protected bool m_enabled = false;

        #endregion

        #region Properties
        public bool Enabled
        {
            get
            {
                return m_enabled;
            }
            set
            {
                m_enabled = value;
            }
        }
        #endregion

        #region Methods

        public virtual void naiveFilter(TImage source, TImage destination)
        {
            source.SaveTo(destination);
        }

        public virtual void FilterImpl(TImage source, TImage destination)
        {
            source.SaveTo(destination);
        }

        public virtual void bilateral_filter(TImage source, TImage destination, double sigma_s, double sigma_r)
        {
            source.SaveTo(destination);
        }


        public void Process(TImage inputImage1, TImage outputImage1, TImage inputImage2, TImage outputImage2, TImage inputImage3, TImage outputImage3)
        {
            if (Enabled)
            {
                DateTime startTime = DateTime.Now;
                naiveFilter(inputImage1, outputImage1);
                DateTime endTime = DateTime.Now;
                TimeSpan ts = endTime - startTime;
                TMedianForm.execTime = ts;


                DateTime startTime1 = DateTime.Now;
                FilterImpl(inputImage2, outputImage2);
                DateTime endTime1 = DateTime.Now;
                TimeSpan ts1 = endTime1 - startTime1;
                TMedianForm.execTime1 = ts1;

                DateTime startTime2 = DateTime.Now;
                bilateral_filter(inputImage3, outputImage3, 6, 0.25);
                DateTime endTime2 = DateTime.Now;
                TimeSpan ts2 = endTime2 - startTime2;
                TMedianForm.execTime2 = ts2;
            }
            else
            {
                inputImage1.SaveTo(outputImage1);
                inputImage2.SaveTo(outputImage2);
                inputImage3.SaveTo(outputImage3);
            }
        }

        #endregion
    }
}
