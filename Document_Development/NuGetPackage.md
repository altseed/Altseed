# NuGetパッケージの生成手順

## 概要

`Script/makeNuGetPackage.py` を実行するとNuGetパッケージを作成します。バージョンを入力するよう求められるので、生成したパッケージに付与したいバージョン番号を入力してください。スクリプトによってC#版がコンパイルされ、NuGetパッケージに同梱されます。

`NuGetPackage`ディレクトリに生成された`Altseed.<version>.nupkg`をNuGet Galleryに公開してください。

## 詳細

バージョンは`1.0.0`のようなドット区切りの文字列である必要があります。また、`1.0.0-build1`のようにハイフンの後に適当な文字列を書くと、NuGet パッケージマネージャーなどからプレリリース版として扱われます。

`Altseed.nuspec`はパッケージのメタデータ(NuGet Galleryで表示されるもの)が含まれています。メタデータを編集したい場合は"NuGet Package Explorer"を使用してください。

`Altseed.targets`は`Altseed_core.dll`がプロジェクトにおいて「新しい場合はコピーする」設定に自動的に設定されるようにするためのファイルです。基本的に編集しないでください。

スクリプトを実行すると`version.txt`ファイルに生成したバージョン番号が出力されます。これは最新のバージョン番号をメンバー全員が確認し、誤って古い番号で生成しないようにするための機能です。

## トラブルシューティング

nuspecファイル(XML形式です)内に`serviceable`タグが含まれているとスクリプトがうまく実行できないようです。NuGet Package Explorerのバグ？ 含まれていた場合は該当行を削除してください。