// GLSL version
#version 460

uniform mat4 u_worldTransform;
uniform mat4 u_viewProj;

// map to vao attribute pointer
layout(location = 0) in vec3 location;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texturecoord;

layout(location = 0) out  vec2 o_texCoord;

void main()
{
    vec4 pos = vec4(location,1.0);
    gl_Position = pos * u_worldTransform * u_viewProj; // 因为是行向量才右乘的

    o_texCoord = texturecoord;
}

