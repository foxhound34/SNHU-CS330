#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D texture4;
uniform sampler2D texture6;

uniform sampler2D ourTexture;

void main()
{
    //FragColor = texture(ourTexture, TexCoord);
    // linearly interpolate between both textures (70% container, 30% caution)
	FragColor = mix(texture(texture4, TexCoord), texture(texture6, TexCoord), 0.3);
}
