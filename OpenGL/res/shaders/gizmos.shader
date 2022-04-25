#shader vertex
#version 150

in vec4 Position;
in vec4 Colour;

out vec4 vColour;

uniform mat4 ProjectionView;

void main()
{
	vColour = Colour; gl_Position = ProjectionView * Position;
}

#shader fragment
#version 150

in vec4 vColour;

out vec4 FragColor;

void main()
{
	FragColor = vColour;
}
