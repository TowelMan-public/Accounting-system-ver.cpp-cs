using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static kaikei0_c.Space;
using Excel = Microsoft.Office.Interop.Excel;
using Microsoft.VisualBasic;
using System.Runtime.InteropServices;
using Microsoft.Office.Interop;

namespace kaikei0_c
{
    public partial class OutWhere : Form
    {
        private bool m_IsClosedByUeser;

        public OutWhere()
        {
            InitializeComponent();
        }

        private bool IsntInt(string str)
        {
            int i;
            return !int.TryParse(str, out i);
        }

        private bool IsntDate(string str)
        {
            DateTime dt;
            return !DateTime.TryParse(str, out dt);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //送信メッセージの作成
            Space.IsTable = false;
            string str;
            str = "SELECT_" + Space.SearchPattern + "(";
            int num=0;
            str += textBox5.Text + "," + textBox1.Text + "," + textBox2.Text + "," + textBox3.Text + "," + textBox4.Text + ",";
            if (checkBox7.Checked)
            {
                Space.IsSearchGroup = true;
                str += "#GROUP";
            }
            if (checkBox1.Checked)
                str += "#KOUMOKU";
            if (checkBox2.Checked)
                str += "#MOOM";
            if (checkBox3.Checked)
                str += "#ALL";
            if (checkBox4.Checked)
                str += "#YERA";
            if (checkBox5.Checked)
                str += "#TRUE";
            str += ')';
            //検索の実行
            Cnet.Connect(Space.Conf.HostName);
            Cnet.Send(str);
            if ((str = Cnet.Get()) == NetMeg.ANS_ERROR)
                goto ERROR;
            Cnet.CutEnd();
            //表の作成
            string str2;
            if(Space.SearchPattern== "KEIHI_KOU")
            {
                str2 = "番号,経費名,比率,有効/無効";
                str = str2 + ",&" + str;
            }
            else if (Space.SearchPattern == "KEIHI")
            {
                if (Space.IsSearchGroup)
                    str2 = "日付,項目,金額";
                else
                    str2 = "番号,日付,項目,金額";
                str = str2 + ",&" + str;
            }
            else if (Space.SearchPattern == "KAISYA")
            {
                str2 = "番号,社名,有効/無効";
                str = str2 + ",&" + str;
            }
            else if (Space.SearchPattern == "KAKEURI")
            {
                if (Space.IsSearchGroup)
                    str2 = "日付,社名,金額";
                else
                    str2 = "番号,日付,社名,金額,消費税,個人名,件名";
                str = str2 + ",&" + str;
            }
            else if (Space.SearchPattern == "PAYD")
            {
                if (Space.IsSearchGroup)
                    str2 = "日付,社名,金額";
                else
                    str2 = "番号,日付,社名,金額,消費税,個人名,件名";
                str = str2 + ",&" + str;
            }
            else if (Space.SearchPattern == "UNPAYD")
            {
                if (Space.IsSearchGroup)
                    str2 = "日付,社名,金額";
                else
                    str2 = "番号,日付,社名,金額,消費税,個人名,件名";
                str = str2 + ",&" + str;
            }
            else if (Space.SearchPattern == "SYOUHI_TAX")
            {
                if (Space.IsSearchGroup)
                    str2 = "日付,社名,金額";
                else
                    str2 = "番号,日付,社名,金額";
                str = str2 + ",&" + str;
            }
            else if (Space.SearchPattern == "SYOTOKU_TAX")
            {
                if (Space.IsSearchGroup)
                    str2 = "日付,社名,金額";
                else
                    str2 = "番号,日付,社名,金額";
                str = str2 + ",&" + str;
            }
            else if (Space.SearchPattern == "SYOTOKU")
            {
                if (Space.IsSearchGroup)
                    str2 = "日付,社名,金額";
                else
                    str2 = "番号,日付,社名,金額";
                str = str2 + ",&" + str;
            }
            DataGridView dateGrid = new DataGridView();
            DateGridBaf baf = new DateGridBaf(str);
            //表のメイン画面への反映
            ((Mian)(this.Owner)).CreateNewGrid(baf);
            Space.IsTable = true;
            //エクセル作成
            if (checkBox6.Checked){
                //パスが未指定
                if (Space.Conf.ExcelExport.Length == 0)
                {
                    MessageBox.Show("エクセルデータの出力先を設定してください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                    return;
                }

                baf = new DateGridBaf(str);
                //Excelオブジェクトの初期化
                Excel.Application ExcelApp = null;
                Excel.Workbooks wbs = null;
                Excel.Workbook wb = null;
                Excel.Sheets shs = null;
                Excel.Worksheet ws = null;

                //Excelシートのインスタンスを作る
                ExcelApp = new Excel.Application();
                wbs = ExcelApp.Workbooks;
                wb = wbs.Add();

                shs = wb.Sheets;
                ws = shs[1];
                ws.Select(Type.Missing);

                ExcelApp.Visible = false;
                //カラムのセット
                for (int i = 1; i <= baf.CountClm(); i++)
                {
                    // Excelのcell指定
                    Excel.Range w_rgn = ws.Cells;
                    Excel.Range rgn = w_rgn[1, i];
                    //セット
                    rgn.Value2 = "hoge";
                    // Excelのオブジェクトはループごとに開放する
                    Marshal.ReleaseComObject(w_rgn);
                    Marshal.ReleaseComObject(rgn);
                    w_rgn = null;
                    rgn = null;
                }
                //タプル（テーブル）のセット
                for (int i = 2; baf.HaveTuple(); i++)
                {
                    for (int j = 1; j <= baf.CountClm(); j++)
                    {
                        // Excelのcell指定
                        Excel.Range w_rgn = ws.Cells;
                        Excel.Range rgn = w_rgn[i, j];
                        //セット
                        rgn.Value2 = "hoge";
                        // Excelのオブジェクトはループごとに開放する
                        Marshal.ReleaseComObject(w_rgn);
                        Marshal.ReleaseComObject(rgn);
                        w_rgn = null;
                        rgn = null;
                    }
                }
                //excelファイルの保存
                wb.SaveAs(Space.Conf.ExcelExport+@"\date"+ DateTime.Now);
                wb.Close(false);
                ExcelApp.Quit();
                //Excelのオブジェクトを開放し忘れているとプロセスが落ちないため注意
                Marshal.ReleaseComObject(ws);
                Marshal.ReleaseComObject(shs);
                Marshal.ReleaseComObject(wb);
                Marshal.ReleaseComObject(wbs);
                Marshal.ReleaseComObject(ExcelApp);
                ws = null;
                shs = null;
                wb = null;
                wbs = null;
                ExcelApp = null;
                GC.Collect();
                MessageBox.Show("エクセルデータを出力しました！", "成功",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            //終了
            this.Closed(e);
            return;

        ERROR:
            {
                MessageBox.Show("失敗しました！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                Cnet.CutEnd();
                this.Closed(e);
                return;
            }
        }

        private void Form7_Load(object sender, EventArgs e)
        {
            m_IsClosedByUeser = true;
        }

        private new void Closed(EventArgs e)
        {
            Space.IsSearch = false;
            this.Close();
        }
    }
}
