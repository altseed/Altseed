
## 空のウインドウを表示する

AmusementCreatorsEngine (ACE) で空のウインドウを表示します。

### C++

```cpp
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// aceを初期化する。
	ace::Engine::Initialize(ace::ToAString("Empty").c_str(), 640, 480, ace::EngineOption());

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		// aceを更新する。
		ace::Engine::Update();
	}

	// aceを終了する。
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
		// aceを初期化する。
		ace.Engine.Initialize("Empty", 640, 480, new ace.EngineOption());

		// aceが進行可能かチェックする。
		while (ace.Engine.DoEvents())
		{
			// aceを更新する。
			ace.Engine.Update();
		}

		// aceを終了する。
		ace.Engine.Terminate();
	}
}
```
