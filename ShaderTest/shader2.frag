#version 120

uniform float time;
uniform vec2 resolution;

uniform sampler2D tex;
varying vec2 texcoord;

void main(void)
{
    vec2 position = ( gl_FragCoord.xy / resolution.xy );
    position.y += (sin((position.x + (time * 0.5)) * 6.0) * 0.1) + (sin((position.x + (time * 0.2)) * 6.0) * 0.01);
    vec4 col = texture2D(tex, position);
    gl_FragColor = col+vec4(position,1.0,1.0);
}