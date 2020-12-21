using System.IO;
using System.Windows.Forms;

namespace kaikei0_c
{
    static class NetMeg
    {//通信時のメッセージ
     //応答メッセージ
        public const string ANS_SUCCESS = "ANS_SUCCESS";
        public const string ANS_DATE_ERROR = "ANS_DATE_ERROR";
        public const string ANS_DATE_NOT_HIT = "ANS_DATE_NOT_HIT";
        public const string ANS_DATE_VIOLATION = "ANS_DATE_VIOLATION";
        public const string ANS_EXCEL_ERROR = "ANS_EXCEL_ERROR";
        public const string ANS_ERROR = "ANS_ERROR";
        //要求メッセージ(1)
        public const string RESEARCH = "RESEARCH";
        public const string CHG = "CHG";
        public const string DEL = "DEL";
        public const string COMMIT = "COMMIT";
        public const string LOGIN = "LOGIN";
        public const string LOGOUT = "LOGOUT";
        public const string INSORT = "INSORT";
        //要求メッセージ(2)
        public const string REQ_KEIHI_KOU = "REQ_KEIHI_KOU";
        public const string REQ_KEIHI = "REQ_KEIHI";
        public const string REQ_KAKEURI = "REQ_KAKEURI";
        public const string REQ_KAISYA = "REQ_KAISYA";
        public const string REQ_PAYD = "REQ_PAYD";
        public const string REQ_UNPAYD = "REQ_UNPAYD";
        public const string REQ_SYOUHI_TAX = "REQ_SYOUHI_TAX";
        public const string REQ_SYOTOKU_TAX = "REQ_SYOTOKU_TAX";
        public const string REQ_SYOTOKU = "REQ_SYOTOKU";
        //要求メッセージ(3)
        public const string WREQ_KOU = "WREQ_KOU";
        public const string WREQ_MOOM = "WREQ_MOOM";
        public const string WREQ_ALL = "WREQ_ALL";
        public const string WREQ_YERA = "WREQ_YERA";
        public const string WREQ_TRUE = "WREQ_TRUE";
        //const std::string MAKE_EXSEL = "MAKE_EXCEL";
    }
    static class Space{
        static public SysConf Conf;
        static public bool IsLogin;
        static public bool IsSearch;
        static public bool IsInsert;
        static public string SearchPattern;
        static public bool IsSearchGroup;
        static public bool IsTable;
        static public DataGridView DateBaf;
        
        static Space()
        {
            Conf = new SysConf();
            IsLogin = false;
            IsSearch = false;
            IsInsert = false;
            SearchPattern = "";
            IsSearchGroup = false;
            IsTable = false;
            DateBaf = null;
        }
    }
    class SysConf//設定情報
    {
        //ファイル形式: エクセル出力先パス,ホスト名（IPアドレス）,ユーザー名

        public string ExcelExport;//エクセル出力先パス
        public string HostName;//ホスト側（サーバ側）のホスト名（IPアドレス）
        public string UesrName;//ユーザー名
        private const string filePass = "conf.txt";

        //ファイルから設定情報を取得する
        public SysConf()
        {
            ExcelExport = "";//1
            HostName = "";//2
            UesrName = "";//3
            //ファイルが存在するときのみ設定情報を取得する
            if (File.Exists(filePass))
            {
                //データ取得
                StreamReader st = new StreamReader(filePass);
                string data = st.ReadToEnd();
                st.Close();
                if (data.Length == 0) return;
                //エクセルの出力先パス取得
                int findIdx = data.IndexOf(",");
                if (findIdx != 0)
                    ExcelExport = data.Substring(0, findIdx);
                //IPアドレス取得
                data = data.Substring(findIdx + 1, data.Length - (findIdx+1));
                findIdx = data.IndexOf(",");
                if (findIdx != 0)
                    HostName = data.Substring(0, findIdx);
                //ユーザ名取得
                data = data.Substring(findIdx + 1, data.Length - (findIdx + 1));
                for(int i = 0; i < data.Length; i++)
                {
                    if (data[i] == '\r' || data[i] == '\n')
                        break;
                    UesrName += data[i];
                }
            }
        }

        //設定情報をファイルに反映させる        
        ~SysConf()
        {
            //ファイルが存在するときのみファイルを削除する
            if (File.Exists(filePass))
                File.Delete(filePass);
            //ファイルに出力
            StreamWriter st = new StreamWriter(filePass);
            st.WriteLine(ExcelExport + "," + HostName + "," + UesrName);
            st.Close();
        }

        //ログインするのに必要な情報が設定されてるか
        public bool HaveConf()
        {
            return HostName != "" && UesrName != "";
        }
    }
}
