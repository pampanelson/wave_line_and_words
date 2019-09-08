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
uniform float wordWordNum;
uniform float wordLineSpacingRatio;
uniform float wordWordSpacingRatio;
uniform float wordOffset;

uniform float waveScale;

uniform float wordColDivid;
uniform float wordWordDivid;
uniform float trackingData[12]; // 12 size()




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

float wave1(float x){
    return x;
}
float wave2(float x,float amplitude,float freq){
    float res = 0.0;
    
    res = amplitude - pow(x*freq,2.0);
    return res;
}
// peak is highest point , narrow is wave wide
float wave3(float x,float peak,float narrow){
    float res = 0.0;
    res = peak - narrow*abs(x*x*x);
    return res;
}

vec3 word_wave(vec2 st,float rotateSpeed,float distort,float colNumber,float offsetY,float lsratio,float wsratio){
    
    float speed = rotateSpeed;// sign means rotate clock(+) or counterclock(-)
    float piFactor = 128.0;// bigger number will avoid jump on loop but no effect on rotate speed
    float m = mod(speed*iTime,piFactor * PI);
    
    // make distort
    
    if(distort > 0.0){
        st.y /= distort;// how ??????????????? divided a number less 1.0 will amplify
    }
    
    
    
    //float colNumber = 10.; //   input parameters ------------------
    st.y *= colNumber;
    
    
    //st1.y = max(4.0,st1.y);
    
    //float offsetY = 7.0; // pass parameters
    
    if(st.y < offsetY){
        return vec3(0.0);
    }
    st.x *= 1.*floor(st.y);
    
    if(bWordRotate > 0.5){
        
        if(mod(floor(st.y),2.0)>0.0){
            st.x += m;
        }else{
            st.x -= m;
        }
        
        
    }
    
    
    vec2 index = floor(st);
    vec2 polar = fract(st);
    
    vec3 col;
    // Time varying pixel color
    //col = vec3(polar.x,polar.y,0.0);
    
    vec2 localPolar = polar;
    
    
    // pass parameter
    localPolar.x *= 1.0/(1.0 - wsratio);
    localPolar.y *= 1.0/(1.0 - lsratio);
    
    
    
    
    
    if(localPolar.x > 1.0 || localPolar.y > 1.0 || localPolar.y < 0.0 || localPolar.x < 0.0){
        col *= 0.0;
    }
    
    else{
        
        
        // try to get part of texture but whole in a word square
        // ratio of part of texture is fixed, column line number is changing , need get module of fixed word column amount predefined
        // devided texture into 4 parts for example
        // -=--------------------------------------------------- =====================  IMPORTANT ==================
        // how many column and line a texture of words
        
        float yDivid = 100.0;
        float indexY = mod(index.y,yDivid);
        localPolar.y = indexY * 1./yDivid + localPolar.y / yDivid;
        
        float xDivid = 160.0;
        float indexX = mod(index.x,xDivid);
        localPolar.x = indexX * 1./xDivid + localPolar.x / xDivid;
        
        
        // convert back to original left upper 0,0, at end of all =====================  IMPORTANT ==================
        localPolar.y = 1. - localPolar.y;
        // maybe need flip x axis also 
        localPolar.x = 1. - localPolar.x;
        col = texture2D(inputTexture,localPolar).xyz;
        
    }
    
    
    return col;
    
}


float wave_distort(float use,vec2 st,float angle,float scale){
    // distort =========================================
    
    
    vec2 st2 = st;
    //st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
    
    float x = st2.x;
    x *= .2; // wave smooth factor
    // x -= fract(iTime*0.1);
    //x += 0.5;
    //x = -x;
    
    // =+++++++++++++ IMPORTANT ++++++++++++++++++++++++++++
    // not use distort or distort angle is 0.0 which is default value
    if(use < 0.5){
        
        x += 1.; // this value makes no distort ================ . TODO
    }
    else{
        x += -.0; // 0.0 is up direct , -0.5 is right direct , 0.5 is left direct
        x += 0.5 - angle;// angle is 0. ~ 1, 1 is right direct , 0 is left direct
        
    }
    
    // float x = uv.x;
    float y = 0.0;
    float a1 = -.2*sin(iTime*5.0);
    float f1 = 12.5;
    float y1 = wave2(x,a1,f1);
    float a2 = 0.0;//
    a2 = sin(iTime*10.)*0.1;
    float f2 = 8.0; // power ===============================
    float y2 = wave2(x+0.1,a2,f2);
    y = smax(y,y1,0.9);
    y = smax(y,y2,0.8);
    // y = smax(y,wave1(x*0.01),-0.9);
    float peak3 = 0.15;//
    float narrow3 = 4.0;//*sin(iTime*10.);
    float y3 = wave3(x+0.2,peak3,narrow3);
    
    y = smax(y,y3,0.5);// 0.1 is shaper wave , ============  TODO
    y = smax(y,0.2,scale);// scale is 0.01 ~ 0.9 , lower is bigger ========  TODO
    
    y *= 1.;// whole scale =======================
    y += sin(iTime) * 0.01; // shaking
    
    return y;
}



void main()
{
    
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y);
    
    // --------------------------
    
    
    vec2 uv = (fragCoord.xy - .5 * iResolution.xy)/iResolution.x; // uv -.5 ~ .5  , x axis is scale t0 1.
    vec2 uv1 = gl_FragCoord.xy / iResolution.xy;
    vec2 uv2 = fragCoord.xy / iResolution.xy;
    // prepare uv for st **********************************
    uv *= 2.0; // -1. ~ 1.
    uv.y += iResolution.y/iResolution.x;// origin point on (0.5 * x , 0.0)
    
    
    // uv.y -= wordOffset;// whole screen offset ======================
    // uv *= 0.5;// 0 ~ 1
    
    vec2 st = vec2(atan(uv.x,uv.y),length(uv));
    //st.x += PI;// 0 ~ 2PI on -y axis
    
    // distort =========================================
    
    // float angle;
    // angle = 0.0;
    // angle = clamp(0.0,1.0,angle);//angle is 0. ~ 1, 1 is right direct , 0 is left direct
    // angle = (sin(iTime) + 1.)*0.5;// pass parameter
    
    float scale = 1.- waveScale;
    scale = clamp(0.001,.9,scale);// save handle
//    float y = wave_distort(bWordTracking,st,trk1Angle,scale);
//    y += wave_distort(bWordTracking,st,trk2Angle,scale);
//    y += wave_distort(bWordTracking,st,trk3Angle,scale);
//
    
    vec3 col;
    // vec3 word_wave(vec2 st,float rotateSpeed,float distort,float colNZumber,float offsetY,float lsratio,float wsratio){
    
//    col = word_wave(st,wordRotateSpeed,y,wordLineNum,wordOffset,wordLineSpacingRatio,wordWordSpacingRatio);
    
    
    // debug distort wave =============
    // bool bWaveDistortDebug = false;
    // if(st.y < y && bWaveDistortDebug){
    //     col = vec3(1.0);
    // }
    
    // Output to screen
    
    //    col = vec3(sin(iTime));
    fragColor = vec4(col,1.0);
    
    
    // for test default st and changed st convertion
    //    uv2.y = 1. - uv2.y;
    //    fragColor = vec4(uv2.x,uv2.y,0.0,.5);
    
    //    fragColor += vec4(texture2D(inputTexture,uv2).xyz,0.5);
    
    
    
    // debug pass float from text into shader
//    if(gl_FragCoord.x < wordWordDivid && gl_FragCoord.y < wordColDivid){
//        fragColor = vec4(1.0);
//
//    }
//    
    
    gl_FragColor = fragColor;
}
                                                        );

#endif /* Frag_h */
