using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace kaikei0_c
{
    public partial class updateStart : Form
    {
        public updateStart()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (checkBox1.Checked)//経費の項目
            {
                //次のウィンドウを開く
                var form = new UpdateJunl();
                form.Show(this.Owner);
            }
            else if (checkBox2.Checked)//経費
            {
                //次のウィンドウを開く
                var form = new UpdateKeihi();
                form.Show(this.Owner);
            }
            else if (checkBox3.Checked)//取引会社
            {
                //次のウィンドウを開く
                var form = new UpdateCustomer();
                form.Show(this.Owner);
            }
            else if (checkBox4.Checked)//掛け売り
            {
                //次のウィンドウを開く
                var form = new UpdateUriage();
                form.Show(this.Owner);
            }
            else if (checkBox5.Checked)//支払われたお金
            {
                //次のウィンドウを開く
                var form = new UpdateUnpaid();
                form.Show(this.Owner);
            }
            else//一つもチェックが入っていない
            {
                MessageBox.Show("チェックボックスのどれか一つにチェックを入れてください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            //this.Close();
        }
    }
}
