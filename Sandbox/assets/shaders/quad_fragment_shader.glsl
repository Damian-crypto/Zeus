#version 450

layout (location = 0) out vec4 a_Color;
layout (location = 1) out int a_ID;

in vec4 f_Color;
in vec2 f_TexCoord;
in flat int f_TexSlot;
in float f_TilingFactor;
in flat int f_ID;

uniform sampler2D u_Textures[8];

void main()
{
	if (f_TexSlot == -1)
	{
		a_Color = f_Color;
	}
	else
	{
		a_Color = texture(u_Textures[f_TexSlot], f_TexCoord) * f_Color;
	}

	a_ID = f_ID;
}
