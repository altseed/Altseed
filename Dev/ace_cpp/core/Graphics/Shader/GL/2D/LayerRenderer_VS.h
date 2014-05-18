static const char* layerrenderer_vs_gl = R"(

attribute vec3 Pos;
attribute vec2 UV;
attribute vec4 Color;

varying vec4 vaTexCoord;
varying vec4 vaColor;

uniform vec2 Size;
uniform vec4 LayerPosU;
uniform vec4 LayerPosL;

void main()
{
	vec2 ul = LayerPosU.xy / Size.xy;
	vec2 ur = LayerPosU.zw / Size.xy;
	vec2 lr = LayerPosL.xy / Size.xy;
	vec2 ll = LayerPosL.zw / Size.xy;

	vec2 pos = vec2( Pos.x, Pos.y );
  
	vec2 u = (ur - ul) * pos.x + ul;
	vec2 l = (lr - ll) * pos.x + ll;
	vec2 p = (l - u) * pos.y + u;

	gl_Position.x = p.x * 2.0 - 1.0;
	gl_Position.y = -(p.y * 2.0 - 1.0);

	gl_Position.z = 0.5;
	gl_Position.w = 1.0;

	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);
	vaColor = Color;
}

)";