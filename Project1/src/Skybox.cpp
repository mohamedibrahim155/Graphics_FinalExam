#include "Skybox.h"

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}

void Skybox::SkyboxPrerender()
{

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    //std::vector<std::string> faces
    //{
    //   ("Textures/skybox/right.jpg"),
    //   ("Textures/skybox/left.jpg"),
    //   ("Textures/skybox/top.jpg"),
    //   ("Textures/skybox/bottom.jpg"),
    //   ("Textures/skybox/front.jpg"),
    //   ("Textures/skybox/back.jpg")
    //};

    std::vector<std::string> faces
    {
       ("Textures/skybox/SpaceBox_back6_negZ.jpg"),
       ("Textures/skybox/SpaceBox_bottom4_negY.jpg"),
       ("Textures/skybox/SpaceBox_front5_posZ.jpg"),
       ("Textures/skybox/SpaceBox_left2_negX.jpg"),
       ("Textures/skybox/SpaceBox_right1_posX.jpg"),
       ("Textures/skybox/SpaceBox_top3_posY.jpg")
    };

    cubemapTexture = skyboxTexture->loadCubemap(faces);

    skyboxShader->Bind();
    skyboxShader->setInt("skybox",0);



}

void Skybox::Skyboxrender()
{
    
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
   


}

void Skybox::SkyboxPostrender()
{

    glDeleteBuffers(1, &skyboxVBO);


}

void Skybox::AssignSkyboxShader(Shader* shader)
{
    this->skyboxShader = shader;

}

