#version 400 core //http://glslsandbox.com/e#39055.2

//in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader

out vec4 vOutputColour;		// The output colour

uniform sampler2D sampler0;  // The texture sampler

in float t;

uniform bool darkmode;

void main()
{
	float time = t /10.0;
	vec2 sp = vTexCoord;//vec2(.4, .7);
	vec2 p = sp * 15.0 - vec2(20.0);
	vec2 i = p;
	float c, inten;
	if (darkmode)
	{
		c = 5.0; // brightness; larger -> darker
		inten = 0.1; // brightness; larger -> brighter
	} else
	{
		c = 2.0; // brightness; larger -> darker
		inten = 0.025; // brightness; larger -> brighter
	}
	float speed = 4.0; // larger -> slower
	float speed2 = 5.0; // larger -> slower
	float freq = 0.5; // ripples
	float xflow = 1.0; // flow speed in x direction
	float yflow = -1.0; // flow speed in y direction

	for (int n = 0; n < 5; n++) {
		float t = time * (1.0 - (3.0 / (float(n) + speed)));
		i = p + vec2(cos(t - i.x * freq) + sin(t + i.y * freq) + (time * xflow), sin(t - i.y * freq) + cos(t + i.x * freq) + (time * yflow));
		c += 1.0 / length(vec2(p.x / (sin(i.x + t * speed2) / inten), p.y / (cos(i.y + t * speed2) / inten)));
	}
	
	c /= float(5);
	c = 1.5 - sqrt(c);
	vOutputColour = vec4(vec3(c * c * c * c), 0.0) + vec4(0.0, 0.4, 0.55, 1.0);
	//vOutputColour *= vec4(1.0,1.0,1.0,0.1);


}
