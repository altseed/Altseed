/// <summary>
/// ジョイスティックにある全てのアナログスティックの入力状態を取得するサンプル。
/// </summary>
class Joystick_Axis : ISample
{
    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("Joystick_Axis", 640, 480, new asd.EngineOption());

        // ジョイスティックの状態を表示するテキストを生成する。
        var font = asd.Engine.Graphics.CreateDynamicFont(string.Empty, 25, new asd.Color(255, 255, 255, 255), 1, new asd.Color(0, 0, 0, 255));

        // アナログスティックの入力状態を表示する文字描画オブジェクトを設定して、エンジンに追加する。
        var stateText = new asd.TextObject2D();
        stateText.Position = new asd.Vector2DF(10, 10);
        stateText.Font = font;
        asd.Engine.AddObject2D(stateText);

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            string displayStr = string.Empty;

            // ジョイスティックが接続されているかどうかを確認する。
            if (!asd.Engine.JoystickContainer.GetIsPresentAt(0))
            {
                displayStr += "ジョイスティックが接続されていません。";
            }
            else
            {
                // 1つ目のジョイスティックの全てのアナログスティックの入力状態を表示する
                var joystick = asd.Engine.JoystickContainer.GetJoystickAt(0);

                for (int axisIndex = 0; axisIndex < joystick.AxesCount; ++axisIndex)
                {
                    var axisVal = joystick.GetAxisState(axisIndex);
                    displayStr += ("軸 " + axisIndex.ToString() + ": ");
                    displayStr += axisVal.ToString();
                    displayStr += "\n";
                }

            }

            stateText.Text = displayStr;

            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.TakeScreenShot("Joystick_Axis", 30);
        }

        //Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
