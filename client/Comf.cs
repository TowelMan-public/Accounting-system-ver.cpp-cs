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
    public partial class Comf : Form
    {
        public Comf()
        {
            InitializeComponent();
        }

        private void Form3_Load(object sender, EventArgs e)
        {
            textBox1.Text = Space.Conf.ExcelExport;
            textBox2.Text = Space.Conf.HostName;
            textBox3.Text = Space.Conf.UesrName;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //SysConf con = new SysConf();
            Space.Conf.ExcelExport = textBox1.Text;
            Space.Conf.HostName = textBox2.Text;
            Space.Conf.UesrName = textBox3.Text;
            //Space.Conf = con;
            MessageBox.Show("設定しました！", "成功",
                MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button2);
            this.Close();
            return;
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }
    }
}
