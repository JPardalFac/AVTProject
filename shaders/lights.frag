#version 330
#pragma optionNV(unroll all)

#define SPOT_LIGHTS 	2
#define DIRECTIONAL 	1
#define POINT_LIGHTS 	6//change this value when pointlights are added to the program

#define TOTAL_LIGHTS SPOT_LIGHTS + DIRECTIONAL + POINT_LIGHTS

uniform sampler2D texmap;
uniform sampler2D texmap1;
uniform sampler2D texParticle;
uniform int texMode;

in vec4 positionForFog;

out vec4 colorOut;

//uniform int numLights1;
 
struct Materials {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
	vec4 emissive;
    float shininess;
	int texCount;
};

uniform Materials mat;
uniform bool spotOn;
uniform bool directionalLightOn;
uniform bool pointLightOn;

uniform struct Light{
	vec4 l_pos;
	vec4 l_spotDir;
	float l_cutoff;
	int type;
}lightsOUT[TOTAL_LIGHTS];

in Data {
    vec3 normal;
    vec3 eye;
	vec4 pos;
//  vec3 lightDir;
//	vec4 spotDir;
	vec2 tex_coord;
} DataIn;
 
 
const vec3 fogColor = vec3 (0.5, 0.5, 0.5);
const float fogDensity = 0.05;

vec4 applyFog(vec4 color, float distance);	//prototype

 // Calculates the specular component and intensity of a spot light
 void calculateSpotLights(out vec4 spec, out float intensity, int ind){
	vec3 ld = normalize(vec3(lightsOUT[ind].l_pos - DataIn.pos));
	vec3 sd = normalize(vec3(-lightsOUT[ind].l_spotDir));  
	
	if (acos(dot(sd,ld)) > radians(lightsOUT[ind].l_cutoff)) {
		vec3 n = normalize(DataIn.normal);
		intensity += max(dot(n,ld), 0.0);
	
		if (intensity > 0.0) {
			vec3 eye = normalize(DataIn.eye);
			vec3 h = normalize(ld + eye);
			float intspec = max(dot(h,n), 0.0);
			spec += mat.specular * pow(intspec, mat.shininess);
		}
	}	
}

 // Calculates the specular component and intensity of a point light
void calculatePointLights(out vec4 spec, out float intensity, int ind){
	vec3 n = normalize(DataIn.normal);
	vec3 l = normalize(vec3(lightsOUT[ind].l_pos - DataIn.pos));
	vec3 e = normalize(DataIn.eye);

	intensity += max(dot(n,l), 0.0);

	if (intensity > 0.0) {
		vec3 h = normalize(l + e);
		float intSpec = max(dot(h,n), 0.0);
		spec += mat.specular * pow(intSpec, mat.shininess);
	}
}

 // Calculates the specular component and intensity of a directional light
void calculateDirectionalLights(out vec4 spec, out float intensity, int ind){
	vec3 n = normalize(DataIn.normal);
	vec3 l = normalize(vec3(lightsOUT[ind].l_pos));
	vec3 e = normalize(DataIn.eye);

	intensity = max(dot(n,l), 0.0);
	
	if (intensity > 0.0) {
		vec3 h = normalize(l + e);
		float intSpec = max(dot(h,n), 0.0);
		spec = mat.specular * pow(intSpec, mat.shininess);
	}
}
 
void main() {
	vec4 spec_spot = vec4(0.0);
	float intensity_spot = 0.0;
	
	vec4 spec_point = vec4(0.0);
	float intensity_point = 0.0;
	
	vec4 spec_dir = vec4(0);
	float intensity_dir = 0.0;	
	
	vec4 texel;
	vec4 texel1;
	vec4 texPart;
	
	for(int i=0; i<TOTAL_LIGHTS;i++){
		if(lightsOUT[i].type == 0 && pointLightOn){
			calculatePointLights(spec_point,intensity_point,i);			
		}
		else if(lightsOUT[i].type == 1 && spotOn){
			calculateSpotLights(spec_spot, intensity_spot,i);
		}		
		else if(lightsOUT[i].type == 2 && directionalLightOn){
			calculateDirectionalLights(spec_dir,intensity_dir,i);
		}
		texel = texture(texmap, DataIn.tex_coord); 
		texel1 = texture(texmap1, DataIn.tex_coord); 
	}
	
	// Calculates the color from the values obtained from all the lights
	float total_intensity = intensity_point + intensity_spot + intensity_dir;
	vec4 total_spec = spec_point + spec_spot + spec_dir;
	if(texMode == 1) // modulate diffuse color with texel color
	{		 
		colorOut = max(total_intensity * mat.diffuse * texel + total_spec,mat.ambient * texel);
	}
	else if(texMode == 0){ //color without texture
		colorOut = max(total_intensity * mat.diffuse + total_spec, mat.ambient);
	}
	else if(texMode == 2){ //particle system (hopefully)
		texPart = texture(texParticle, DataIn.tex_coord);
		texPart.a = texPart.r;     //this is a trick because the particle.bmp does not have alpha channel
		colorOut = mat.diffuse * texPart;
	}
	
	//*************************************************
	float distance = length(positionForFog);
	vec4 finalColor = applyFog(colorOut, distance);	
	colorOut = finalColor;
}

//color - original color of the fragment 
//distance - camera to point distance
vec4 applyFog(vec4 color, float distance) 
{
	//float fogAmount = exp(-distance * fogDensity);
	float fogAmount = 1.0 / exp(distance * fogDensity);
	fogAmount = clamp(fogAmount, 0.0, 1.0);
	vec3 finalColor = mix(fogColor, color.xyz, fogAmount);
	
	return vec4(finalColor, color.a);
}




