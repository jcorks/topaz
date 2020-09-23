#version 100

uniform int useTexturing;
uniform sampler2D sampler;

varying vec2 uv_frag;
varying vec4 rgba_frag;

void main() {   
    if (int(useTexturing) == 0)
        gl_FragColor = rgba_frag;
    else
        gl_FragColor = texture2D(sampler, uv.x, uv.y) * rgba_frag;
}
