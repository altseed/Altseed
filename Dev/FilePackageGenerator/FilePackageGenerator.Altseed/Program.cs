using System;

namespace FilePackageGenerator.Altseed
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            if (args.Length > 0) Console.WriteLine(CUI.MainProgram(args));
            else GUI.MainProgram();
        }
    }
}
