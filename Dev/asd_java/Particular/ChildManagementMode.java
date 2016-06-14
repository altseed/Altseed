package asd.Particular;

public class ChildManagementMode
{
	public static int ToInt(asd.ChildManagementMode v)
	{
		return (int)v.swigValue();
	}

	public static int And(asd.ChildManagementMode v1, asd.ChildManagementMode v2)
	{
		return v1.swigValue() & v2.swigValue();
	}
}
