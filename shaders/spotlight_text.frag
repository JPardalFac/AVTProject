#version 330
 
#define SPOT_LIGHTS 2 
#define IN 2

uniform sampler2D texmap;
//uniform sampler2D texmap1;
//uniform sampler2D texmap2;

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
uniform int numLights;
uniform bool spotOn;

uniform struct Light{
	float l_cutoff;
	int type;
}lightsOUT[SPOT_LIGHTS];



in Data {
    vec3 normal;
    vec3 eye;
    vec3 lightDir;
	vec4 spotDir;
	vec2 tex_coord;
} DataIn[IN];
 
void main() {
	float intensity_spot = 0.0;
	vec4 spec_spot = vec4(0.0);
	vec4 spec_point = vec4(0.0);
	float intensity_point = 0.0;
	vec4 texel;
	
	for(int i=0; i<numLights;i++){
		//spotlight
		if(lightsOUT[i].type == 1 && spotOn){
			vec3 ld = normalize(DataIn[i].lightDir);
			vec3 sd = normalize(vec3(-DataIn[i].spotDir));  
			
			if (acos(dot(sd,ld)) > radians(lightsOUT[i].l_cutoff)) {
				vec3 n = normalize(DataIn[i].normal);
				intensity_spot += max(dot(n,ld), 0.0);
			
				if (intensity_spot > 0.0) {
					vec3 eye = normalize(DataIn[i].eye);
					vec3 h = normalize(ld + eye);
					float intSpec = max(dot(h,n), 0.0);
					spec_spot += mat.specular * pow(intSpec, mat.shininess);
				}
			}	
		}
		
		//pointlight
		if(lightsOUT[i].type == 0){
			vec3 n = normalize(DataIn[i].normal);
			vec3 l = normalize(DataIn[i].lightDir);
			vec3 e = normalize(DataIn[i].eye);

			intensity_point = max(dot(n,l), 0.0);

		
			if (intensity_point > 0.0) {

				vec3 h = normalize(l + e);
				float intSpec = max(dot(h,n), 0.0);
				spec_point = mat.specular * pow(intSpec, mat.shininess);
			}
			
		}
		
		texel = texture(texmap, DataIn[i].tex_coord); 
	}
	
	float total_intensity = intensity_point + intensity_spot;
	vec4 total_spec = spec_point + spec_spot;
	if(texMode == 0) // modulate diffuse color with texel color
	{
		 // texel from lighwood.tga
		colorOut = max(total_intensity * mat.diffuse * texel + total_spec,mat.ambient * texel);
	}

	
	//colorOut = max(total_intensity * mat.diffuse + total_spec, mat.ambient);
	
}