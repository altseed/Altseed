using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace unitTest_Engine_cs.IO
{
    class StaticFile_Package : EngineTest
    {

        public StaticFile_Package()
            : base(3)
        {
        }

        protected override void OnStart()
        {
            //普通に読み込んだバイナリ
            List<byte> loadData;

            using (FileStream fileStream = new FileStream("Data/Texture/Surface/Tile_Spec.png", FileMode.Open, FileAccess.Read))
            {

                byte[] buf = new byte[fileStream.Length];
                fileStream.Read(buf, 0, buf.Length);
                loadData = new List<byte>(buf);
            }

            //ファイル機能から読み込んだバイナリ
            ace.Engine.File.AddRootDirectories("Data/Texture.pack");
            var staticFile = ace.Engine.File.CreateStaticFile("Surface/Tile_Spec.png");
            var staticFileData = staticFile.ReadAllBytes();

            for (int i = 0; i < loadData.Count; ++i)
            {
                byte byteFromRaw = loadData[i];

                byte byteFromFile = staticFileData[i];

                Assert.Equals(byteFromRaw, byteFromFile);
            }
        }
    }
}
