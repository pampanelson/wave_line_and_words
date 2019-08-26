#pragma once

#include "FFGLShader.h"
#include "FFGLPluginSDK.h"
#include <string>

class AddSubtract : public CFreeFrameGLPlugin
{
public:
	AddSubtract();
	~AddSubtract();

	///////////////////////////////////////////////////
	// FreeFrame plugin methods
	///////////////////////////////////////////////////
	
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
    

    
    bool bLineRipple;
    bool bLineTracking;
    
    float bigDistort;
    float lineNum;
    float lineWidth;
    float lineOffset;
    float lineMiRippleSize;
    float lineMiRippleSpeed;
    float lineMaRippleSize;
    float lineMaRippleSpeed;
    
    float waveScale;
    
//    lineNum = 20;
//    lineWidth = 0.02;
//    lineOffset = 2.0;
//    lineMiRippleSize = 0.3;
//    lineMiRippleSpeed = 1.0;
//    lineMaRippleSize = 0.3;
//    lineMaRippleSpeed = 1.0;
//

//case FFPARAM_lineNum :
//    retValue = lineNum / 100,0;
//    return retValue;
//case FFPARAM_lineWidth:
//    retValue = lineWidth * 10.0;
//    return retValue;
//case FFPARAM_lineOffset:
//    retValue = lineOffset / 20.0;
//    return retValue;
//case FFPARAM_lineMiRippleSize:
//    retValue = lineMiRippleSize;
//    return retValue;
//case FFPARAM_lineMiRippleSpeed:
//    retValue = lineMiRippleSpeed / 10.;
//    return retValue;
//case FFPARAM_lineMaRippleSize:
//    retValue = lineMaRippleSize;
//    return retValue;
//case FFPARAM_lineMaRippleSpeed:
//    retValue = lineMaRippleSpeed / 10.;
//    return retValue;

    float trk1Angle;
    float trk1Power;
    
    float trk2Angle;
    float trk2Power;
    
    float trk3Angle;
    float trk3Power;
    
    
    GLint bLineRippleLoc;
    GLint bLineTrackingLoc;
    
    GLint bigDistortLoc;
    GLint lineNumLoc;
    GLint lineWidthLoc;
    GLint lineOffsetLoc;
    GLint lineMiRippleSizeLoc;
    GLint lineMiRippleSpeedLoc;
    GLint lineMaRippleSizeLoc;
    GLint lineMaRippleSpeedLoc;
    
    GLint waveScaleLoc;

    GLint trk1AngleLoc;
    GLint trk1PowerLoc;
    GLint trk2AngleLoc;
    GLint trk2PowerLoc;
    GLint trk3AngleLoc;
    GLint trk3PowerLoc;
    
    
    FFGLShader m_shader;
    GLint m_inputTextureLocation;
    GLint m_TicksLocation;
    
    GLint m_WidthLocation;
    GLint m_HeightLocation;
    
    
    GLint m_SwitchTexLocation;
    GLint m_Float1Location;
    
    double ticks;

};
