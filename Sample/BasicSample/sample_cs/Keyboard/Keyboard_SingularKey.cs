/// <summary>
/// キーボードの一つのキーの入力状態を取得するサンプル。
/// </summary>
public class Keyboard_SingularKey : ISample
{
	public void Run()
    {   
		// Altseedを初期化する。
        asd.Engine.Initialize("Keyboard_SingularKey", 640, 480, new asd.EngineOption());

        var font = asd.Engine.Graphics.CreateDynamicFont(string.Empty, 20, new asd.Color(255, 255, 255, 255), 1, new asd.Color(0, 0, 0, 255));

        // キーの入力状態を表示する文字描画オブジェクトを設定して、エンジンに追加する。
        var keyStateText = new asd.TextObject2D();
        keyStateText.Position = new asd.Vector2DF(10, 10);
        keyStateText.Font = font;
        asd.Engine.AddObject2D(keyStateText);

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // キーボードのZキーの入力状態を取得する。
            // キー入力状態を示す文字列を更新する。
            switch (asd.Engine.Keyboard.GetKeyState(asd.Keys.Z))
            {
                case asd.KeyState.Free: // 前フレームと本フレームで非押下
                    keyStateText.Text = "Zキーを離しています。";
                    break;
                case asd.KeyState.Hold: // 前フレームと本フレームで押下
                    keyStateText.Text = "Zキーを押しています。";
                    break;
                case asd.KeyState.Release: // 前フレームで押下、本フレームで非押下
                    keyStateText.Text = "Zキーを離しました!";
                    break;
                case asd.KeyState.Push: // 前フレームで非押下、本フレームで押下
                    keyStateText.Text = "Zキーを押しました!";
                    break;
            }

            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.TakeScreenShot("Keyboard_SingularKey", 30);
        }

        //Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
