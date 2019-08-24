#include "FFGL.h"
#include "FFGLLib.h"

#include "AddSubtract.h"
#include "Vert.h"
#include "Frag.h"
#include "../../lib/ffgl/utilities/utilities.h"

#define FFPARAM_SwitchTex   (0)
#define FFPARAM_Float1      (1)
#define FFPARAM_Line_Number (2)
#define FFPARAM_Line_Width  (3)
#define FFPARAM_OffsetY     (4)
#define FFPARAM_Line_Saturation  (5)


////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo PluginInfo ( 
	AddSubtract::CreateInstance,		// Create method
	"PWL0_Ripple_Line",								// Plugin unique ID
	"Ripple Line",					// Plugin name
	1,						   			// API major version number 													
	500,								// API minor version number
	1,									// Plugin major version number
	000,								// Plugin minor version number
	FF_EFFECT,							// Plugin type
	"Ripple lines",			// Plugin description
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
    // Parameters
    SetParamInfo(FFPARAM_SwitchTex, "Switch Tex", FF_TYPE_BOOLEAN, false);
    
    m_SwitchTex = false;
    
    SetParamInfo(FFPARAM_Float1,"Float 1",FF_TYPE_STANDARD,0.0f);
    m_Float1 = 0.0f;

    
    lineNum = 40.0f;
    SetParamInfo(FFPARAM_Line_Number,"Line Number",FF_TYPE_STANDARD,lineNum/300.0f);
    
    lineWidth = 0.2f;
    SetParamInfo(FFPARAM_Line_Width,"Line Width",FF_TYPE_STANDARD,lineWidth);
    
    offsetY = 0.1f;
    SetParamInfo(FFPARAM_OffsetY,"Offset Y",FF_TYPE_STANDARD,offsetY);
    
    lineSaturation = 4.0f;
    SetParamInfo(FFPARAM_Line_Saturation,"Line Saturation",FF_TYPE_STANDARD,lineSaturation/10.0f);
    

    
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
    lineWidthLoc = m_shader.FindUniform("lineWidth");
    offsetYLoc = m_shader.FindUniform("offsetY");
    lineSaturationLoc = m_shader.FindUniform("lineSaturation");
    
    
    
    
    
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
    glUniform1f(lineWidthLoc,lineWidth);
    glUniform1f(offsetYLoc,offsetY);
    glUniform1f(lineSaturationLoc,lineSaturation);
    
    
    
    
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
        case FFPARAM_SwitchTex:
            retValue = m_SwitchTex;
            return retValue;
        case FFPARAM_Float1:
            retValue = m_Float1;
            return retValue;
            
        case FFPARAM_Line_Number:
            retValue = lineNum / 300.0;
            return retValue;
        case FFPARAM_Line_Width:
            retValue = lineWidth;
            return retValue;
        case FFPARAM_OffsetY:
            retValue = offsetY;
            return retValue;
        case FFPARAM_Line_Saturation:
            retValue = lineSaturation / 10.0;
            return retValue;
            

        default:
            return retValue;
    }
}

FFResult AddSubtract::SetFloatParameter(unsigned int dwIndex, float value)
{
	switch (dwIndex)
	{
    case FFPARAM_Float1:
        m_Float1 = value;
        break;
    case FFPARAM_SwitchTex:
        m_SwitchTex = value > 0.5;
        break;
    case FFPARAM_Line_Number:
        lineNum = value * 300.0;
        break;
    case FFPARAM_Line_Width:
       lineWidth = value;
        break;
    case FFPARAM_OffsetY:
       offsetY = value;
        break;
    case FFPARAM_Line_Saturation:
       lineSaturation = value*10.0;
        break;

	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}
