
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

環境変数を設定します。

JAVA_HOME　に　C:\Program Files\Java\jdk1.8.0_xx

PATH　に　C:\Program Files\Java\jdk1.8.0_xx\bin

を設定します。xxの部分はインストールしたバージョンに合わせて変更します。

### コマンドラインで実行する場合

Altseed.jarが存在するフォルダにMain.javaという空のテキストファイルを用意します。

Main.java を以下のソースコードに書き換えます。

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

以下のようにコマンドラインから打ち込みます。

```javac Main.java -classpath ./Altseed.jar```
```java -classpath ./;Altseed.jar Main```

黒いウインドウが表示されれば成功です。

### Eclipseを使用する場合

[Eclipse](https://eclipse.org/)をインストールします。

新規にプロジェクトを作成します。

Mainクラスを追加します。

「パッケージ・エクスプローラー」でプロジェクトを選択し、右クリックして「ビルド・パス」>「外部アーカイブの追加」を選択します。

Altseed.jarを選択します。

#### ネイティブの追加

下記リンクを参考にしてネイティブのDLLを追加します。

http://sgrit.hatenablog.com/entry/2014/04/27/043015

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

黒いウインドウが表示されれば成功です。
