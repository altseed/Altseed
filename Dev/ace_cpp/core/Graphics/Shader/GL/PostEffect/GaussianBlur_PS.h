static const char* gaussianblur_ps_gl = R"(
uniform sampler2D g_texture;
uniform vec3      g_weight;
void main()
{
#ifdef BLUR_X
	vec2 accum = vec2(1.0 / float(textureSize(g_texture, 0).x), 0.0);
	vec2 half_  = vec2(0.5 / float(textureSize(g_texture, 0).x), 0.0);
	vec2 adder = vec2(2.0 / float(textureSize(g_texture, 0).x), 0.0);
#endif

#ifdef BLUR_Y
	vec2 accum = vec2(0.0, 1.0 / float(textureSize(g_texture, 0).y));
	vec2 half_  = vec2(0.0, 0.5 / float(textureSize(g_texture, 0).y));
	vec2 adder = vec2(0.0, 2.0 / float(textureSize(g_texture, 0).y));
#endif

	vec4 output_ = (texture2D(g_texture, inUV.xy + half_ + accum)  +
	               texture2D(g_texture, inUV.xy + half_ - accum)) *
	               g_weight.x;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.y;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.z;

	output_.a = 1.0;

	outOutput = output_; 
}

)";
