using System;
using System.Collections.Generic;
using MySql.Data;
using MySql.Data.MySqlClient;

namespace Sins.SQL.MySql
{
    class  Helper
    {
        private string server = "127.0.0.1";
        private string dbName = "airportdb";
        private string userName = "root";
        private string password = "shiyanshi236";
        /// <summary>
        /// 构造
        /// </summary>
        /// <param name="server"></param>
        /// <param name="dbName"></param>
        /// <param name="userName"></param>
        /// <param name="password"></param>
        public Helper(string server, string dbName, string userName, string password)
        {
            this.server = server;
            this.dbName = dbName;
            this.userName = userName;
            this.password = password;
        }
        /// <summary>
        /// 执行SQL语句
        /// </summary>
        /// <param name="sql"></param>
        /// <returns></returns>
        public int ExecSQL(string sql)
        {
            MySqlCommand cmd = this.GetCommand(sql, null, null);
            return cmd.ExecuteNonQuery();
        }
        /// <summary>
        /// 执行存储过程
        /// </summary>
        /// <param name="porcName">存储过程名</param>
        /// <param name="paramList">参数列表</param>
        /// <param name="valueList">值列表</param>
        public System.Data.DataTable ExeProcudere(string porcName,string[] paramList,object[] valueList)
        {
            MySqlCommand cmd = this.GetCommand(porcName, paramList, valueList);
            if (cmd == null) return null;
            cmd.CommandType = System.Data.CommandType.StoredProcedure;
            System.Data.DataTable dt = new System.Data.DataTable();
            MySqlDataAdapter dpa = null;
            try
            {
                dpa = new MySqlDataAdapter(cmd);
                dpa.Fill(dt);
            }
            catch
            {

            }
            finally
            {
                dpa.Dispose();
            }
            return dt;
        }
        /// <summary>
        /// 获取Command
        /// </summary>
        /// <param name="sqlcmd"></param>
        /// <param name="paramList"></param>
        /// <param name="valueList"></param>
        /// <param name="isSql"></param>
        /// <returns></returns>
        private MySqlCommand GetCommand(string sqlcmd, string[] paramList = null, object[] valueList = null, bool isSql = true)
        {
            MySqlConnection conn = this.GetConn();
            if (conn == null) return null;
            MySqlParameter[] parms = this.GetParmers(paramList, valueList);
            try
            {
                MySqlCommand cmd = new MySqlCommand(sqlcmd, conn);
                if (parms != null) cmd.Parameters.AddRange(parms);
                return cmd;
            }
            catch
            {
                return null;
            }
        }
        private MySqlConnection GetConn()
        {
            MySqlConnection conn = null;
            try
            {
                string connstring = string.Format("Data Source={0};Initial Catalog={1};User ID={2};Password={3};",this. server, this.dbName,  this.userName, this.password);
                conn = new MySqlConnection(connstring);
                conn.Open();
                return conn;
            }
            catch
            {
                return conn;
            }
        }
        /// <summary>
        /// 创建参数
        /// </summary>
        /// <param name="paramList"></param>
        /// <param name="valueList"></param>
        /// <returns></returns>
        private  MySqlParameter[] GetParmers(string[] paramList=null,object[] valueList=null)
        {
            if (paramList == null || valueList == null) return null;
            int count = paramList.Length > valueList.Length ? valueList.Length : paramList.Length;
            MySqlParameter[] parms = new MySqlParameter[count];
            for(int i=0;i<count;i++)
            {
                parms[i] = new MySqlParameter { ParameterName = string.Format("@{0}", paramList[i]), Value = valueList[i] };
            }
            return parms;
        }
    }
}
