
## 破棄イベント

オブジェクト、レイヤー、シーンには、使わなくなって破棄した時に発生させたい処理を書けるイベントが用意されています。
オブジェクトの破棄時にストリームを閉じたり、C#でのイベントの購読解除をしたりすることができます。

### オブジェクトの破棄イベント

オブジェクトのVanishメソッドを呼び出すとオブジェクトが破棄されます。そのときOnVanish, OnDisposeメソッドが呼びだされます。

* include_basic_sample Dispose_Object2D

#### 実行結果

```
Objectを破棄します
MessageObject.OnVanish
MessageObject.OnDispose
```

### レイヤーの破棄イベント

レイヤーのVanishメソッドを呼び出すとレイヤーとそれに属するすべてのオブジェクトが破棄されます。
そのときレイヤーOnVanish, OnDisposeメソッドと、各オブジェクトのOnDisposeが呼びだされます。
オブジェクトのOnVanishは呼び出されないことに注意してください。

* include_basic_sample Dispose_Layer2D

#### 実行結果

```
Layerを破棄します
MessageLayer.OnVanish
MessageObject.OnDispose
MessageLayer.OnDispose
```