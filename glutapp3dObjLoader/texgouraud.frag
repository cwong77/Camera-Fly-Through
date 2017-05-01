# version 400

in vec4 Color;
in vec2 TexCoord;
out vec4 fColor;

uniform sampler2D Tex1;

//uniform float influence;

void main()
{
	fColor = .1*Color + texture2D( Tex1, TexCoord );
	//fColor = influence*Color + texture2D( Tex1, TexCoord );
}