using System.Text;
using System.IO;
using System.Linq;
using FilePackageGenerator.Packaging;

namespace FilePackageGenerator.Altseed
{
    static class GUI
    {
        static sbyte[] Password = new sbyte[512];
        static string SrcPath = string.Empty;
        static string DstPath = string.Empty;
        static bool IsIgnoreHidden = true;

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
            asd.Engine.Tool.Begin("FilePackageGenerator");

            asd.Engine.Tool.Text("Source:");
            asd.Engine.Tool.Text(SrcPath);
            if (asd.Engine.Tool.Button("Open..."))
            {
                var result = asd.Engine.Tool.OpenDialog("*", Directory.GetCurrentDirectory());
                if (result != string.Empty)
                    SrcPath = Path.GetDirectoryName(result);
            }

            asd.Engine.Tool.Text("Setting:");
            if (asd.Engine.Tool.Button(IsIgnoreHidden ? "Ignore Hidden" : "All Files"))
            {
                IsIgnoreHidden = !IsIgnoreHidden;
            }

            asd.Engine.Tool.Text("Password:");
            asd.Engine.Tool.InputText(string.Empty, Password, 512);

            asd.Engine.Tool.Text("Package:");
            if (asd.Engine.Tool.Button("Save as...") && SrcPath != string.Empty)
            {
                DstPath = asd.Engine.Tool.SaveDialog("pack", Directory.GetCurrentDirectory());
                if (DstPath != string.Empty) Package(Password, SrcPath, DstPath, IsIgnoreHidden);
            }

            asd.Engine.Tool.End();
        }

        private static void Package(sbyte[] password, string srcPath, string dstPath, bool isIgnoreHidden)
        {
            var pass = Encoding.UTF8.GetString(password.Select(sb => (byte)sb).ToArray(), 0, password.Length);
            var setting = isIgnoreHidden ? PackagingSetting.IgnoreHiddenAttribute : PackagingSetting.AllFiles;
            Packing.Run(srcPath, dstPath, PackagingSetting.IgnoreHiddenAttribute, new string[] { }, pass);
        }
    }
}
