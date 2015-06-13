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
            var scene = new asd.Scene();
            var layer = new asd.Layer2D();
            var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
            var geometryObj1 = new asd.GeometryObject2D();
            var geometryObj2 = new asd.GeometryObject2D();
            var geometryObj3 = new asd.GeometryObject2D();
            var geometryObj4 = new asd.GeometryObject2D();
            var geometryObj5 = new asd.GeometryObject2D();
            var geometryObj6 = new asd.GeometryObject2D();

            layer.AddObject(geometryObj1);
            layer.AddObject(geometryObj2);
            layer.AddObject(geometryObj3);
            layer.AddObject(geometryObj4);
            layer.AddObject(geometryObj5);
            layer.AddObject(geometryObj6);

            scene.AddLayer(layer);
            asd.Engine.ChangeScene(scene);

            {
                var circle = new asd.CircleShape();
                circle.OuterDiameter = 100.0f;
                circle.InnerDiameter = 10.0f;
                circle.NumberOfCorners = 96;
                circle.Position = new asd.Vector2DF(100, 50);

                geometryObj1.Shape = circle;
                geometryObj1.Texture = texture;
                geometryObj1.Position = new asd.Vector2DF(0, 0);
            }

            {
                var arc = new asd.ArcShape();
                arc.OuterDiameter = 100;
                arc.InnerDiameter = 10;
                arc.NumberOfCorners = 96;
                arc.Position = new asd.Vector2DF(300, 50);
                arc.StartingCorner = 90;
                arc.EndingCorner = 5;

                geometryObj2.Shape = arc;
                geometryObj2.Texture = texture;
                geometryObj2.Position = new asd.Vector2DF(0, 0);
            }

            {
                var line = new asd.LineShape();
                line.StartingPosition = new asd.Vector2DF(410, 50);
                line.EndingPosition = new asd.Vector2DF(630, 50);
                line.Thickness = 5.0f;

                geometryObj3.Shape = line;
                geometryObj3.Position = new asd.Vector2DF(0, 0);
            }

            {
                var rect = new asd.RectangleShape();
                rect.DrawingArea = new asd.RectF(10, 110, 300, 200);
                rect.UV = new asd.RectF(0.0f, 0.0f, 0.5f, 0.5f);

                geometryObj4.Shape = rect;
                geometryObj4.Texture = texture;
                geometryObj4.Position = new asd.Vector2DF(0, 0);
            }

            {
                var triangle = new asd.TriangleShape();
                triangle.SetPointByIndex(new asd.Vector2DF(320, 350), 0);
                triangle.SetPointByIndex(new asd.Vector2DF(100, 450), 1);
                triangle.SetPointByIndex(new asd.Vector2DF(540, 450), 2);

                triangle.SetUVByIndex(new asd.Vector2DF(0.5f, 0.2f), 0);
                triangle.SetUVByIndex(new asd.Vector2DF(0.1f, 0.5f), 1);
                triangle.SetUVByIndex(new asd.Vector2DF(0.9f, 0.7f), 2);

                geometryObj5.Shape = triangle;
                geometryObj5.Texture = texture;
                geometryObj5.Position = new asd.Vector2DF(0, 0);
            }

            {
                var polygon = new asd.PolygonShape();

                for (int i = 0; i < 10; ++i)
                {
                    asd.Vector2DF vec = new asd.Vector2DF(1, 0);
                    vec.Degree = i * 36;
                    vec.Length = (i % 2 == 0) ? 100 : 55;
                    polygon.AddVertex(vec + new asd.Vector2DF(500, 250));

                }

                geometryObj6.Shape = polygon;
                geometryObj6.Texture = texture;
                geometryObj6.Position = new asd.Vector2DF(0, 0);
            }
        }
    }
}
