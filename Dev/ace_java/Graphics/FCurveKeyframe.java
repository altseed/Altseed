
package ace;

public class FCurveKeyframe
{

	public Vector2DF KeyValue;

	public Vector2DF LeftHandle;

	public Vector2DF RightHandle;

	public InterpolationType InterpolationType;

	public FCurveKeyframe()
	{
		LeftHandle = new Vector2DF();
		RightHandle = new Vector2DF();
		KeyValue = new Vector2DF();
		InterpolationType = ace.InterpolationType.Linear;
	}

	public FCurveKeyframe(float kvx, float kvy, float lhx, float lhy, float rhx, float rhy, int type)
	{
		LeftHandle = new Vector2DF();
		RightHandle = new Vector2DF();
		KeyValue = new Vector2DF();
		InterpolationType = ace.InterpolationType.Linear;

		KeyValue.X = kvx;
		KeyValue.Y = kvy;

		LeftHandle.X = lhx;
		LeftHandle.Y = lhy;

		RightHandle.X = rhx;
		RightHandle.Y = rhy;

		InterpolationType = InterpolationType.valueOf(type);
	}
}