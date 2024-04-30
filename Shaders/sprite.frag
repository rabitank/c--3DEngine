
#version 460

/// 
layout(location = 0) in vec2 i_fragTexCoord;

out vec4 o_color;

uniform sampler2D u_texture;

void main()
{
    o_color = texture(u_texture,i_fragTexCoord);
    
}
