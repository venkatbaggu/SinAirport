using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Sins.Helper
{
    public class WriteLog
    {
        private static string path = AppDomain.CurrentDomain + @"ErrorLog\error.log";
        private static StreamWriter ws = null;
        private static bool Exception = false;
        public static  void Write(string msg)
        {
#if DEBUG
            try
            {
                if (WriteLog.ws == null) ws = new StreamWriter(path, true);
                WriteLog.ws.WriteLine(msg);
            }
            catch
            {
                WriteLog.Exception = true;
            }
            finally
            {
              WriteLog.Close();
            }
#endif
        }

        public static void Close()
        {
            if (WriteLog.ws != null && WriteLog.Exception)
              {
                  WriteLog.ws.Close();
                  WriteLog.ws.Dispose();
                  GC.Collect();
              }
        }
    }
}
