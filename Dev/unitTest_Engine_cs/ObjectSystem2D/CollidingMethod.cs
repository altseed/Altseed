using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
    class CollidingMethod : EngineTest
    {
        class CollidingObject1 : asd.TextureObject2D
        {
            Vector2DF velocity;

            protected override void OnAdded()
            {

                var rectCollider = new RectangleCollider();
                rectCollider.Area = new RectF(0, 0, 100, 100);
                rectCollider.Visible = true;
                rectCollider.Tag = "A";
                AddCollider(rectCollider);

                var lineCollider = new LineCollider();
                lineCollider.StartingPosition = new Vector2DF(110, 110);
                lineCollider.EndingPosition = new Vector2DF(200, 200);
                lineCollider.Thickness = 10;
                lineCollider.Tag = "B";
                lineCollider.Visible = true;
                AddCollider(lineCollider);

                var circleCollider = new CircleCollider();
                circleCollider.Center = new Vector2DF(300, 300);
                circleCollider.Radius = 90;
                circleCollider.Tag = "C";
                circleCollider.Visible = true;
                AddCollider(circleCollider);

                Scale = new Vector2DF(0.5f, 0.5f);
                velocity = new Vector2DF(4, 0);
            }

            protected override void OnUpdate()
            {
                if (Position.X < 0)
                {
                    velocity = new Vector2DF(4, 0);
                }
                else if (Position.X > 600)
                {
                    velocity = new Vector2DF(-4, 0);
                }
                Angle += 1;

                var collisionsInfo = Collisions2DInfo;

                foreach(var collisionInfo in collisionsInfo)
                {
                    switch (collisionInfo.CollisionType)
                    {
                        case CollisionType.Enter:
                            Console.WriteLine("Enter: " + collisionInfo.SelfCollider.Tag);
                            break;
                        case CollisionType.Stay:
                            break;
                        case CollisionType.Exit:
                            Console.WriteLine("Exit: " + collisionInfo.SelfCollider.Tag);
                            break;
                    }
                }
            }
        };

        class CollidingObject2 : TextureObject2D
        {
            protected override void OnAdded()
            {
                var rectCollider = new RectangleCollider();
                rectCollider.Area = new RectF(0, 0, 100, 100);
                rectCollider.Tag = "D";
                rectCollider.Visible = true;
                AddCollider(rectCollider);
                Scale = new Vector2DF(0.5f, 0.5f);
            }

            protected override void OnUpdate()
            {
                Position = Engine.Mouse.Position;
            }
        };

        public CollidingMethod()
                : base(600)
        {
        }

        protected override void OnStart()
        {
            var obj1 = new CollidingObject1();
            obj1.Position = new Vector2DF(320, 240);
            var obj2 = new CollidingObject2();
            obj2.Position = new Vector2DF(600, 20);

            var scene = new Scene();
            var layer = new Layer2D();
            Engine.ChangeScene(scene);
            scene.AddLayer(layer);
            layer.AddObject(obj1);
            layer.AddObject(obj2);
        }
    }
}
