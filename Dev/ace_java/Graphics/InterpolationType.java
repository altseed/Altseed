
package ace;


public class InterpolationType
{
	public final static InterpolationType Constant = new InterpolationType( ace.swig.InterpolationType.Constant.swigValue() );
	public final static InterpolationType Linear = new InterpolationType( ace.swig.InterpolationType.Linear.swigValue() );
	public final static InterpolationType Cubic = new InterpolationType( ace.swig.InterpolationType.Cubic.swigValue() );

	private final int swigValue;

	public final int swigValue() {
		return swigValue;
	}

	InterpolationType(int value)
	{
		swigValue = value;
	}
}
