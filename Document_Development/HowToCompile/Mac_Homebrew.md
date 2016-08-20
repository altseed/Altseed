# Homebrewを使用した場合のコンパイル手順

oggとvorbisをインストールする。x11は必要なし。

```
brew install libogg --universal
brew install libvorbis --universal
```

.dylibにリンクされるのを防ぐために、/usr/local/lib/にあるoggとvorbis関連の.dylibをすべてリネームする。

コンパイラから参照できるようにパスを通す。

```
export LIBRARY_PATH=/usr/local/lib/
```

