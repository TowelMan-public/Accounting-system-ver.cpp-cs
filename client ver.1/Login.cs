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
    public partial class Login : Form
    {
        public Login()
        {
            InitializeComponent();
        }

        private void Form4_Load(object sender, EventArgs e)
        {
            //何もなし
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string str = textBox1.Text;
            //未入力
            if (str==""||str==null)
            {
                MessageBox.Show("パスワードを入力してください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }

            /*ログインを試みる*/
            Cnet.Connect(Space.Conf.HostName);
            Cnet.Send(NetMeg.LOGIN + "(" + Space.Conf.UesrName + "," + str + ")");
            if (Cnet.Get() != NetMeg.ANS_SUCCESS)//失敗
            {
                MessageBox.Show("ログインに失敗しました！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                Cnet.CutEnd();
                return;
            }
            else//成功
            {
                MessageBox.Show("ログインに成功しました！", "成功",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                Cnet.CutEnd();
                Space.IsLogin = true;
                this.Close();
                return;
            }
        }
    }
}
