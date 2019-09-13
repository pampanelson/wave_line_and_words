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
                                                        

int kTrackingDataSize = 8;
float kTrackingDataSizeF = 8.0;
float PI = 3.1415926535;
float aPI = acos(-1.);

float rand(float n){return fract(sin(n) * 43758.5453123);}

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
    float piFactor = 496.0;// bigger number will avoid jump on loop but no effect on rotate speed
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
        
        float yDivid = wordColDivid;// passed uniform float
        float indexY = mod(index.y,yDivid);
        localPolar.y = indexY * 1./yDivid + localPolar.y / yDivid;
        
        float xDivid = wordWordDivid;// passed uniform float
        float indexX = mod(index.x,xDivid);
        localPolar.x = indexX * 1./xDivid + localPolar.x / xDivid;
        
        
        // convert back to original left upper 0,0, at end of all =====================  IMPORTANT ==================
        localPolar.y = 1. - localPolar.y;
        // maybe need flip x axis also 
//        localPolar.x = 1. - localPolar.x;
        col = texture2D(inputTexture,localPolar).bbb;
        
    }
    
    
    return col;
    
}


float wave_distort11(bool use,vec2 st,float angle,float factor){
    // distort =========================================
    
    
    vec2 st2 = st;
    //st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0
    
    float x = st2.x;
    x *= factor; // wave smooth factor
    // x -= fract(iTime*0.1);
    //x += 0.5;
    //x = -x;
    
    // =+++++++++++++ IMPORTANT ++++++++++++++++++++++++++++
    if(!use){
        
        x += 1.; // this value makes no distort ================ . TODO
    }
    else{
        x += -.0; // 0.0 is up direct , -0.5 is right direct , 0.5 is left direct
        x += 0.5 - angle;// angle is 0. ~ 1, 1 is right direct , 0 is left direct
        
    }
    
    // float x = uv.x;
    float y = 0.0;
    float a1 = -.2*sin(iTime*5.0);
    
    float f1 = 3.5 + sin(iTime) ;
    float y1 = wave2(x,a1,f1);
    float a2 = 0.0;//
    a2 = sin(iTime*10.)*0.1;
    float f2 = 9.0; // power ===============================
    float y2 = wave2(x+0.1,a2,f2);
    y = smax(y,y1,0.1);
    y = smax(y,y2,0.2);
    //y = smax(y,wave1(x*0.01),-0.9);
    float peak3 = 0.1;//
    float narrow3 = 5.0 + 2.*sin(iTime*2.);
    float y3 = wave3(x+0.1*sin(iTime*5.),peak3,narrow3);
    
    y = smax(y,y3,0.8);
    //y = smax(y,0.2,0.9);
    
    //y *= 1.2;// whole scale =======================
    
    return y;
}



float wave_distort1(float use,vec2 st,float angle,float amp){
    // after trying for +y axis , angle center is 0.5 , range is about 3.5, means -3.0~4.0 is its range
    
    // normalize from 0.0 ~ 1.0
    
    // important ========================================
    angle *= 7.;// 0~7.;
    angle -= 3.0;// -3 ~ 4.  original 0.5 now is 0.5 still

    // input angle should be < -0.1 or > 1.1 for save no motion at all
    
    float y = 0.0;
    if(use > 0.0){
        bool bUseWaveDistort = true;
        y = wave_distort11(bUseWaveDistort,st,angle,0.2+ rand(iTime*0.000001));
        float y1 = wave_distort11(bUseWaveDistort,st,angle,0.3 + rand(iTime*0.000001));
        y = mix(y,y1,0.9 + rand(iTime*0.000001) );
        float y2 = wave_distort11(bUseWaveDistort,st,angle,0.9 + rand(iTime*0.000001));
        y = mix(y,y2,0.9+ rand(iTime*0.000001));
        
        float y3 = wave_distort11(bUseWaveDistort,st,angle,1.9);
        
        y = mix(y,y3,0.1+ rand(iTime*0.000001));
        
        float waveBottom = amp;// smaller means bigger wave peak to the lower wave bottom;
        // 0.05 ~ 0.6
        y = smax(y,waveBottom,0.9);
        
    }
    
    
    
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
    vec2 st1 = st;

    st1.x += PI;
    st1.x /= 2. * PI;  // st1 is from 0~1
    float y = 0.0;
    
    // y += wave_distort(bWordTracking,st,0.5,0.8);
    
    float colAdd = 0.;
    if(bWordTracking>0.0){
//        for (int i = 0; i < kTrackingDataSize; i++)
//        {

           float angle;

           // data texture 0 ~ 640 has detecting data
           // need map st1.x to it .
            // float mark = texture2D(inputTexture,vec2(st1.x/6.0,0.5)).r;

           // for strenched texture data

           float mark;
            mark = texture2D(inputTexture,vec2(st1.x/6.0,uv.y)).r;


            if(mark < 0.2){
                colAdd = 0.2;
                angle = st1.x;
                    
            }
        

           float amp = globalWaveAmp;// smaller means bigger wave peak to the lower wave bottom;
           // 0.05 ~ 0.6

           // y = smax(y,wave_distort1(bWordTracking,st,angle,amp),0.8);
           y = smax(y,wave_distort11(true,st,angle,amp),0.5);

//        }
        
        
        // read position from texture;
        // float x = fragCoord.x/iResolution.x;
        // float y = 1. - st1.x;
        // distort = texture2D(inputTexture,vec2(x,y)).r;

        // float amp = globalWaveAmp;


    }else{
        y = globalWaveAmp;
    }
    
    
    
    

    vec3 col;
    // vec3 word_wave(vec2 st,float rotateSpeed,float distort,float colNZumber,float offsetY,float lsratio,float wsratio){
    
    col = word_wave(st,wordRotateSpeed,y,wordLineNum,wordOffset,wordLineSpacingRatio,wordWordSpacingRatio);
    

    col.r -= colAdd;// use color change to debug if get parameter from texture


    // combine with another data channel from texture ================
    // vec3 col1 = texture2D(inputTexture,uv1).rrr;
    // col = mix(col,col1,0.5);



    float mark;
    mark = texture2D(inputTexture,vec2(gl_FragCoord.x/iResolution.x,gl_FragCoord.y/iResolution.y)).r;


    col = vec3(mark);


    // debug distort wave =============
    // bool bWaveDistortDebug = false;
    // if(st.y < y && bWaveDistortDebug){
    //     col = vec3(1.0);
    // }
    
    // Output to screen
    
    //    col = vec3(sin(iTime));




    // debug tracking data ---------------------------
    
    // uv.x *= 12.0;
    
    // int index = int(floor(uv.x));
    // col = vec3(trackingData[index]);
    


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
