
## 破棄イベント

オブジェクト、レイヤー、シーンには、使わなくなって破棄した時に発生させたい処理を書けるイベントが用意されています。
オブジェクトの破棄時にストリームを閉じたり、C#でのイベントの購読解除をしたりすることなどに利用することができます。

### オブジェクトの破棄イベント

オブジェクトのDisposeメソッドを呼び出すとオブジェクトが破棄されます。そのときオブジェクトのOnDisposeメソッドが呼びだされますので、OnDisposeメソッドをオーバーライドしていればこのタイミングで自由な処理を実行できます。

* include_basic_sample Dispose_Object2D+Dispose_Object2D_MessageObject

#### 実行結果

```
Objectを破棄します
MessageObject.OnDispose
```

### レイヤーの破棄イベント

レイヤーのVanishメソッドを呼び出すとレイヤーとそれに属するすべてのオブジェクトが破棄されます。
そのときレイヤーとそれに属するオブジェクトのOnDisposeメソッドが呼びだされますので、OnDisposeメソッドをオーバーライドしていればこのタイミングで自由な処理を実行できます。

* include_basic_sample Dispose_Layer2D+Dispose_Layer2D_MessageObject+Dispose_Layer2D_MessageLayer

#### 実行結果

```
Layerを破棄します
MessageLayer.OnDispose
MessageObject.OnDispose
```