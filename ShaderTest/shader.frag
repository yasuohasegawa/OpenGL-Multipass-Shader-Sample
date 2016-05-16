#version 120

uniform float time;
uniform vec2 resolution;

void main( void ) {
    vec2 position = gl_FragCoord.xy;
    vec2 p = fract(position / 20.0);
    float a = 0.7;
    float b = 0.3;
    float r = 0.4;
    float delta = 300.0;
    vec2 mov = vec2(r * cos(a * time*1.5),r * sin(b * time*1.5 + delta))+vec2(0.5);
    float s = min(pow(length(position - mov * resolution) / 100.0-abs(sin(time)*0.2), 1.5), 0.45);
    float d = smoothstep(s - 0.025, s + 0.025, 0.5 - length(p - vec2(0.5, 0.5)));
    
    vec2 mov2 = vec2(r * cos(a * time*1.0),r * sin(b * time*1.0 + delta))+vec2(0.5);
    float s2 = min(pow(length(position - mov2 * resolution) / 50.0-abs(sin(time)*0.5), 1.5), 0.45);
    float d2 = smoothstep(s2 - 0.025, s2 + 0.025, 0.5 - length(p - vec2(0.5, 0.5)));
    
    gl_FragColor = vec4(d, d2, 0.0, 1.0 );
}