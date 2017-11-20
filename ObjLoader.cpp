
#include "ObjLoader.h"

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

void ObjectFromLoad::get_bounding_box_for_node(const aiNode* nd,
	aiVector3D* min,
	aiVector3D* max)

{
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = object->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n], min, max);
	}
}


void ObjectFromLoad::get_bounding_box(aiVector3D* min, aiVector3D* max)
{

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(object->mRootNode, min, max);
}


void ObjectFromLoad::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

void ObjectFromLoad::color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}



bool ObjectFromLoad::Import3DFromFile(const std::string& pFile)
{

	//check if file exists
	std::ifstream fin(pFile.c_str());
	if (!fin.fail()) {
		fin.close();
	}
	else {
		printf("Couldn't open file: %s\n", pFile.c_str());
		printf("%s\n", importer.GetErrorString());
		return false;
	}

	object = importer.ReadFile(pFile, aiProcessPreset_TargetRealtime_Quality); //aiProcess_Triangulate

	// If the import failed, report it
	if (!object)
	{
		printf("%s\n", importer.GetErrorString());
		return false;
	}

	numNodes = object->mRootNode->mNumChildren;
	// Now we can access the file's contents.
	printf("Import of object %s succeeded.\n", pFile.c_str());
	//initObject(pos,rot,rotAxis);

	aiVector3D scene_min, scene_max, scene_center;
	get_bounding_box(&scene_min, &scene_max);
	float tmp;
	tmp = scene_max.x - scene_min.x;
	tmp = scene_max.y - scene_min.y > tmp ? scene_max.y - scene_min.y : tmp;
	tmp = scene_max.z - scene_min.z > tmp ? scene_max.z - scene_min.z : tmp;
	//scaleFactor = 1.f / tmp;

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}



//int ObjectFromLoad::LoadGLTextures()
//{
//	ILboolean success;
//
//	/* initialization of DevIL */
//	ilInit();
//
//	/* scan objs[objId]'s materials for textures */
//	for (unsigned int m = 0; m<object->mNumMaterials; ++m)
//	{
//		int texIndex = 0;
//		aiString path;	// filename
//
//		aiReturn texFound = object->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
//		while (texFound == AI_SUCCESS) {
//			//fill map with textures, OpenGL image ids set to 0
//			textureIdMap[path.data] = 0;
//			// more textures?
//			texIndex++;
//			texFound = object->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
//		}
//	}
//
//	int numTextures = textureIdMap.size();
//
//	/* create and fill array with DevIL texture ids */
//	ILuint* imageIds = new ILuint[numTextures];
//	ilGenImages(numTextures, imageIds);
//
//	/* create and fill array with GL texture ids */
//	GLuint* textureIds = new GLuint[numTextures];
//	glGenTextures(numTextures, textureIds); /* Texture name generation */
//
//											/* get iterator */
//	std::map<std::string, GLuint>::iterator itr = textureIdMap.begin();
//	int i = 0;
//	for (; itr != textureIdMap.end(); ++i, ++itr)
//	{
//		//save IL image ID
//		std::string filename = (*itr).first;  // get filename
//		(*itr).second = textureIds[i];	  // save texture id for filename in map
//
//		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
//		ilEnable(IL_ORIGIN_SET);
//		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
//		success = ilLoadImage((ILstring)filename.c_str());
//
//		if (success) {
//			/* Convert image to RGBA */
//			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
//
//			/* Create and load textures to OpenGL */
//			glBindTexture(GL_TEXTURE_2D, textureIds[i]);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
//				ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE,
//				ilGetData());
//		}
//		else
//			printf("Couldn't load Image: %s\n", filename.c_str());
//	}
//	/* Because we have already copied image data into texture data
//	we can release memory used by image. */
//	ilDeleteImages(numTextures, imageIds);
//
//	//Cleanup
//	delete[] imageIds;
//	delete[] textureIds;
//
//	//return success;
//	return true;
//}

std::string ObjectFromLoad::setMeshName(int nMeshesInNode) {
	//std::cout << "numMeshes in node: "<< object->mRootNode->mChildren[node]->mNumMeshes << std::endl;
	if (counted < nMeshesInNode) {
		counted++;
		return object->mRootNode->mChildren[node]->mName.C_Str();
	}
	else {
		node++;
		counted = 1;
		std::string s = object->mRootNode->mChildren[node]->mName.C_Str();
		return s;
	}

}

void ObjectFromLoad::genVAOsAndUniformBuffer() {

	struct MyMesh2 aMesh;
	struct MyMaterial aMat;
	GLuint buffer;
	// For each mesh
	for (unsigned int n = 0; n < object->mNumMeshes; n++)
	{
		const aiMesh* mesh = object->mMeshes[n];
		// create array with faces
		// have to convert from Assimp format to array
		unsigned int *faceArray;
		faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
		unsigned int faceIndex = 0;

		for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];

			memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
			faceIndex += 3;
		}
		aMesh.numFaces = object->mMeshes[n]->mNumFaces;
		aMesh.name = setMeshName(object->mRootNode->mChildren[node]->mNumMeshes);

		// generate Vertex Array for mesh
		glGenVertexArrays(1, &(aMesh.vao));
		glBindVertexArray(aMesh.vao);

		// buffer for faces
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

		// buffer for vertex positions
		if (mesh->HasPositions()) {
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
			glVertexAttribPointer(VERTEX_COORD_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
		}

		// buffer for vertex normals
		if (mesh->HasNormals()) {
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
			glEnableVertexAttribArray(NORMAL_ATTRIB);
			glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
		}

		// buffer for vertex texture coordinates
		if (mesh->HasTextureCoords(0)) {
			float *texCoords = (float *)malloc(sizeof(float) * 2 * mesh->mNumVertices);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

				texCoords[k * 2] = mesh->mTextureCoords[0][k].x;
				texCoords[k * 2 + 1] = mesh->mTextureCoords[0][k].y;

			}
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB);
			glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
		}

		// unbind buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		myMeshes.push_back(aMesh);
	}
}




void ObjectFromLoad::renderModel(GLuint programId) {
	for (int n = 0; n< myMeshes.size(); n++) {
		std::string mName = myMeshes[n].name;
		if (mName.substr(0, mName.find(".")) == "glass") {
			glDepthMask(GL_FALSE);
		}
		GLint loc;
		loc = glGetUniformLocation(programId, "mat.ambient");
		glUniform4fv(loc, 1, myMeshes[n].mat.ambient);
		
		loc = glGetUniformLocation(programId, "mat.diffuse");
		glUniform4fv(loc, 1, myMeshes[n].mat.diffuse);
		
		loc = glGetUniformLocation(programId, "mat.specular");
		glUniform4fv(loc, 1, myMeshes[n].mat.specular);

		loc = glGetUniformLocation(programId, "mat.emissive");
		glUniform4fv(loc, 1, myMeshes[n].mat.emissive);

		loc = glGetUniformLocation(programId, "mat.shininess");
		glUniform1f(loc, myMeshes[n].mat.shininess);

		loc = glGetUniformLocation(programId, "mat.texCount");
		glUniform1f(loc, myMeshes[n].mat.texCount);
		
		glBindVertexArray(myMeshes[n].vao);
		glDrawElements(GL_TRIANGLES, myMeshes[n].numFaces * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
	}
}

void ObjectFromLoad::sendMaterial(GLuint programId)
{
	for (int n = 0; n< myMeshes.size(); n++) {
		GLint loc;
		loc = glGetUniformLocation(programId, "mat.ambient");
		glUniform4fv(loc, 1, myMeshes[n].mat.ambient);
		//std::cout << myMeshes[n].mat.ambient[0] << ", " << myMeshes[n].mat.ambient[1] << ", " << myMeshes[n].mat.ambient[2] << ", " << myMeshes[n].mat.ambient[3] << std::endl;
		
		loc = glGetUniformLocation(programId, "mat.diffuse");
		glUniform4fv(loc, 1, myMeshes[n].mat.diffuse);
		//std::cout << myMeshes[n].mat.diffuse[0] << ", " << myMeshes[n].mat.diffuse[1] << ", " << myMeshes[n].mat.diffuse[2] << ", " << myMeshes[n].mat.diffuse[3] << std::endl;
		
		loc = glGetUniformLocation(programId, "mat.specular");
		glUniform4fv(loc, 1, myMeshes[n].mat.specular);
		
		loc = glGetUniformLocation(programId, "mat.shininess");
		glUniform1f(loc, myMeshes[n].mat.shininess);
		//std::cout << myMeshes[n].mat.shininess << std::endl;

		loc = glGetUniformLocation(programId, "mat.texCount");
		glUniform1f(loc, myMeshes[n].mat.texCount);
		
	}
}

void ObjectFromLoad::clear() {
	// cleaning up
	textureIdMap.clear();

	// clear myMeshes stuff
	for (unsigned int i = 0; i < myMeshes.size(); ++i) {

		glDeleteVertexArrays(1, &(myMeshes[i].vao));
		glDeleteTextures(1, &(myMeshes[i].texIndex));
	}
}

void ObjectFromLoad::setMaterial(int n, float amb[4], float diff[4], float spec[4], float emiss[4], float shi, int texC)
{
	for (int i = 0; i < myMeshes.size(); i++) {
		if (i == n) {
			memcpy(myMeshes[i].mat.ambient, amb, 4 * sizeof(float));
			memcpy(myMeshes[i].mat.diffuse, diff, 4 * sizeof(float));
			memcpy(myMeshes[i].mat.specular, spec, 4 * sizeof(float));
			memcpy(myMeshes[i].mat.emissive, emiss, 4 * sizeof(float));
			myMeshes[i].mat.shininess = shi;
			myMeshes[i].mat.texCount = texC;
			break;
		}
	}
}



