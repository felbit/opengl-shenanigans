#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shader.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoordinates;
};

struct Texture {
    unsigned int ID;
    std::string Type;
    std::string Path;
};

class Mesh
{
  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);

  private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // vertex normal vectors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    // vertex normal vectors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TextureCoordinates));

    glBindVertexArray(0);
}

// We don't know from the start how many (if any) textures the mesh has and what type they may have. So how do we set
// the texture units and samplers in the shaders? To solve the issue we're going to assume a certain naming convention:
// each diffuse texture is named texture_diffuseN, and each specular texture should be named texture_specularN where N
// is any number ranging from 1 to the maximum number of texture samplers allowed. Let's say we have 3 diffuse textures
// and 2 specular textures for a particular mesh, their texture samplers should then be called:
//
//   uniform sampler2D texture_diffuse1;
//   uniform sampler2D texture_diffuse2;
//   uniform sampler2D texture_diffuse3;
//   uniform sampler2D texture_specular1;
//   uniform sampler2D texture_specular2;
//
// By this convention we can define as many texture samplers as we want in the shaders (up to OpenGL's maximum) and if a
// mesh actually does contain (so many) textures, we know what their names are going to be. By this convention we can
// process any amount of textures on a single mesh and the shader developer is free to use as many of those as he wants
// by defining the proper samplers.

void Mesh::Draw(Shader &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].Type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}