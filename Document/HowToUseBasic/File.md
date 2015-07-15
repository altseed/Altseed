
## ファイルの扱い

### ルートディレクトリの指定

多くの画像などの素材ファイルが属しているフォルダがある場合、ルートディレクトリに登録することでそのフォルダへのパスを省略できるようにできます。

* include_basic_sample File_RootDirectory

### パッケージファイルの使い方

Altseed用のパッケージファイルから素材をロードする場合は、ルートディレクトリにパッケージファイルを追加します。

* include_basic_sample File_PackFile

### StaticFileの使い方

Altseedの機能でテキスト/バイナリファイルを読み込みます。この機能を使うことでパッケージファイル内のファイルも読み込むことができます。

* include_basic_sample File_StaticFile

### StreamFileの使い方

* include_basic_sample File_StreamFile