#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    // when calculating lightning, magnitude or position of vectors don't 
    // matter; only direction is important 
    // => All calculations can be done with unit vectors (much simpler!)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // calculate diffuse impact of the light on the current fragment
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // calculate specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    //                        ^ reflect needs a vector that points from the 
    //                          light source to the fragment 

    // specular component
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //                                                   ^ the value of 
    //                                      "shininess" as a power of 2;
    //                                      the higher, the laser

    vec3 specular = specularStrength * spec * lightColor;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
