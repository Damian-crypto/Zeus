#version 450

layout (location = 0) out vec4 a_Color;
layout (location = 1) out int a_ID;

in vec4 f_Color;
in vec2 f_TexCoord;
in float f_TexId;
in float f_TilingFactor;

uniform sampler2D u_Textures[8];

void main()
{
	int texIndex = int(f_TexId);
	if (texIndex == -1.0)
	{
		a_Color = f_Color;
	}
	else
	{
		a_Color = texture(u_Textures[texIndex], f_TexCoord) * f_Color;
	}

	a_ID = 98;
}
