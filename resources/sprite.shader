#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 projection;
out vec2 TexCoord;

void main(){
	gl_Position = projection * vec4(aPos.x,aPos.y, aPos.z, 1.0);
	TexCoord = aTexCoord;
}

#shader fragment
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D ourTexture;

void main()
{
FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
//FragColor = texture(ourTexture, TexCoord);
}
