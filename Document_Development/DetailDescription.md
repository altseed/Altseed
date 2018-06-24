# コーディングに関する解説

## ファイル群

### dll.h/dll.cxx

`generate_swig.py` スクリプトによって生成されるファイル。DLLに関数を公開するためのファイルのペア。DLLに公開したい関数を増やした場合は生成し直す必要がある。

ヘッダファイルには、cxx側で使うクラスのincludeが並ぶ。
cxx側にはCore側のDLLに公開する関数をC関数(`extern "C"`)が並ぶ。

### asd.Core.Base.h/asd.Core.Base_Imp.h

プロジェクト全体にわたって参照されるクラスや列挙体を定義するための場所。クラスの部分定義を書けば、各クラスのヘッダファイルなどで部分定義を使ってシグネチャを書くことができる。列挙体はここで実体を定義してしまってもよい。

無印版のファイルには基底クラスの部分定義と列挙体を書き、Imp版のファイルには実装クラスの部分定義を書く。

以上のことは推奨項目であり、これらのファイルに部分定義を書かないこともあるし、これらのファイルをincludeしないこともある。

## 列挙体

Core側に追加した列挙体をC#Engineのコードに持っていくには、[HowToCompile.md](HowToCompile.md)で解説されている手順を踏む必要がある。その手順の前に `generateCoreToEngineHeader.py`, `generateEngineHeader.py` を編集して、追加した生成ファイルが `Altseed.h` に含まれるように設定する必要があるが、 `asd.Core.Base.h` に追加されたものは現状ですでに `generateCoreToEngineHeader.py` が含むように設定されているので大丈夫なはず。

# メモリ管理に関する解説

## GC制御

AltseedのGC制御にはいくつかの登場人物がいる。

* `GC` : `GarbageCollector`, `IDObjectContainer` クラスをまとめるクラス。swigクラスからC#インスタンスへの変換も担う。
* `GarbageCollector` : Releaseすべきswigクラスを `AddObject` メソッドで登録することで、適切なタイミングでReleaseするためのクラス。
* `IDObjectContaiter` : 次節「オブジェクトの使い回し」で示すオブジェクトの使い回しのために、C#インスタンスをキャッシュしておくクラス。

ネイティブリソースの参照カウンタを減らしたり、C#インスタンスを開放する流れは以下の通り。

1. C#インスタンスがGCされる
2. C#インスタンスのデストラクタが呼ばれる
3. `ForceToRelease` が `GarbageCollector.AddObject` でネイティブリソースの破棄を登録する
4. 次のフレームで `GarbageCollector.Collect` がネイティブリソースの参照カウンタを減らす
5. `IDObjectContainer.Collect` がC#インスタンスへの弱参照をキャッシュ解除する

または

1. `Engine.Terminate` が呼ばれる
2. `asd.GC.Terminate`が呼ばれる
3. `IDObjectContainer.DestroyAll` がネイティブリソースの破棄を登録する
4. すぐに`GarbageCollector.Collect` がネイティブリソースの参照カウンタを減らす
5. 念のためC#インスタンスへの弱参照をキャッシュ解除する

前者の例では、C#インスタンスのデストラクタが呼ばれた次のフレームまでにネイティブリソースの参照カウンタが減る。

後者の例では、Altseedが終了するのだからすべてのC#インスタンスを破棄し、ネイティブリソースの参照カウンタも減る。

結果的に、ネイティブリソースの参照カウンタが減るタイミングはC#インスタンスが破棄されるタイミングと同時になる。C#インスタンスが先に消えて、可能な限りすぐにネイティブリソースの参照カウンタが減る仕組みになっている。

### Q&A

**Q.** `GarbageCollector.Collect` の直後に `IDOjectContainer.Collect` を呼び出しているのはなぜ？

**A.** ネイティブリソースの参照カウンタを減らす必要があったということは、対応するC#インスタンスが破棄されていると考えられるため。

**Q.** `GarbageCollector.AddObject` の呼び出し元では `GC.SuppressFinalize` メソッドを呼び出している。これだとそもそもC#インスタンスはGCされようがないのでは？

**A.** 調査中。デストラクタから呼ばれた場合はそもそもGCされる必要がないが、Terminateから呼ばれた場合にはどうなる？

**Q.** デストラクタで `ForceToRelease` をよく呼んでいるが、もうGCされているのだから `CoreInstance` を `null` にする意味も、 `GC.SuppressFinalize` する意味もないのでは？

**A.** `ForceToRelease` にはもう一つの役割がある。それは、そのインスタンスの保持していたネイティブリソースの参照カウンタを減らすことである。 `asd.GC.Collector.AddObject` を呼び出すことで、ネイティブリソースを開放するよう登録している。

## オブジェクトの使い回し

Altseedでは、ファイル・アドレス・C#インスタンスがそれぞれ一対一に対応している。2つの同じファイルを読み込むと同じアドレスに割り当てられ、同じアドレスに対しては同じC#インスタンスが取得される。`asd.GC` クラスを使って、C++側のオブジェクト生成関数から返ってきたアドレスをC#インスタンスに変換することができる。

以下は `File` クラスのメソッドの例である。 `CreateStaticFile` メソッドでCore側でインスタンスを作って返してもらい、 `asd.GC.GenerateStaticFile` メソッドを呼び出してそのアドレスに対応するインスタンスを取得している。

```csharp
public StaticFile CreateStaticFile(string path)
{
    var file_ = CoreInstance.CreateStaticFile(path);
    if (file_ == null) return null;
    return GC.GenerateStaticFile(file_, GenerationType.Create);
}
```

ちなみに `asd.GC` の内部でインスタンスのキャッシュが行われており、存在すればそこからC#インスタンスが返される。開放されたインスタンスはキャッシュ解除される。

### Q&A

**Q.** GCされたC#インスタンスはキャッシュ解除されてしまう。C++側でメモリ上に存在するのにC#側インスタンスがGCされてしまった場合は使い回せなくなってしまうのでは？

**A.** 前節「GC制御」で述べたように、C#インスタンスがGCされるとCore側のネイティブリソースの参照カウンタが減るようになっており、開放漏れは起きない。（ネイティブリソースの参照カウンタが1である前提？）