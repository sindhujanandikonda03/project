using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SergejusZ.Base;
//using System.IO;


namespace CSColorMedian2D
{
    public partial class TMedianForm : Form
    {
        #region Local Variables

        private System.Drawing.Bitmap m_bitmap1 = null;
        private System.Drawing.Bitmap m_bitmap2 = null;
        private System.Drawing.Bitmap m_bitmap3 = null;

        private Random m_random = new Random((DateTime.Now.Hour * 3600 + DateTime.Now.Minute * 60 + DateTime.Now.Second) * 1000 + DateTime.Now.Millisecond);
        //private TColorMedianFilter2D m_medianFilter = new TMedianFilter2D(3, 3);
        public delegate void SetBitmapMethod(TColorImage image1, TColorImage image2, TColorImage image3);
        public SetBitmapMethod myDelegate;
        private TMedianThread m_thread = null;
        #endregion

        // path variable
        public static String imagePath;
        // time var
        public static TimeSpan execTime;
        // time var1
        public static TimeSpan execTime1;
        // time var2
        public static TimeSpan execTime2;
        // filter window size
        public static int filterWindowSize = 9;
        


        public TMedianForm()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            myDelegate = SetBitmap;
            m_thread = new TMedianThread(this);
            m_thread.Start();

        }

        public System.Drawing.Bitmap CreateGrayScaleBitmap(int width, int height)
        {
            System.Drawing.Bitmap bmp = new System.Drawing.Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format8bppIndexed);
            System.Drawing.Imaging.ColorPalette palette = bmp.Palette;
            for (int j = 0; j < 256; j++)
                palette.Entries[j] = Color.FromArgb(j, j, j);
            bmp.Palette = palette;
            return bmp;
        }


        public void SetBitmap(TColorImage image1, TColorImage image2, TColorImage image3)
        {
            if (m_bitmap1 == null)
                m_bitmap1 = new System.Drawing.Bitmap(image1.Width, image1.Height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            if (m_bitmap2 == null)
                m_bitmap2 = new System.Drawing.Bitmap(image2.Width, image2.Height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            if (m_bitmap3 == null)
                m_bitmap3 = new System.Drawing.Bitmap(image2.Width, image2.Height, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            
            image1.SaveToBitmap(m_bitmap1);
            image2.SaveToBitmap(m_bitmap2);
            image3.SaveToBitmap(m_bitmap3);
            TImagePool.Enqueue(image1);
            //pictureBox1.Image = m_bitmap;
            pictureBox2.Image = m_bitmap1;
            TImagePool.Dequeue();
            TImagePool.Enqueue(image2);
            pictureBox3.Image = m_bitmap2;
            pictureBox4.Image = m_bitmap3;


            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            openFileDialog1.ShowDialog();
            imagePath = openFileDialog1.FileName;

            pictureBox1.Image = Image.FromFile(openFileDialog1.FileName);

        }
   

        private void TMedianForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // post shutdowm message to thread
            m_thread.PostMessage(new SergejusZ.Base.Multithreading.TShutDownMessage());
            // and wait for thread termination
            m_thread.WaitFor();
        }

   
        private void btn_filter_Click(object sender, EventArgs e)
        {
            m_thread.PostMessage(new TStartProcessingMessage());
            m_thread.PostMessage(new TFilterOnMessage());
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

       

        private void button2_Click(object sender, EventArgs e)
        {

            double i = Convert.ToInt32(execTime.Milliseconds);
            i *= 3;
            label3.Text = i.ToString();
            //label3.Text = execTime.Milliseconds.ToString();
            
    
        }

        private void button3_Click(object sender, EventArgs e)
        {

            //label4.Text = execTime1.Milliseconds.ToString();
            double i = Convert.ToInt32(execTime1.Milliseconds);
            i *= 3;
            label4.Text = i.ToString();


        }

       

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (m_thread != null && textBox1.Text != null)
            {
                m_thread.PostMessage(new TWindowSizeXMessage(Convert.ToInt32(textBox1.Text)));
                //m_thread.PostMessage(new TWindowSizeYMessage(Convert.ToInt32(textBox2.Text)));

            }
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            if (m_thread != null && textBox2.Text != null)
            {
               // m_thread.PostMessage(new TWindowSizeYMessage(Convert.ToInt32(textBox1.Text)));
                m_thread.PostMessage(new TWindowSizeYMessage(Convert.ToInt32(textBox2.Text)));

            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //label11.Text = execTime2.TotalMilliseconds.ToString();


            double i = Convert.ToInt32(execTime2.Milliseconds);
            i *= 3;
            label11.Text = i.ToString();
        }

  

     

   

      

    

    }
}
