#version 450

layout (location = 0) out vec4 a_Color;

in vec4 f_Color;

void main()
{
	a_Color = f_Color;
}
