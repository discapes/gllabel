#version 330 core
layout(location = 0) out vec4 color;
in vec2 oUV;
uniform sampler2D uSampler;
uniform vec2 uT;
uniform vec2 uAmt;

vec2 SineWave(vec2 p)
{
	// convert Vertex position <-1,+1> to texture coordinate <0,1> and some shrinking so the effect dont overlap screen
	// p.x=( 0.55*p.x)+0.5;
	// p.y=(-0.55*p.y)+0.5;
	// // wave distortion
	float x = sin( 25.0*p.y + 30.0*p.x + 6.28*uT.x) * uAmt.x;
	float y = sin( 25.0*p.y + 30.0*p.x + 6.28*uT.y) * uAmt.y;
	return vec2(p.x+x, p.y+y);
}

void main()
{
	color = texture(uSampler, SineWave(oUV));
}
