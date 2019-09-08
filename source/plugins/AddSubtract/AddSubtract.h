#pragma once

#include "FFGLShader.h"
#include "FFGLPluginSDK.h"
#include <string>



#include <iostream>
#include <limits>
#include <vector>
#include <sstream>

// split string combine with "," or other mark seperator as vector
std::vector<std::string> MySplitS1(std::string input){
    std::vector<std::string> result;
    if(input.size()>0){
        
        std::istringstream iss(input);
        for(std::string s; iss >> s; )
            result.push_back(s);
    }
    
    return result;
}



std::vector<float> MyConvertStingToFloatVector(std::string input){
    
    std::vector<float> vect;
    
    std::stringstream ss(input);
    
    float i;
    
    while (ss >> i)
    {
        vect.push_back(i);
        
        if (ss.peek() == ',')
            ss.ignore();
    }
    
    
    
    return vect;
}





class AddSubtract : public CFreeFrameGLPlugin
{
public:
	AddSubtract();
	~AddSubtract();

	///////////////////////////////////////////////////
	// FreeFrame plugin methods
	///////////////////////////////////////////////////
	
    FFResult SetTextParameter(unsigned int dwIndex, const char *value) override;
    char* GetTextParameter(unsigned int index) override;

    
	FFResult SetFloatParameter(unsigned int dwIndex, float value) override;		
	float GetFloatParameter(unsigned int index) override;					
	FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
	FFResult InitGL(const FFGLViewportStruct *vp) override;
	FFResult DeInitGL() override;

	///////////////////////////////////////////////////
	// Factory method
	///////////////////////////////////////////////////

	static FFResult __stdcall CreateInstance(CFreeFrameGLPlugin **ppOutInstance)
  {
	  *ppOutInstance = new AddSubtract();
	  if (*ppOutInstance != NULL)
		  return FF_SUCCESS;
	  return FF_FAIL;
  }


protected:	
	// Parameters
    
    int m_initResources;
//    bool  m_SwitchTex;
//    float m_Float1;
    

    bool bWordRotate;
    bool bWordTracking;
    

    float wordLineNum;
    float wordWordNum;
    float wordLineSpacingRatio;
    float wordWordSpacingRatio;
    float wordOffset;
    float wordRotateSpeed;
    
    float waveScale;
    
    // control wave shape and change rate
    float waveDelta;
    float waveMax;
    
    
    
    std::string rawOscTextData = "hello";
    std::string rawOscTextData1 = "hello";
    int kTrackingDataSize = 12;
    GLfloat trackingData[12];
    
    
    std::string wordColDivid = "0";
    std::string wordWordDivid = "0"; 
    
//case FFPARAM_wordRotateSpeed:
//    retValue = wordRotateSpeed / 10.0;
//    return retValue;
//case FFPARAM_wordLineNum :
//    retValue = wordLineNum / 300.0;
//    return retValue;
//case FFPARAM_wordLineSpacingRatio:
//    retValue = wordLineSpacingRatio;
//    return retValue;
//case FFPARAM_wordWordSpacingRatio:
//    retValue = wordWordSpacingRatio;
//    return retValue;
//case FFPARAM_wordOffset :
//    retValue = wordOffset / 20.0;
//    return retValue;
//    
    GLint bWordRotateLoc;
    GLint bWordTrackingLoc;
    

    
    GLint wordRotateSpeedLoc;
    GLint wordLineNumLoc;
    GLint wordWordNumLoc;
    GLint wordLineSpacingRatioLoc;
    GLint wordWordspacingRatioLoc;
    GLint wordOffsetLoc;
    
    GLint trackingDataLoc;


    GLint waveScaleLoc;
    
    GLint wordWordDividLoc;
    GLint wordColDividLoc;
    
    FFGLShader m_shader;
    GLint m_inputTextureLocation;
    GLint m_TicksLocation;
    
    GLint m_WidthLocation;
    GLint m_HeightLocation;
    
    
    GLint m_SwitchTexLocation;
    GLint m_Float1Location;
    
    double ticks;

};
