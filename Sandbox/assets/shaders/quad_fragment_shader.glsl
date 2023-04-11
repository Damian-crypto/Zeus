#version 450

layout (location = 0) out vec4 a_Color;
layout (location = 1) out int a_ID;

in vec4 f_Color;
in vec3 f_Position;
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
		//if (f_TexCoord.x < 0.01 || f_TexCoord.x > 0.99 || f_TexCoord.y < 0.01 || f_TexCoord.y > 0.99)
		//	a_Color = vec4(1.0, 1.0, 0.0, 1.0);
	}
	else
	{
		a_Color = texture(u_Textures[f_TexSlot], f_TexCoord) * f_Color;
	}

	a_ID = f_ID;
}
