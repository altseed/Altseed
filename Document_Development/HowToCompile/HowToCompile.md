# コンパイル手順

コンパイルするために以下の手順を踏んでください。

1. 必要環境を満たしているか確認してください。
2. Altseedリポジトリに同梱されたスクリプトを実行するなどしてライブラリを導入してください。
3. 必要な自動生成コードを生成してください。

ちなみに、Altseedのプログラムは**Core側**と**Engine側**の2層構造になっており、Core側はC++で書かれていますが、Engine側はC++,C#など対応言語ごとに同じ機能が書かれています。

## 1. 必要環境

### 共通

* Python(3.0以上)
* cmake(2.8.11以上)
* swig(3.0.6以上)

Windowsでは実行ファイルへパスを通す必要があります。

Windowsではパッケージ管理ツール Chocolatey を使用することで容易に導入できます。
Chocolateyは管理者として実行する必要があります。

Ubuntuでは、```Script/install_swig_ubuntu.sh```　を実行することでswigを容易にインストールすることができます。

### Windows

* Visual Studio 2015

* DirectX Software Development Kit

[ダウンロード](http://www.microsoft.com/en-us/download/details.aspx?id=6812)

インストール後、DirectX SDKへパスを通します。

[パスを通す方法](DirectXSDK.md)

### Linux

* gcc(4.7以上)

* 各種ライブラリ
  - Ubuntu
```
libx11-dev libgl1-mesa-dev libxrandr-dev libxi-dev x11proto-xf86vidmode-dev xorg-dev libglu1-mesa-dev libpulse-dev libvorbis-dev libogg-dev
```

### Mac

* Xcode コマンドラインツール

* 各種ライブラリ
MacPorts等を用いてインストールします。
```
libogg libvorbis libvorbisfiles libSM libICE libX11 libXext libXrandr libXi libXxf86vm
```

## 2. 外部ライブラリの導入

Core側のコンパイルやEngineのテストなど、様々な部分で使われるライブラリの導入手順です。

### スクリプトで導入できるライブラリ

スクリプトを実行することで導入できるライブラリを説明します。スクリプトは`Script`ディレクトリに置いてあります。

|ライブラリ|スクリプト|説明|
|---|---|---|
|glfw for Altseed|`download_glfw.py`||
|zip/libpng|`download_libpng.py`|Windowsでは実行する必要はありません。|
|glew|`download_glew.py`|Windows以外ではスクリプトによる導入はできませんので、次節を参照してください。|
|Box2D|`download_Box2D.py`||
|bullet|`download_bullet.py`||
|GoogleTest|`download_gtest.py`||
|Effekseer|`download_effekseer.py`||
|OpenSoundMixer|`download_OpenSoundMixer.py`|スクリプト実行中にエラーが起きても問題ない場合があります。Altseedチームによるライブラリ。|
|FreeType|`download_freetype.py`||
|GD Library|`download_gd.py`||
|2Dカリング|`download_culling2d.py`|Altseedチームによるライブラリ|
|3Dカリング|`download_culling3d.py`|Altseedチームによるライブラリ|

### その他のライブラリの導入手順

#### glew
* Windows：前節で示したスクリプトを実行すればOKです。
* Linux：パッケージ管理ツールで導入してください。
    * たとえばUbuntuの場合「libglew-dev」パッケージです。
* Mac：使用しませんので、導入の必要はありません。


## 3. コードの自動生成

Altseedのコードをコンパイルするには、いくつかのスクリプトによってコードを生成する必要があります。

C++版Altseedをコンパイルするための準備の手順は以下のとおりです。

1. `Dev/generateCoreToEngineHeader.py`
2. `Dev/generateEngineHeader.py`
3. `Dev/generate_swig.py`

C#版Altseedをコンパイルするための準備の手順は以下のとおりです。

1. `Dev/generateCoreToEngineHeader.py`
2. `Dev/generateEngineHeader.py`
3. `Dev/generate_swig.py`
4. `Script/export_cpp_xml.py`
5. `Script/generateSwigWrapper.py`

### スクリプトの説明

|スクリプト|用途|
|---|---|
|`Dev/generateCoreToEngineHeader.py`|Core側コードを元にC++ Engine側コードを生成します。|
|`Dev/generateEngineHeader.py`|C++ Engineのユーザーがincludeするヘッダファイルを生成します。|
|`Dev/generate_swig.py`|Core側とC# Engine側を繋ぐC#コードを生成します。|
|`Script/export_cpp_xml.py`|Core側のコードのdoxygenドキュメントを生成します。 `generateSwigWrapper.py`を実行する前に実行する必要があります。|
|`Script/generateSwigWrapper.py`|swigで生成されたラッパーをC# Engine側のためにラップするコードを生成します。|

## 4. コンパイル

### Windows

以下のソリューションを実行します。

```
Dev/unitTest_Engine_cpp.sln
Dev/unitTest_Engine_cs.sln
```

### 他

```Script/compile.py``` を実行します。

## Java版への対応

* antをインストールします。
* ANT_HOME環境変数を作り、antのインストールパスを指定します。？
* javacにパスを通します。？

```Script/generateTranslatedCode.py``` を実行します。
警告が出ても変換はされている？

Dev/asd_java/ で ```ant``` を実行します。

## リリース

DoxygenとPandocをインストールします。
```Script/release.py``` を実行します。現在、WindowsとMacのみ対応しております。
