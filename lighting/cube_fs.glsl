#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
    // when calculating lightning, magnitude or position of vectors don't 
    // matter; only direction is important 
    // => All calculations can be done with unit vectors (much simpler!)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // calculate diffuse impact of the light on the current fragment
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;

    FragColor = vec4(result, 1.0);
}
