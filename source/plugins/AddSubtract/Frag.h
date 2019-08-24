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
uniform float switchTex;

                                                        
// parameters ---------------------------------------
uniform float lineNum;//
//float lineGap = 0.03;
uniform float lineWidth; //= 0.2;
uniform float offsetY; //=0.1;
uniform float lineSaturation;// = 4.0;
//uniform bool  bRipple = true;
// ---------------------------------------------------

float smtLine(float lineWidth,float f,float lineSaturation){
    float res;
    res = smoothstep(lineWidth,0.0,f);
    res *= smoothstep(0.0,lineWidth,f)*lineSaturation;
    return res;
}

void main()
{
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    float mPI = 3.1415926535;

    vec3 col;
    
    vec2 uv = (fragCoord - .5 * iResolution.xy)/iResolution.y; // uv -.5 ~ .5
    uv.y += 0.5;
    //vec2 uv = fragCoord.xy/iResolution.xy;
    vec2 st = vec2(atan(uv.x,uv.y),length(uv));
    st.x = st.x/(mPI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
    
    // ===================================================================== ripples
    vec2 st1 = st;
    st1.y += .06;
    //float wave = -(sin(st.y*0.05*iTime) + 1.0)*0.5;
    
    
    float index = floor(st1.y * lineNum);
    
    float f = fract(st1.y * lineNum);
    
    // ripples -----------------------------
    
//    if(bRipple){
        float wave = sin(0.02*iTime*index*4.0*mPI);
        //f += 0.06*clamp(0.2,0.8,wave);
        //wave = max(0.1,wave);
        f += 0.05 * wave;
//    }
    //------------------------------------------
    float line = smtLine(lineWidth,f,lineSaturation);
    col = vec3(line);
    
    if(st.y <= offsetY){
        col *= 0.0;
    }

    
//    col += 1.0;
    fragColor = vec4(col,1.0);
    gl_FragColor = fragColor;
}
);



//    // name convert ---------------
//    vec4 fragColor;
//    iTime = ticks/1000.0;
//    iResolution = vec2(width,height);
//    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
//
//    // --------------------------
//    vec2 p = fragCoord.xy / iResolution.xy;
//
//    vec2 uv = p*vec2(iResolution.x/iResolution.y,1.0);
//
//    // --------------------------
//
//
//    vec3 col;
//
//    col += sin(iTime);
//    // Output to screen
//    fragColor = vec4(col,1.0);
//



#endif /* Frag_h */
