## テクスチャ2D (Texture2D)

### 概要

2次元の画像を定義します。グラフィックスから生成できます。
基本的にはファイルから生成しますが、空のテクスチャを生成しプログラムから画像を編集することもできます。

Engine.Graphics.CreateEmptyTexture2Dにより空のテクスチャを生成できます。
このテクスチャはLock関数を実行すると画素を編集できます。
Lockを実行し成功すると引数からTextureLockInfomationを取得できます。
ここには画像サイズや1画素あたりのメモリサイズ、画素の先頭ポインタが格納されています。
それらの情報を用いて、画素に情報を書き込みます。
書き込み終わった後はUnlockを実行します。

### 主なメソッド

| 名称 | 説明 |
|---|---|
| Lock | テクスチャの編集を開始します。 |
| Unlock | テクスチャの編集を終了します。 |

### 主なプロパティ

なし

### 主なイベント

なし

### 使用方法

テクスチャオブジェクト2Dのサンプルを参照してください。

これは書き込みを行うサンプルコードの1つです。textureにdataにある情報を書き込みます。

* C++
```
ace::TextureLockInfomation info = ace::TextureLockInfomation();
if (texture->Lock(&info))
{
	
	auto dst = (uint8_t*)info.GetPixels();
	auto src = d;

	for(int i = 0; i < info.Size.X * info.Size.Y * info.Pitch; i++)
	{
		dst[i] = src[i];
	}

	texture->Unlock();
}
```

* C#
```
ace.TextureLockInfomation info = new ace.TextureLockInfomation();
if (texture.Lock(info))
{
	fixed(byte* d = &data[0])
	{
		var dst = (byte*)info.Pixels.ToPointer();
		var src = d;

		for(int i = 0; i < info.Size.X * info.Size.Y * info.Pitch; i++)
		{
			dst[i] = src[i];
		}
	}

	texture.Unlock();
}
```

