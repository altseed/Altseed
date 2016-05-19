# イベント機能

あるオブジェクトに対して毎フレーム実行したい処理などがある場合、**イベント機能**を使うと便利です。たとえば、キャラクターを何秒かかけて横に移動させたい場合は、１フレームごとにオブジェクトのX座標を増減させることで実現できます。

このページでは、シーン、レイヤー、オブジェクトを**コンテンツ**と呼ぶことにします。オブジェクトからみたレイヤー、レイヤーから見たシーンを**コンテナ**と呼ぶことにします。

## イベントの利用方法

シーン、レイヤー、オブジェクトには`OnUpdate`や`OnAdded`, `OnDispose`などのイベントがメソッドとして定義してあり、シーン、レイヤー、オブジェクトを継承したクラスでこれらをオーバーライドすることで独自のイベント処理を特定のタイミングで実行することができます。どんなイベントがあるかを次に紹介します。

### OnUpdate系

毎フレーム呼び出されます。シーンとレイヤーでは`OnUpdating`, `OnUpdated`というペアになっており、`OnUpdating`に書いた処理はコンテンツの`OnUpdate`が呼ばれる直前に実行されます。`OnUpdated`に書いた処理はコンテンツの`OnUpdate`が呼ばれた直後に実行されます。

### OnDrawAdditionally

毎フレーム呼び出される点は`OnUpdate`系と同じですが、主に追加の描画に関する処理を書く場所として用意してあり、`Object.DrawRectangleAdditionally`などの追加描画系メソッドはこの中でのみ呼び出すことができます。

### OnAdded系

コンテンツがコンテナに登録されたときに呼び出されます。オブジェクトのコンストラクタが呼ばれる時点では`Object.Layer`が`null`になっていて自分を登録しているレイヤーを扱うことができませんが、このメソッドが呼ばれるときはコンテナにアクセスできることが保証されています。

シーンの`OnAdded`に相当するイベントはシーン遷移に関するタイミングに合わせて３つに分かれています。

### OnRemoved系

コンテンツがコンテナから登録解除されたときに呼び出されます。このメソッドが呼ばれるときはコンテナにアクセスできることが保証されています。

シーンの`OnRemoved`に相当するイベントはシーン遷移に関するタイミングに合わせて３つに分かれています。

### OnDispose

コンテンツが`Dispose`メソッドや`Engine.Terminate`によって破棄されるときに呼ばれます。コンテンツが確保しているリソースの破棄などに利用することができます。

## イベント早見表

シーン、レイヤー、オブジェクトには上で述べたイベントが次のように実装されています。

||OnAdded|OnRemoved|OnDispose|OnUpdate|OnDrawAdditionally|
|---|---|---|---|---|---|
|Scene|OnRegistered, OnStartUpdating, OnTransitionFinished|OnTransitionBegin, OnStopUpdating, OnUnregistered|○|Updating, Updated| |
|Layer|○|○|○|Updating, Updated|○|
|Object|○|○|○|○|○|

### シーンのOnAdded, OnRemoved系イベント

シーンの`OnAdded`, `OnRemoved`系イベントは次のようになっています。

![シーンのイベント](img/SceneEvents.png)

* `OnRegistered`：Engine.ChangeSceneによるこのシーンへの遷移開始時
* `OnStartUpdating`：このシーンへのフェードイン開始時。シーンのUpdateが始まるタイミング
* `OnTransitionFinished`：このシーンへのフェードイン終了時
* `OnTransitionBegin`：このシーンからのフェードアウト開始時
* `OnStopUpdating`：このシーンからのフェードアウト終了時。シーンのUpdateが止まるタイミング
* `OnUnregistered`：Engine.ChangeSceneによるこのシーンからの遷移終了時