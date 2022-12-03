#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main()
{
    // ambient 
    vec3 ambient = light.ambient * material.ambient;

    // diffuse 
    // when calculating lightning, magnitude or position of vectors don't 
    // matter; only direction is important 
    // => All calculations can be done with unit vectors (much simpler!)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    // calculate diffuse impact of the light on the current fragment
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular 
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    //                        ^ reflect needs a vector that points from the 
    //                          light source to the fragment 

    // specular component
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
