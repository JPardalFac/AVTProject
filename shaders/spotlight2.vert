#version 330
#define SPOT_LIGHTS 	2
#define DIRECTIONAL 	1
#define POINT_LIGHTS 	0 //change this value when pointlights are added to the program

#define TOTAL_LIGHTS SPOT_LIGHTS + DIRECTIONAL + POINT_LIGHTS

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;

uniform struct light{
	vec4 l_pos;
	vec4 l_spotDir;
}lightsIn[TOTAL_LIGHTS];	// <--------- CAREFUL WHEN ADDING POINTLIGHTS, SEE IF THIS STRUCT MAKES SENSE FOR THE POINTLIGHTS

in vec4 position;
in vec4 normal;    //por causa do gerador de geometria

out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
	vec4 spotDir;
} DataOut[TOTAL_LIGHTS];

void main () {

	vec4 pos = m_viewModel * position;
	
	for(int i =0; i < SPOT_LIGHTS; i++){
		DataOut[i].normal = normalize(m_normal * normal.xyz);
		DataOut[i].lightDir = vec3(lightsIn[i].l_pos - pos);
		DataOut[i].eye = vec3(-pos);
		DataOut[i].spotDir = m_viewModel*lightsIn[i].l_spotDir;
	}
	
	//this bit of code assumes the order in lightsIn is spotlights, followed by directional light
	if(lightsIn[SPOT_LIGHTS].l_pos.w == 0)
	{ //if it is a directional light
		DataOut[SPOT_LIGHTS].normal = normalize(m_normal * normal.xyz);
		DataOut[SPOT_LIGHTS].lightDir = normalize(vec3(lightsIn[SPOT_LIGHTS].l_pos));
		DataOut[SPOT_LIGHTS].eye = vec3(-pos);
	}
	

	gl_Position = m_pvm * position;	
}