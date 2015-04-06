using System;
using System.Collections.Generic;
using System.Text;

namespace CSColorMedian2D
{
    class TMedianFilter2D : TBaseFilter2D
    {

        #region Local Variables
        protected int m_width = 3;  // default filter window width
        protected int m_height = 3;  // default filter window height
        double kernelRadius;
        double[,] kernelD;
        double[] gaussSimilarity;
        #endregion

        #region Ctors
        public TMedianFilter2D()
            : base()
        {
        }

        public TMedianFilter2D(int width, int height)
            : base()
        {
            m_width = width;
            m_height = height;
        }
        #endregion

        #region Properties

        public int WindowWidth
        {
            get
            {
                return m_width;
            }
            set
            {
                m_width = value;
            }
        }

        public int WindowHeight
        {
            get
            {
                return m_height;
            }
            set
            {
                m_height = value;
            }
        }

        #endregion


        #region Methods

        
        //implement the simple median filter
        public override void naiveFilter(TImage inputImage, TImage outputImage)
        {
            inputImage.SaveTo(outputImage);

            int height = inputImage.Height;
            int width = inputImage.Width;
            int size = m_height * m_width;
            int hh = m_height / 2;
            int hw = m_width / 2;

            for (int j = hh; j < height - hh; j++)
            {
                for (int i = hw; i < width - hw; i++)
                {
        
                    int k = 0;
                    int[] window;
                    window = new int[size];
                    for (int jj = j - hh; jj < j + m_height - hh; ++jj)
                        for (int ii = i - hw; ii < i + m_width - hw; ++ii)
                            window[k++] = inputImage.getValue(jj, ii);
                    //   Order elements (only half of them)
                    for (int m = 0; m < size / 2 + 1; ++m)
                    {
                        int min = m;
                        for (int n = m + 1; n < size; ++n)
                            if (window[n] < window[min])
                                min = n;
                        //   Put found minimum element in its place
                        int temp = window[m];
                        window[m] = window[min];
                        window[min] = temp;
                    }
                    outputImage.setValue(j, i, (byte)window[size / 2]);
                }
            }
            
        }


        //Huang's algorithm
        public override void FilterImpl(TImage inputImage, TImage outputImage)
        {
      
           
            byte med;
            int delta_l;
            int[] hist = new int[256];
            int row, col;
            int directionValue = 1;
            int wx2 = m_width / 2;
            int wy2 = m_height / 2;
            int middle = (m_height * m_width + 1) / 2;

            for (int j = 0; j < 256; j++)
                hist[j] = 0;

            // Histogram For (0,0)-element
            for (row = -wy2; row <= wy2; row++)
                for (col = -wx2; col <= wx2; col++)
                    hist[inputImage.getValue(row, col)]++;

            // Median
            int m = 0;
            for (med = 0; med <= 255; med++)
            {
                m += hist[med];
                if (m >= middle)
                    break;
            }
            delta_l = m - hist[med];

            // Now, Median Is Defined For (0,0)-element
            // Begin Scanning: direction - FORWARD
            outputImage.setValue(0, 0, med);

            int prev, next;

            // main loop
            col = 1;
            for (row = 0; row < inputImage.Height; row++)
            {
                while (col >= 0 && col < inputImage.Width)
                {
                    // Determine Previous and Next Columns
                    // Pay Attention To Defined Direction !!!
                    prev = col - directionValue * (wx2 + 1);
                    next = col + directionValue * wx2;
                    // Now Change Old Histogram
                    // New Histogram
                    // delete previous
                    for (int r = row - wy2; r <= row + wy2; r++)
                    {
                        byte value_out = inputImage.getValue(r, prev);
                        byte value_in = inputImage.getValue(r, next);
                        if (value_out == value_in)
                            continue;
                        hist[value_out]--;
                        if (value_out < med)
                            delta_l--;
                        hist[value_in]++;
                        if (value_in < med)
                            delta_l++;
                    }

                    // Update new median
                    if (delta_l >= middle)
                    {
                        while (delta_l >= middle)
                        {
                            if (hist[--med] > 0)
                                delta_l -= hist[med];
                        }
                    }
                    else
                    {
                        while (delta_l + hist[med] < middle)
                        {
                            if (hist[med] > 0)
                                delta_l += hist[med];
                            med++;
                        }
                    }
                    outputImage.setValue(row, col, med);
                    // end of column loop
                    col += directionValue;
                }

                if (row < inputImage.Height - 1)
                {
                    // go back to the last/first pixel of the line
                    col -= directionValue;
                    // change direction to the opposite
                    directionValue *= -1;

                    // Shift Down One Line
                    prev = row - wy2;
                    next = row + wy2 + 1;


                    for (int c = col - wx2; c <= col + wx2; c++)
                    {
                        byte value_out = inputImage.getValue(prev, c);
                        byte value_in = inputImage.getValue(next, c);
                        if (value_out == value_in)
                            continue;
                        hist[value_out]--;
                        if (value_out < med)
                            delta_l--;
                        hist[value_in]++;
                        if (value_in < med)
                            delta_l++;
                    }

                    if (delta_l >= middle)
                    {
                        while (delta_l >= middle)
                        {
                            if (hist[--med] > 0)
                                delta_l -= hist[med];
                        }
                    }
                    else
                    {
                        while (delta_l + hist[med] < middle)
                        {
                            if (hist[med] > 0)
                                delta_l += hist[med];
                            med++;
                        }
                    }
                    outputImage.setValue(row + 1, col, med);
                    col += directionValue;
                }
            }
            
        }


        //Bilateral filter
        double gauss(double sigma, int x, int y) { return Math.Exp(-((double)(x * x + y * y) / (double)(2 * sigma * sigma))); }

        double similarity(int p, int s) { return gaussSimilarity[Math.Abs(p - s)]; /*return Math.Exp(-(( Math.Abs(p-s)) /  2 * sigmaR * sigmaR));*/   }

        double getSpatialWeight(int m, int n, int i, int j) { return kernelD[(int)(i - m + kernelRadius), (int)(j - n + kernelRadius)]; }

        void BilateralFilter(TImage inputImage, double sigmaD, double sigmaR)
        {

            int sigmaMax = (int)Math.Max(sigmaD, sigmaR);
            kernelRadius = Math.Ceiling((double)2 * sigmaMax);
            double twoSigmaRSquared = 2 * sigmaR * sigmaR;

            int kernelSize = (int)(kernelRadius * 2 + 1);
            kernelD = new double[kernelSize, kernelSize];

            int center = (kernelSize - 1) / 2;
            for (int x = -center; x < -center + kernelSize; x++)
            {
                for (int y = -center; y < -center + kernelSize; y++)
                {
                    kernelD[x + center, y + center] = gauss(sigmaD, x, y);
                }
            }

            gaussSimilarity = new double[256];
            for (int i = 0; i < 256; i++)
            {
                gaussSimilarity[i] = Math.Exp((double)-((i) / twoSigmaRSquared));
            }



        }

        bool isInsideBoundaries(int m, int n, TImage inputImage)
        {
            if (m > -1 && n > -1 && m < inputImage.Height && n < inputImage.Width)
                return true;
            else
                return false;
        }


        void runFilter(TImage inputImage, TImage outputImage)
        {
            for (int i = 0; i < inputImage.Height; i++)
            {
                for (int j = 0; j < inputImage.Width; j++)
                {
                    //
                    if (i > 0 && j > 0 && i < inputImage.Height && j < inputImage.Width)
                    {
                        double sum = 0;
                        double totalWeight = 0;
                        int intensityCenter = inputImage.getValue(i, j);

                        int mMax = i + (int)kernelRadius;
                        int nMax = j + (int)kernelRadius;
                        double weight;

                        for (int m = i - (int)kernelRadius; m < mMax; m++)
                        {
                            for (int n = j - (int)kernelRadius; n < nMax; n++)
                            {
                                if (isInsideBoundaries(m, n, inputImage))
                                {
                                    int intensityKernelPos = inputImage.getValue(m, n);
                                    weight = getSpatialWeight(m, n, i, j) * similarity(intensityKernelPos, intensityCenter);
                                    totalWeight += weight;
                                    sum += (weight * intensityKernelPos);
                                }
                            }
                        }
                        int newvalue = (int)Math.Floor(sum / totalWeight);
                        outputImage.setValue(i, j, (byte)newvalue);
                    }


                
                }
            }
        }



        public override void bilateral_filter(TImage inputImage, TImage outputImage, double sigma_s, double sigma_r)
        { /* sigma_s is the parameter for spatial distance Gaussian f 
             sigma_r is the parameter for intensity difference Gaussian
           */
            BilateralFilter(inputImage, 6, 3);
            runFilter(inputImage, outputImage);
        }
        
        #endregion
    }
}


