

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Vector3DF.h"
#include "ace.Matrix43.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::Indentity()
{
	static const Matrix43 indentity = {{
		{1.0f, 0.0f, 0.0f}, 
		{0.0f, 1.0f, 0.0f}, 
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 0.0f}
	}};
	memcpy( Values, indentity.Values, sizeof(indentity) );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::Scaling( float x, float y, float z )
{
	memset( Values, 0, sizeof(float) * 12 );
	Values[0][0] = x;
	Values[1][1] = y;
	Values[2][2] = z;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationX( float angle )
{
	float c, s;
	::ace::SinCos( angle, s, c );

	Values[0][0] = 1.0f;
	Values[0][1] = 0.0f;
	Values[0][2] = 0.0f;

	Values[1][0] = 0.0f;
	Values[1][1] = c;
	Values[1][2] = s;

	Values[2][0] = 0.0f;
	Values[2][1] = -s;
	Values[2][2] = c;

	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationY( float angle )
{
	float c, s;
	::ace::SinCos( angle, s, c );

	Values[0][0] = c;
	Values[0][1] = 0.0f;
	Values[0][2] = -s;

	Values[1][0] = 0.0f;
	Values[1][1] = 1.0f;
	Values[1][2] = 0.0f;

	Values[2][0] = s;
	Values[2][1] = 0.0f;
	Values[2][2] = c;

	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationZ( float angle )
{
	float c, s;
	::ace::SinCos( angle, s, c );

	Values[0][0] = c;
	Values[0][1] = s;
	Values[0][2] = 0.0f;

	Values[1][0] = -s;
	Values[1][1] = c;
	Values[1][2] = 0.0f;

	Values[2][0] = 0.0f;
	Values[2][1] = 0.0f;
	Values[2][2] = 1;

	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationXYZ( float rx, float ry, float rz )
{
	float cx, sx, cy, sy, cz, sz;
	
	if( rx != 0.0f )	::ace::SinCos( rx, sx, cx );
	else				sx = 0.0f, cx = 1.0f;
	if( ry != 0.0f )	::ace::SinCos( ry, sy, cy );
	else				sy = 0.0f, cy = 1.0f;
	if( rz != 0.0f )	::ace::SinCos( rz, sz, cz );
	else				sz = 0.0f, cz = 1.0f;

	Values[0][0] = cy * cz;
	Values[0][1] = cy * sz;
	Values[0][2] = -sy;

	Values[1][0] = sx * sy * -sz + cx * -sz;
	Values[1][1] = sx * sy *  sz + cx *  cz;
	Values[1][2] = sx * cy;

	Values[2][0] = cx * sy * cz + sx * sz;
	Values[2][1] = cx * sy * sz - sx * cz;
	Values[2][2] = cx * cy;

	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationZXY( float rz, float rx, float ry )
{
	float cx, sx, cy, sy, cz, sz;

	if( rx != 0.0f )	::ace::SinCos( rx, sx, cx );
	else				sx = 0.0f, cx = 1.0f;
	if( ry != 0.0f )	::ace::SinCos( ry, sy, cy );
	else				sy = 0.0f, cy = 1.0f;
	if( rz != 0.0f )	::ace::SinCos( rz, sz, cz );
	else				sz = 0.0f, cz = 1.0f;
	
	Values[0][0] = cz * cy + sz * sx * sy;
	Values[0][1] = sz * cx;
	Values[0][2] = cz * -sy + sz * sx * cy;

	Values[1][0] = -sz * cy + cz * sx * sy;
	Values[1][1] = cz * cx;
	Values[1][2] = -sz * -sy + cz * sx * cy;

	Values[2][0] = cx * sy;
	Values[2][1] = -sx;
	Values[2][2] = cx * cy;
	
	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationAxis( const Vector3DF& axis, float angle )
{
	const float c = cosf( angle );
	const float s = sinf( angle );
	const float cc = 1.0f - c;

	Values[0][0] = cc * (axis.X * axis.X) + c;
	Values[0][1] = cc * (axis.X * axis.Y) + (axis.Z * s);
	Values[0][2] = cc * (axis.Z * axis.X) - (axis.Y * s);

	Values[1][0] = cc * (axis.X * axis.Y) - (axis.Z * s);
	Values[1][1] = cc * (axis.Y * axis.Y) + c;
	Values[1][2] = cc * (axis.Y * axis.Z) + (axis.X * s);

	Values[2][0] = cc * (axis.Z * axis.X) + (axis.Y * s);
	Values[2][1] = cc * (axis.Y * axis.Z) - (axis.X * s);
	Values[2][2] = cc * (axis.Z * axis.Z) + c;

	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationAxis( const Vector3DF& axis, float s, float c )
{
	const float cc = 1.0f - c;

	Values[0][0] = cc * (axis.X * axis.X) + c;
	Values[0][1] = cc * (axis.X * axis.Y) + (axis.Z * s);
	Values[0][2] = cc * (axis.Z * axis.X) - (axis.Y * s);

	Values[1][0] = cc * (axis.X * axis.Y) - (axis.Z * s);
	Values[1][1] = cc * (axis.Y * axis.Y) + c;
	Values[1][2] = cc * (axis.Y * axis.Z) + (axis.X * s);

	Values[2][0] = cc * (axis.Z * axis.X) + (axis.Y * s);
	Values[2][1] = cc * (axis.Y * axis.Z) - (axis.X * s);
	Values[2][2] = cc * (axis.Z * axis.Z) + c;

	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::Translation( float x, float y, float z )
{
	Indentity();
	Values[3][0] = x;
	Values[3][1] = y;
	Values[3][2] = z;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::GetSRT( Vector3DF& s, Matrix43& r, Vector3DF& t ) const
{
	t.X = Values[3][0];
	t.Y = Values[3][1];
	t.Z = Values[3][2];

	float sc[3];
	for( int m = 0; m < 3; m++ )
	{
		sc[m] = 0;
		for( int n = 0; n < 3; n++ )
		{
			sc[m] += Values[m][n] * Values[m][n];
		}
		sc[m] = sqrt( sc[m] );
	}
	
	s.X = sc[0];
	s.Y = sc[1];
	s.Z = sc[2];

	for( int m = 0; m < 3; m++ )
	{
		for( int n = 0; n < 3; n++ )
		{
			r.Values[m][n] = Values[m][n] / sc[m];
		}
	}
	r.Values[3][0] = 0.0f;
	r.Values[3][1] = 0.0f;
	r.Values[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::SetSRT( const Vector3DF& s, const Matrix43& r, const Vector3DF& t )
{
	Matrix43 mats;
	mats.Scaling( s.X, s.Y, s.Z );

	Multiple( *this, mats, r );

	Values[3][0] = t.X;
	Values[3][1] = t.Y;
	Values[3][2] = t.Z;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::Multiple( Matrix43& out, const Matrix43& in1, const Matrix43& in2 )
{
#if 1
	Matrix43 temp;
	// outとin1が共通の場合は一時変数にコピー
	const Matrix43& s = (&out == &in1) ? (temp = in1) : in1;
	
	out.Values[0][0] = s.Values[0][0] * in2.Values[0][0] + s.Values[0][1] * in2.Values[1][0] + s.Values[0][2] * in2.Values[2][0];
	out.Values[0][1] = s.Values[0][0] * in2.Values[0][1] + s.Values[0][1] * in2.Values[1][1] + s.Values[0][2] * in2.Values[2][1];
	out.Values[0][2] = s.Values[0][0] * in2.Values[0][2] + s.Values[0][1] * in2.Values[1][2] + s.Values[0][2] * in2.Values[2][2];
	
	out.Values[1][0] = s.Values[1][0] * in2.Values[0][0] + s.Values[1][1] * in2.Values[1][0] + s.Values[1][2] * in2.Values[2][0];
	out.Values[1][1] = s.Values[1][0] * in2.Values[0][1] + s.Values[1][1] * in2.Values[1][1] + s.Values[1][2] * in2.Values[2][1];
	out.Values[1][2] = s.Values[1][0] * in2.Values[0][2] + s.Values[1][1] * in2.Values[1][2] + s.Values[1][2] * in2.Values[2][2];
	
	out.Values[2][0] = s.Values[2][0] * in2.Values[0][0] + s.Values[2][1] * in2.Values[1][0] + s.Values[2][2] * in2.Values[2][0];
	out.Values[2][1] = s.Values[2][0] * in2.Values[0][1] + s.Values[2][1] * in2.Values[1][1] + s.Values[2][2] * in2.Values[2][1];
	out.Values[2][2] = s.Values[2][0] * in2.Values[0][2] + s.Values[2][1] * in2.Values[1][2] + s.Values[2][2] * in2.Values[2][2];
	
	out.Values[3][0] = s.Values[3][0] * in2.Values[0][0] + s.Values[3][1] * in2.Values[1][0] + s.Values[3][2] * in2.Values[2][0] + in2.Values[3][0];
	out.Values[3][1] = s.Values[3][0] * in2.Values[0][1] + s.Values[3][1] * in2.Values[1][1] + s.Values[3][2] * in2.Values[2][1] + in2.Values[3][1];
	out.Values[3][2] = s.Values[3][0] * in2.Values[0][2] + s.Values[3][1] * in2.Values[1][2] + s.Values[3][2] * in2.Values[2][2] + in2.Values[3][2];
#else
	Matrix43 temp;

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			float v = 0.0f;
			for( int k = 0; k < 3; k++ )
			{
				v += in1.Values[i][k] * in2.Values[k][j];
			}
			temp.Values[i][j] = v;
		}
	}

	for( int i = 0; i < 3; i++ )
	{
		temp.Values[3][i] += in2.Values[3][i];
	}

	out = temp;
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------