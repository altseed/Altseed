﻿## ジョイスティックコンテナ (JoystickContainer)

### 概要

PCに接続されている全てのジョイスティックに関する情報を返します。

[エンジン](../Basic/Engine.md)のJoystickContainerプロパティから現在のフレームでの全てのジョイスティックの状態をコンテナとして持つ当クラスのインスタンスを取得できます。

個々のジョイスティックに関する入力状態については、GetJoystickAtメソッドの返り値であるJoystickクラスのインスタンスより取得可能です。また、必ず、個々のジョイスティックの入力状態を持つインスタンスを取得する前に、IsPresentAtでそれが有効かどうか確かめてください。

Joystickクラスに関しては[ジョイスティック](../Joystick.md)の項目をご覧ください。

### 主なメソッド


| 名称 | 説明 |
|---|---|
| IsPresentAt | 指定した番号のジョイスティックが有効かどうか |
| GetJoystickAt | 指定した番号のジョイスティックの入力状態を持ったインスタンス(指定したジョイスティックが無効の場合はヌルポインタが返ってきます) |

### 主なプロパティ

なし

### 主なイベント

なし

### 使用方法

Joystickクラスのサンプルをご覧ください。