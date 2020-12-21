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
    public partial class InCustomer : Form
    {
        private const string m_meg1 = NetMeg.INSORT;
        private const string m_meg2 = NetMeg.REQ_KAISYA;
        private bool m_IsClosedByUeser;

        public InCustomer()
        {
            InitializeComponent();
        }

        private void InCustomer_Load(object sender, EventArgs e)
        {
            m_IsClosedByUeser = true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //未入力の項目がある
            if (textBox1.TextLength == 0)
            {
                MessageBox.Show("必要な項目に入力してください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            //登録
            Cnet.Connect(Space.Conf.HostName);
            Cnet.Send("INSERT_CUSTOMER(" + textBox1.Text + ")");
            if (Cnet.Get() != NetMeg.ANS_SUCCESS)
                goto ERROR;
            Cnet.CutEnd();
            if (!checkBox1.Checked)
            {
                this.OnClosed(e);
                m_IsClosedByUeser = false;
            }
            return;

        ERROR:
            {
                Cnet.CutEnd();
                MessageBox.Show("登録に失敗しました！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
        }

        private new void Closed(EventArgs e)
        {
            Space.IsInsert = false;
            this.Close();
        }
    }
}
