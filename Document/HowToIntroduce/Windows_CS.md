
## C#版の導入(Windows)

C#版の導入方法について説明しています。

### VisualStudio2013の導入

[VisualStudioのWebページ](https://www.visualstudio.com/ja-jp/downloads/download-visual-studio-vs#DownloadFamilies_2)から
「Visual Studio Community 2013 Update 4」をダウンロードします。ただし、会社内でAltseedを使用する場合は「Visual Studio Express 2013 with Update 4 for Windows Desktop」をダウンロードします。
これらはプログラムのソースコードから実行可能なアプリケーションを生成するツールです。個人で開発する分には無料で使用できます。

ダウンロードが終わったら、「Visual Studio」をインストールします。

### Altseedのダウンロード
[Altseed](https://github.com/altseed/Altseed/releases/download/20160412/Altseed_CS_20160412_WIN.zip)　をダウンロードします。
ダウンロードしたファイルを解凍します。

開発には以下のファイルを使用します。

* Runtime/Altseed.dll
* Runtime/Altseed.XML
* Runtime/Altseed\_core.dll

### プロジェクトの作成

1. ```ファイル->新規作成->プロジェクト```を選択します。
2. 左のカラムから```インストール済み->テンプレート->Visual C#```を選択した後、中央のカラムから```コンソールアプリケーション```を選択します。
3. 下のテキストボックスの```名前```にプロジェクト名を入力して```OK```を選択します。ここからは```Altseed_Test```と入力したと仮定して説明をします。

これでプロジェクトの作成が完了しました。

### プロジェクトの設定
1. ソリューションエクスプローラーの```Altseed_Test```プロジェクトを右クリックします。```追加->既存の項目```を選択して、```Altseed.dll```と```Altseed_core.dll``` を追加します。
2. 追加された```Altseed.dll```を右クリックして```プロパティ```を開き、```出力ディレクトリーにコピー```の項目を「新しい場合はコピーする」に変更します。同様に```Altseed_core.dll```に対しても```出力ディレクトリーにコピー```の項目を変更します。
3. ```Altseed_Test->参照設定```を右クリックして```参照の追加```を選択します。ここで、```参照```ボタンをクリックし、1で追加された```Altseed.dll```を選択して```OK```を押します。

以上でAltseedが使えるようになります。

また、```Altseed.XML```を```Altseed.dll```と同じディレクトリに配置すると関数等の説明がVisualC#上で表示されるようになります。

### ビルド、実行

Program.cs を以下のソースコードに書き換えます。

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Altseed_Test
{
	class Program
	{
		[STAThread]
		static void Main(string[] args)
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("Empty", 640, 480, new asd.EngineOption());

			// Altseedが進行可能かチェックする。
			while (asd.Engine.DoEvents())
			{
				// Altseedを更新する。
				asd.Engine.Update();
			}

			// Altseedを終了する。
			asd.Engine.Terminate();
		}
	}
}

```

そして、メニューバーからデバッグ->デバッグ開始を選択します。 黒いウインドウが表示されれば成功です。

### 今後開発する上での設定

「構築されているプロジェクトのプロセッサ アーキテクチャ “MSIL” と、参照...」のような警告が表示される場合は```ビルド->構成マネージャー```を選択し```プラットフォーム```をx86に変更されると表示されなくなります。
