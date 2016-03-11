# 親子関係機能

## 概要

親子関係機能とは、オブジェクトに親子関係を設定することで、親のオブジェクトのパラメータの値を子のオブジェクトに受け継がせる機能です。たとえば、あるオブジェクトの周りをぐるぐる回るオブジェクトなどはこの機能を使うと手軽に実現できます。親子関係は2Dオブジェクト(`Object2D`の派生クラス)に対して設定できます。

## 使い方

`Object2D.AddChild`メソッドを呼び出すことでオブジェクトを子オブジェクトとして親オブジェクトに追加できます。引数に渡すための列挙体がいくつか定義されていて、以下のように使います。

```csharp
// ChildManagementMode, ChildTransformingModeは列挙体
// parentを親オブジェクト、childを子オブジェクトとして親子関係を設定する。変形情報のみを同期する。
parent.AddChild(child, ChildManagementMode.Nothing, ChildTransformingMode.All);
```

`DrawnObject2D.AddDrawnChild`メソッドを呼び出すことで、通常の親子関係で同期できる項目に加えて色と描画優先度も同期することができます。以下のように使います。

```csharp
// ChildDrawingModeは列挙体
// 変形情報と色を同期する。
parent.AddDrawnChild(child,
	ChildManagementMode.Nothing,
	ChildTransformingMode.All,
	ChildDrawingMode.Color);
```

`ChildManagementMode`と`ChildDrawingMode`型の引数にはOR演算子でつなげて複数の項目を指定できます。

```csharp
// レイヤーへの登録状況、更新するかどうかのフラグ、色、描画優先度を同期する。
parent.AddChild(child,
	ChildManagementMode.RegistrationToLayer | ChildManagementMode.IsUpdated,
	ChildTransformingMode.Nothing,
	ChildDrawingMode.Color | ChildDrawingMode.DrawingPriority);
```

## 同期できるものと同期方法

以下のパラメータを同期することができます。

### オブジェクト管理系(ChildManagementMode)

|パラメータ|同期方法|
|---|---|
|RegistrationToLayer|親が追加されているレイヤーに子も自動的に追加されます。親がレイヤーから取り除かれると、子も取り除かれます。|
|Disposal|親が`Dispose`メソッドによって削除されるとき、子も`Dispose`メソッドによって削除されます。|
|IsUpdated|親が更新されないときに子も更新されないようにします。|
|IsDrawn|親が描画されないときに子も描画されないようにします。|

### 変形系(ChildTransformingMode)

|パラメータ|同期方法|
|---|---|
|位置(`Position`)|親の位置を子の位置の基準点にします。|
|すべての変形(`All`)|親の位置・拡大・回転を子の変形の基準にします。|

### 描画系(ChildDrawingMode)

これらのパラメータは`DrawnObject2D`の派生クラスでのみ同期することができます。

|パラメータ|同期方法|
|---|---|
|Color|親の色を子の色に乗算します。|
|DrawingPriority|親の描画優先度を子の描画優先度に加算します。|

## 親子関係関連のオブジェクトのプロパティ

`Object2D`クラスには以下のプロパティが用意されており、親子関係に関する情報にアクセスできます。

|名称|説明|
|---|---|
|Parent|このオブジェクトの親オブジェクトを取得します。親オブジェクトがなければnull。|
|Children|このオブジェクトの子オブジェクトのコレクションを取得します。|