using System;
using System.IO;
using System.Linq;
using FilePackageGenerator.Packaging;

namespace FilePackageGenerator.Altseed
{
    static class GUI
    {
        public static void MainProgram()
        {
            if (!asd.Engine.Initialize("FilePackageGenerator", 640, 480, new asd.EngineOption())) return;
            asd.Engine.OpenTool();

            while (asd.Engine.DoEvents())
            {
                RenderTool();
                asd.Engine.Update();
            }

            asd.Engine.CloseTool();
            asd.Engine.Terminate();
        }

        private static void RenderTool()
        {
            sbyte[] password = new sbyte[512];
            string srcPath = string.Empty;
            string dstPath = string.Empty;
            bool isIgnoreHidden = true;

            asd.Engine.Tool.Begin("FilePackageGenerator");

            asd.Engine.Tool.Text("Source:");
            asd.Engine.Tool.Text(srcPath);
            if (asd.Engine.Tool.Button("Open..."))
            {
                var result = asd.Engine.Tool.OpenDialog("*", Directory.GetCurrentDirectory());
                if (result != string.Empty)
                    srcPath = Path.GetDirectoryName(result);
            }

            asd.Engine.Tool.Text("Setting:");
            if (asd.Engine.Tool.Button(isIgnoreHidden ? "Ignore Hidden" : "All Files"))
            {
                isIgnoreHidden = !isIgnoreHidden;
            }

            asd.Engine.Tool.Text("Password:");
            asd.Engine.Tool.InputText(string.Empty, password, 512);

            asd.Engine.Tool.Text("Package:");
            if (asd.Engine.Tool.Button("Save as...") && srcPath != string.Empty)
            {
                dstPath = asd.Engine.Tool.SaveDialog("pack", Directory.GetCurrentDirectory());
                if (dstPath != string.Empty) Package(password, srcPath, dstPath, isIgnoreHidden);
            }

            asd.Engine.Tool.End();
        }

        private static void Package(sbyte[] password, string srcPath, string dstPath, bool isIgnoreHidden)
        {
            var pass = BitConverter.ToString(password.Select(sb => (byte)sb).ToArray());
            var setting = isIgnoreHidden ? PackagingSetting.IgnoreHiddenAttribute : PackagingSetting.AllFiles;
            Packing.Run(srcPath, dstPath, PackagingSetting.IgnoreHiddenAttribute, new string[] { }, pass);
        }
    }
}
