# DirectXSDKへのパスの通し方

プロパティ マネージャーマネージャーを起動します。

![プロパティマネージャー](img/PropertyManager.png)

Microsoft.Cpp.Win32.userのプロパティを起動します。
![Microsoft.Cpp.Win32.user](img/user.png)

VC++ ディレクトリを開き、インクルードディレクトリにDirectX SDKのインクルードファイルへのパスを追加します。
IncludeとLibを取り違えるケースが見られるので、パスをよく確認するようお願いします。

デフォルトのインストール先にインストールした場合「C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include」を追加します。

WindowsのSDKより優先度を高くする必要があるため、右上の矢印でDirectX SDKを可能な限り、上に移動させます。

![DirectXSDK01](img/DirectXSDK01.png)

ライブラリディレクトリにDirectX SDKのインクルードファイルへのパスを追加します。
IncludeとLibを取り違えるケースが見られるので、パスをよく確認するようお願いします。

デフォルトのインストール先にインストールした場合「C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86」を追加します。

WindowsのSDKより優先度を高くする必要があるため、右上の矢印でDirectX SDKを可能な限り、上に移動させます。

![DirectXSDK02](img/DirectXSDK02.png)
