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
    public partial class Delete : Form
    {
        public Delete()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string sendMeg = "DELETE_";

            if (checkBox1.Checked)
            {
                sendMeg += "JUNL";
            }
            else if (checkBox2.Checked)
            {
                sendMeg += "KEIHI";
            }
            else if (checkBox3.Checked)
            {
                sendMeg += "CUSTOMER";
            }
            else if (checkBox4.Checked)
            {
                sendMeg += "KAKEURI";
            }
            else//一つもチェックが入っていない
            {
                MessageBox.Show("チェックボックスのどれか一つにチェックを入れてください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            int outNumber;
            if(!int.TryParse(textBoxNumber.Text,out outNumber))
            {
                MessageBox.Show("整数を入力して下さい！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            sendMeg += "(" + textBoxNumber.Text + ")";
            string str;
            //検索の実行
            Cnet.Connect(Space.Conf.HostName);
            Cnet.Send(sendMeg);
            if ((str = Cnet.Get()) == NetMeg.ANS_ERROR)
            {
                MessageBox.Show("失敗しました！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
            }
            Cnet.CutEnd();
        }
    }
}
