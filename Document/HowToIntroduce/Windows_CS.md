
## C#版の導入(Windows)

C#版の導入方法について説明しています。

### VisualStudio2013の導入

[VisualStudioのWebページ](https://www.visualstudio.com/ja-jp/downloads/download-visual-studio-vs#DownloadFamilies_2)から
「Visual Studio Community 2013 Update 4」をダウンロードします。ただし、会社内でACEを使用する場合は「Visual Studio Express 2013 with Update 4 for Windows Desktop」をダウンロードします。
これらはプログラムのソースコードから実行可能なアプリケーションを生成するツールです。個人で開発する分には無料で使用できます。

ダウンロードが終わったら、「Visual Studio」をインストールします。

### ACEのダウンロード
[Download](https://drive.google.com/folderview?id=0B1gZCvEfcQAiMjhaU1VZOVRTUWM&usp=sharing)　からACE_CS～.7zをダウンロードします。
7-Zipファイルを解凍できる解凍ソフトを用いて解凍します。

開発には以下のファイルを使用します。

* Runtime/ace\_cs.dll
* Runtime/ace\_cs.XML
* Runtime/ace\_core.dll

### プロジェクトの作成

1. ```ファイル->新規作成->プロジェクト```を選択します。
2. 左のカラムから```インストール済み->テンプレート->Visual C#```を選択した後、中央のカラムから```コンソールアプリケーション```を選択します。
3. 下のテキストボックスの```名前```にプロジェクト名を入力して```OK```を選択します。ここからは```ACE_Test```と入力したと仮定して説明をします。
4. ソリューションエクスプローラから```ACE_Test```プロジェクトを右クリックします。```追加>既存の項目```を選択して、ace\_cs.dllとace\_core.dll (あるいはace\_core.Debug.dll)を追加します。
5. 追加された```ace\_cs.dll```を右クリックして```プロパティ```を開き、出力ディレクトリの項目を「新しい場合はコピーする」に変更します。同様に```ace\_core.dll```に対しても出力ディレクトリの項目を変更します。
6. ```ACE_Test>参照設定```を右クリックして```参照の追加```を選択します。ここで、4にて追加されたace_cs.dllにチェックを入れて```OK```を押します。

以上でACEが使えるようになります。

また、ace_cs.XMLをace_cs.dllと同じディレクトリに配置すると関数等の説明がVisualC#上で表示されるようになります。

※「構築されているプロジェクトのプロセッサ アーキテクチャ “MSIL” と、参照...」のような警告が出る方は「ビルド>構成マネージャー」から「プラットフォーム」をx86とすれば治ります。


### ファイルの追加

### ビルド、実行
