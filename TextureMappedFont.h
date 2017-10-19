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

#pragma once

#include <string>
#include "vsShaderLib.h"
#include "load_bmp.h"
#include "AVTMathLib.h"

class TextureMappedFont
{
public:
	TextureMappedFont(void);	
	TextureMappedFont(const std::string& fontTexture, float fontSize=16.0f);
	~TextureMappedFont(void);

    bool Init();
    void DrawString(float x, float y, const std::string& string, bool selected=false );

private:
    GLuint _textureID, _vaoID; 
    
    GLuint _texCoordBuffer;
    GLuint _vertexBuffer;
    float _fontSize;  

	//matrix_4x4_type P; //orthographic projection matrix
    VSShaderLib _shader;
};
