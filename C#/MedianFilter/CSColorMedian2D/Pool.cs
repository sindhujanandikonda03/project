using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSColorMedian2D
{
    class TPool<T> 
    {
        protected static Queue<T> m_pool = new Queue<T>();

        #region Methods

        public static void Enqueue(T obj)
        {
            System.Threading.Monitor.Enter(m_pool);
            try
            {
                m_pool.Enqueue(obj);
            }
            catch (Exception ex)
            {
                throw ex;
            }

            finally
            {
                System.Threading.Monitor.Exit(m_pool);
            }
        }


        public static T Dequeue()
        {
            System.Threading.Monitor.Enter(m_pool);
            try
            {
                if (m_pool.Count > 0)
                    return m_pool.Dequeue();
                return default(T);
            }
            catch (Exception ex)
            {
                throw ex;
            }
            finally
            {

                System.Threading.Monitor.Exit(m_pool);
            }
        }

        #endregion

    }
}
