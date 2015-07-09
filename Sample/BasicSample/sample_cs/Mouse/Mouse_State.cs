/// <summary>
/// マウスの様々な状態を取得するサンプル。
/// </summary>
public class Mouse_State : ISample
{
    public void Run()
    {
        //Altseedを初期化する。
        asd.Engine.Initialize("Mouse_State", 640, 480, new asd.EngineOption());

        // マウスの状態を表示するテキストを生成する。
        var font = asd.Engine.Graphics.CreateDynamicFont(string.Empty, 20, new asd.Color(255, 255, 255, 255), 1, new asd.Color(0, 0, 0, 255));

        // マウスの中ボタンの入力状態を表示する文字描画オブジェクトを設定して、エンジンに追加する。
        var buttonStateText = new asd.TextObject2D();
        buttonStateText.Position = new asd.Vector2DF(10, 10);
        buttonStateText.Font = font;
        asd.Engine.AddObject2D(buttonStateText);

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // マウスの中央クリック状態を取得して表示する。
            switch (asd.Engine.Mouse.MiddleButton.ButtonState)
            {
                case asd.MouseButtonState.Free: //前フレームと本フレームで非押下
                    buttonStateText.Text = "中ボタンを離しています。";
                    break;
                case asd.MouseButtonState.Hold: //前フレームと本フレームで押下
                    buttonStateText.Text = "中ボタンを押しています。";
                    break;
                case asd.MouseButtonState.Release: //前フレームで押下、本フレームで非押下
                    buttonStateText.Text = "中ボタンを離しました!";
                    break;
                case asd.MouseButtonState.Push: //前フレームで非押下、本フレームで押下
                    buttonStateText.Text = "中ボタンを押しました!";
                    break;
            }

            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.TakeScreenShot("Mouse_State", 30);
        }

        //Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
