#version 140

in vec2 position;
in vec4 rgba;
in vec2 uv;

out vec2 uv_frag;
out vec4 rgba_frag;

uniform mat4 localMat;
uniform mat4 proj;
uniform mat4 mv;

void main() {
    uv_frag = uv;    
    rgba_frag = rgba;
    gl_Position = proj*(mv*(localMat * vec4(position, 0.0, 1.0)));    
}
