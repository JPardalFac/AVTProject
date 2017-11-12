#version 330
#pragma optionNV(unroll all)

//#define SPOT_LIGHTS 	2
//#define DIRECTIONAL 	1
//#define POINT_LIGHTS 	6 //change this value when pointlights are added to the program

//#define TOTAL_LIGHTS SPOT_LIGHTS + DIRECTIONAL + POINT_LIGHTS
//#define TOTAL_LIGHTS 8

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;

//uniform int numLights;

//uniform struct light{
//	vec4 l_pos;
//	vec4 l_spotDir;
//}lightsOUT[TOTAL_LIGHTS];	// <--------- CAREFUL WHEN ADDING POINTLIGHTS, SEE IF THIS STRUCT MAKES SENSE FOR THE POINTLIGHTS

//uniform struct Light{
//	vec4 l_pos;
//	vec4 l_spotDir;
//	float l_cutoff;
//	int type;
//}lightsOUT[TOTAL_LIGHTS];

in vec4 position;
in vec4 normal;    //por causa do gerador de geometria
in vec4 texCoord;

out Data {
	vec3 normal;
	vec3 eye;
	vec4 pos;
	vec2 tex_coord;
} DataOut;

void main () {

	DataOut.pos = m_viewModel * position;
	DataOut.normal = normalize(m_normal * normal.xyz);
	DataOut.eye = vec3(- DataOut.pos);
	DataOut.tex_coord = texCoord.st;
	gl_Position = m_pvm * position;
	//for(int i =0; i < TOTAL_LIGHTS; i++){
	//	DataOut.normal = normalize(m_normal * normal.xyz);
	//	if(lightsOUT[i].l_pos.w == 0){ // <---------------------------------- needed to diferentiate the directional light from point and spot
	//		DataOut.lightDir = normalize(vec3(lightsOUT[i].l_pos));
	//	}
	//	else{
	//		DataOut.lightDir = vec3(lightsOUT[i].l_pos - pos);
	//	}
	//	DataOut.eye = vec3(- DataOut.pos);		
	//	DataOut.spotDir = m_viewModel*lightsOUT[i].l_spotDir;
	//	DataOut.tex_coord = texCoord.st;
	//}
	//gl_Position = m_pvm * position;
}



