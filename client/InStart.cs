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
    public partial class InStart : Form
    {
        private bool m_IsClosedByUeser;

        public InStart()
        {
            InitializeComponent();
        }

        private void Form5_Load(object sender, EventArgs e)
        {
            m_IsClosedByUeser = true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (checkBox1.Checked)//経費の項目
            {
                //次のウィンドウを開く
                var form = new InJunl();
                form.Show(this.Owner);
            }
            else if (checkBox2.Checked)//経費
            {
                //次のウィンドウを開く
                var form = new InKeihi();
                form.Show(this.Owner);
            }
            else if (checkBox3.Checked)//取引会社
            {
                //次のウィンドウを開く
                var form = new InCustomer();
                form.Show(this.Owner);
            }
            else if (checkBox4.Checked)//掛け売り
            {
                //次のウィンドウを開く
                var form = new InUriage();
                form.Show(this.Owner);
            }
            else if (checkBox5.Checked)//支払われたお金
            {
                //次のウィンドウを開く
                var form = new InUnpaid();
                form.Show(this.Owner);
            }
            else//一つもチェックが入っていない
            {
                MessageBox.Show("チェックボックスのどれか一つにチェックを入れてください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            m_IsClosedByUeser = false;
            this.OnClosed(e);
        }
        private new void Closed(EventArgs e)
        {
            Space.IsInsert = !m_IsClosedByUeser;
            this.Close();
        }
    }
}
