using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace course_itoi
{
    public partial class Form1 : Form
    {
        public string input1;
        public string input2;

        public int N;
        public int T;
        public int T1;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            input1 = "";
            input2 = "";
            N = 500;
            T = 70;
            T1 = 40;
        }

        private void buttonInput1_Click(object sender, EventArgs e)
        {
            DialogResult dr = openFileDialog1.ShowDialog();
            if (dr != DialogResult.Cancel) {
                textBoxInput1.Text = openFileDialog1.FileName;
            }
        }

        private void textBoxInput1_TextChanged(object sender, EventArgs e)
        {
            input1 = textBoxInput1.Text;
        }

        private void buttonInput2_Click(object sender, EventArgs e)
        {
            DialogResult dr = folderBrowserDialog1.ShowDialog();
            if (dr != DialogResult.Cancel)
            {
                textBoxInput2.Text = folderBrowserDialog1.SelectedPath;
            }
        }

        private void textBoxInput2_TextChanged(object sender, EventArgs e)
        {
            input2 = textBoxInput2.Text;
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            N = (int)numericUpDown1.Value;
        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            T = (int)numericUpDown2.Value;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            input1 = textBoxInput1.Text;
            input2 = textBoxInput2.Text;
            N = (int)numericUpDown1.Value;
            T = (int)numericUpDown2.Value;
            T1 = (int)numericUpDown3.Value;
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void numericUpDown3_ValueChanged(object sender, EventArgs e)
        {
            T1 = (int)numericUpDown3.Value;
        }
    }
}
