using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace kaikei0_c
{
    static class Cnet
    {
        /*必要なやつ*/
        private static System.Net.Sockets.TcpClient m_Tcp;
        private static System.Net.Sockets.NetworkStream m_NetworkStream;
        /*ここで使う定数*/
        private const int PORT = 5555;
        private const int TimeOut = 10000;
        private const int BAF_SIZE = 256;
        
        /*接続*/
        public static void Connect(string ip)
        {
            m_Tcp = new System.Net.Sockets.TcpClient(ip, PORT);//ソケット生成
            m_NetworkStream = m_Tcp.GetStream();//対応するストリームの取得
            /*タイムアウトの設定*/
            m_NetworkStream.ReadTimeout = TimeOut;
            m_NetworkStream.WriteTimeout = TimeOut;
        }
        
        private static string m_Get(int m_size= BAF_SIZE)
        {
            System.IO.MemoryStream ms = new System.IO.MemoryStream();
            byte[] baf = new byte[m_size];
            int size;
            /*受信*/
            do
            {
                size = m_NetworkStream.Read(baf, 0, baf.Length);//読み込み
                if (size == 0) return null;//エラー
                ms.Write(baf, 0, size);//メモリに書き込み
            } while (m_NetworkStream.DataAvailable/* || baf[size - 1] != '\0'*/);

            /*終了処理*/
            string rtn = System.Text.Encoding.UTF8.GetString(ms.GetBuffer(), 0, (int)ms.Length);//文字列の取得
            ms.Close();
            return rtn;
        }

        private static void m_Send(string date)
        {
            /*文字列をバイトに変換*/
            //もとはUTF-8 (Shift_JIS)
            System.Text.Encoding enc = System.Text.Encoding.UTF8;
            byte[] sdb = enc.GetBytes(date);
            /*送信*/
            m_NetworkStream.Write(sdb,0,sdb.Length);
        }

        /*受信*/
        public static string Get()
        {
            string num, date;
            int size;
            //受け取りたいデータのサイズの取得(応答メッセージも送る)
            num = m_Get(4);
            size = int.Parse(num);
            m_Send(NetMeg.ANS_SUCCESS);
            //ここで取得する
            date = m_Get(size);
            m_Send(NetMeg.ANS_SUCCESS);
            return date;
        }

        /*送信*/
        public static void Send(string date)
        {
            System.Text.Encoding enc = System.Text.Encoding.UTF8;
            byte[] sdb = enc.GetBytes(date);
            //送りたいデータのサイズの送信
            m_Send(sdb.Length.ToString("C").Substring(1, sdb.Length.ToString("C").Length - 1));
            m_Get();
            //データの送信
            m_Send(date);
            m_Get();
        }

        /*切断*/
        public static void CutEnd()
        {
            /*閉じるものを閉じる*/
            m_NetworkStream.Close();
            m_Tcp.Close();
        }
    }
}
