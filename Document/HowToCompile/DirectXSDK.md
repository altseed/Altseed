# DirectXSDKへのパスの通し方

プロパティ マネージャーマネージャーを起動します。

![プロパティマネージャー](img/PropertyManager.png)

Microsoft.Cpp.Win32.userのプロパティを起動します。
![Microsoft.Cpp.Win32.user](img/user.png)

VC++ ディレクトリを開き、インクルードディレクトリにDirectX SDKのインクルードファイルへのパスを追加します。

デフォルトのインストール先なら「C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include」です。

WindowsのSDKより優先度を高くする必要があるため、右上の矢印でDirectX SDKを可能な限り、上に移動させます。

![DirectXSDK01](img/DirectXSDK01.png)

ライブラリディレクトリにDirectX SDKのインクルードファイルへのパスを追加します。

x86のデフォルトのインストール先なら「C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86」です。

WindowsのSDKより優先度を高くする必要があるため、右上の矢印でDirectX SDKを可能な限り、上に移動させます。

![DirectXSDK02](img/DirectXSDK02.png)
