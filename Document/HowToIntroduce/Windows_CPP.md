
## C++版の導入(Windows)

C++版の導入方法について説明しています。

### VisualStudio2013の導入

[VisualStudioのWebページ](https://www.visualstudio.com/ja-jp/downloads/download-visual-studio-vs#DownloadFamilies_2)から
「Visual Studio Community 2013 Update 4」をダウンロードします。ただし、会社内でACEを使用する場合は「Visual Studio Express 2013 with Update 4 for Windows Desktop」をダウンロードします。
これらはプログラムのソースコードから実行可能なアプリケーションを生成するツールです。個人で開発する分には無料で使用できます。

ダウンロードが終わったら、「Visual Studio」をインストールします。

### ACEのダウンロード
[Download](https://drive.google.com/folderview?id=0B1gZCvEfcQAiMjhaU1VZOVRTUWM&usp=sharing)　からACE_CPP～.7zをダウンロードします。
7-Zipファイルを解凍できる解凍ソフトを用いて解凍します。

開発には以下のファイルを使用します。

* Runtime/ace.h
* Runtime/ace_core.Debug.dll
* Runtime/ace_core.dll
* Runtime/Debug/ace\_engine.lib
* Runtime/Release/ace\_engine.lib

### プロジェクトの作成

Visual Studioで開発するためには、最初にプロジェクトを作成する必要があります。以下の手順でプロジェクトを作成します。

1. ```ファイル->新規作成->プロジェクト```を選択します。
2. 左のカラムから```インストール済み->テンプレート->Visual C++```を選択した後、中央のカラムから```コンソールアプリケーション```を選択します。
3. 下のテキストボックスの```名前```にプロジェクト名を入力して```OK```を選択します。ここからは```ACE_Test```と入力したと仮定して説明をします。
4. ダイアログボックスが表示されますが、何も設定を変更せずにOKを押します。

これでプロジェクトの作成が完了しました。

### プロジェクトの設定

プロジェクトの設定をします。

1. ソリューションエクスプローラから```ACE_Test```プロジェクトを右クリックし、プロパティを選択します。
2. ```構成プロパティ->C++->コード設定```を選択します。そして、ランタイムライブラリを```マルチスレッドデバッグDLL```から```マルチスレッドデバッグ```に変更します。
3. ```適用```をクリックします。
4. 左上の```アクティブ(Debug)```を選択し、```Release```に切り替えます。
5. ```構成プロパティ->C++->コード設定```を選択します。そして、ランタイムライブラリを```マルチスレッドデバッグ```から```マルチスレッド```に変更します。
6. ```OK```をクリックします。

プロジェクトの設定が終わったらACEを追加します。

1. Runtimeの中身を全て、```ACE_Test/ACE_Test/```にコピーします。

最低限の動作をさせるならば、とりあえずコピーするだけで動作します。

### ビルド、実行

ACE_Test.cpp を以下のソースコードに書き換えます。

```
// ACE_Test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

// ACEのヘッダを読み込む
#include "ace.h"

// ACEのライブラリを読み込む
#if _DEBUG
#pragma comment(lib,"Debug/ace_engine.lib")
#else
#pragma comment(lib,"Release/ace_engine.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	// aceを初期化する。
	ace::Engine::Initialize(ace::ToAString("Empty").c_str(), 640, 480, ace::EngineOption());

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		// aceを更新する。
		ace::Engine::Update();
	}

	// aceを終了する。
	ace::Engine::Terminate();

	return 0;
}


```

そして、メニューバーから```デバッグ->デバッグ開始```を選択します。
黒いウインドウが表示されれば成功です。

### 今後開発する上での設定

今回は、ACEのランタイムをコピーで済ませました。しかし、実際に開発する際は全てのファイルを配置し直すことをおすすめします。

```Debug/ace_engine.lib``` と ```Release/ace_engine.lib``` は名称を変更し、 ```ACE_Test.vcxproj``` と同じディレクトリに移動しましょう。 ```Debug``` と ```Release``` というフォルダは
VisualStudioがコンパイルをする際に一時的に生成されるファイルが配置されるディレクトリでもあります。不要なファイルを消すときに間違えて消してしまうかもしれません。

```ace_core.Debug.dll``` と ```ace_core.dll``` は生成された実行ファイルが存在するディレクトリにもコピーしましょう。VisualStudioからデバッグしている間は問題は置きませんが、
実行ファイルを起動する際は、実行ファイルと同じディレクトリにこれらのdllが存在する必要があります。




