package ace;

public class Matrix44
{
	public float[] Values;

	public Matrix44()
	{
		Values = new float[16];
	}

	public Matrix44(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
	{
		Values[0] = m00;
		Values[1] = m01;
		Values[2] = m02;
		Values[3] = m03;

		Values[0+4] = m10;
		Values[1+4] = m11;
		Values[2+4] = m12;
		Values[3+4] = m13;

		Values[0+8] = m20;
		Values[1+8] = m21;
		Values[2+8] = m22;
		Values[3+8] = m23;

		Values[0+12] = m30;
		Values[1+12] = m31;
		Values[2+12] = m32;
		Values[3+12] = m33;

	}
}
