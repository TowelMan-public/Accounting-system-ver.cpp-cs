using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace kaikei0_c
{
    public class DateGridBaf
    {
        private string[] m_clm;//カラム名
        private int m_clm_num;//カラムの個数
        private string[][] m_tuple;//レコード[カラム数][レコード数]
        private int m_r;//m_tupleのレコード数に対応
        private int m_c;//m_tupleのカラム数に対応(カーソル用)
        private int m_tup_num;//m_clmに対応

        /*コンストラクタ*/
        public DateGridBaf(string date)//カラム名とレコードとの境を",&"にする
        {
            m_clm = new string[10];
            m_tuple = new string[10][];
            for (int i = 0; i < 10; i++)
                m_tuple[i] = new string[1000];
            int a = 0;
            char sub;
            m_clm_num = 0;
            m_r = -1;
            m_c = 0;
            m_tup_num = 0;
            for (int i = 0;true; i++)//カラム名の取得
            {
                m_clm[i] = "";
                for (; a < date.Length; a++)
                {
                    sub = date[a];
                    if (sub == ',') break;
                    m_clm[i] += sub;
                }
                if (date[++a] == '&')
                {
                    m_clm_num = i + 1;
                    //++a;
                    break;
                }
            }
            for (; a < date.Length-1;)//データ取得
            {
                for (int i = 0; i < m_clm_num; i++)
                {
                    while (date[++a] != ','&&date[a]!='\n')
                        m_tuple[i][m_tup_num] += date[a];
                }
                ++m_tup_num;
            }
        }

        /*カラム名の取得*/
        public string GetClm()
        {
            if (m_r == -1 && m_c < m_clm_num)//取り出すべきカラム名がある
                return m_clm[m_c++];
            else return null;//既に全てのカラム名が取得されている
        }

        /*取得するべきカラム名があるか*/
        public bool HaveClm()
        {
            if (m_r == -1 && m_c < m_clm_num)//取り出すべきカラム名がある
                return true;
            else return false;//既に全てのカラム名が取得されている
        }

        /*レコード（タプル）の取得*/
        public string GetTuple()
        {
            int r, c;
            if (m_r == -1)//このクラスを生成して初めてレコード（タプル）を取得するとき
            {
                m_r = 0;
                m_c = 0;
            }
            if (m_r < m_tup_num)//取得するべきレコード(タプル）がある
            {
                r = m_r;
                c = m_c;
                if (++m_c == m_clm_num)
                {
                    ++m_r;
                    m_c = 0;
                }
                return m_tuple[c][r];
            }
            else return null;//取得するべきレコード（タプル）がない
        }

        /*取得するべきレコード（タプル）があるかどうか*/
        public bool HaveTuple()
        {
            if (m_r < m_tup_num || m_r == -1)//取得するべきレコード(タプル）がある
                return true;
            else return false;//取得するべきレコード（タプル）がない
        }

        public int CountClm()
        {
            return m_clm_num;
        }
    }
}
