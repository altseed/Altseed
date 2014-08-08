ACE Tutorial (C#)
=================

ACEの導入
------------------

### 用意するもの

* Visual Studio 2013
* ace\_cs.dll
* ace\_core.dll

[https://drive.google.com/folderview?id=0B1gZCvEfcQAiMjhaU1VZOVRTUWM&usp=sharing](Download)

現在、ここからACE本体をダウンロードできます。

### 導入

1. 「ファイル>新規作成>プロジェクト」を選択
2. 左のカラムから「インストール済み>テンプレート>Visual C#」を選択して、中央のカラムから「コンソールアプリケーション」を選択
3. 下の入力からプロジェクト名を適当に決めて「OK」を選択。（ここでは名前を「ACEtest」としたとして話を進めます。）
4. ソリューションエクスプローラから「ACEtest」プロジェクトを右クリックして「追加>既存の項目」を選択して、ace\_cs.dllとace\_core.dll (あるいはace\_core.Debug.dll)を追加
5. 追加した「ace\_cs.dll」を右クリックして「プロパティ」を開き、出力ディレクトリを「新しい場合はコピーする」を選択。「ace\_core.dll」に対しても同様の操作を行う。
6. 「ACEtest>参照設定」を右クリックして「参照の追加」、ここで4で追加したace_cs.dllにチェックを入れて「OK」

以上でACEが使えるようになります。

また、ace_cs.XMLをace_cs.dllと同じディレクトリに配置すると関数等の説明がVisualC#上で表示されるようになります。

※「構築されているプロジェクトのプロセッサ アーキテクチャ “MSIL” と、参照...」のような警告が出る方は「ビルド>構成マネージャー」から「プラットフォーム」をx86とすれば治ります。

### 実行

Program.csを以下のように書き換えてみてください。

```csharp
// usingディレクティブでace内のクラスを完全修飾名で呼び出さずに済ませる
using ace;
using System;

namespace ACEtest
{
    class Program
    {
        static void Main(string[] args)
        {
            // エンジンのオプション
            var option = new EngineOption { IsFullScreen = false };
            
            // エンジンの初期化（タイトル、画面横サイズ、画面縦サイズ、オプション）
            Engine.Initialize("ACエンジンテスト", 800, 600, option);

            // 実行
            while (Engine.DoEvents())
            {
                Engine.Update();
            }

            // 終了処理
            Engine.Terminate();
        }
    }
}

```

「開始」ボタンを押して実行しましょう。画面ウィンドウが出れば成功です。