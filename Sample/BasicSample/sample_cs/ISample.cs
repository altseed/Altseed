
interface ISample
{
	void Run();
	string Description { get; }
	string Title { get; }
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

	public static void CaptureScreen(string name, int time, int frame, float freq_rate, float scale)
	{
		if (name == currentName)
		{
			if (time == currentTime)
			{
				var path = "../../../Sample_SS/" + name + ".gif";
				asd.Engine.CaptureScreenAsGifAnimation(path, frame, freq_rate, scale);
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