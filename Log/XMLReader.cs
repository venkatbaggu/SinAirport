using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Sins.Helper
{
    /// <summary>
    /// 摄像机设置类
    /// </summary>
    public class CameraSettings
    {
        /// <summary>
        /// 地址名
        /// </summary>
        public string ServerName { set; get; }
        /// <summary>
        /// 摄像机IP
        /// </summary>
        public string IP { set; get; }
        /// <summary>
        /// 摄像机端口
        /// </summary>
        public int Port { set; get; }
        /// <summary>
        /// 用户名
        /// </summary>
        public string UserName { set; get; }
        /// <summary>
        /// 用户密码
        /// </summary>
        public string Password { set; get; }
        /// <summary>
        /// 摄像机通道
        /// </summary>
        public List<int> Channels { set; get; }
    }

    /// <summary>
    /// XML文件解析类
    /// </summary>
    public class XmlReader
    {
        /// <summary>
        /// 摄像机设置信息
        /// </summary>
        private static Dictionary<int, CameraSettings> camerasInfo = new Dictionary<int, CameraSettings>();
        public static Dictionary<int, CameraSettings> CamerasInfo { get { return camerasInfo; } }
        /// <summary>
        /// 配置文件名
        /// </summary>
        private static string fileName = "CameraSet.xml";
        /// <summary>
        /// 摄像机最大数量
        /// </summary>
        public const int MaxCount = 9;

        public static bool GetCameraInfo()
        {
            if (!System.IO.File.Exists(fileName)) { return false; }
            List<CameraSettings> list = new List<CameraSettings>();
            bool success = true;
            try
            {
                XElement emt = XElement.Load(fileName);
                int Count = emt.Elements("Server").Count() > MaxCount ? MaxCount : emt.Elements("Server").Count();
                int idx = 0;
                foreach (XElement e in emt.Elements("Server"))
                {
                    if (idx > Count) break;
                    try
                    {
                        camerasInfo.Add(idx++, GetCameraInfo(idx, e));
                    }
                    catch
                    {
                        continue;
                    }
                }
            }
            catch
            {
                success = false;
            }

            return !success || camerasInfo.Count == 0 ? false : true;
        }
        /// <summary>
        /// 获取摄像机配置信息
        /// </summary>
        /// <param name="idx"></param>
        /// <param name="emt"></param>
        /// <returns>摄像机设置</returns>
        private static CameraSettings GetCameraInfo(int idx, XElement emt)
        {
            try
            {
                string channel = ReadAttrib(idx, "Channel", emt);
                List<int> temp = null;
                if (channel != null)
                {
                    string[] strs = channel.Split(new string[] { "," }, StringSplitOptions.RemoveEmptyEntries);
                    if (strs != null && strs.Length > 0) try { temp = strs.Select(c => int.Parse(c)).ToList(); }
                        catch { temp = new List<int> { 1 }; }
                    else temp = new List<int> { 1 };
                }
                return new CameraSettings
                {
                    ServerName = ReadAttrib(idx, "Name", emt),
                    IP = ReadAttrib(idx, "IP", emt),
                    Port = int.Parse(ReadAttrib(idx, "Port", emt)),
                    UserName = ReadAttrib(idx, "UserName", emt),
                    Password = ReadAttrib(idx, "Password", emt),
                    Channels = temp
                };
            }
            catch
            {
                return null;
            }
        }
        /// <summary>
        /// 读取属性
        /// </summary>
        ///  <param name="idx">参数序号</param>
        /// <param name="attribName">属性名</param>
        /// <param name="emt">XML对象</param>
        /// <returns>属相字符串</returns>
        private static string ReadAttrib(int idx, string attribName, XElement emt)
        {
            try
            {
                if (string.IsNullOrEmpty(emt.Attribute(attribName).Value)) return "";
                return emt.Attribute(attribName).Value.ToString();
            }
            catch
            {
                Write(idx, attribName);
                return null;
            }
        }
        /// <summary>
        /// 写错误日志
        /// </summary>
        /// <param name="idx"></param>
        /// <param name="attrib"></param>
        private static void Write( int idx,string attrib)
        {
#if DEBUG
            WriteLog.Write(string.Format("读取第{0}个摄像机参数 \"{1}\" 数据失败，请检查CameraSetting.xml文件设置。", idx,attrib));
#endif
        }
    }
}
