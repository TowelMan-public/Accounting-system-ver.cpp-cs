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

namespace kaikei0_c
{
    public partial class Mian : Form
    {
        //private Stats stats;

        public Mian()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //無
        }

        //検索
        private void button1_Click(object sender, EventArgs e)
        {
            //ログインしていない
            if (!Space.IsLogin)
            {
                MessageBox.Show("ログインしてください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            //ダイアログ生成（親をロックしない）
            Space.IsSearch = true;
            var form = new OutStart();
            form.Show(this);
        }

        //登録
        private void button2_Click(object sender, EventArgs e)
        {
            //ログインしていない
            if (!Space.IsLogin)
            {
                MessageBox.Show("ログインしてください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            //ダイアログ生成（親をロックしない）
            Space.IsInsert = true;
            var form = new InStart();
            form.Show(this);
        }

        //設定
        private void button3_Click(object sender, EventArgs e)
        {
            //ダイアログ生成（親をロックする）
            var form = new Comf();
            form.ShowDialog();
            form.Dispose();
        }

        //削除
        private void button4_Click(object sender, EventArgs e)
        {
            var form = new Delete();
            form.ShowDialog();
            form.Dispose();
        }

        //変更
        private void button5_Click(object sender, EventArgs e)
        {
            var form = new updateStart();
            form.ShowDialog();
            form.Dispose();
        }

        //反映
        private void button6_Click(object sender, EventArgs e)
        {
            //無
        }

        //ログイン
        private void button7_Click(object sender, EventArgs e)
        {
            //設定に不備有
            if (!Space.Conf.HaveConf())
            {
                MessageBox.Show("ホスト名（IPアドレス）とユーザー名を設定してください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            //既にログインされてる
            if (Space.IsLogin)
                return;
            //ダイアログ生成（親をロックする）
            var form = new Login();
            form.ShowDialog();
            form.Dispose();
        }

        //表をクリックした
        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            //無
        }

        //表の作成・反映
        public void CreateNewGrid(DateGridBaf baf)
        {
            dataGridView1.Rows.Clear();
            dataGridView1.ColumnCount = baf.CountClm() + 2;
            for (int i = 0; i < baf.CountClm(); i++)
                dataGridView1.Columns[i].HeaderText = baf.GetClm();
            dataGridView1.Columns[baf.CountClm()].HeaderText = "削除";
            dataGridView1.Columns[baf.CountClm() + 1].HeaderText = "変更";
            switch (baf.CountClm())
            {
                case 2:
                    while (baf.HaveTuple())
                        dataGridView1.Rows.Add(baf.GetTuple(), baf.GetTuple(), false, false);
                    break;
                case 3:
                    while (baf.HaveTuple())
                        dataGridView1.Rows.Add(baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), false, false);
                    break;
                case 4:
                    while (baf.HaveTuple())
                        dataGridView1.Rows.Add(baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), false, false);
                    break;
                case 5:
                    while (baf.HaveTuple())
                        dataGridView1.Rows.Add(baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), false, false);
                    break;
                case 6:
                    while (baf.HaveTuple())
                        dataGridView1.Rows.Add(baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), false, false);
                    break;
                case 7:
                    while (baf.HaveTuple())
                        dataGridView1.Rows.Add(baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), baf.GetTuple(), false, false);
                    break;
                default:
                    throw (new System.Exception());
            }
        }
    }
}
