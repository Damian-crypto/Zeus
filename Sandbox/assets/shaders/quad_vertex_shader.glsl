#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_TexId;
layout (location = 4) in float a_TilingFactor;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 f_Color;
out vec2 f_TexCoord;
out float f_TexId;
out float f_TilingFactor;

void main()
{
	f_Color = a_Color;
	f_TexCoord = a_TexCoord;
	f_TexId = a_TexId;
	f_TilingFactor = a_TilingFactor;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}
