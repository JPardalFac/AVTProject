#pragma once
// Information to render each assimp node
#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/PostProcess.h"
#include "assimp/Scene.h"
// assimp include files. These three are usually needed.


#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "VertexAttrDef.h"
struct MyMaterial {
	float diffuse[4];
	float ambient[4];
	float specular[4];
	float emissive[4];
	float shininess;
	int texCount;
};

struct MyMesh2 {

	GLuint vao;
	GLuint texIndex;
	GLuint uniformBlockIndex;
	int numFaces;
	struct MyMaterial mat;
	std::string name;
};
//#define PI 3.1415289
class ObjectFromLoad {
public:

	const aiScene* object = NULL;
	int numNodes = 0;
	int counted = 0, node = 0;
	Assimp::Importer importer;
	std::vector<struct MyMesh2> myMeshes;
	
	std::map<std::string, GLuint> textureIdMap;
	//// Vertex Attribute Locations

	void set_float4(float f[4], float a, float b, float c, float d);
	void color4_to_float4(const aiColor4D *c, float f[4]);
	bool Import3DFromFile(const std::string& pFile);
	//int LoadGLTextures();
	std::string setMeshName(int nMeshesInNode);
	void genVAOsAndUniformBuffer();
	void get_bounding_box_for_node(const aiNode* nd, aiVector3D* min, aiVector3D* max);
	void get_bounding_box(aiVector3D* min, aiVector3D* max);
	void renderModel(GLuint programId);
	void sendMaterial(GLuint programId);

	void clear();
	void setMaterial(int n, float amb[4], float diff[4], float spec[4], float emiss[4], float shi, int texC);
	//void initObject(float pos[3], float rot, float rotAxis[3]);
	//void move(int  dir);
	//void rotate(int dir);
};