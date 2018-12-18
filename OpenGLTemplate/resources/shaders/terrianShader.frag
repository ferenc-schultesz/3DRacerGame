#version 400 core
uniform sampler2D sampler0, sampler1; 
uniform float fMinHeight, fMaxHeight; 

in vec3 n; 
in vec4 vWorldPosition,p;
in vec2 vTexCoord;
out vec4 vOutputColour;

struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};


// Structure holding material information:  its ambient, diffuse, and specular colours, and shininess
struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};
// Lights and materials passed in as uniform variables from client programme
uniform LightInfo light1;
uniform MaterialInfo material1; 

vec3 PhongModel(vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light1.position - p));
	vec3 v = normalize(-p.xyz);
	vec3 r = reflect(-s, n);
	vec3 ambient = light1.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0f);
	vec3 diffuse = light1.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0f);
	float eps = 0.000001f; // add eps to shininess below -- pow not defined if second argument is 0 (as described in GLSL documentation)
	if (sDotN > 0.0f) 
		specular = light1.Ls * material1.Ms * pow(max(dot(r, v), 0.0f), material1.shininess + eps);
	

	return ambient + diffuse + specular;

}

void main() {
	
	vec3 vColour =  PhongModel(p, normalize(n));

	vec4 vTexColour0 = texture(sampler0, vTexCoord);
	vec4 vTexColour1 = texture(sampler1, vTexCoord); 

	float f = clamp((vWorldPosition.y - fMinHeight) / (fMaxHeight - fMinHeight), 0, 1); 
	vec4 vTexColour; 

	
	vTexColour = mix(vTexColour0, vTexColour1, f);
	
	vOutputColour = vTexColour * vec4 (vColour, 1.0f);
}