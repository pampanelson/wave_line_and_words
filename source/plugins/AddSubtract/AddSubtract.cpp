#include "FFGL.h"
#include "FFGLLib.h"

#include "AddSubtract.h"
#include "Vert.h"
#include "Frag.h"
#include "../../lib/ffgl/utilities/utilities.h"

//#define FFPARAM_SwitchTex   (0)
//#define FFPARAM_Float1      (1)


#define FFPARAM_bLineRipple       (0)
#define FFPARAM_bLineTracking     (1)

#define FFPARAM_lineNum       (2)
#define FFPARAM_lineWidth     (3)
#define FFPARAM_lineOffset        (4)
#define FFPARAM_lineMiRippleSize      (5)
#define FFPARAM_lineMiRippleSpeed     (6)
#define FFPARAM_lineMaRippleSize      (7)
#define FFPARAM_lineMaRippleSpeed     (8)

#define FFPARAM_trk1Angle     (9)
#define FFPARAM_trk1Power     (10)

#define FFPARAM_trk2Angle     (11)
#define FFPARAM_trk2Power     (12)

#define FFPARAM_trk3Angle     (13)
#define FFPARAM_trk3Power     (14)

#define FFPARAM_bigDistort    (15)






////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo PluginInfo ( 
	AddSubtract::CreateInstance,		// Create method
	"PWL201908",								// Plugin unique ID
	"P Wave Line",					// Plugin name
	1,						   			// API major version number 													
	500,								// API minor version number
	1,									// Plugin major version number
	000,								// Plugin minor version number
	FF_EFFECT,							// Plugin type
	"P Wave Line",			// Plugin description
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
//    SetParamInfo(FFPARAM_SwitchTex, "Switch Tex", FF_TYPE_BOOLEAN, false);
//
//    m_SwitchTex = false;
//
//    SetParamInfo(FFPARAM_Float1,"Float 1",FF_TYPE_STANDARD,0.0f);
//    m_Float1 = 0.0f;

    
    // default value;
    
    bLineRipple = true;
    bLineTracking = false;
    

    
    lineNum = 20;
    lineWidth = 0.02;
    lineOffset = 2.0;
    lineMiRippleSize = 0.3;
    lineMiRippleSpeed = 1.0;
    lineMaRippleSize = 0.3;
    lineMaRippleSpeed = 1.0;
    bigDistort = 0.2;


    SetParamInfo(FFPARAM_bLineRipple ,"line ripple",FF_TYPE_BOOLEAN,bLineRipple);
    SetParamInfo(FFPARAM_bLineTracking,"line tracking",FF_TYPE_BOOLEAN,bLineTracking);
    
    SetParamInfo(FFPARAM_lineNum ,"line number",FF_TYPE_STANDARD,lineNum/100.0f);
    SetParamInfo(FFPARAM_lineWidth,"line width",FF_TYPE_STANDARD,lineWidth * 10.0f);
    SetParamInfo(FFPARAM_lineOffset,"line offset",FF_TYPE_STANDARD,lineOffset / 50.0f);
    
    SetParamInfo(FFPARAM_lineMiRippleSize,"line mi size",FF_TYPE_STANDARD,lineMiRippleSize);
    SetParamInfo(FFPARAM_lineMiRippleSpeed,"line mi speed",FF_TYPE_STANDARD,lineMiRippleSpeed / 10.0f);
    SetParamInfo(FFPARAM_lineMaRippleSize,"line ma size",FF_TYPE_STANDARD,lineMaRippleSize);
    SetParamInfo(FFPARAM_lineMaRippleSpeed,"line ma speed",FF_TYPE_STANDARD,lineMaRippleSpeed / 10.0f);
    

    SetParamInfo(FFPARAM_bigDistort,"big distort",FF_TYPE_STANDARD,bigDistort);

    
    
    
    SetParamInfo(FFPARAM_trk1Angle,"trk 1 angle",FF_TYPE_STANDARD,trk1Angle);
    SetParamInfo(FFPARAM_trk1Power,"trk 1 power",FF_TYPE_STANDARD,trk1Power);
    
    SetParamInfo(FFPARAM_trk2Angle,"trk 2 angle",FF_TYPE_STANDARD,trk2Angle);
    SetParamInfo(FFPARAM_trk2Power,"trk 2 power",FF_TYPE_STANDARD,trk2Power);
    
    SetParamInfo(FFPARAM_trk3Angle,"trk 3 angle",FF_TYPE_STANDARD,trk3Angle);
    SetParamInfo(FFPARAM_trk3Power,"trk 3 power",FF_TYPE_STANDARD,trk3Power);
    
    
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
//    m_SwitchTexLocation = m_shader.FindUniform("switchTex");
//    m_Float1Location = m_shader.FindUniform("float1");
    
    
    
    bLineRippleLoc = m_shader.FindUniform("bLineRipple");
    bLineTrackingLoc = m_shader.FindUniform("bLineTracking");

    


    
    lineNumLoc = m_shader.FindUniform("lineNum");
    lineWidthLoc = m_shader.FindUniform("lineWidth");
    lineOffsetLoc = m_shader.FindUniform("lineOffset");
    lineMiRippleSizeLoc = m_shader.FindUniform("lineMiRippleSize");
    lineMiRippleSpeedLoc = m_shader.FindUniform("lineMiRippleSpeed");
    lineMaRippleSizeLoc = m_shader.FindUniform("lineMaRippleSize");
    lineMaRippleSpeedLoc = m_shader.FindUniform("lineMaRippleSpeed");
    
    bigDistortLoc = m_shader.FindUniform("bigDistort");
    
    trk1AngleLoc = m_shader.FindUniform("trk1Angle");
    trk1PowerLoc = m_shader.FindUniform("trk1Power");
    
    
    trk2AngleLoc = m_shader.FindUniform("trk2Angle");
    trk2PowerLoc = m_shader.FindUniform("trk2Power");
    
    
    trk3AngleLoc = m_shader.FindUniform("trk3Angle");
    trk3PowerLoc = m_shader.FindUniform("trk3Power");
    
    
    
    
    
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
    
    
//    glUniform1f(m_Float1Location, m_Float1);
    
    
    glUniform1f(lineNumLoc,lineNum);
    glUniform1f(lineWidthLoc,lineWidth);
    glUniform1f(lineOffsetLoc,lineOffset);
    glUniform1f(lineMiRippleSizeLoc,lineMiRippleSize);
    glUniform1f(lineMiRippleSpeedLoc,lineMiRippleSpeed);
    glUniform1f(lineMaRippleSizeLoc,lineMaRippleSize);
    glUniform1f(lineMaRippleSpeedLoc,lineMaRippleSpeed);
    
    glUniform1f(bigDistortLoc, bigDistort);
    
    
    glUniform1f(trk1AngleLoc,trk1Angle);
    glUniform1f(trk1PowerLoc,trk1Power);

    
    glUniform1f(trk2AngleLoc,trk2Angle);
    glUniform1f(trk2PowerLoc,trk2Power);
    
    
    glUniform1f(trk3AngleLoc,trk3Angle);
    glUniform1f(trk3PowerLoc,trk3Power);
    
    
    

    

    if(bLineRipple){
        glUniform1f(bLineRippleLoc,1.0);
        
    }else{
        glUniform1f(bLineRippleLoc,0.0);
        
    }
    
    
    

    if(bLineTracking){
        glUniform1f(bLineTrackingLoc,1.0);
        
    }else{
        glUniform1f(bLineTrackingLoc,0.0);
        
    }
    

    
    
    
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

        case FFPARAM_bLineRipple :
            retValue = bLineRipple;
            return retValue;
        case FFPARAM_bLineTracking:
            retValue = bLineTracking;
            return retValue;
            

        case FFPARAM_lineNum :
            retValue = lineNum / 100,0;
            return retValue;
        case FFPARAM_lineWidth:
            retValue = lineWidth * 10.0;
            return retValue;
        case FFPARAM_lineOffset:
            retValue = lineOffset / 50.0;
            return retValue;
        case FFPARAM_lineMiRippleSize:
            retValue = lineMiRippleSize;
            return retValue;
        case FFPARAM_lineMiRippleSpeed:
            retValue = lineMiRippleSpeed / 10.;
            return retValue;
        case FFPARAM_lineMaRippleSize:
            retValue = lineMaRippleSize;
            return retValue;
        case FFPARAM_lineMaRippleSpeed:
            retValue = lineMaRippleSpeed / 10.;
            return retValue;

        case FFPARAM_bigDistort:
            retValue = bigDistort;
            return retValue;
            
        case FFPARAM_trk1Angle:
            retValue = trk1Angle;
            return retValue;
        case FFPARAM_trk1Power:
            retValue = trk1Power;
            return retValue;
            
            
        case FFPARAM_trk2Angle:
            retValue = trk2Angle;
            return retValue;
        case FFPARAM_trk2Power:
            retValue = trk2Power;
            return retValue;
        
            
        case FFPARAM_trk3Angle:
            retValue = trk3Angle;
            return retValue;
        case FFPARAM_trk3Power:
            retValue = trk3Power;
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
            
            
        case FFPARAM_bLineRipple:
            bLineRipple = value > 0.5;
            break;
        case FFPARAM_bLineTracking:
            bLineTracking = value > 0.5;
            break;
            


        case FFPARAM_lineNum:
            lineNum = value * 100.0;
            break;
        case FFPARAM_lineWidth:
            lineWidth = value / 10.;
            break;
        case FFPARAM_lineOffset:
            lineOffset = value * 50.0;
            break;
        case FFPARAM_lineMiRippleSize:
            lineMiRippleSize = value;
            break;
        case FFPARAM_lineMiRippleSpeed:
            lineMiRippleSpeed = value * 10.;
            break;
        case FFPARAM_lineMaRippleSize:
            lineMaRippleSize = value;
            break;
        case FFPARAM_lineMaRippleSpeed:
            lineMaRippleSpeed = value * 10.;
            break;
            
        case FFPARAM_bigDistort:
            bigDistort = value;
            break;
            
        case FFPARAM_trk1Angle:
            trk1Angle = value;
            break;
        case FFPARAM_trk1Power:
            trk1Power = value;
            break;
            
        case FFPARAM_trk2Angle:
            trk2Angle = value;
            break;
        case FFPARAM_trk2Power:
            trk2Power = value;
            break;
            
            
        case FFPARAM_trk3Angle:
            trk3Angle = value;
            break;
        case FFPARAM_trk3Power:
            trk3Power = value;
            break;
            
            


        default:
            return FF_FAIL;
	}

	return FF_SUCCESS;
}
