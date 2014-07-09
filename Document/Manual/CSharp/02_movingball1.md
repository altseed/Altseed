ACE Tutorial (C#)
=================

MovingBall
--------------

### 概要

ここではACEに限らず、ゲームエンジンでのゲーム作成における基本概念、Scene、Layerなどの説明を交えながら、ボールが画面内を動き回るといったごく簡単なアニメーションを作っていきます。

### プロジェクトの用意

「ACEの導入」を参考にしてプロジェクト名を「MovingBall」にしたプロジェクトを作成して下さい。そして、生成されたProgram.csを以下のように書き換えてください。

```csharp
using ace;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MovingBall
{
    class Program
    {
        // 他クラスからウィンドウサイズを読み込めるようにする
        public static readonly int WIDTH = 854, HEIGHT = 480;

        static void Main(string[] args)
        {
            var option = new EngineOption { GraphicsType = GraphicsType.DirectX11, IsFullScreen = false };

            Engine.Initialize("Moving Ball", WIDTH, HEIGHT, option);

            while (Engine.DoEvents())
            {
                Engine.Update();

                // 現在のSceneをコンソールに出力
                Console.Clear();
                Console.WriteLine("Current Scene : " + Engine.CurrentScene);
            }

            Engine.Terminate();
        }
    }
}
```

導入のときと違うことは、他クラスからウィンドウサイズを読み込めるように変数を追加したことと、while文の中に現在のSceneが分かるように`Console.WriteLine()`を追加したことです。

### Sceneの追加

Sceneクラスはゲーム画面を構成するクラスです。普通、ゲームにはスタート画面、プレイ画面、ゲームオーバー画面などが存在すると思います。その画面ごとにSceneを継承した新しいクラスを定義してゲーム画面を作っていきます。

ボールを動かすための画面、`BallScene()`を作成します。ソリューションエクスプローラから「MovingBall」プロジェクトを右クリックして「追加>既存の項目」を選択、名前を「BallScene.cs」にしたクラスファイルを作ってください。そして、その中身を以下のように書き換えます。

```csharp
using ace;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MovingBall
{
    class BallScene : Scene
    {
        // コンストラクター
        public BallScene()
        {

        }

        // このSceneがアップデートされるごとに呼び出されるメソッド
        protected override void OnUpdated()
        {
            base.OnUpdated();
        }
    }
}
```

Visual Studioを使っている方はこのソースの`Scene`を選択し、F12キーを押して、このクラスが用意しているメソッドなどを見てみましょう。メソッドの名前などから、Sceneクラスがどういう機能を持っているのかがなんとなく分かると思います。Sceneが更新される度に呼び出される`OnUpdate()`をオーバーライドしておきます。

これでとりあえず、Sceneクラスが用意出来ました。これをEngineに反映させます。「Program.cs」の`Engine`を選択してF12で定義を見てみましょう。中に`ChangeScene()`メソッドが含まれているはずです。ACEではこれを使ってSceneの変更を行います。

それでは、以下のコードを「Program.cs」のwhile文の直前に追加し、デバッグで実行してみてください（F5キーでDebugモードで開始）

```csharp
var scene = new BallScene();
Engine.ChangeScene(scene);
```

新しい変数を用意しBallSceneクラスをインスタンス化して代入し、Engineの`ChangeScene()`メソッドでSceneの切り替えをしました。コンソールを見るとCurrent SceneがMovingBall.BallSceneとなっており、たしかにSceneが追加されたことが分かると思います。

Sceneによってゲーム画面の用意が出来ました。これにLayerや実際に画面上を動くオブジェクトなどを追加し、ゲームを構成していきます。