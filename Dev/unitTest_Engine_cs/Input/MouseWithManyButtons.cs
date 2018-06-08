using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using asd;

namespace unitTest_Engine_cs.Input
{
    class MouseWithManyButtons : EngineTest
    {
        private List<TextObject2D> textObjects;

        public MouseWithManyButtons()
            : base(300)
        {
        }

        protected override void OnStart()
        {
            var scene = new Scene();
            var layer = new Layer2D();

            textObjects = new List<TextObject2D>();
            for (int i = 0; i < 8; ++i)
            {
                var text = new TextObject2D();
                text.Font = Engine.Graphics.CreateDynamicFont("", 32, new Color(255, 0, 0, 255), 0, new Color(0, 0, 0, 0));
                text.Position = new Vector2DF(40, 40 + 50 * i);
                textObjects.Add(text);
                layer.AddObject(text);
            }

            scene.AddLayer(layer);
            asd.Engine.ChangeScene(scene);
        }

        protected override void OnUpdating()
        {
            for (int i = 0; i < 8; ++i)
            {
                var inputState = Engine.Mouse.GetButtonInputState((MouseButtons)i);

                if (inputState == ButtonState.Push)
                {
                    textObjects[i].Text = "ボタン" + i + "を押しました。";
                }
                else if (inputState == ButtonState.Hold)
                {
                    textObjects[i].Text = "ボタン" + i + "を押し続けています。";
                }
                else if (inputState == ButtonState.Free)
                {
                    textObjects[i].Text = "ボタン" + i + "を離し続けています。";
                }
                else if (inputState == ButtonState.Release)
                {
                    textObjects[i].Text = "ボタン" + i + "を離しました。";
                }
            }
        }
    }
}
