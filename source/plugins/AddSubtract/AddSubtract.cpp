#include "FFGL.h"
#include "FFGLLib.h"

#include "AddSubtract.h"
#include "Vert.h"
#include "Frag.h"
#include "../../lib/ffgl/utilities/utilities.h"

//#define FFPARAM_SwitchTex   (0)
//#define FFPARAM_Float1      (1)
#define FFPARAM_lineNum     (0)
#define FFPARAM_rotateSpeed (1)
#define FFPARAM_lsration    (2)
#define FFPARAM_wsration    (3)
#define FFPARAM_offsetY     (4)

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo PluginInfo ( 
	AddSubtract::CreateInstance,		// Create method
	"PWL1_word_round",								// Plugin unique ID
	"P Word Round",					// Plugin name
	1,						   			// API major version number 													
	500,								// API minor version number
	1,									// Plugin major version number
	000,								// Plugin minor version number
	FF_EFFECT,							// Plugin type
	"P Word Round",			// Plugin description
	"by Pampa -- lohosoft.com"				// About
);


AddSubtract::AddSubtract()
:CFreeFrameGLPlugin(),
 m_initResources(1),
 m_inputTextureLocation(-1)
{
	// Input properties
	SetMinInputs(1);
	SetMaxInputs(1);

	// Parameters
    
//    SetParamInfo(FFPARAM_SwitchTex, "Switch Tex", FF_TYPE_BOOLEAN, false);
//
//    m_SwitchTex = false;
//
//    SetParamInfo(FFPARAM_Float1,"Float 1",FF_TYPE_STANDARD,0.0f);
//    m_Float1 = 0.0f;
    
    lineNum =  10.0f;// 0.0 ~ 200;
    rotateSpeed = 2.0f; // 1. ~ 10.
    lsration = 0.1f; // 0.0 ~ 1.0
    wsration = 0.1f; // 0.0 ~ 1.0
    offsetY = 2.0f; // 0.0 ~ 50;

    
    SetParamInfo(FFPARAM_lineNum,"Line Number",FF_TYPE_STANDARD,lineNum/200.0f);
    SetParamInfo(FFPARAM_rotateSpeed,"Rotate Speed",FF_TYPE_STANDARD,rotateSpeed/10.0f);
    SetParamInfo(FFPARAM_lsration,"Line Spacing",FF_TYPE_STANDARD,lsration);
    SetParamInfo(FFPARAM_wsration,"Word Spacing",FF_TYPE_STANDARD,wsration);
    SetParamInfo(FFPARAM_offsetY,"Offset",FF_TYPE_STANDARD,offsetY/50.0f);


}

AddSubtract::~AddSubtract()
{
	
}

FFResult AddSubtract::InitGL(const FFGLViewportStruct *vp)
{

	m_initResources = 0;


	//initialize gl shader
	m_shader.Compile(vertexShaderCode,fragmentShaderCode);

	//activate our shader
	m_shader.BindShader();

    //to assign values to parameters in the shader, we have to lookup
    //the "location" of each value.. then call one of the glUniform* methods
    //to assign a value
    m_inputTextureLocation = m_shader.FindUniform("inputTexture");
    m_TicksLocation = m_shader.FindUniform("ticks");
    m_WidthLocation = m_shader.FindUniform("width");
    m_HeightLocation = m_shader.FindUniform("height");
    m_SwitchTexLocation = m_shader.FindUniform("switchTex");
    m_Float1Location = m_shader.FindUniform("float1");
    
    
    
    lineNumLoc = m_shader.FindUniform("lineNum");
    rotateSpeedLoc = m_shader.FindUniform("rotateSpeed");
    lsrationLoc = m_shader.FindUniform("lsration");
    wsrationLoc = m_shader.FindUniform("wsration");
    offsetYLoc = m_shader.FindUniform("offsetY");
    
    
    
    
	//the 0 means that the 'inputTexture' in
	//the shader will use the texture bound to GL texture unit 0
	glUniform1i(m_inputTextureLocation, 0);
	
	m_shader.UnbindShader();

  return FF_SUCCESS;
}

FFResult AddSubtract::DeInitGL()
{
  m_shader.FreeGLResources();


  return FF_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Methods
////////////////////////////////////////////////////////////////////////////////////////////////////



FFResult AddSubtract::ProcessOpenGL(ProcessOpenGLStruct *pGL)
{
	if (pGL->numInputTextures<1)
		return FF_FAIL;

	if (pGL->inputTextures[0]==NULL)
		return FF_FAIL;

    ticks = getTicks();
    
	//activate our shader
	m_shader.BindShader();

	FFGLTextureStruct &Texture = *(pGL->inputTextures[0]);

	//get the max s,t that correspond to the 
	//width,height of the used portion of the allocated texture space
	FFGLTexCoords maxCoords = GetMaxGLTexCoords(Texture);

    //get the width of the viewport
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    
    
    
    // assign ticks in millisecond
    glUniform1f(m_TicksLocation,ticks);
    
    // assign width and height
    glUniform1f(m_WidthLocation, (float)viewport[2]);
    glUniform1f(m_HeightLocation, (float)viewport[3]);
    
    
    glUniform1f(m_Float1Location, m_Float1);
    
    if(m_SwitchTex){
        glUniform1f(m_SwitchTexLocation,1.0);
        
    }else{
        glUniform1f(m_SwitchTexLocation,0.0);
        
    }
    
    
    glUniform1f(lineNumLoc,lineNum);
    glUniform1f(rotateSpeedLoc,rotateSpeed);
    glUniform1f(lsrationLoc,lsration);
    glUniform1f(wsrationLoc,wsration);
    glUniform1f(offsetYLoc,offsetY);

    
    
    
    
	//activate texture unit 1 and bind the input texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture.Handle);
    
	//draw the quad that will be painted by the shader/textures
	//note that we are sending texture coordinates to texture unit 1..
	//the vertex shader and fragment shader refer to this when querying for
	//texture coordinates of the inputTexture
	glBegin(GL_QUADS);

	//lower left
	glMultiTexCoord2f(GL_TEXTURE0, 0,0);
	glVertex2f(-1,-1);

	//upper left
	glMultiTexCoord2f(GL_TEXTURE0, 0, maxCoords.t);
	glVertex2f(-1,1);

	//upper right
	glMultiTexCoord2f(GL_TEXTURE0, maxCoords.s, maxCoords.t);
	glVertex2f(1,1);

	//lower right
	glMultiTexCoord2f(GL_TEXTURE0, maxCoords.s, 0);
	glVertex2f(1,-1);
	glEnd();

	//unbind the input texture
	glBindTexture(GL_TEXTURE_2D,0);


	//unbind the shader
	m_shader.UnbindShader();

	return FF_SUCCESS;
}

float AddSubtract::GetFloatParameter(unsigned int dwIndex)
{
	float retValue = 0.0;

    switch (dwIndex)
    {
//        case FFPARAM_SwitchTex:
//            retValue = m_SwitchTex;
//            return retValue;
//        case FFPARAM_Float1:
//            retValue = m_Float1;
//            return retValue;
        case FFPARAM_lineNum:
            retValue = lineNum / 200.0f;
            return retValue;
        case FFPARAM_rotateSpeed:
            retValue = rotateSpeed / 10.0f;
            return retValue;
        case FFPARAM_lsration:
            retValue = lsration;
            return retValue;
        case FFPARAM_wsration:
            retValue = wsration;
            return retValue;
        case FFPARAM_offsetY:
            retValue = offsetY/50.0f;
            return retValue;
        
        default:
            return retValue;
    }
}

FFResult AddSubtract::SetFloatParameter(unsigned int dwIndex, float value)
{
	switch (dwIndex)
	{
//    case FFPARAM_Float1:
//        m_Float1 = value;
//        break;
//    case FFPARAM_SwitchTex:
//        m_SwitchTex = value > 0.5;
//        break;
    case FFPARAM_lineNum:
        lineNum = value * 200.0f;
        break;
    case FFPARAM_rotateSpeed:
        rotateSpeed = value * 10.0f;
        break;
    case FFPARAM_lsration:
        lsration = value;
        break;
    case FFPARAM_wsration:
        wsration = value;
        break;
    case FFPARAM_offsetY:
        offsetY = value * 50.0f;
        break;
	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}
