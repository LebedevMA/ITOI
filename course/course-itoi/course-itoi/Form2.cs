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
    public partial class Form2 : Form
    {
        public struct Result
        {
            public string fileName;
			public double x;
            public double y;
            public double size;
            public double angle;
            public double width;
            public double height;
            public double weight;
            public Result(string fileName, double x, double y, double size, double angle, double width, double height, double weight)
            {
                this.fileName = fileName;
                this.x = x;
                this.y = y;
                this.size = size;
                this.angle = angle;
                this.width = width;
                this.height = height;
                this.weight = weight;
            }
            Result(Result a)
            {
                this.fileName = a.fileName;
                x = a.x;
                y = a.y;
                size = a.size;
                angle = a.angle;
                width = a.width;
                height = a.height;
                weight = a.weight;
            }
        };

        public List<Result> Results;

        public Form2()
        {
            InitializeComponent();
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            for (int i = 0; i < Results.Count; i++) {
                listBox1.Items.Add(Results[i].fileName);
            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

            Pen pen = new Pen(Color.Red);
            Result r = Results[listBox1.SelectedIndex];
            Graphics g = pictureBox1.CreateGraphics();
            g.DrawImage(Image.FromFile(r.fileName),0,0);

            Point[] points = new Point[4];
            points[0] = new Point((int)(r.x + 0.5 * r.width * Math.Cos(r.angle + 0.00) - 0.5 * r.height * Math.Sin(r.angle + 0.00)), (int)(r.y + 0.5 * r.width * Math.Sin(r.angle + 0.00) + 0.5 * r.height * Math.Cos(r.angle + 0.00)));
            points[1] = new Point((int)(r.x + 0.5 * r.width * Math.Cos(r.angle + 1.57) - 0.5 * r.height * Math.Sin(r.angle + 1.57)), (int)(r.y + 0.5 * r.width * Math.Sin(r.angle + 1.57) + 0.5 * r.height * Math.Cos(r.angle + 1.57)));
            points[2] = new Point((int)(r.x + 0.5 * r.width * Math.Cos(r.angle + 3.14) - 0.5 * r.height * Math.Sin(r.angle + 3.14)), (int)(r.y + 0.5 * r.width * Math.Sin(r.angle + 3.14) + 0.5 * r.height * Math.Cos(r.angle + 3.14)));
            points[3] = new Point((int)(r.x + 0.5 * r.width * Math.Cos(r.angle + 4.72) - 0.5 * r.height * Math.Sin(r.angle + 4.72)), (int)(r.y + 0.5 * r.width * Math.Sin(r.angle + 4.72) + 0.5 * r.height * Math.Cos(r.angle + 4.72)));

            g.DrawPolygon(pen, points);
        }
    }
}
