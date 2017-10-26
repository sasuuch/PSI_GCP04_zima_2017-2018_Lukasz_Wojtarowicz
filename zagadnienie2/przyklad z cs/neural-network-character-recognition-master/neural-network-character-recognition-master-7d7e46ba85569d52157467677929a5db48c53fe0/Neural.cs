using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Drawing.Imaging;

namespace WindowsFormsApplication
{
    public partial class Main : Form
    {
        private class Network
        {
            public struct InputLayer
            {
                public double Value;
                public double[] Weights;
            }

            public struct OutputLayer
            {
                public double InputSum;
                public double Output;
                public double Error;
                public double Target;
                public string Value;
            }

            public double learningRate = 0.2;

            public int ImageSize = 0;
            public int InputNum = 0;
            public int OutputNum = 0;

            public InputLayer[] inputLayer = null;
            public OutputLayer[] outputLayer = null;

            public double[] errors = null;

            public int currentIteration = 0;
            public int maximumIteration = 10;

            public void Initialize(int inputSize, int outputSize)
            {
                InputNum = inputSize;
                OutputNum = outputSize;

                inputLayer = new Network.InputLayer[inputSize];
                outputLayer = new Network.OutputLayer[outputSize];

                // Zainicjuj wagi losowymi wartościami z zakresu 0.01 - 0.02.
                Random random = new Random();

                for (int i = 0; i < InputNum; ++i)
                {
                    inputLayer[i].Weights = new double[OutputNum];
                    
                    for (int j = 0; j < OutputNum; ++j)
                    {
                        inputLayer[i].Weights[j] = random.Next(1, 3) / 100.0;
                    }
                }
            }

            public bool TrainNetwork(double[][] inputs, double[][] outputs)
            {
                double currentError = 0.0, maximumError = 0.01;

                currentIteration = 0;

                // Utwórz tablicę do przechowywania wartości kolejnych błędów.
                errors = new double[maximumIteration];

                do
                {
                    currentError = 0;

                    for (int i = 0; i < inputs.Length; ++i)
                    {
                        CalculateOutput(inputs[i], outputLayer[i].Value);
                        BackPropagation();

                        currentError += GetError();
                    }

                    errors[currentIteration] = currentError;

                    ++currentIteration;
                }
                while (currentError > maximumError && currentIteration < maximumIteration);

                // Jeżeli maksymalny błąd został osiągnięty w mniejszej liczbie iteracji,
                // to nauka sieci zakończyła się pomyślnie.
                if (currentIteration <= maximumIteration)
                {
                    return true;
                }

                return false;
            }

            public void CalculateOutput(double[] pattern, string output)
            {
                // Przypisz wejście do warstwy wejściowej.
                for (int i = 0; i < pattern.Length; i++)
                {
                    inputLayer[i].Value = pattern[i];
                }

                // Oblicz wejście, wyjście, wartość oczekiwaną oraz błąd pierwszej warstwy.
                for (int i = 0; i < OutputNum; i++)
                {
                    double total = 0.0;

                    for (int j = 0; j < InputNum; j++)
                    {
                        total += inputLayer[j].Value * inputLayer[j].Weights[i];
                    }

                    outputLayer[i].InputSum = total;
                    outputLayer[i].Output = Activation(total);
                    outputLayer[i].Target = outputLayer[i].Value.CompareTo(output) == 0 ? 1.0 : 0.0;
                    outputLayer[i].Error = (outputLayer[i].Target - outputLayer[i].Output) * (outputLayer[i].Output) * (1 - outputLayer[i].Output);
                }
            }

            public void BackPropagation()
            {
                // Popraw wagi warstwy wejściowej.
                for (int j = 0; j < OutputNum; j++)
                {
                    for (int i = 0; i < InputNum; i++)
                    {
                        inputLayer[i].Weights[j] += learningRate * (outputLayer[j].Error) * inputLayer[i].Value;
                    }
                }
            }

            public double GetError()
            {
                double total = 0.0;

                for (int j = 0; j < OutputNum; j++)
                {
                    total += Math.Pow((outputLayer[j].Target - outputLayer[j].Output), 2.0) / 2.0;
                }

                return total;
            }

            public double Activation(double x)
            {
                return (1.0 / (1.0 + Math.Exp(-x)));
            }

            public void Recognize(double[] Input)
            {
                for (int i = 0; i < InputNum; i++)
                {
                    inputLayer[i].Value = Input[i];
                }

                for (int i = 0; i < OutputNum; i++)
                {
                    double total = 0.0;

                    for (int j = 0; j < InputNum; j++)
                    {
                        total += inputLayer[j].Value * inputLayer[j].Weights[i];
                    }

                    outputLayer[i].InputSum = total;
                    outputLayer[i].Output = Activation(total);
                }
            }
        }

        private Network network = new Network();

        public Main()
        {
            InitializeComponent();           
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            pictureBox1.Load(openFileDialog1.FileName);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            openFileDialog1.ShowDialog();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string[] files = Directory.GetFiles(folderBrowserDialog1.SelectedPath, "*.bmp");

            // Pobierz wysokość pierwszego obrazu.
            // Zakładamy, że obrazy są kwadratami.
            int imageSize = Bitmap.FromFile(files[0]).Width;

            // Utwórz warstwy.
            network.Initialize(imageSize * imageSize, files.Length);

            // Pobierz dane obrazów i przekształć je do tablicy double.
            double[][] inputs = new double[files.Length][];
            double[][] outputs = new double[files.Length][];

            for (int i = 0; i < files.Length; ++i)
            {
                inputs[i] = new double[imageSize * imageSize];

                Bitmap image = new Bitmap(files[i]);

                for (int x = 0; x < imageSize; ++x)
                {
                    for (int y = 0; y < imageSize; ++y)
                    {
                        Color pixel = image.GetPixel(x, y);

                        inputs[i][x * imageSize + y] = (1.0 - (pixel.R / 255.0 + pixel.G / 255.0 + pixel.B / 255.0) / 3.0) < 0.5 ? 0.0 : 1.0;
                    }
                }

                outputs[i] = new double[files.Length];

                for (int j = 0; j < files.Length; ++j)
                {
                    outputs[i][j] = i == j ? 1.0 : 0.0;
                }

                FileInfo info = new FileInfo(files[i]);
                network.outputLayer[i].Value = info.Name.Replace(".bmp", "");
            }
            
            // Trenuj sieć.
            network.TrainNetwork(inputs, outputs);

            // Wyświetl wartości błędów w kolejnych iteracjach.
            // listView1.Items.Clear();

            for (int i = 0; i < network.currentIteration; ++i)
            {
                ListViewItem item = listView1.Items.Add(i.ToString());
                item.SubItems.Add(network.errors[i].ToString("#0.000000"));
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            // Zakładamy, że obrazy są kwadratami.
            int imageSize = pictureBox1.Image.Width;

            // Pobierz dane obrazu i przekształć je do tablicy double.
            double[] sample = new double[imageSize * imageSize];

            Bitmap image = new Bitmap(pictureBox1.ImageLocation);

            for (int x = 0; x < imageSize; ++x)
            {
                for (int y = 0; y < imageSize; ++y)
                {
                    Color pixel = image.GetPixel(x, y);

                    sample[x * imageSize + y] = (1.0 - (pixel.R / 255.0 + pixel.G / 255.0 + pixel.B / 255.0) / 3.0) < 0.5 ? 0.0 : 1.0;
                }
            }

            network.Recognize(sample);

            // Wyświetl wartości wyjściowe.
            listView2.Items.Clear();

            for (int i = 0; i < network.outputLayer.Length; ++i)
            {
                ListViewItem item = listView2.Items.Add(network.outputLayer[i].Value);
                item.SubItems.Add(network.outputLayer[i].Output.ToString("#0.000000"));
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            try
            {
                network.maximumIteration = int.Parse(textBox1.Text);
            }
            catch
            {
            }
        }
    }
}
