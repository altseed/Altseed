## 2Dコライダ (Collider2D)

### 概要

Object2Dに登録することで衝突判定を行う2Dのコライダを扱うクラスです。ただしこのクラスのインスタンスは生成できず、サブクラスである[円](./CircleCollider.md)、[矩形](./RectangleCollider.md)、[線分](./LineCollider.md)ならびに[多角形](./PolygonCollider.md)からインスタンスを生成します。

Object2Dに関しては[オブジェクト2D](../2D/Object2D.md)の項を参照してください。

### 形状の情報について

以下の図に示したように、コライダはObject2DのTransformプロパティ(Scale, Angle, Position)に応じて自動的に変形されます。

よって、コライダの形状を決定づけるパラメータはそれらのTransformプロパティがデフォルトの状態におけるObject2Dの原点から見た相対的な位置に配置してください。

![コライダの形状について](img/Collider2D_Transform.png)

### 主なプロパティ

| 名称 | 説明 |
|---|---|
| Collider2DShapeType | この2Dコライダクラスの実体がどの形状であるか（円、矩形、線分、多角形のいずれか） |
| Tag | この2Dコライダクラスにつけるタグ（文字列） |
| IsVisible | この2Dコライダを所属するObject2Dが描画される際に可視化するか否か（デバッグ用オプション） |
| OwnerObject | この2Dコライダが属するObject2Dのインスタンス |

### 使用方法

サンプル準備中...