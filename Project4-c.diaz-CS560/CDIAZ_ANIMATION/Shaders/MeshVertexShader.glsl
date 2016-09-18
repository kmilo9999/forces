#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 UVs;                                             

out vec2 textCoord;
uniform mat4 camera;

uniform mat4 mpvMatrix,model;


void main()
{
	textCoord = UVs;
	gl_Position =  camera * model * vec4(Position,1.0f);

}