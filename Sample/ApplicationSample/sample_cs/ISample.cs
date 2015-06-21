
interface ISample
{
	void Run();
}

class Recorder
{
	static string currentName = string.Empty;

	static int currentTime = 0;

	public static void TakeScreenShot(string name, int time)
	{
		if(name == currentName)
		{
			if(time == currentTime)
			{
				var path = "../../../Sample_SS/" + name + ".jpg";
				asd.Engine.TakeScreenshot(path);
			}

			currentTime++;
		}
		else
		{
			currentTime = 0;
			currentName = name;
		}
	}
}