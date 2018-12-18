#version 400 core //http://glslsandbox.com/e#38987.1

//in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader
out vec4 vOutputColour;		// The output colour
uniform sampler2D sampler0;  // The texture sampler
in float t;
uniform bool darkmode;

void main()
{
	if(!darkmode)
	{
		vec4 vTexColour = texture(sampler0, vTexCoord);	
		vOutputColour = vTexColour;
	} else
	{
	float time = t/10.0;
	vec2 uv = ( vTexCoord.xy / vec2(40.0,40) ) * 2.0 - 1.0;
	vec3 finalColor = vec3 ( 0.27, 0.0275, 0.75 );
	finalColor *= abs( 1.0 / (tan( uv.x + tan(uv.y-time)/tan(uv.y*time)* 0.27 ) * 27.0) );
	vOutputColour = vec4( finalColor, 1.0 );
	}
}
