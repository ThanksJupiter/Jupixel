#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord) * u_Color;
}
