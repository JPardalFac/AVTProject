#version 330

#define SPOT_LIGHTS 	2
#define DIRECTIONAL 	1
#define POINT_LIGHTS 	0 //change this value when pointlights are added to the program

#define TOTAL_LIGHTS SPOT_LIGHTS + DIRECTIONAL + POINT_LIGHTS

uniform sampler2D texmap;
uniform sampler2D texmap1;
uniform int texMode;

out vec4 colorOut;
 
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

uniform struct Light{
	float l_cutoff;
	int type;
}lightsOUT[TOTAL_LIGHTS];

in Data {
    vec3 normal;
    vec3 eye;
    vec3 lightDir;
	vec4 spotDir;
	vec2 tex_coord;
} DataIn[TOTAL_LIGHTS];
 
 // Calculates the specular component and intensity of a spot light
 void calculateSpotLights(out vec4 spec, out float intensity, int ind){
	vec3 ld = normalize(DataIn[ind].lightDir);
	vec3 sd = normalize(vec3(-DataIn[ind].spotDir));  
	
	if (acos(dot(sd,ld)) > radians(lightsOUT[ind].l_cutoff)) {
		vec3 n = normalize(DataIn[ind].normal);
		intensity += max(dot(n,ld), 0.0);
	
		if (intensity > 0.0) {
			vec3 eye = normalize(DataIn[ind].eye);
			vec3 h = normalize(ld + eye);
			float intspec = max(dot(h,n), 0.0);
			spec += mat.specular * pow(intspec, mat.shininess);
		}
	}	
}

 // Calculates the specular component and intensity of a point light
void calculatePointLights(out vec4 spec, out float intensity, int ind){
	vec3 n = normalize(DataIn[ind].normal);
	vec3 l = normalize(DataIn[ind].lightDir);
	vec3 e = normalize(DataIn[ind].eye);

	intensity = max(dot(n,l), 0.0);

	if (intensity > 0.0) {
		vec3 h = normalize(l + e);
		float intSpec = max(dot(h,n), 0.0);
		spec = mat.specular * pow(intSpec, mat.shininess);
	}
}

 // Calculates the specular component and intensity of a directional light
void calculateDirectionalLights(out vec4 spec, out float intensity, int ind){
	vec3 n = normalize(DataIn[ind].normal);
	vec3 l = normalize(DataIn[ind].lightDir);
	vec3 e = normalize(DataIn[ind].eye);

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
	
	for(int i=0; i<TOTAL_LIGHTS;i++){
		if(lightsOUT[i].type == 0){
			calculatePointLights(spec_point,intensity_point,i);			
		}
		else if(lightsOUT[i].type == 1 && spotOn){
			calculateSpotLights(spec_spot, intensity_spot,i);
		}		
		else if(lightsOUT[i].type == 2 && directionalLightOn){
			calculateDirectionalLights(spec_dir,intensity_dir,i);
		}
		texel = texture(texmap, DataIn[i].tex_coord); 
		texel1 = texture(texmap1, DataIn[i].tex_coord); 
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
	else if(texMode == 2){ //multitexturing
		colorOut = max(total_intensity * mat.diffuse + total_spec, mat.ambient) * texel * texel1;
	}
}





