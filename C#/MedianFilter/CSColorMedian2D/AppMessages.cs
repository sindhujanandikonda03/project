
using System;
using System.Collections.Generic;
using System.Text;

namespace CSColorMedian2D
{

    class TStartProcessingMessage : SergejusZ.Base.Multithreading.TVoidMessage
    {
        public const int ID = 1000;
        public TStartProcessingMessage()
            : base(ID)
        {
        }
    }

    class TProcessMessage : SergejusZ.Base.Multithreading.TVoidMessage
    {
        public const int ID = 1002;
        public TProcessMessage()
            : base(ID)
        {
        }
    }

    class TFilterOnMessage : SergejusZ.Base.Multithreading.TVoidMessage
    {
        public const int ID = 1004;
        public TFilterOnMessage()
            : base(ID)
        {
        }
    }

    class TFilterOffMessage : SergejusZ.Base.Multithreading.TVoidMessage
    {
        public const int ID = 1005;
        public TFilterOffMessage()
            : base(ID)
        {
        }
    }

    class TWindowSizeXMessage : SergejusZ.Base.Multithreading.TClassMessage<int>
    {
        public const int ID = 1006;
        public TWindowSizeXMessage(int value)
            : base(ID, value)
        {
        }
    }

    class TWindowSizeYMessage : SergejusZ.Base.Multithreading.TClassMessage<int>
    {
        public const int ID = 1007;
        public TWindowSizeYMessage(int value)
            : base(ID, value)
        {
        }
    }
}
