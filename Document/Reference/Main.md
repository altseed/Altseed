## リファレンス

各機能ごとの説明を記述します。

プロパティはC++の場合、プロパティに相当する機能が存在しないため、Get○○、Set○○といったメソッドで実装されています。

* [基本設計](./BasicDesign.md)
* [対応ファイルフォーマット](./AvailableFileFormats.md)

### 基本

* [エンジン](./Basic/Engine.md)
* [シーン](./Basic/Scene.md)
* [レイヤー](./Basic/Layer.md)

### 描画

* [グラフィックス](./Graphics/Graphics.md)
* [テクスチャ2D](./Graphics/Texture2D.md)
* [エフェクト](./Graphics/Effect.md)
* [フォント](./Graphics/Font.md)
* [画像パッケージ](./Graphics/ImagePackage.md)

#### 2D

* [レイヤー2D](./2D/Layer2D.md)
* [オブジェクト2D](./2D/Object2D.md)
* [テクスチャオブジェクト2D](./2D/TextureObject2D.md)
* [エフェクトオブジェクト2D](./2D/EffectObject2D.md)
* [テキストオブジェクト2D](./2D/TextObject2D.md)
* [マップオブジェクト2D](./2D/MapObject2D.md)
* [図形オブジェクト2D](./2D/GeometryObject2D.md)
* [カメラオブジェクト2D](./2D/CameraObject2D.md)

* [チップ2D](./2D/Chip2D.md)

#### 2D図形

* [図形](./Shape/Shape.md)
* [弧](./Shape/ArcShape.md)
* [円](./Shape/CircleShape.md)
* [三角形](./Shape/TriangleShape.md)
* [矩形](./Shape/RectangleShape.md)
* [多角形](./Shape/PolygonShape.md)
* [線分](./Shape/LineShape.md)

#### 3D

* 開発中

#### ポストエフェクト

* [ポストエフェクト](./PostEffect/PostEffect.md)
* [グレースケール](./PostEffect/PostEffectGrayScale.md)
* [セピア](./PostEffect/PostEffectSepia.md)
* [ガウスぼかし](./PostEffect/PostEffectGaussianBlur.md)
* [ライトブルーム](./PostEffect/PostEffectLightBloom.md)

#### シーン遷移

* [トランジション](./Transition/Transition.md)
* [フェード](./Transition/TransitionFade.md)

### 音

* [サウンド](./Sound/Sound.md)
* [音源](./Sound/SoundSource.md)

### 入力

* [マウス](./Input/Mouse.md)
* [キーボード](./Input/Keyboard.md)
* [ジョイスティック](./Input/Joystick.md)
* [ジョイスティックコンテナ](./Input/JoystickContainer.md)

### ファイル

* [ファイル](./IO/File.md)
* [スタティックファイル](./IO/StaticFile.md)
* [ストリームファイル](./IO/StreamFile.md)

### ツール

* [フォントジェネレーター](./Tool/FontGenerator.md)
* [イメージパッケージジェネレーター](./Tool/ImagePackageGenerator.md)
* [ファイルパッケージングツール](./Tool/FilePackagingTool.md)

### 他
* [リロード](./Misc/Reloading.md)
