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
    public partial class UpdateUnpaid : Form
    {
        public UpdateUnpaid()
        {
            InitializeComponent();
        }

        private bool IsntInt(string str)
        {
            int i;
            return !int.TryParse(str, out i);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (IsntInt(textBoxMoney.Text) || IsntInt(textBoxNumber.Text)){
                MessageBox.Show("必要な項目に入力してください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            //メッセージ作成
            string sendMeg = "UPDATE_PAID(";
            sendMeg += textBoxNumber.Text + "," + textBoxMoney.Text + ")";
            //送信
            Cnet.Connect(Space.Conf.HostName);
            Cnet.Send(sendMeg);
            if (Cnet.Get() != NetMeg.ANS_SUCCESS)
            {
                MessageBox.Show("失敗しました！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
            }
            Cnet.CutEnd();
        }
    }
}
