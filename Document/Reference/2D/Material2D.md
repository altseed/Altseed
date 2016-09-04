## マテリアル2D (Material2D)

### 概要

[ポストエフェクト](./../PostEffect/PostEffect.md)やスプライトの描画をユーザーが実装するために使用するマテリアルクラスです。
シェーダー2Dを元に生成され、シェーダー2Dに記述されたシェーダーを実行し、画像を加工します。

マテリアル2Dクラスはシェーダーに記述された変数に値を設定する機能を持ちます。
変数を設定して、ポストエフェクトクラスの```DrawOnTexture2DWithMaterial``` にマテリアル2Dのインスタンスと出力先を渡して実行すると引数に設定された出力先に画像を描画します。

基本的には、PostEffectクラスを継承した時の ```OnDraw ``` w内で使用します。

### 主なメソッド

Shaderの変数の型に対応したGetSetが存在します。

| 名称 | 説明 |
|---|---|
| GetVector2DF | シェーダー内の指定された変数名のVector2DF型の値を取得します。 |
| SetVector2DF | シェーダー内の指定された変数名のVector2DF型の値を設定します。 |
| GetTexture2D | シェーダー内の指定された変数名のTexture2D型の値を取得します。 |
| SetTexture2D | シェーダー内の指定された変数名のTexture2D型の値を設定します。 |


### 主なプロパティ

なし

### 主なイベント

なし

### 使用方法

マテリアル2D経由で変数に値を設定し、モザイクを一部領域にかけるサンプルです。

* include_application_sample CustomPostEffect_Mosaic



