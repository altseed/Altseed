# 必要環境

## 共通

* Python(3.0以上)
* cmake(2.8.11以上)
* swig(3.0.5以上)

Windowsでは実行ファイルへパスを通す必要があります。

Windowsではパッケージ管理ツール Chocolatey を使用することで容易に導入できます。
Chocolateyは管理者として実行する必要があります。

Ubuntuでは、```Script/install_swig_ubuntu.sh```　を実行することでswigを容易にインストールすることができます。

## Windows

* Visual Studio 2013

* DirectX Software Development Kit

[ダウンロード](http://www.microsoft.com/en-us/download/details.aspx?id=6812)

インストール後、DirectX SDKへパスを通します。

[パスを通す方法](DirectXSDK.md)

## Linux

* gcc(4.7以上)

* 各種ライブラリ
  - Ubuntu
```
libx11-dev libgl1-mesa-dev libxrandr-dev libxi-dev x11proto-xf86vidmode-dev xorg-dev libglu1-mesa-dev libpulse-dev libvorbis-dev libogg-dev
```

## Mac

* Xcode コマンドラインツール

* 各種ライブラリ
MacPorts等を用いてインストールします。
```
libogg libvorbis libvorbisfiles libSM libICE libX11 libXext libXrandr libXi libXxf86vm
```

# 外部ライブラリの導入

## glfw for ACE

```Script/download_glfw.py``` を実行します。

## zip,libpng

### Windows

自動化できないため最初から追加済み

### 他

```Script/download_libpng.py``` を実行します。


## glew

### Windows

自動化できないため最初から追加済み

### Linux

パッケージ管理ツールで入手します。
  - Ubuntu
```
libglew-dev
```

### Mac

使用しません。

## Box2D

```Script/download_Box2D.py``` を実行します。

## bullet

```Script/download_bullet.py``` を実行します。

## GoogleTest

```Script/download_gtest.py``` を実行します。

## Effekseer

```Script/download_effekseer.py``` を実行します。

## OpenSoundMixer

```Script/download_OpenSoundMixer.py``` を実行します。

## Freetype

```Script/download_freetype.py``` を実行します。

## GD library

```Script/download_gd.py``` を実行します。

## カリング

以下のスクリプトを実行します。

```
Script/download_culling2d.py
Script/download_culling3d.py
```

# ヘッダの生成

以下のスクリプトを実行します。

```
Dev/generateCoreToEngineHeader.py
Dev/generateEngineHeader.py
Dev/generate_swig.py
```

# コンパイル

## Windows

以下のソリューションを実行します。

```
Dev/unitTest_Engine_cpp.sln
Dev/unitTest_Engine_cs.sln
```

## 他

```Script/compile.py``` を実行します。

# リリース

DoxygenとPandocをインストールします。
```Script/release.py``` を実行します。現在、Windowsのみ対応しております。
