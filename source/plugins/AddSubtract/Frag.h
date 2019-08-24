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

void main()
{
    
    
    // name convert ---------------
    vec4 fragColor;
    iTime = ticks/1000.0;
    iResolution = vec2(width,height);
    fragCoord = vec2(gl_FragCoord.x,iResolution.y - gl_FragCoord.y) ;
    
    // --------------------------
    vec2 p = fragCoord.xy / iResolution.xy;
    
    vec2 uv = p*vec2(iResolution.x/iResolution.y,1.0);

    // --------------------------

    
    vec3 col;

    col += sin(iTime);
    // Output to screen
    fragColor = vec4(col,1.0);
    
    gl_FragColor = fragColor;
}
);

#endif /* Frag_h */
