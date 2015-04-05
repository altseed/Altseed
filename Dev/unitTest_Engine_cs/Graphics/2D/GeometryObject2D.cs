using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace unitTest_Engine_cs.Graphics._2D
{
    class GeometryObject2D : EngineTest
    {
        public GeometryObject2D()
            : base(20)
        {


        }

        protected override void OnStart()
        {
            var scene = new ace.Scene();
            var layer = new ace.Layer2D();
            var texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
            var geometryObj1 = new ace.GeometryObject2D();
            var geometryObj2 = new ace.GeometryObject2D();
            var geometryObj3 = new ace.GeometryObject2D();
            var geometryObj4 = new ace.GeometryObject2D();
            var geometryObj5 = new ace.GeometryObject2D();

            layer.AddObject(geometryObj1);
            layer.AddObject(geometryObj2);
            layer.AddObject(geometryObj3);
            layer.AddObject(geometryObj4);
            layer.AddObject(geometryObj5);

            scene.AddLayer(layer);
            ace.Engine.ChangeScene(scene);

            {
                var circle = new ace.CircleShape();
                circle.OuterDiameter = 100.0f;
                circle.InnerDiameter = 10.0f;
                circle.NumberOfCorners = 96;
                circle.Position = new ace.Vector2DF(100, 50);

                geometryObj1.Shape = circle;
                geometryObj1.Texture = texture;
                geometryObj1.Position = new ace.Vector2DF(0, 0);
            }

            {
                var arc = new ace.ArcShape();
                arc.OuterDiameter = 100;
                arc.InnerDiameter = 10;
                arc.NumberOfCorners = 96;
                arc.Position = new ace.Vector2DF(300, 50);
                arc.StartingCorner = 90;
                arc.EndingCorner = 5;

                geometryObj2.Shape = arc;
                geometryObj2.Texture = texture;
                geometryObj2.Position = new ace.Vector2DF(0, 0);
            }

            {
                var line = new ace.LineShape();
                line.StartingPosition = new ace.Vector2DF(410, 50);
                line.EndingPosition = new ace.Vector2DF(630, 50);
                line.Thickness = 5.0f;

                geometryObj3.Shape = line;
                geometryObj3.Position = new ace.Vector2DF(0, 0);
            }

            {
                var rect = new ace.RectangleShape();
                rect.DrawingArea = new ace.RectF(10, 110, 300, 200);
                rect.UV = new ace.RectF(0.0f, 0.0f, 0.5f, 0.5f);

                geometryObj4.Shape = rect;
                geometryObj4.Texture = texture;
                geometryObj4.Position = new ace.Vector2DF(0, 0);
            }

            {
                var triangle = new ace.TriangleShape();
                triangle.SetPointByIndex(new ace.Vector2DF(320, 350), 0);
                triangle.SetPointByIndex(new ace.Vector2DF(100, 450), 1);
                triangle.SetPointByIndex(new ace.Vector2DF(540, 450), 2);

                triangle.SetUVByIndex(new ace.Vector2DF(0.5f, 0.2f), 0);
                triangle.SetUVByIndex(new ace.Vector2DF(0.1f, 0.5f), 1);
                triangle.SetUVByIndex(new ace.Vector2DF(0.9f, 0.7f), 2);

                geometryObj5.Shape = triangle;
                geometryObj5.Texture = texture;
                geometryObj5.Position = new ace.Vector2DF(0, 0);
            }
        }
    }
}
