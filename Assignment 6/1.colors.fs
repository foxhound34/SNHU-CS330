#version 330 core
out vec4 FragColor;
 
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform vec3 lightColor;


void main()
{
    //FragColor = texture(ourTexture, TexCoord);
    FragColor = vec4(lightColor * objectColor, 1.0);
}
