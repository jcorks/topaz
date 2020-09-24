#version 100

attribute vec2 position;
attribute vec4 rgba;
attribute vec2 uv;

varying vec2 uv_frag;
varying vec4 rgba_frag;

uniform mat4 localMat;
uniform mat4 proj;
uniform mat4 mv;

void main() {
    uv_frag = uv;    
    rgba_frag = rgba;
    gl_Position = proj*(mv*(localMat * vec4(position, 0.0, 1.0)));    
}
