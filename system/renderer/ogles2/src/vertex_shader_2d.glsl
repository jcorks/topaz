#version 100

attribute vec3 position;
attribute vec2 uv;
attribute vec4 rgba;

varying vec2 uv_frag;
varying vec4 rgba_frag;

uniform mat4 localMat;
uniform mat4 proj;
uniform mat4 mv;

void main() {
    uvw_frag = uvw;    
    rgba_frag = rgba;
    gl_Position = proj*(mv*(localMat * vec4(position, 1.0)));    
}
