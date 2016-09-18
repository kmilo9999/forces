#version 410

out vec4 frag_colour;
uniform bool anchor;
void main () {
	if(anchor)
	{
		frag_colour = vec4 (1.0, 0.0, 1.0, 1.0);
	}
	else
	{
		frag_colour = vec4 (1.0, 1.0, 1.0, 1.0);
	}
	
}
