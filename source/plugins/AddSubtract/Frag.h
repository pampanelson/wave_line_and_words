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


float distort(vec2 st,float angle){
    angle *= 0.5;
    angle += 0.25;

    float r;
    float peakSharp = 0.3;// 0.3~ 0.4
    r = 1. - pow(abs(st.x-angle),peakSharp);
    r = smoothstep(0.0,0.77,r);
    r *= 1. + distortScale;
    return r;
}

void main()
{
    
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);

    vec2 fragCoord1 = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y);
    
    vec2 uv = gl_FragCoord.xy / iResolution.xy;// --------------  default uv
    // --------------------------
    vec3 col;
    

    

   vec2 uv1 = (fragCoord1.xy - .5 * iResolution.xy)/iResolution.y; // uv -.5 ~ .5
   uv1 *= 2.0; // -1. ~ 1.
   uv1.y += 1.0;


   vec2 st = vec2(atan(uv1.x,uv1.y),length(uv1));

   st.x = st.x/(PI*2.0) + .5; // before st.x is -π ~ π after is  normalized 0.0 ~ 1.0


   float d = distort(st,trk1Angle);
   d += distort(st,trk2Angle);
   d += distort(st,trk3Angle);


   if(st.y < d){

//        col += 1.0;
   }
   float thetaX = d * cos(st.x);
   float thetaY = d * sin(st.x);



   uv.x = uv.x + thetaX;
   uv.y = uv.y + thetaY;


    
    // for debug default uv domain of input texture------------------- 
    //fragColor = vec4(uv.x,uv.y,0.0,.5);

    col = texture2D(inputTexture,uv).xyz;
    
    fragColor = vec4(col,1.0);
    
    gl_FragColor = fragColor;
}
);

#endif /* Frag_h */
