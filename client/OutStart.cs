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
    public partial class OutStart : Form
    {
        private bool m_IsClosedByUeser;

        public OutStart()
        {
            InitializeComponent();
        }

        private void Form6_Load(object sender, EventArgs e)
        {
            m_IsClosedByUeser = true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
                Space.SearchPattern = "KEIHI_KOU";
            else if (checkBox2.Checked)
                Space.SearchPattern = "KEIHI";
            else if (checkBox3.Checked)
                Space.SearchPattern = "KAISYA";
            else if (checkBox4.Checked)
                Space.SearchPattern = "KAKEURI";
            else if (checkBox5.Checked)
                Space.SearchPattern = "PAYD";
            else if (checkBox6.Checked)
                Space.SearchPattern = "UNPAYD";
            else if (checkBox7.Checked)
                Space.SearchPattern = "SYOUHI_TAX";
            else if (checkBox8.Checked)
                Space.SearchPattern = "SYOTOKU_TAX";
            else if (checkBox9.Checked)
                Space.SearchPattern = "SYOTOKU";
            else//何もチェックされてない
            {
                MessageBox.Show("どれか一つにチェックを入れてください！", "失敗",
                        MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
                return;
            }
            var form = new OutWhere();
            form.Show(this.Owner);
            m_IsClosedByUeser = false;
            this.OnClosed(e);
        }

        private new void Closed(EventArgs e)
        {
            Space.IsSearch = m_IsClosedByUeser;
            this.Close();
        }
    }
}
