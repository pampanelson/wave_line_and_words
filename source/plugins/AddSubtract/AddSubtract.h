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
    

    bool bWordRotate;
    bool bWordTracking;
    

    float wordLineNum;
    float wordLineSpacingRatio;
    float wordWordSpacingRatio;
    float wordOffset;
    float wordRotateSpeed;
    
    float trk1Angle;
    float trk1Power;
    

    GLint bWordRotateLoc;
    GLint bWordTrackingLoc;
    
    GLint wordRotateSpeedLoc;
    GLint wordLineNumLoc;
    GLint wordLineSpacingRatioLoc;
    GLint wordWordspacingRatioLoc;
    GLint wordOffsetLoc;
    
    
    GLint trk1AngleLoc;
    GLint trk1PowerLoc;
    
    
    FFGLShader m_shader;
    GLint m_inputTextureLocation;
    GLint m_TicksLocation;
    
    GLint m_WidthLocation;
    GLint m_HeightLocation;
    
    
    GLint m_SwitchTexLocation;
    GLint m_Float1Location;
    
    double ticks;

};
