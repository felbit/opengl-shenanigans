#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));

    // calculate normal vector in world space (this requires some fancy 
    // matrix-calculation).
    // TODO: Understand this!
    // ALSO: inverse operations are expensive on the GPU; whenever possible,
    //       move the inverse calculation to the CPU and send it to shaders 
    //       via uniform before drawing!
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
