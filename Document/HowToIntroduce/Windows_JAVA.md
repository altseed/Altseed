
## Java版の導入(Windows)

Java版の導入方法について説明しています。

Java版はWindows(64bit)でないと動作しません。ご注意ください。

### Altseedのダウンロード

[公式サイト](http://altseed.github.io/download.html)からAltseedのJava版の最新版をダウンロードします。
ダウンロードしたファイルを解凍します。

開発には以下のファイルを使用します。

* Runtime/Altseed.jar
* Runtime/Altseed\_core.dll

### JDKのインストール

[JDK](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)をインストールします。

環境変数を以下のように設定します。

| 環境変数名 | 値 |
|:-----------|------------:|
| JAVA_HOME | C:\Program Files\Java\jdk1.8.0_xx |
| PATH | C:\Program Files\Java\jdk1.8.0_xx\bin |

パスは標準インストールの場合です。
xxの部分はインストールしたバージョンに合わせて変更します。

### コマンドラインで実行する場合

```Altseed.jar``` が存在するフォルダに```Main.java```　という空のテキストファイルを用意します。

```Main.java``` を以下のソースコードに書き換えます。

```
class Main
{
	static
	{
		System.loadLibrary("Altseed_core");
	}

	public static void main(String args[])
	{
		asd.Engine.Initialize("Empty", 640, 480, new asd.EngineOption());

		while(asd.Engine.DoEvents())
		{
			asd.Engine.Update();
		}
		
		asd.Engine.Terminate();
	}
}

```

コマンドラインから以下のように打ち込みます。

```javac Main.java -classpath ./Altseed.jar```

```java -classpath ./;Altseed.jar Main```

黒いウインドウが表示されれば成功です。

### Eclipseを使用する場合

[Eclipse](https://eclipse.org/)をインストールします。

新規にプロジェクトを作成します。

```ファイル->新規->プロジェクト``` と選択します。
生成できるプロジェクトが表示されるので```Javaプロジェクト```を選択します。

Mainクラスを追加します。

```src``` を右クリックして、```新規->クラス```　と選択します。名前にはMainと入力して完了を押します。

#### Altseed.jarの追加

「パッケージ・エクスプローラー」でプロジェクトを選択します。
プロジェクトを右クリックして、```ビルド・パス->外部アーカイブの追加``` を選択します。
Jarファイルを選択できるので、```Altseed.jar```　を選択します。

#### Altseed_core.dllの追加

Altseed_core.dllを追加します。

「パッケージ・エクスプローラー」でプロジェクトを選択します。
プロジェクトを右クリックして、```ビルド・パス->ビルドパスの構成``` を選択します。

そうすると```ソース、プロジェクト、ライブラリー、順序およびエクスポート```　というタブが表示されるプロパティのウインドウが表示されます。

```ライブラリー``` のタブをクリックします。

```ライブラリーの追加``` のボタンをクリックします。

追加できるライブラリーの種類一覧が表示されるので ```ユーザーライブラリー``` を選択します。

次に選択できるユーザーライブラリーの一覧が表示されますが、何もないので```ユーザーライブラリー```　のボタンをクリックし、
表示されたウインドウの```新規```　ボタンを押します。

ライブラリ名を入力できるので、適当な名前を入力します。

追加されたライブラリを選択して```完了```　のボタンを押します。

追加されたライブラリをクリックすると、```ネイティブ・ライブラリー・ロケーション```　という項目が表示されます。

ダブルクリックすると、ディレクトリを指定できるので```Altseed_core.dll```　が存在するディレクトリを選択します。

#### ソースコードの記述

```Main.java``` を以下のソースコードに書き換えます。

```
class Main
{

	static
	{
		System.loadLibrary("Altseed_core");
	}

	public static void main(String args[])
	{
		asd.Engine.Initialize("Empty", 640, 480, new asd.EngineOption());

		while(asd.Engine.DoEvents())
		{
			asd.Engine.Update();
		}
		
		asd.Engine.Terminate();
	}
}

```

実行ボタンを押すと黒いウインドウが表示されれば成功です。
