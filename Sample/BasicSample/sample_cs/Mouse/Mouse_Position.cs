/// <summary>
/// マウスの座標を取得を取得するサンプル。
/// </summary>
public class Mouse_Position : ISample
{
    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("Mouse_Position", 640, 480, new asd.EngineOption());

        // マウスの状態を表示するテキストを生成する。
        var font = asd.Engine.Graphics.CreateDynamicFont(string.Empty, 25, new asd.Color(255, 255, 255, 255), 1, new asd.Color(0, 0, 0, 255));

        // マウスカーソルの座標を表示する文字描画オブジェクトを設定して、エンジンに追加する。
        var stateText = new asd.TextObject2D();
        stateText.Position = new asd.Vector2DF(10, 10);
        stateText.Font = font;
        asd.Engine.AddObject2D(stateText);

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // マウスカーソルの座標を取得して表示する。
            asd.Vector2DF pos = asd.Engine.Mouse.Position;
            stateText.Text = "マウスカーソルの位置 : (" + pos.X.ToString() + "," + pos.Y.ToString() + ")";

            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.CaptureScreen("Mouse_Position", 30, 15, 2, 1);
        }

        //Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
