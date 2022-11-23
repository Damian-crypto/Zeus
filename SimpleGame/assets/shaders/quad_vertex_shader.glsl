#version 450

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in int a_TexSlot;
layout (location = 4) in float a_TilingFactor;
layout (location = 5) in int a_ID;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 f_Color;
out vec2 f_TexCoord;
out flat int f_TexSlot;
out float f_TilingFactor;
out flat int f_ID;

void main()
{
	f_Color = a_Color;
	f_TexCoord = a_TexCoord;
	f_TexSlot = a_TexSlot;
	f_TilingFactor = a_TilingFactor;
	f_ID = a_ID;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}
