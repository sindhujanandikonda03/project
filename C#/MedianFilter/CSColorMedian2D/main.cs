using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace CSColorMedian2D
{

    static class main
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new TMedianForm());
        }
    }
}
