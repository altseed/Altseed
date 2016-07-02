using System.Collections.Generic;
/// <summary>
/// キーボードの複数のキーの入力状態を取得するサンプル。
/// </summary>
public class Keyboard_PuralKeys : ISample
{
    public string Description
    {
        get { return ""; }
    }
    public string Title
    {
        get { return ""; }
    }

    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("Keyboard_PuralKeys", 640, 480, new asd.EngineOption());

        var font = asd.Engine.Graphics.CreateDynamicFont("", 40, new asd.Color(255, 255, 255, 255), 1, new asd.Color(0, 0, 0, 255));

        // キーの入力状態を表示する文字描画オブジェクトを設定して、エンジンに追加する。
        var keyStateText = new asd.TextObject2D();
        keyStateText.Position = new asd.Vector2DF(10, 10);
        keyStateText.Font = font;
        asd.Engine.AddObject2D(keyStateText);

        // 入力チェックするキー一覧。(Z, X, C, V, B)
        List<asd.Keys> keys = new List<asd.Keys>();
        keys.Add(asd.Keys.Z);
        keys.Add(asd.Keys.X);
        keys.Add(asd.Keys.C);
        keys.Add(asd.Keys.V);
        keys.Add(asd.Keys.B);

        //キーの文字列表現。
        List<string> keyStrs = new List<string>();
        keyStrs.Add("Z");
        keyStrs.Add("X");
        keyStrs.Add("C");
        keyStrs.Add("V");
        keyStrs.Add("B");

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            string displayStr = "";

            //すべてのキーに対して入力状態を確認してから表示する。
			for (int i = 0; i < keys.Count; i++)
            {
                string keystr = keyStrs[i];

                // キーボードのZキーの入力状態を取得する。
                asd.KeyState state = asd.Engine.Keyboard.GetKeyState(keys[i]);

                if (state == asd.KeyState.Free) // 前フレームと本フレームで非押下
                {
                    displayStr += (keystr + "キーを離しています。");
                }
                else if (state == asd.KeyState.Hold) // 前フレームと本フレームで押下
                {
                    displayStr += (keystr + "キーを押しています。");
                }
                else if (state == asd.KeyState.Release) // 前フレームで押下、本フレームで非押下
                {
                    displayStr += (keystr + "キーを離しました!");
                }
                else if (state == asd.KeyState.Push) // 前フレームで非押下、本フレームで押下
                {
                    displayStr += (keystr + "キーを押しました!");
                }

                displayStr += "\n";
            }

            // キー入力状態を示す文字列を更新する。
            keyStateText.Text = displayStr;

            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.CaptureScreen("Keyboard_PuralKeys", 30, 60, 0.2f, 0.5f);
        }

        //Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
