
package ace;


public enum InterpolationType
{
	Constant( ace.swig.InterpolationType.Constant.swigValue() ),
	Linear( ace.swig.InterpolationType.Linear.swigValue() ),
	Cubic( ace.swig.InterpolationType.Cubic.swigValue() );

	private final int id;

	private InterpolationType(final int id)
	{
		this.id = id;
	}

	public int getID()
	{
		return id;
	}

	public static InterpolationType valueOf(int id)
	{
		for (InterpolationType e : values() )
		{
			if (e.getID() == id)
			{
				return e;
			}
		}

		throw new IllegalArgumentException("Not found : " + id);
	}
}
