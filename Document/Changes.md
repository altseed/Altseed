## 変更点

### 2015/07/07

* 画面をgifアニメーションとして録画する機能を追加(asd.Engine.CaptureScreenAsGifAnimation)
* LayerにOnStart(レイヤーがシーンに追加された時に呼ばれるイベント)を追加
* SceneにOnStart(シーンが実行され始めた時に呼ばれるイベント)を追加
* 動的フォント生成機能を追加(asd.Graphics.CreateDynamicFont)

* asd.Font.CalcTextureSize() のconstを除去(C++のみ破壊的変更)
* ポストエフェクトを自作する時に記述する定義部分を変更(破壊的変更)
* asd.StaticFile.Bufferの型がbyte[]に変更(C#のみ破壊的変更)

* asd.Texture2D.Lockの挙動がDirectXとOpenGLで異なっていた不具合を修正

### 2015/06/16

* GeometryObject2Dで図形が設定されていない時に表示しようとすると落ちる不具合の修正

### 2015/05/31

* 様々な不具合の修正

### 2015/05/30

* 一般公開