/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Original Author: Damiano Vitulli
 * Porting to OpenGL3.3: Movania Muhammad Mobeen
 * Shaders Functions: Movania Muhammad Mobeen
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 */

#include "TextureMappedFont.h"
#include <cassert>
#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);
extern int screen_width;
extern int screen_height;

TextureMappedFont::TextureMappedFont(void)
{
	_textureID =-1; 
	_fontSize=16; 
}

TextureMappedFont::~TextureMappedFont(void)
{
	glDeleteVertexArrays(1, &_vaoID);
	glDeleteBuffers(1, &_vertexBuffer);
	glDeleteBuffers(1, &_texCoordBuffer);
	glDeleteTextures(1, &_textureID);
}

TextureMappedFont::TextureMappedFont(const std::string& textureName, float fontSize)
{  
	_textureID = LoadBMP(const_cast<char*>(textureName.c_str()));    
	_shader.init();
	_shader.loadShader(VSShaderLib::VERTEX_SHADER,"font.vert");
	_shader.loadShader(VSShaderLib::FRAGMENT_SHADER,"font.frag");
	//_shader.CreateAndLinkProgram();
	GL_CHECK_ERRORS
	_shader.Use();	
		glBindFragDataLocation(_shader.getProgramIndex(), 0, "colorOut");
		glBindAttribLocation(_shader.getProgramIndex(), 0, "vVertex");
		glBindAttribLocation(_shader.getProgramIndex(), 1, "vTextCoord");	
		glUniform1i(glGetUniformLocation(_shader.getProgramIndex(), "texture0"),0);
		glUniform1i(glGetUniformLocation(_shader.getProgramIndex(), "selected"),0);
	_shader.UnUse();
    _fontSize = fontSize;

	Init();
}

bool TextureMappedFont::Init()
{
    float vertices [] = {
        0.0f, 0.0f,
        _fontSize, 0.0f, 
        _fontSize, _fontSize,
        0.0f, _fontSize
    };

	std::string vertex = "vVertex";
	std::string texcoord = "vTexCoord";
	GL_CHECK_ERRORS

	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(_shader.getProgramIndex(), vertex.c_str()));
    glVertexAttribPointer(glGetAttribLocation(_shader.getProgramIndex(), vertex.c_str()), 2, GL_FLOAT, GL_FALSE, 0, 0);


	GL_CHECK_ERRORS

    //Just initialize with something for now, the tex coords are updated
    //for each character printed
    float texCoords [] = {
        0.0f, 0.0f,
        0.0f, 0.0f, 
        0.0f, 0.0f,
        0.0f, 0.0f
    };

    glGenBuffers(1, &_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &texCoords[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(glGetAttribLocation(_shader.getProgramIndex(), texcoord.c_str()));
    glVertexAttribPointer(glGetAttribLocation(_shader.getProgramIndex(), texcoord.c_str()), 2, GL_FLOAT, GL_FALSE, 0, 0);

	GL_CHECK_ERRORS
	//set the orthographic projection matrix
	//Orthographic(0.f, float(screen_width), 0.f , float(screen_height),-1.f,1.f, P);
    return true;
}



void TextureMappedFont::DrawString(float x, float y, const std::string& str, bool sel) 
{
   // static float modelviewMatrix[16];
    //static float projectionMatrix[16];

	GL_CHECK_ERRORS
	
    GLint test[1];
	glGetIntegerv(GL_DEPTH_TEST, test);
    glDisable(GL_DEPTH_TEST);
	 
    _shader.Use();
    
    float texCoords[8];

    glBindTexture(GL_TEXTURE_2D, _textureID);
 
	GL_CHECK_ERRORS

	matrix_4x4_type translate,I, MV, oldMV, MVP;
	MatrIdentity_4x4(MV);
	MatrIdentity_4x4(MVP);
	MatrIdentity_4x4(translate);
	MatrIdentity_4x4(I);
	//Position our text
	translate[3][0]=x;
	translate[3][1]=y;

	MatrMul_4x4_4x4(translate, I, MV);
	glBindVertexArray(_vaoID);
   // glTranslatef(x, y, 0.0); //Position our text
    for(string::size_type i = 0; i < str.size(); ++i) 
    {
		MatrMul_4x4_4x4(MV, P, MVP);
        const float oneOverSixteen = 1.0f / 16.0f;

        int ch = int(str[i]);
        float xPos = float(ch % 16) * oneOverSixteen;
        float yPos = float(ch / 16) * oneOverSixteen;

        texCoords[0] = xPos;
        texCoords[1] = 1.0f - yPos - oneOverSixteen;

        texCoords[2] = xPos + oneOverSixteen;
        texCoords[3] = 1.0f - yPos - oneOverSixteen;

        texCoords[4] = xPos + oneOverSixteen;
        texCoords[5] = 1.0f - yPos - 0.001f;

        texCoords[6] = xPos;
        texCoords[7] = 1.0f - yPos - 0.001f;

        glBindBuffer(GL_ARRAY_BUFFER, _texCoordBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 8, &texCoords[0]);

       	glUniformMatrix4fv(glGetUniformLocation(_shader.getProgramIndex(), "m_pvm"), 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(glGetUniformLocation(_shader.getProgramIndex(), "selected"), sel);

		GL_CHECK_ERRORS

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
		GL_CHECK_ERRORS

		MatrCopy_4x4(oldMV,MV);
		matrix_4x4_type t2;
		MatrIdentity_4x4(t2);
		t2[3][0]=_fontSize * 0.8f;
		MatrMul_4x4_4x4(t2, oldMV, MV);

		GL_CHECK_ERRORS 
    }  
	glBindVertexArray(0);
	GL_CHECK_ERRORS

	_shader.UnUse();

	GL_CHECK_ERRORS

	//if(test[0])
		glEnable(GL_DEPTH_TEST);
}