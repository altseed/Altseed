
## C++版の導入(Windows)

C++版の導入方法について説明しています。

### VisualStudio2013の導入

[VisualStudioのWebページ](https://www.visualstudio.com/ja-jp/downloads/download-visual-studio-vs#DownloadFamilies_2)から
「Visual Studio Community 2013 Update 4」をダウンロードします。ただし、会社内でAltseedを使用する場合は「Visual Studio Express 2013 with Update 4 for Windows Desktop」をダウンロードします。
これらはプログラムのソースコードから実行可能なアプリケーションを生成するツールです。個人で開発する分には無料で使用できます。

ダウンロードが終わったら、「Visual Studio」をインストールします。

### Altseedのダウンロード
[Altseed](https://github.com/altseed/Altseed/releases/download/20160412/Altseed_CPP_20160412_WIN.zip)　をダウンロードします。
ダウンロードしたファイルを解凍します。

開発には以下のファイルを使用します。

* Runtime/Altseed.h
* Runtime/Altseed\_core_Debug.dll
* Runtime/Altseed\_core.dll
* Runtime/Debug/Altseed.lib
* Runtime/Release/Altseed.lib

### プロジェクトの作成

Visual Studioで開発するためには、最初にプロジェクトを作成する必要があります。以下の手順でプロジェクトを作成します。

1. ```ファイル->新規作成->プロジェクト```を選択します。
2. 左のカラムから```インストール済み->テンプレート->Visual C++```を選択した後、中央のカラムから```コンソールアプリケーション```を選択します。
3. 下のテキストボックスの```名前```にプロジェクト名を入力して```OK```を選択します。ここからは```Altseed_Test```と入力したと仮定して説明をします。
4. ダイアログボックスが表示されますが、何も設定を変更せずにOKを押します。

これでプロジェクトの作成が完了しました。

### プロジェクトの設定

プロジェクトの設定をします。

1. ソリューションエクスプローラから```Altseed_Test```プロジェクトを右クリックし、プロパティを選択します。
2. ```構成プロパティ->C++->コード設定```を選択します。そして、ランタイムライブラリを```マルチスレッドデバッグDLL```から```マルチスレッドデバッグ```に変更します。
3. ```適用```をクリックします。
4. 左上の```アクティブ(Debug)```を選択し、```Release```に切り替えます。
5. ```構成プロパティ->C++->コード設定```を選択します。そして、ランタイムライブラリを```マルチスレッドデバッグ```から```マルチスレッド```に変更します。
6. ```OK```をクリックします。

プロジェクトの設定が終わったらAltseedを追加します。

1. Runtimeの中身を全て、```Altseed_Test/Altseed_Test/```にコピーします。

最低限の動作をさせるならば、とりあえずコピーするだけで動作します。

### ビルド、実行

Altseed_Test.cpp を以下のソースコードに書き換えます。

```
// Altseed_Test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

// Altseedのヘッダを読み込む
#include "Altseed.h"

// Altseedのライブラリを読み込む
#if _DEBUG
#pragma comment(lib,"Debug/Altseed.lib")
#else
#pragma comment(lib,"Release/Altseed.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Empty").c_str(), 640, 480, asd::EngineOption());

	// Altseedが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();

	return 0;
}


```

そして、メニューバーから```デバッグ->デバッグ開始```を選択します。
黒いウインドウが表示されれば成功です。

### 今後開発する上での設定

今回は、Altseedのランタイムをコピーで済ませました。しかし、実際に開発する際は全てのファイルを配置し直すことをおすすめします。

```Debug/Altseed.lib``` と ```Release/Altseed.lib``` は名称を変更し、 ```Altseed_Test.vcxproj``` と同じディレクトリに移動しましょう。 ```Debug``` と ```Release``` というフォルダは
VisualStudioがコンパイルをする際に一時的に生成されるファイルが配置されるディレクトリでもあります。不要なファイルを消すときに間違えて消してしまうかもしれません。

```Altseed_core.Debug.dll``` と ```Altseed_core.dll``` は生成された実行ファイルが存在するディレクトリにもコピーしましょう。VisualStudioからデバッグしている間は問題は置きませんが、
実行ファイルを起動する際は、実行ファイルと同じディレクトリにこれらのdllが存在する必要があります。




