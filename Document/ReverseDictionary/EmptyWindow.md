
## 空のウインドウを表示する

AC-Engineで空のウインドウを表示します。

### C++

```cpp
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("Empty").c_str(), 640, 480, ace::EngineOption());

	// AC-Engineが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	// AC-Engineを終了する。
	ace::Engine::Terminate();

	return 0;
}
```

### C#　

```cs
class Program
{
	[STAThread]
	static void Main(string[] args)
	{
		// AC-Engineを初期化する。
		ace.Engine.Initialize("Empty", 640, 480, new ace.EngineOption());

		// AC-Engineが進行可能かチェックする。
		while (ace.Engine.DoEvents())
		{
			// AC-Engineを更新する。
			ace.Engine.Update();
		}

		// AC-Engineを終了する。
		ace.Engine.Terminate();
	}
}
```
