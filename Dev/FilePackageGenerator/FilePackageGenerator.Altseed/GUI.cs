using System;
using System.Text;
using System.IO;
using System.Linq;
using FilePackageGenerator.Packaging;

namespace FilePackageGenerator.Altseed
{

    static class GUI
    {
        public static void MainProgram()
        {
            var options = new asd.EngineOption()
            {
                IsWindowResizable = true,
            };

            if (!asd.Engine.Initialize("FilePackageGenerator", 480, 640, options)) return;
            asd.Engine.OpenTool();

            TryLoadFont(
                @"C:\Windows\Fonts\meiryo.ttc",
                @"/Library/Fonts/ヒラギノ丸ゴ Pro W4.otf",
                @"/Library/Fonts/ヒラギノ丸ゴ ProN W4.ttc",
                @"/System/Library/Fonts/ヒラギノ丸ゴ ProN W4.ttc"
            );

            asd.Engine.ChangeScene(new ToolScene());
            while (asd.Engine.DoEvents())
            {
                asd.Engine.Update();
            }

            asd.Engine.CloseTool();
            asd.Engine.Terminate();
        }

        private static void TryLoadFont(params string[] fonts)
        {
            foreach (var font in fonts)
            {
                if (!File.Exists(font)) continue;
                asd.Engine.Tool.AddFontFromFileTTF(font, 14);
                break;
            }
        }
    }

    class ToolScene : asd.Scene
    {
        private sbyte[] Password = new sbyte[512];
        private string SrcPath = string.Empty;
        private string DstPath = string.Empty;
        private string FileList = string.Empty;
        private PackagingSetting Setting;

        protected override void OnUpdated()
        {
            if (asd.Engine.Tool.BeginFullscreen("FilePackageGenerator", 0))
            {

                asd.Engine.Tool.Text("Source:");
                if (asd.Engine.Tool.Button("Open...")) Open();

                asd.Engine.Tool.Text("Setting:");
                if (asd.Engine.Tool.Button(Setting == PackagingSetting.IgnoreHiddenAttribute ? "Ignore Hidden" : "All Files"))
                {
                    Setting = (Setting == PackagingSetting.AllFiles) ?
                        PackagingSetting.IgnoreHiddenAttribute :
                        PackagingSetting.AllFiles;
                    RefleshList();
                }
                asd.Engine.Tool.Text("Password:");
                asd.Engine.Tool.InputText(string.Empty, Password, 512);

                asd.Engine.Tool.Text("Package:");
                if (asd.Engine.Tool.Button("Save as...") && SrcPath != string.Empty) Save();

                asd.Engine.Tool.Text("");
                asd.Engine.Tool.Text("Selected Folder:");
                asd.Engine.Tool.Text(SrcPath);
                asd.Engine.Tool.Text("File List:");

                asd.Engine.Tool.ListBox(string.Empty, new[] { 0 }, FileList);
            }
            asd.Engine.Tool.End();
        }

        private void Save()
        {
            DstPath = asd.Engine.Tool.SaveDialog("pack", Directory.GetCurrentDirectory());
            if (!string.IsNullOrEmpty(DstPath)) Package();
        }

        private void Open()
        {
            var result = asd.Engine.Tool.PickFolder(Directory.GetCurrentDirectory());
            if (!string.IsNullOrEmpty(result))
            {
                SrcPath = result;
                RefleshList();
            }
        }

        private void RefleshList()
        {
            if (string.IsNullOrEmpty(SrcPath)) return;

            var directoryUri = new Uri(Path.GetFullPath(SrcPath));
            var builder = new StringBuilder();
            foreach (var f in Directory.EnumerateFiles(SrcPath, "*", SearchOption.AllDirectories))
            {
                if (Setting == PackagingSetting.IgnoreHiddenAttribute &&
                        File.GetAttributes(f).HasFlag(FileAttributes.Hidden))
                    continue;

                builder.Append(f.Substring(SrcPath.Length + 1));
                builder.Append(";");
            }
            FileList = builder.ToString();
        }

        private void Package()
        {
            var bPass = Password.TakeWhile(c => c != 0x00).Select(sb => (byte)sb).ToArray();
            var pass = Encoding.UTF8.GetString(bPass, 0, sbPass.Length);

            Packing.Run(SrcPath, DstPath, Setting, new string[] { }, pass);
        }
    }
}
