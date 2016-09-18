#version 410 


out vec4 out_Color;
in vec2 textCoord;
uniform sampler2D mytexture;
uniform bool hasTexture;
uniform bool endEffector;
uniform bool isRoot;

void main()
{
	if(hasTexture)
	{
		if(endEffector)
		{
			//vec4 texturedColor = texture(mytexture,textCoord);
			out_Color = vec4(1.0f,0.0f,0.0f, 1.0f);
		}
		else if(isRoot)
		{
			out_Color = vec4(1.0f,1.0f,0.0f, 1.0f);
		}
		else{
			out_Color = texture(mytexture,textCoord);
		}
	}else{
		out_Color = vec4(0.5f,0.5f,0.5f, 1.0f);
	}
	
}
