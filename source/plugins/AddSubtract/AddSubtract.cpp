#include "FFGL.h"
#include "FFGLLib.h"

#include "AddSubtract.h"
#include "Vert.h"
#include "Frag.h"
#include "../../lib/ffgl/utilities/utilities.h"

//#define FFPARAM_SwitchTex   (0)
//#define FFPARAM_Float1      (1)

#define FFPARAM_bUseLine      (0)
#define FFPARAM_bUseWord      (1)

#define FFPARAM_bLineRipple       (2)
#define FFPARAM_bLineTracking     (3)

#define FFPARAM_bWordRotate       (4)
#define FFPARAM_bWordTracking     (5)

#define FFPARAM_lineNum       (6)
#define FFPARAM_lineWidth     (7)
#define FFPARAM_lineOffset        (8)
#define FFPARAM_lineMiRippleSize      (9)
#define FFPARAM_lineMiRippleSpeed     (10)
#define FFPARAM_lineMaRippleSize      (11)
#define FFPARAM_lineMaRippleSpeed     (12)

#define FFPARAM_wordLineNum       (13)
#define FFPARAM_wordLineSpacingRatio      (14)
#define FFPARAM_wordWordSpacingRatio      (15)
#define FFPARAM_wordOffset        (16)


#define FFPARAM_trk1Angle     (17)
#define FFPARAM_trk1Power     (18)



////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo PluginInfo ( 
	AddSubtract::CreateInstance,		// Create method
	"Pwlw20190824",								// Plugin unique ID
	"P Wave Line Word",					// Plugin name
	1,						   			// API major version number 													
	500,								// API minor version number
	1,									// Plugin major version number
	000,								// Plugin minor version number
	FF_EFFECT,							// Plugin type
	"P Wave Line Word",			// Plugin description
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
    
    bUseLine = true;
    bUseWord = !bUseLine;
    
    bLineRipple = true;
    bLineTracking = false;
    
    bWordRotate = true;
    bWordTracking = false;
    
    lineNum = 20;
    lineWidth = 0.02;
    lineOffset = 2.0;
    lineMiRippleSize = 0.3;
    lineMiRippleSpeed = 1.0;
    lineMaRippleSize = 0.3;
    lineMaRippleSpeed = 1.0;
    
    wordLineNum = 20.;
    wordLineSpacingRatio = 0.1;
    wordWordSpacingRatio = 0.1;
    wordOffset = 2.;
    
    

    
    
    SetParamInfo(FFPARAM_bUseLine,"line",FF_TYPE_BOOLEAN,bUseLine);
    SetParamInfo(FFPARAM_bUseWord,"word",FF_TYPE_BOOLEAN,bUseWord);
    
    SetParamInfo(FFPARAM_bLineRipple ,"line ripple",FF_TYPE_BOOLEAN,bLineRipple);
    SetParamInfo(FFPARAM_bLineTracking,"line tracking",FF_TYPE_BOOLEAN,bLineTracking);
    
    SetParamInfo(FFPARAM_bWordRotate ,"word rotate",FF_TYPE_BOOLEAN,bWordRotate);
    SetParamInfo(FFPARAM_bWordTracking,"word tracking",FF_TYPE_BOOLEAN,bWordTracking);
    
    SetParamInfo(FFPARAM_lineNum ,"line number",FF_TYPE_STANDARD,lineNum);
    SetParamInfo(FFPARAM_lineWidth,"line width",FF_TYPE_STANDARD,lineWidth);
    SetParamInfo(FFPARAM_lineOffset,"line offset",FF_TYPE_STANDARD,lineOffset);
    
    SetParamInfo(FFPARAM_lineMiRippleSize,"line mi size",FF_TYPE_STANDARD,lineMiRippleSize);
    SetParamInfo(FFPARAM_lineMiRippleSpeed,"line mi speed",FF_TYPE_STANDARD,lineMiRippleSpeed);
    SetParamInfo(FFPARAM_lineMaRippleSize,"line ma size",FF_TYPE_STANDARD,lineMaRippleSize);
    SetParamInfo(FFPARAM_lineMaRippleSpeed,"line ma speed",FF_TYPE_STANDARD,lineMaRippleSpeed);
    
    SetParamInfo(FFPARAM_wordLineNum ,"word line number",FF_TYPE_STANDARD,wordLineNum);
    SetParamInfo(FFPARAM_wordLineSpacingRatio,"word line spacing",FF_TYPE_STANDARD,wordLineSpacingRatio);
    SetParamInfo(FFPARAM_wordWordSpacingRatio,"word word spacing",FF_TYPE_STANDARD,wordWordSpacingRatio);
    SetParamInfo(FFPARAM_wordOffset,"word offset",FF_TYPE_STANDARD,wordOffset);
    
    
    SetParamInfo(FFPARAM_trk1Angle,"trk 1 angle",FF_TYPE_STANDARD,trk1Angle);
    SetParamInfo(FFPARAM_trk1Power,"trk 1 power",FF_TYPE_STANDARD,trk1Power);
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
    
    bUseLineLoc = m_shader.FindUniform("bUseLine");
    bUseWordLoc = m_shader.FindUniform("bUseWord");
    
    bUseLineLoc = m_shader.FindUniform("bUseLine");
    bUseWordLoc = m_shader.FindUniform("bUseWord");
    
    bLineRippleLoc = m_shader.FindUniform("bLineRipple");
    bLineTrackingLoc = m_shader.FindUniform("bLineTracking");

    
    bWordRotateLoc = m_shader.FindUniform("bWordRotate");
    bWordTrackingLoc = m_shader.FindUniform("bWordTracking");

    
    lineNumLoc = m_shader.FindUniform("lineNum");
    lineWidthLoc = m_shader.FindUniform("lineWidth");
    lineOffsetLoc = m_shader.FindUniform("lineOffset");
    lineMiRippleSizeLoc = m_shader.FindUniform("lineMiRippleSize");
    lineMiRippleSpeedLoc = m_shader.FindUniform("lineMiRippleSpeed");
    lineMaRippleSizeLoc = m_shader.FindUniform("lineMaRippleSize");
    lineMaRippleSpeedLoc = m_shader.FindUniform("lineMaRippleSpeed");
    
    wordLineNumLoc = m_shader.FindUniform("wordLineNum");
    wordLineSpacingRatioLoc = m_shader.FindUniform("wordLineSpacingRatio");
    wordWordspacingRatioLoc = m_shader.FindUniform("wordWordSpacingRatio");
    wordOffsetLoc = m_shader.FindUniform("wordOffset");
    
    
    trk1AngleLoc = m_shader.FindUniform("trk1Angle");
    trk1PowerLoc = m_shader.FindUniform("trk1Power");
    
    
    
    
    
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
    
    // pick one effect at a time
    bUseWord = !bUseLine;
    
    
    
    
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
    
    glUniform1f(wordLineNumLoc,wordLineNum);
    glUniform1f(wordLineSpacingRatioLoc,wordLineSpacingRatio);
    glUniform1f(wordWordspacingRatioLoc,wordWordSpacingRatio);
    glUniform1f(wordOffsetLoc,wordOffset);
    
    
    glUniform1f(trk1AngleLoc,trk1Angle);
    glUniform1f(trk1PowerLoc,trk1Power);

    
    
    if(bUseLine){
        glUniform1f(bUseLineLoc,1.0);
        
    }else{
        glUniform1f(bUseLineLoc,0.0);
        
    }
    
    
    
    
    if(bUseWord){
        glUniform1f(bUseWordLoc,1.0);
        
    }else{
        glUniform1f(bUseWordLoc,0.0);
        
    }
    
    
    

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
    
    
    

    if(bWordRotate){
        glUniform1f(bWordRotateLoc,1.0);
        
    }else{
        glUniform1f(bWordRotateLoc,0.0);
        
    }
    
    
    if(bWordTracking){
        glUniform1f(bWordTrackingLoc,1.0);
        
    }else{
        glUniform1f(bWordTrackingLoc,0.0);
        
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
        case FFPARAM_bUseLine:
            retValue = bUseLine;
            return retValue;
        case FFPARAM_bUseWord:
            bUseWord = !bUseLine;
            retValue = bUseWord;
            return retValue;
            
        case FFPARAM_bLineRipple :
            retValue = bLineRipple;
            return retValue;
        case FFPARAM_bLineTracking:
            retValue = bLineTracking;
            return retValue;
            
        case FFPARAM_bWordRotate :
            retValue = bWordRotate;
            return retValue;
        case FFPARAM_bWordTracking:
            retValue = bWordTracking;
            return retValue;
            
        case FFPARAM_lineNum :
            retValue = lineNum;
            return retValue;
        case FFPARAM_lineWidth:
            retValue = lineWidth;
            return retValue;
        case FFPARAM_lineOffset  :
            retValue = lineOffset;
            return retValue;
        case FFPARAM_lineMiRippleSize:
            retValue = lineMiRippleSize;
            return retValue;
        case FFPARAM_lineMiRippleSpeed:
            retValue = lineMiRippleSpeed;
            return retValue;
        case FFPARAM_lineMaRippleSize:
            retValue = lineMaRippleSize;
            return retValue;
        case FFPARAM_lineMaRippleSpeed:
            retValue = lineMaRippleSpeed;
            return retValue;
            
        case FFPARAM_wordLineNum :
            retValue = wordLineNum;
            return retValue;
        case FFPARAM_wordLineSpacingRatio:
            retValue = wordLineSpacingRatio;
            return retValue;
        case FFPARAM_wordWordSpacingRatio:
            retValue = wordWordSpacingRatio;
            return retValue;
        case FFPARAM_wordOffset :
            retValue = wordOffset;
            return retValue;
            
            
        case FFPARAM_trk1Angle:
            retValue = trk1Angle;
            return retValue;
        case FFPARAM_trk1Power:
            retValue = trk1Power;
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
            
            
    case FFPARAM_bUseLine:
        bUseLine = value > 0.5;
        break;
    case FFPARAM_bUseWord:
        bUseWord = value > 0.5;
        break;
        
    case FFPARAM_bLineRipple:
        bLineRipple = value > 0.5;
        break;
    case FFPARAM_bLineTracking:
        bLineTracking = value > 0.5;
        break;
        
    case FFPARAM_bWordRotate:
        bWordRotate = value > 0.5;
        break;
    case FFPARAM_bWordTracking:
        bWordTracking = value > 0.5;
        break;
        
        

    case FFPARAM_lineNum:
        lineNum = value;
        break;
    case FFPARAM_lineWidth:
        lineWidth = value;
        break;
    case FFPARAM_lineOffset:
        lineOffset = value;
        break;
    case FFPARAM_lineMiRippleSize:
        lineMiRippleSize = value;
        break;
    case FFPARAM_lineMiRippleSpeed:
        lineMiRippleSpeed = value;
        break;
    case FFPARAM_lineMaRippleSize:
        lineMaRippleSize = value;
        break;
    case FFPARAM_lineMaRippleSpeed:
        lineMaRippleSpeed = value;
        break;
        
    case FFPARAM_wordLineNum :
        wordLineNum = value;
        break;
    case FFPARAM_wordLineSpacingRatio:
        wordLineSpacingRatio = value;
        break;
    case FFPARAM_wordWordSpacingRatio:
        wordWordSpacingRatio = value;
        break;
    case FFPARAM_wordOffset:
        wordOffset = value;
        break;
        
        
    case FFPARAM_trk1Angle:
        trk1Angle = value;
        break;
    case FFPARAM_trk1Power:
        trk1Power = value;
        break;
        
            

	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}
