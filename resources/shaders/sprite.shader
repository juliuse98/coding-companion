#shader vertex
#version 330 core

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec2  aTexCoord;
layout (location = 2) in float textureId;
layout (location = 3) in mat4 modelMatrix;

uniform mat4 projection;
out vec2 TexCoord;
flat out float TextureId;

void main(){
	gl_Position = projection * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	TexCoord = aTexCoord;
	TextureId = textureId;
}

#shader fragment
#version 330 core

in vec2 TexCoord;
flat in float TextureId;
out vec4 FragColor;
uniform sampler2D ourTexture[8];

void main()
{
FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
FragColor = texture(ourTexture[int(TextureId)], TexCoord);
}
