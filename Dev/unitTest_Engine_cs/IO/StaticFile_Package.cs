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
            : base(1)
        {
        }

        protected override void OnStart()
        {
            //普通に読み込んだバイナリ
            byte[] loadData;

            using (FileStream fileStream = new FileStream("Data/Texture/Sample1.png", FileMode.Open, FileAccess.Read))
            {

                loadData = new byte[fileStream.Length];
                fileStream.Read(loadData, 0, loadData.Length);
            }

            /*
            ace.Engine.File.AddRootDirectories("Data/Texture");
            var staticFile = ace.Engine.File.CreateStaticFile("Sample1.png");
            var staticFileData = staticFile->ReadAllBytes();

            int cnt = 0;
            while (!reader.IsEmpty())
            {
                byte byteFromRaw = reader.Get<int8_t>();

                byte byteFromFile = staticFileData[cnt++];

                Assert(byteFromRaw == byteFromFile);
            }
             */
        }
    }
}
