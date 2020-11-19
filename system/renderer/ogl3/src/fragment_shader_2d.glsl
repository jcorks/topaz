#version 140

uniform int useTexturing;
uniform sampler2D sampler;

in vec2 uv_frag;
in vec4 rgba_frag;

void main() {   
    if (int(useTexturing) == 0)
        gl_FragColor = rgba_frag;
    else
        gl_FragColor = texture2D(sampler, uv_frag) * rgba_frag;
}
