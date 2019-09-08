#include "FFGL.h"
#include "FFGLLib.h"

#include "AddSubtract.h"
#include "Vert.h"
#include "Frag.h"
#include "../../lib/ffgl/utilities/utilities.h"

#include <algorithm>


//#define FFPARAM_SwitchTex   (0)
//#define FFPARAM_Float1      (1)




#define FFPARAM_bWordRotate       (0)
#define FFPARAM_bWordTracking     (1)

#define FFPARAM_wordRotateSpeed     (2)
#define FFPARAM_wordLineNum       (3)
#define FFPARAM_wordLineSpacingRatio      (4)
#define FFPARAM_wordWordSpacingRatio      (5)
#define FFPARAM_wordOffset        (6)

#define FFPARAM_waveScale   (7)

#define FFPARAM_wordWordNum       (8)

#define FFPARAM_word_col_divid     (9)
#define FFPARAM_word_word_divid     (10)


#define FFPARAM_text_data     (11)

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo PluginInfo ( 
	AddSubtract::CreateInstance,		// Create method
	"PWW201908",								// Plugin unique ID
	"P Wave Word",					// Plugin name
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
    


    bWordRotate = true;
    bWordTracking = false;
    
    wordRotateSpeed = 2.0;
    wordLineNum = 20.;
    wordWordNum = 10.;
    wordLineSpacingRatio = 0.1;
    wordWordSpacingRatio = 0.1;
    wordOffset = 2.;
    
    waveScale = 0.0;


    

    SetParamInfo(FFPARAM_bWordRotate ,"word rotate",FF_TYPE_BOOLEAN,bWordRotate);
    SetParamInfo(FFPARAM_bWordTracking,"word tracking",FF_TYPE_BOOLEAN,bWordTracking);

    SetParamInfo(FFPARAM_wordRotateSpeed ,"word rotate speed",FF_TYPE_STANDARD,wordRotateSpeed / 10.0f);
    SetParamInfo(FFPARAM_wordLineNum ,"word line number",FF_TYPE_STANDARD,wordLineNum / 300.0f);
    SetParamInfo(FFPARAM_wordWordNum ,"word word number",FF_TYPE_STANDARD,wordWordNum / 300.0f);
    SetParamInfo(FFPARAM_wordLineSpacingRatio,"word line spacing",FF_TYPE_STANDARD,wordLineSpacingRatio);
    SetParamInfo(FFPARAM_wordWordSpacingRatio,"word word spacing",FF_TYPE_STANDARD,wordWordSpacingRatio);
    SetParamInfo(FFPARAM_wordOffset,"word offset",FF_TYPE_STANDARD,wordOffset / 20.0f);
    
    rawOscTextData = "hello";
    SetParamInfo(FFPARAM_text_data, "osc text data", FF_TYPE_TEXT, rawOscTextData.c_str());
    

    SetParamInfo(FFPARAM_word_col_divid, "column divid", FF_TYPE_TEXT, wordColDivid.c_str());

    SetParamInfo(FFPARAM_word_word_divid, "word divid", FF_TYPE_TEXT, wordWordDivid.c_str());
    

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
    
    bWordRotateLoc = m_shader.FindUniform("bWordRotate");
    bWordTrackingLoc = m_shader.FindUniform("bWordTracking");

    
    wordRotateSpeedLoc = m_shader.FindUniform("wordRotateSpeed");
    wordLineNumLoc = m_shader.FindUniform("wordLineNum");
    wordWordNumLoc = m_shader.FindUniform("wordWordNum");
    wordLineSpacingRatioLoc = m_shader.FindUniform("wordLineSpacingRatio");
    wordWordspacingRatioLoc = m_shader.FindUniform("wordWordSpacingRatio");
    wordOffsetLoc = m_shader.FindUniform("wordOffset");
    

    waveScaleLoc = m_shader.FindUniform("waveScale");
    
    
    trackingDataLoc = m_shader.FindUniform("trackingData");

    wordWordDividLoc = m_shader.FindUniform("wordWordDivid");
    wordColDividLoc = m_shader.FindUniform("wordColDivid");
    
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
    

    std::vector<float> oscDataInFloatVec = MyConvertStingToFloatVector(rawOscTextData);
    
    
//    
//    for (int i = 0; i < kTrackingDataSize; i++) {
//        //        data[i] = 0.003 * float(i);
//        float mark = oscDataInFloatVec[i];
//        
//        if (mark > 0.0) {
//            trackingData[i] += waveDelta;
//        }
//        else{
//            trackingData[i] -= waveDelta;
//        }
//        
//        
//        // clamp
//        if (trackingData[i] < 0.0) {
//            trackingData[i] = 0.0;
//        }
//        
//        if(trackingData[i] > waveMax){
//            trackingData[i] = waveMax;
//        }
//    }
    
    
    
    float colDividFloat = std::stod(wordColDivid);
    float wordDividFloat = std::stod(wordWordDivid);
    
    
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
    
    
    glUniform1f(wordRotateSpeedLoc, wordRotateSpeed);
    glUniform1f(wordLineNumLoc,wordLineNum);
    glUniform1f(wordWordNumLoc,wordWordNum);
    glUniform1f(wordLineSpacingRatioLoc,wordLineSpacingRatio);
    glUniform1f(wordWordspacingRatioLoc,wordWordSpacingRatio);
    glUniform1f(wordOffsetLoc,wordOffset);
    
    
    glUniform1fv(trackingDataLoc,kTrackingDataSize,trackingData);

    
    
    glUniform1f(waveScaleLoc, waveScale);

    glUniform1f(wordColDividLoc, colDividFloat);
    
    glUniform1f(wordWordDividLoc, wordDividFloat);
    

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

        case FFPARAM_bWordRotate :
            retValue = bWordRotate;
            return retValue;
        case FFPARAM_bWordTracking:
            retValue = bWordTracking;
            return retValue;
        

        case FFPARAM_wordRotateSpeed:
            retValue = wordRotateSpeed / 10.0;
            return retValue;
        case FFPARAM_wordLineNum :
            retValue = wordLineNum / 300.0;
            return retValue;
        case FFPARAM_wordWordNum :
            retValue = wordWordNum / 300.0;
            return retValue;
            
        case FFPARAM_wordLineSpacingRatio:
            retValue = wordLineSpacingRatio;
            return retValue;
        case FFPARAM_wordWordSpacingRatio:
            retValue = wordWordSpacingRatio;
            return retValue;
        case FFPARAM_wordOffset :
            retValue = wordOffset / 20.0;
            return retValue;
            
            
        case FFPARAM_waveScale:
            retValue = waveScale;
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
            
            

        case FFPARAM_bWordRotate:
            bWordRotate = value > 0.5;
            break;
        case FFPARAM_bWordTracking:
            bWordTracking = value > 0.5;
            break;
                
                
        case FFPARAM_wordRotateSpeed:
            wordRotateSpeed = value * 10.0;
            break;
        
        case FFPARAM_wordLineNum :
            wordLineNum = value * 300.0;
            break;
        case FFPARAM_wordWordNum :
            wordWordNum = value * 300.0;
            break;
        case FFPARAM_wordLineSpacingRatio:
            wordLineSpacingRatio = value;
            break;
        case FFPARAM_wordWordSpacingRatio:
            wordWordSpacingRatio = value;
            break;
        case FFPARAM_wordOffset:
            wordOffset = value * 20.0;
            break;
            
        
        case FFPARAM_waveScale:
            waveScale = value;
            break;
            



        default:
            return FF_FAIL;
	}

	return FF_SUCCESS;
}


char* AddSubtract::GetTextParameter(unsigned int dwIndex)
{
    
    char* retValue;
    switch (dwIndex) {
        case FFPARAM_text_data:
            retValue = const_cast<char*>(rawOscTextData.c_str());
            break;
            
        case FFPARAM_word_col_divid:
            retValue = const_cast<char*>(wordColDivid.c_str());
            break;
        case FFPARAM_word_word_divid:
            retValue = const_cast<char*>(wordWordDivid.c_str());
            break;
            
        default:
            return (char *)FF_FAIL;
    }
    return retValue;
}

FFResult AddSubtract::SetTextParameter(unsigned int dwIndex, const char *value){
    switch (dwIndex)
    {
            
        case FFPARAM_text_data:
            rawOscTextData = value;
            break;
            
            
        case FFPARAM_word_col_divid:
            wordColDivid = value;
            break;
            
        case FFPARAM_word_word_divid:
            wordWordDivid = value;
            break;
            
        default:
            return FF_FAIL;
    }
    
    return FF_SUCCESS;
}
