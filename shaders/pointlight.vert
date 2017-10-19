#version 330

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;


uniform vec4 l_pos;
uniform vec4 l_spotDir;
uniform float l_spotCutOff;
uniform int l_type;

out vec4 spotDir;
out float spotCutOff;
flat out int type;

in vec4 position;
in vec4 normal;    //por causa do gerador de geometria

out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
} DataOut;

void main () {

	vec4 pos = m_viewModel * position;

	DataOut.normal = normalize(m_normal * normal.xyz);
	DataOut.lightDir = vec3(l_pos - pos);
	DataOut.eye = vec3(-pos);
	
	spotDir = m_viewModel*l_spotDir;
	spotCutOff = l_spotCutOff;
	type = l_type;

	gl_Position = m_pvm * position;	
}