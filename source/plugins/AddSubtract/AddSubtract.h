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
    
    // =========== not use
    bool  m_SwitchTex;
    float m_Float1;

    
    // parameters ---------------------------------------
    float lineNum;// = 40.0;
    float lineWidth;// = 0.2;
    float offsetY;// = 0.1;
    float lineSaturation;// = 4.0;
    bool  bRipple;// = true;
    // ---------------------------------------------------
    

    GLint lineNumLoc;
    GLint lineWidthLoc;
    GLint offsetYLoc;
    GLint lineSaturationLoc;
    GLint bRippleLoc;
    
    FFGLShader m_shader;
    
    // default parameters =====================
    GLint m_inputTextureLocation;
    GLint m_TicksLocation;
    
    GLint m_WidthLocation;
    GLint m_HeightLocation;
    
    
    GLint m_SwitchTexLocation;
    GLint m_Float1Location;
    
    
    double ticks;

};
