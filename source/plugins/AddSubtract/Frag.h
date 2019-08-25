//
//  Frag.h
//  AddSubtract
//
//  Created by Pampa Nie on 2019/8/24.
//

#ifndef Frag_h
#define Frag_h

#include "../../lib/ffgl/utilities/utilities.h"

static const std::string fragmentShaderCode = STRINGIFY(
uniform sampler2D inputTexture;
uniform float ticks;
uniform float width;
uniform float height;
float iTime;
vec2 iResolution;
vec2 fragCoord;
// uniform float switchTex;

    
uniform float trk1Angle;
uniform float trk2Angle;
uniform float trk3Angle;
uniform float distortScale;

                                                        
                                                        
float PI = 3.1415926535;
float aPI = acos(-1.);


// Polynomial smooth min (for copying and pasting into your shaders)
float smin(float a, float b, float k) {
        float h = clamp(0.5 + 0.5*(a-b)/k, 0.0, 1.0);
    return mix(a, b, h) - k*h*(1.0-h);
}

float smax(float a,float b,float k){
    return smin(a,b,-k);
}


void main()
{
    
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y);
    
    // --------------------------
    vec3 col;
    
    

    
    
    fragColor = vec4(col,1.0);
    
    gl_FragColor = fragColor;
}
);

#endif /* Frag_h */
