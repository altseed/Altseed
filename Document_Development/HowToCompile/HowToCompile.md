# コンパイル手順

コンパイルするために以下の手順を踏んでください。

1. 必要環境を満たしているか確認してください。
2. Altseedリポジトリに同梱されたスクリプトを実行するなどしてライブラリを導入してください。
3. 必要な自動生成コードを生成してください。

ちなみに、Altseedのプログラムは**Core側**と**Engine側**の2層構造になっており、Core側はC++で書かれていますが、Engine側はC++,C#など対応言語ごとに同じ機能が書かれています。

## 1. 必要環境

### 共通

* Python(3.0以上)
* cmake(3.6以上)
* swig(3.0.6以上)

Windowsでは実行ファイルへパスを通す必要があります。

Windowsではパッケージ管理ツール Chocolatey を使用することで容易に導入できます。
Chocolateyは管理者として実行する必要があります。

また、Altseedと外部ライブラリのソースコードを取得するために、Gitクライアントツール(GitHub for Windows, TortoiseGit等)が必要です。

### Windows

* Visual Studio 2015

* DirectX Software Development Kit

[ダウンロード](http://www.microsoft.com/en-us/download/details.aspx?id=6812)

インストール後、DirectX SDKへパスを通します。

[パスを通す方法](DirectXSDK.md)

### Linux

* gcc(4.7以上)

* X11 / OpenGL 関連
  - Ubuntu/Debian
```
libx11-dev libgl1-mesa-dev libxrandr-dev libxi-dev x11proto-xf86vidmode-dev xorg-dev libglu1-mesa-dev libpulse-dev libvorbis-dev libogg-dev
```

#### Linux用ツール類

##### 共通

OpenGL サポートのために glew が必要です。

[その他のライブラリの導入手順](#その他のライブラリの導入手順) の項をご覧ください。

cmake が ``` find_package(JNI) ``` を解決するために、有効な Java(OpenJDK) 環境と、環境変数 JAVA_HOME が必要です。

JAVA_HOME を簡易的に設定するには、

```bash
$ export JAVA_HOME=$(readlink -f /usr/bin/javac | sed "s:/bin/javac::")
```

を実行するか、 .bashrc などに追記してください。

GNU Make を使う場合、 JAVA_HOME が定義されていない場合は自動的に上記のコマンドを実行します。

##### C# 関連

Linux で .NET を動かすために Mono 3.0 以上が必要です。

できるだけ新しいバージョンの Mono をインストールしてください。

また、ドキュメントの生成に ``` doxygen ``` が必要です。

##### Java 関連

Java版 Altseed をコンパイルするために Ant が必要です。

[Java版への対応](#java版への対応) の項をご覧ください。

### Mac

* Xcode コマンドラインツール

* 各種ライブラリ
MacPorts等を用いてインストールします。

```
libogg libvorbis libvorbisfiles libSM libICE libX11 libXext libXrandr libXi libXxf86vm
```

[Homebrewを使用する場合](Mac_Homebrew.md)

## 2. 外部ライブラリの導入

Core側のコンパイルやEngineのテストなど、様々な部分で使われるライブラリの導入手順です。

### Git Submodule

初めに、外部ライブラリのソースコードを取得するために、Altseedの Git サブモジュールを初期化し、再帰的にアップデートしてください。

[使用しているサブモジュール一覧](../submodules.md)

#### Windowsの場合

[参考:Git のサブモジュールってどんなもの？](http://qiita.com/go_astrayer/items/8667140aef8875742a36)

TortoiseGitを使用している場合、 <kbd>右クリック</kbd> > <kbd>TortoiseGit</kbd> > <kbd>サブモジュールのアップデート</kbd> と選択し、「サブモジュールを初期化する」と「再帰的」にチェックを入れて実行します。

#### Linux/Macの場合、またはWindowsで git コマンドを使える場合

Altseedのディレクトリで、コマンド

``` git submodule update --init --recursive ```

を実行します。

GitHub for Windows を使用している場合、レポジトリ一覧からAltseedを右クリックし、 <kbd>open a shell here</kbd> をクリックするとPowerShellが起動するので、上記のコマンドを実行してください。


### スクリプトの実行

サブモジュールをアップデートした後、実行するスクリプトについて説明します。スクリプトは`Script`ディレクトリに置いてあります。

これらのスクリプトを実行することで、ライブラリがコンパイルされます。

GNU Make を使う場合は自動的に実行されるので、 [4. コンパイル](#4-コンパイル) の項まで飛ばしてください。

|ライブラリ|スクリプト|説明|
|---|---|---|
|AltseedPlatform|`build_ap.py`||
|AltseedRHI|`build_ar.py`||
|glfw for Altseed|`build_glfw.py`||
|zlib/libpng|`build_libpng.py`|Windowsでは実行する必要はありません。|
|glew|`build_glew.py`|Windows以外ではスクリプトによる導入はできませんので、次節を参照してください。|
|Box2D|`build_Box2D.py`||
|bullet|`build_bullet.py`||
|GoogleTest|`build_gtest.py`||
|Effekseer|`build_effekseer.py`||
|OpenSoundMixer|`build_OpenSoundMixer.py`|スクリプト実行中にエラーが起きても問題ない場合があります。Altseedチームによるライブラリ。|
|FreeType|`build_freetype.py`||
|GD Library|`build_libgd.py`||
|2Dカリング|`build_culling2d.py`|Altseedチームによるライブラリ|
|3Dカリング|`build_culling3d.py`|Altseedチームによるライブラリ|

#### スクリプトに対するVisual Studioバージョンの指定

Windows上で上記のスクリプトを実行するためには、特定のバージョンのVisual Studioがインストールされていることが必要です。
必要なバージョンのVisual Studioがインストールされていないことが原因と思われるエラーが発生した場合、スクリプトの実行時にオプションでVisual Studioのバージョンなどを指定する必要があります。

pythonコマンドでスクリプトを実行するとき、オプションは以下のように指定します。

```
python <スクリプト> <CMakeの構成名(x86)> <CMakeの構成名(x64)> <msbuildへのパス>
```

たとえば、Visual Studio 2017をデフォルトの場所にインストールしているなら次のように指定します。

```
python build_ap.py "Visual Studio 15" "Visual Studio 15 Win64" "C:\Program Files(x86)\Microsoft Visual Studio\VS2017\Community\MSBuild\15.0\Bin\msbuild"
```

Visual StudioバージョンとCMakeの構成名の対応は次の通りです。
msbuildへのパスはインストール場所にもよるので各自で調べてください。

|Visual Studio|構成名(x86)|構成名(x64)|
|---|---|---|
|VS2012|Visual Studio 11|Visual Studio 11 Win64|
|VS2013|Visual Studio 12|Visual Studio 12 Win64|
|VS2015|Visual Studio 14|Visual Studio 14 Win64|
|VS2017|Visual Studio 15|Visual Studio 15 Win64|

### その他のライブラリの導入手順

#### glew
* Windows：前節で示したスクリプトを実行すればOKです。
* Linux：パッケージ管理ツールで導入してください。
    * たとえばUbuntuの場合「libglew-dev」パッケージです。
* Mac：使用しませんので、導入の必要はありません。


## 3. コードの自動生成

Altseedのコードをコンパイルするには、いくつかのスクリプトによってコードを生成する必要があります。

GNU Make を使う場合は自動的に実行されるので、 [4. コンパイル](#4-コンパイル) まで飛ばしてください。

C++版Altseedをコンパイルするための準備の手順は以下のとおりです。

1. `Dev/generateCoreToEngineHeader.py`
2. `Dev/generateEngineHeader.py`
3. `Dev/generate_swig.py`

C#版Altseedをコンパイルするための準備の手順は以下のとおりです。

1. `Dev/generateCoreToEngineHeader.py`
2. `Dev/generateEngineHeader.py`
3. `Dev/generate_swig.py`
4. `Script/export_doxygen_core.py`
5. `Script/generateSwigWrapper.py`

### スクリプトの説明

|スクリプト|用途|
|---|---|
|`Dev/generateCoreToEngineHeader.py`|Core側コードを元にC++ Engine側コードを生成します。|
|`Dev/generateEngineHeader.py`|C++ Engineのユーザーがincludeするヘッダファイルを生成します。|
|`Dev/generate_swig.py`|Core側とC# Engine側を繋ぐC#コードを生成します。|
|`Script/export_doxygen_core.py`|Core側のコードのdoxygenドキュメントを生成します。 `generateSwigWrapper.py`を実行する前に実行する必要があります。|
|`Script/generateSwigWrapper.py`|swigで生成されたラッパーをC# Engine側のためにラップするコードを生成します。|

## 4. コンパイル

### Windows

以下のソリューションを実行します。

```
Dev/unitTest_Engine_cpp.sln
Dev/unitTest_Engine_cs.sln
```

このふたつのソリューションにはテスト プロジェクトが含まれています。

Windows10上でテストのDebugビルド版を実行するには、スタート > 設定 > オプション機能を追加する から「Graphics Tools」をインストールする必要があります。

### 他

#### GNU Make を使う場合

C++版のみコンパイルする場合は ```make cpp```

C#版をコンパイルする場合は ```make csharp```

Java版をコンパイルする場合は ```make java```

を実行します。

##### 高度な使い方

```make csharp-clean``` で C# 版の Altseed をコンパイル前の状態に戻すことができます。C++ 版と Java 版も同様です。

```make all``` で全ての言語版の Altseed をコンパイルでき、 ```make clean``` で全ての言語版の Altseed を clean できます。

```make [ライブラリ名]``` で指定したライブラリを明示的にコンパイルでき、 ```make [ライブラリ名]-clean``` で指定したライブラリをコンパイル前の状態に戻すことができます。

```make plugins-all``` で外部ライブラリを全てコンパイル、```make clean-all-plugins``` で全てcleanすることができます。

```make clean-absolute``` で clone した直後の状態に戻すことができます。コンパイルされたバイナリなどは、外部ライブラリも含めて全て削除されます。

#### GNU Make を使わない場合

```Script/compile.py``` を実行します。


## Java版への対応

* antをインストールします。
* ANT_HOME環境変数を作り、antのインストールパスを指定します。？
* javacにパスを通します。？

```Script/generateTranslatedCode.py``` を実行します。
警告が出ても変換はされている？

Dev/asd_java/ で ```ant``` を実行します。

## サンプル

```Script/generate_sample.py``` を実行します。

C++のサンプルを生成する場合、```Script/generate_sample.py cpp```

C#のサンプルを生成する場合、```Script/generate_sample.py cs```

Javaのサンプルを生成する場合、```Script/generate_sample.py java```

と入力します。

## リリース

DoxygenとPandocをインストールします。
```Script/release.py``` を実行します。現在、WindowsとMacのみ対応しております。
