using System;
using System.Collections.Generic;
using System.Text;

namespace CSColorMedian2D
{


    class TMedianThread : SergejusZ.Base.Multithreading.TBaseThread
    {
        #region Local Variables
        private System.Drawing.Bitmap m_bitmap = null;
       // private System.Drawing.Bitmap m_bitmap = null;
        private TColorImage m_image1 = null;
        private TColorImage m_image2 = null;
        private TColorImage m_image3 = null;
        private TMedianForm m_form = null;
        private TColorMedianFilter2D m_filter2d = new TColorMedianFilter2D(3, 3);
        private bool m_working = false;
        #endregion

        #region Ctors

        public TMedianThread(TMedianForm f)
            : base()
        {
            m_form = f;
        }

        #endregion

        protected override SergejusZ.Base.Common.Queues.IQueue<SergejusZ.Base.Multithreading.TBaseMessage> CreateQueue()
        {
            return new SergejusZ.Base.Common.Queues.TQueue<SergejusZ.Base.Multithreading.TBaseMessage>();
        }


        protected override bool DoPreambulae()
        {
            m_filter2d.Enabled = false;
            //m_bitmap = new System.Drawing.Bitmap("d:/test.jpg");
            //m_image1 = new TColorImage(m_bitmap.Width, m_bitmap.Height);
            return true;
        }

        protected override bool DoPostscriptum()
        {
            return true;
        }


        public override void ProcessMessage(SergejusZ.Base.Multithreading.TBaseMessage msg)
        {
            switch (msg.GetID)
            {
                case SergejusZ.Base.Multithreading.TShutDownMessage.ID:
                    SetStopped();
                    m_working = false;
                    break;

                case TStartProcessingMessage.ID:
                    m_working = true;
                    DoWork();
                    break;

                case TProcessMessage.ID:
                    DoWork();
                    break;

                case TFilterOnMessage.ID:
                    m_filter2d.Enabled = true;
                    break;

                case TFilterOffMessage.ID:
                    m_filter2d.Enabled = false;
                    break;

                case TWindowSizeXMessage.ID:
                    m_filter2d.WindowWidth = ((TWindowSizeXMessage)msg).Value;
                    break;

                case TWindowSizeYMessage.ID:
                    m_filter2d.WindowHeight = ((TWindowSizeYMessage)msg).Value;
                    break;
            }
        }

        private void DoWork()
        {
            if (m_working)
            {
               /* if (m_bitmap != null)
                {
                    m_bitmap = null;
                    m_image1 = null;
                    //image2 = null;
                }*/
                m_bitmap = new System.Drawing.Bitmap(TMedianForm.imagePath);
                m_image1 = new TColorImage(m_bitmap.Width, m_bitmap.Height);
                m_image2 = new TColorImage(m_bitmap.Width, m_bitmap.Height);
                m_image3 = new TColorImage(m_bitmap.Width, m_bitmap.Height);
                m_image1.LoadFromBitmap(m_bitmap);
                m_image2.LoadFromBitmap(m_bitmap);
                m_image3.LoadFromBitmap(m_bitmap);
             
                TColorImage image2 = TImagePool.CreateImage(m_image1.Width, m_image1.Height);
                TColorImage image3 = TImagePool.CreateImage(m_image2.Width, m_image2.Height);
                TColorImage image4 = TImagePool.CreateImage(m_image3.Width, m_image3.Height);
                m_filter2d.Process(m_image1, image2, m_image2, image3, m_image3, image4);
                m_form.BeginInvoke(m_form.myDelegate, image2, image3, image4);
                //image2 = null;
                
                this.PostMessage(new TProcessMessage());
            }
        }

    }

}
