#version 330
#define NUMLIGHTS 2

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;

uniform struct light{
	vec4 l_pos;
	vec4 l_spotDir;
}lightsIn[NUMLIGHTS];

in vec4 position;
in vec4 normal;    //por causa do gerador de geometria
in vec4 texCoord;

out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
	vec4 spotDir;
	vec2 tex_coord;
} DataOut[NUMLIGHTS];

void main () {

	vec4 pos = m_viewModel * position;
	
	for(int i =0; i < NUMLIGHTS; i++){
		DataOut[i].normal = normalize(m_normal * normal.xyz);
		DataOut[i].lightDir = vec3(lightsIn[i].l_pos - pos);
		DataOut[i].eye = vec3(-pos);
		DataOut[i].spotDir = m_viewModel*lightsIn[i].l_spotDir;
		DataOut[i].tex_coord = texCoord.st;
	}
	gl_Position = m_pvm * position;	
}