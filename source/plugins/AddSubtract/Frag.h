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


uniform float bWordRotate; // false 0.0 ; true 1.0
uniform float bWordTracking;

uniform float wordRotateSpeed;
uniform float wordLineNum;
uniform float wordLineSpacingRatio;
uniform float wordWordSpacingRatio;
uniform float wordOffset;

uniform float globalWaveAmp;// smaller means bigger wave peak to the lower wave bottom;

uniform float wordColDivid;
uniform float wordWordDivid;
uniform float trackingData[8]; // 12 size()
uniform float frame;
uniform float waveFFTFactor;
                                                        
float PI = 3.1415926535;
float aPI = acos(-1.);



void main()
{
    
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y);
    
    // --------------------------
    
    
    vec2 uv = (fragCoord.xy - .5 * iResolution.xy)/iResolution.x; // uv -.5 ~ .5  , x axis is scale t0 1.
    // prepare uv for st **********************************
    uv *= 2.0; // -1. ~ 1.
    uv.y += iResolution.y/iResolution.x;// origin point on (0.5 * x , 0.0)
    
    // uv.y -= wordOffset;// whole screen offset ======================
    // uv *= 0.5;// 0 ~ 1
    

    vec2 uv3 = uv;
    
    vec2 st = vec2(atan(uv3.x,uv3.y),length(uv3));
    st.x += PI;// 0 ~ 2PI on -y axis
    st.x /= PI;// 0~2 from -y axis
    st.x -= 0.5; // 0~1 from -x axis

    // coordination for read texture data;
    float x = -length(uv)*cos(st.x*PI); // -1 ~ 1
    float sty = fragCoord.y / iResolution.y;// 0 ~ 1
    float stx = fragCoord.x / iResolution.x;// 0~1
    
    
    vec3 col;

    vec2 uv1 = fragCoord.xy/iResolution.xy;
    uv1.y *= iResolution.y/iResolution.x;
    
    float angle = 0.1;
    float r = 0.3;
    vec2 center = vec2(0.5,0.);
    
    float x3 = center.x - cos(angle*PI)*r;
    float y3 = center.y + sin(angle*PI)*r;

    if(length(uv1-vec2(x3,y3))< 0.01){
        col += 1.;
    }
    
    if(length(uv1-center)< 0.03){
        col.r += 1.;
    }
    



    fragColor = vec4(col,1.0);
    
    

    
    gl_FragColor = fragColor;
}
);

#endif /* Frag_h */
