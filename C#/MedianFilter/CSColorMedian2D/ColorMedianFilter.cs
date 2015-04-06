using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSColorMedian2D
{
    class TColorMedianFilter2D
    {
        #region Local Variables
        private TMedianFilter2D m_filter = null;
        private TImage m_red1 = null;
        private TImage m_red2 = null;
        private TImage m_red3 = null;
        private TImage m_red4 = null;
        private TImage m_red5 = null;
        private TImage m_red6 = null;
        private TImage m_green1 = null;
        private TImage m_green2 = null;
        private TImage m_green3 = null;
        private TImage m_green4 = null;
        private TImage m_green5 = null;
        private TImage m_green6 = null;
        private TImage m_blue1 = null;
        private TImage m_blue2 = null;
        private TImage m_blue3 = null;
        private TImage m_blue4 = null;
        private TImage m_blue5 = null;
        private TImage m_blue6 = null;
        #endregion

        #region Ctors

        public TColorMedianFilter2D()
        {
            m_filter = new TMedianFilter2D();
        }

        public TColorMedianFilter2D(int width, int height)
        {
            m_filter = new TMedianFilter2D(width, height);
        }

        #endregion

        #region Properties

        public bool Enabled
        {
            get
            {
                return m_filter.Enabled;
            }
            set
            {
                m_filter.Enabled = value;
            }
        }

        public int WindowWidth
        {
            get
            {
                return m_filter.WindowWidth;
            }
            set
            {
                m_filter.WindowWidth = value;
            }
        }

        public int WindowHeight
        {
            get
            {
                return m_filter.WindowHeight;
            }
            set
            {
                m_filter.WindowHeight = value;
            }
        }


        #endregion


        #region Methods

        public void Process(TColorImage inputImage1, TColorImage outputImage1, TColorImage inputImage2, TColorImage outputImage2, TColorImage inputImage3, TColorImage outputImage3)
        {
            m_red1 = inputImage1.Red;
            m_green1 = inputImage1.Green;
            m_blue1 = inputImage1.Blue;

            m_red2 = outputImage1.Red;
            m_green2 = outputImage1.Green;
            m_blue2 = outputImage1.Blue;

            m_red3 = inputImage2.Red;
            m_green3 = inputImage2.Green;
            m_blue3 = inputImage2.Blue;

            m_red4 = outputImage2.Red;
            m_green4 = outputImage2.Green;
            m_blue4 = outputImage2.Blue;

            m_red5 = inputImage3.Red;
            m_green5 = inputImage3.Green;
            m_blue5 = inputImage3.Blue;

            m_red6 = outputImage3.Red;
            m_green6 = outputImage3.Green;
            m_blue6 = outputImage3.Blue;

            System.Threading.Thread tRed = new System.Threading.Thread(new System.Threading.ParameterizedThreadStart(ProcessRed));
            System.Threading.Thread tGreen = new System.Threading.Thread(new System.Threading.ParameterizedThreadStart(ProcessGreen));
            System.Threading.Thread tBlue = new System.Threading.Thread(new System.Threading.ParameterizedThreadStart(ProcessBlue));

            // run 3 independent fast median filters on red, green and blue images
            tRed.Start(this);
            tGreen.Start(this);
            tBlue.Start(this);

            // wait for processing termination
            tRed.Join();
            tGreen.Join();
            tBlue.Join();
        }

        public void ProcessRedImage()
        {
            m_filter.Process(m_red1, m_red2, m_red3, m_red4, m_red5, m_red6);
        }

        public void ProcessGreenImage()
        {
            m_filter.Process(m_green1, m_green2, m_green3, m_green4, m_green5, m_green6);
        }

        public void ProcessBlueImage()
        {
            m_filter.Process(m_blue1, m_blue2, m_blue3, m_blue4, m_blue5, m_blue6);
        }

        public static void ProcessRed(object data)
        {
            ((TColorMedianFilter2D)data).ProcessRedImage();
        }

        public static void ProcessGreen(object data)
        {
            ((TColorMedianFilter2D)data).ProcessGreenImage();
        }

        public static void ProcessBlue(object data)
        {
            ((TColorMedianFilter2D)data).ProcessBlueImage();
        }
        #endregion
    }
}
