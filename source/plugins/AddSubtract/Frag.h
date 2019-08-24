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

uniform float lineNum; // 0.0 ~ 200;
uniform float rotateSpeed; // 1. ~ 10.
uniform float lsration; // 0.0 ~ 1.0
uniform float wsration; // 0.0 ~ 1.0
uniform float offsetY; // 0.0 ~ 50;

void main()
{
    
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    float PI = 3.1415926535;
    float aPI = acos(-1.);
    // --------------------------

    vec2 uv = (fragCoord.xy - .5 * iResolution.xy)/iResolution.x; // uv -.5 ~ .5  , x axis is scale t0 1.
    uv *= 2.0; // -1. ~ 1.
    uv.y += iResolution.y/iResolution.x;// origin point on (0.5 * x , 0.0)
    // uv *= 0.5;// 0 ~ 1
    vec2 st = vec2(atan(uv.x,uv.y),length(uv));
    st.x += PI;// 0 ~ 2PI on -y axis
    float letterWidith = 0.001;
    
    // pass parameters
    float speed = rotateSpeed;// sign means rotate clock(+) or counterclock(-)
    float piFactor = 128.0;// bigger number will avoid jump on loop but no effect on rotate speed
    float m = mod(speed*iTime,piFactor * PI);
    
    
    
    vec2 st1 = st;
    
    st1.y *= lineNum;
    
    
//    float offsetY = 2.0; // pass parameters
    
    if(st1.y < offsetY){
        return;
    }
    
    
    st1.x *= 1.*floor(st1.y);
    if(mod(floor(st1.y),2.0)>0.0){
        st1.x += m;
    }else{
        st1.x -= m;
    }
    
    vec2 polar = fract(st1);
    
    vec3 col;
    // Time varying pixel color
    //col = vec3(polar.x,polar.y,0.0);
    
    vec2 localPolar = polar;

    
    localPolar.x *= 1.0/(1.0 - wsration);
    localPolar.y *= 1.0/(1.0 - lsration);
    
    col = texture2D(inputTexture,localPolar).xyz;
    
    // make black space between line and word
    if(localPolar.x > 1.0 || localPolar.y > 1.0){
        col *= 0.0;
    }
    
    
    
    
    
    
    
    
    
    // Output to screen
    fragColor = vec4(col,1.0);
    
    gl_FragColor = fragColor;
}
);

#endif /* Frag_h */
