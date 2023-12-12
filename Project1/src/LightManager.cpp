#include"LightManager.h"


//void LightManager::SetUniformLocations(Light light, ShaderClass& lightingShader)
//{
//    switch (light.lightType)
//    {
//    case DIRECTION_LIGHT:
//        //lightingShader.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
//        //lightingShader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
//        //lightingShader.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
//        lightingShader.SetVec3("dirLight.direction",light.direction);
//        lightingShader.SetVec3("dirLight.ambient", light.ambient);
//        lightingShader.SetVec3("dirLight.diffuse", light.diffuse);
//        lightingShader.SetVec3("dirLight.specular", light.specular);
//
//        break;
//    case SPOT_LIGHT:
//
//        lightingShader.SetVec3("dirLight.direction",light.direction);
//        lightingShader.SetVec3("dirLight.ambient", light.ambient);
//        lightingShader.SetVec3("dirLight.diffuse", light.diffuse);
//        lightingShader.SetVec3("dirLight.specular", light.specular);
//        lightingShader.SetVec3("pointLights[0].position", light.position);
//        lightingShader.SetVec3("pointLights[0].ambient", light.ambient);
//        lightingShader.SetVec3("pointLights[0].diffuse", light.diffuse);
//        lightingShader.SetVec3("pointLights[0].specular", light.specular);
//        lightingShader.SetFloat("pointLights[0].constant", light.constant);
//        lightingShader.SetFloat("pointLights[0].linear", light.linear);
//        lightingShader.SetFloat("pointLights[0].quadratic", light.quadratic);
//
//        //lightingShader.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
//        //lightingShader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
//        //lightingShader.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
//        //lightingShader.SetFloat("pointLights[0].constant", 1.0f);
//        //lightingShader.SetFloat("pointLights[0].linear", 0.09f);
//        //lightingShader.SetFloat("pointLights[0].quadratic", 0.032f);
//        break;
//    case POINT_LIGHT:
//
//        lightingShader.SetVec3("spotLight.position", light.position);
//        lightingShader.SetVec3("spotLight.direction", light.direction);
//        lightingShader.SetVec3("spotLight.ambient", light.ambient);
//        lightingShader.SetVec3("spotLight.diffuse", light.diffuse);
//        lightingShader.SetVec3("spotLight.specular", light.specular);
//        lightingShader.SetFloat("spotLight.constant", light.constant);
//        lightingShader.SetFloat("spotLight.linear", light.linear);
//        lightingShader.SetFloat("spotLight.quadratic", light.quadratic);
//        lightingShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(light.cutOffAngle)));
//        lightingShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(light.outerCutOffAngle)));
//
//
//
//        //lightingShader.SetVec3("spotLight.position", camera.Position);
//        //lightingShader.SetVec3("spotLight.direction", camera.Front);
//        //lightingShader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
//        //lightingShader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
//        //lightingShader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
//        //lightingShader.SetFloat("spotLight.constant", 1.0f);
//        //lightingShader.SetFloat("spotLight.linear", 0.09f);
//        //lightingShader.SetFloat("spotLight.quadratic", 0.032f);
//        //lightingShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
//        //lightingShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
//
//        break;
//    }
//
//    //// point light 1
//
//    //// point light 2
//    //lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
//    //lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
//    //lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
//    //lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
//    //lightingShader.setFloat("pointLights[1].constant", 1.0f);
//    //lightingShader.setFloat("pointLights[1].linear", 0.09f);
//    //lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
//    //// point light 3
//    //lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
//    //lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
//    //lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
//    //lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
//    //lightingShader.setFloat("pointLights[2].constant", 1.0f);
//    //lightingShader.setFloat("pointLights[2].linear", 0.09f);
//    //lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
//    //// point light 4
//    //lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
//    //lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
//    //lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
//    //lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
//    //lightingShader.setFloat("pointLights[3].constant", 1.0f);
//    //lightingShader.setFloat("pointLights[3].linear", 0.09f);
//    //lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
//    // spotLight
//
//}


void LightManager::SetUniformLocations(Light light, Shader& modelShader)
{
 
       // lightingShader.SetInt("LIGHT_TYPE", (int)light.lightType);

        modelShader.setVec3("dirLight.direction", light.direction);
        modelShader.setVec3("dirLight.ambient", light.ambient);
        modelShader.setVec3("dirLight.diffuse", light.diffuse);
        modelShader.setVec3("dirLight.specular", light.specular);





     //   modelShader.SetVec3("pointLights[0].position", light.position);
        modelShader.setVec3("pointLights[0].ambient", light.ambient);
        modelShader.setVec3("pointLights[0].diffuse", light.diffuse);
        modelShader.setVec3("pointLights[0].specular", light.specular);
        modelShader.setFloat("pointLights[0].constant", light.constant);
        modelShader.setFloat("pointLights[0].linear", light.linear);
        modelShader.setFloat("pointLights[0].quadratic", light.quadratic);



      //  modelShader.SetVec3("spotLight.position", light.position);
        modelShader.setVec3("spotLight.direction", light.direction);
        modelShader.setVec3("spotLight.ambient", light.ambient);
        modelShader.setVec3("spotLight.diffuse", light.diffuse);
        modelShader.setVec3("spotLight.specular", light.specular);
        modelShader.setFloat("spotLight.constant", light.constant);
        modelShader.setFloat("spotLight.linear", light.linear);
        modelShader.setFloat("spotLight.quadratic", light.quadratic);
        modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(light.cutOffAngle)));
        modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(light.outerCutOffAngle)));




}

//void LightManager::SetUniforms(GLuint shaderID)
//{
//    this->LightToIlluminate[0].position_UL = glGetUniformLocation(shaderID, "lights[0].position");
//    //        vec4 diffuse;	// Colour of the light (used for diffuse)
//    this->LightToIlluminate[0].diffuse_UL = glGetUniformLocation(shaderID, "lights[0].diffuse");
//    //        vec4 specular;	// rgb = highlight colour, w = power
//    this->LightToIlluminate[0].specular_UL = glGetUniformLocation(shaderID, "lights[0].specular");
//    //        vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
//    this->LightToIlluminate[0].ambient_UL = glGetUniformLocation(shaderID, "lights[0].ambient");
//    //        vec4 direction;	// Spot, directional lights
//    this->LightToIlluminate[0].direction_UL = glGetUniformLocation(shaderID, "lights[0].direction");
//    //        vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//    this->LightToIlluminate[0].lightType_UL = glGetUniformLocation(shaderID, "lights[0].lightType");
//    //        vec4 param2;	// x = 0 for off, 1 for on
//    this->LightToIlluminate[0].linear_UL = glGetUniformLocation(shaderID, "lights[0].linear");
//    this->LightToIlluminate[0].quadratic_UL = glGetUniformLocation(shaderID, "lights[0].quadratic");
//    this->LightToIlluminate[0].constant_UL = glGetUniformLocation(shaderID, "lights[0].constant");
//    this->LightToIlluminate[0].cuttOffAngle_UL = glGetUniformLocation(shaderID, "lights[0].cutOff");
//    this->LightToIlluminate[0].outerCutOffAngle_UL = glGetUniformLocation(shaderID, "lights[0].outerCutOff");
//
//
//    // vec4 position;
//  
//
//    this->LightToIlluminate[1].position_UL = glGetUniformLocation(shaderID, "lights[1].position");
//    //        vec4 diffuse;	/ Colour of the light (used for diffuse)
//    this->LightToIlluminate[1].diffuse_UL = glGetUniformLocation(shaderID, "lights[1].diffuse");
//    //        vec4 specular	// rgb = highlight colour, w = power
//    this->LightToIlluminate[1].specular_UL = glGetUniformLocation(shaderID, "lights[1].specular");
//    //        vec4 atten;	// x = constant, y = linear, z = quadratic, w = DistanceCutOff
//    this->LightToIlluminate[1].ambient_UL = glGetUniformLocation(shaderID, "lights[1].ambient");
//    //        vec4 direction1	// Spot, directional lights
//    this->LightToIlluminate[1].direction_UL = glGetUniformLocation(shaderID, "lights[1].direction");
//    //        vec4 param1;	/ x = lightType, y = inner angle, z = outer angle, w = TBD
//    this->LightToIlluminate[1].lightType_UL = glGetUniformLocation(shaderID, "lights[1].lightType");
//    //        vec4 param2;	/ x = 0 for off, 1 for on
//    this->LightToIlluminate[1].linear_UL = glGetUniformLocation(shaderID, "lights[1].linear");
//    this->LightToIlluminate[1].quadratic_UL = glGetUniformLocation(shaderID, "lights[1].quadratic");
//    this->LightToIlluminate[1].constant_UL = glGetUniformLocation(shaderID, "lights[1].constant");
//    this->LightToIlluminate[1].cuttOffAngle_UL = glGetUniformLocation(shaderID, "lights[1].cutOff");
//    this->LightToIlluminate[1].outerCutOffAngle_UL = glGetUniformLocation(shaderID, "lights[1].outerCutOff");
//
//
//
//
//    this->LightToIlluminate[2].position_UL = glGetUniformLocation(shaderID, "lights[2].position");
//    //        vec4 diffuse;	2 Colour of the light (used for diffuse)
//    this->LightToIlluminate[2].diffuse_UL = glGetUniformLocation(shaderID, "lights[2].diffuse");
//    //        vec4 specular	2/ rgb = highlight colour, w = power
//    this->LightToIlluminate[2].specular_UL = glGetUniformLocation(shaderID, "lights[2].specular");
//    //        vec4 atten;	2/ x = constant, y = linear, z = quadratic, w = DistanceCutOff
//    this->LightToIlluminate[2].ambient_UL = glGetUniformLocation(shaderID, "lights[2].ambient");
//    //        vec4 direction2	// Spot, directional lights
//    this->LightToIlluminate[2].direction_UL = glGetUniformLocation(shaderID, "lights[2].direction");
//    //        vec4 param1;	2 x = lightType, y = inner angle, z = outer angle, w = TBD
//    this->LightToIlluminate[2].lightType_UL = glGetUniformLocation(shaderID, "lights[2].lightType");
//    //        vec4 param2;	2 x = 0 for off, 1 for on
//    this->LightToIlluminate[2].linear_UL = glGetUniformLocation(shaderID, "lights[2].linear");
//    this->LightToIlluminate[2].quadratic_UL = glGetUniformLocation(shaderID, "lights[2].quadratic");
//    this->LightToIlluminate[2].constant_UL = glGetUniformLocation(shaderID, "lights[2].constant");
//    this->LightToIlluminate[2].cuttOffAngle_UL = glGetUniformLocation(shaderID, "lights[2].cutOff");
//    this->LightToIlluminate[2].outerCutOffAngle_UL = glGetUniformLocation(shaderID, "lights[2].outerCutOff");          
//
//
//}

void LightManager::SetUniforms(GLuint shaderID)
{

    for (size_t i = 0; i < lightList.size(); i++)
    {
        std::string index = std::to_string(i);
        this->lightList[i].position_UL =         glGetUniformLocation(shaderID, ("lights[" + index + "].position").c_str());
        this->lightList[i].diffuse_UL =          glGetUniformLocation(shaderID, ("lights[" + index + "].diffuse").c_str());
        this->lightList[i].specular_UL =         glGetUniformLocation(shaderID, ("lights[" + index + "].specular").c_str());
        this->lightList[i].ambient_UL =          glGetUniformLocation(shaderID, ("lights[" + index + "].ambient").c_str());
        this->lightList[i].direction_UL =        glGetUniformLocation(shaderID, ("lights[" + index + "].direction").c_str());
        this->lightList[i].lightType_UL =        glGetUniformLocation(shaderID, ("lights[" + index + "].lightType").c_str());
        this->lightList[i].linear_UL =           glGetUniformLocation(shaderID, ("lights[" + index + "].linear").c_str());
        this->lightList[i].quadratic_UL =        glGetUniformLocation(shaderID, ("lights[" + index + "].quadratic").c_str());
        this->lightList[i].constant_UL =         glGetUniformLocation(shaderID, ("lights[" + index + "].constant").c_str());
        this->lightList[i].cuttOffAngle_UL =     glGetUniformLocation(shaderID, ("lights[" + index + "].cutOff").c_str());
        this->lightList[i].outerCutOffAngle_UL = glGetUniformLocation(shaderID, ("lights[" + index + "].outerCutOff").c_str());
        this->lightList[i].color_UL =            glGetUniformLocation(shaderID, ("lights[" + index + "].color").c_str());
    }

}

void LightManager::AddNewLight(Light& light)
{
    lightList.push_back(light);
}




void LightManager::UpdateUniformValues(GLuint shaderID)
{
    for (unsigned int index = 0;index <lightList.size(); index++)
    {
        if (lightList.size()>LightManager::MAX_LIGHT)
        {
            std::cout << "Light exceeded ...  File : " << __FILE__<< " Line : "  << __LINE__ << std::endl;
            break;
        }
        //lightList[index].lightModel->transform.position
        GLCALL(glUniform3f(lightList[index].position_UL, lightList[index].lightModel->transform.position.x ,lightList[index].lightModel->transform.position.y, lightList[index].lightModel->transform.position.z));
       
        glm::vec3 value = lightList[index].lightModel->transform.GetForward();
        lightList[index].direction = value;

            GLCALL(glUniform3f(lightList[index].direction_UL, lightList[index].direction.x, lightList[index].direction.y, lightList[index].direction.z));

        glUniform4f(lightList[index].diffuse_UL, lightList[index].diffuse.x,lightList[index].diffuse.y,   lightList[index].diffuse.z, lightList[index].diffuse.w);
                 
        glUniform4f(lightList[index].specular_UL,lightList[index].specular.x,lightList[index].specular.y, lightList[index].specular.z, lightList[index].diffuse.w);
                 
        glUniform4f(lightList[index].ambient_UL, lightList[index].ambient.x, lightList[index].ambient.y,  lightList[index].ambient.z, lightList[index].diffuse.w);
                 
        glUniform3f(lightList[index].direction_UL,lightList[index].direction.x, lightList[index].direction.y,lightList[index].direction.z);

        glUniform1i(lightList[index].lightType_UL,  (int)lightList[index].lightType);


        glUniform1f(lightList[index].constant_UL,lightList[index].constant);


        glUniform1f(lightList[index].linear_UL,lightList[index].linear);


        glUniform1f(lightList[index].quadratic_UL,lightList[index].quadratic);


        glUniform1f(lightList[index].cuttOffAngle_UL, glm::cos(glm::radians( lightList[index].cutOffAngle)));

        glUniform1f(lightList[index].outerCutOffAngle_UL, glm::cos(glm::radians(lightList[index].outerCutOffAngle)));
        float lightIntensity = lightList[index].intensity;
        glUniform4f(lightList[index].color_UL,  lightList[index].color.x * lightIntensity,
            lightList[index].color.y * lightIntensity, 
            lightList[index].color.z * lightIntensity, lightList[index].color.w);
    }


    return;

}

void LightManager::UpdateUniformValuesToShader(Shader* shader)
{
    for (size_t i = 0; i < lightList.size(); i++)
    {
        if (lightList.size() > LightManager::MAX_LIGHT)
        {
            std::cout << "Light exceeded ...  File : " << __FILE__ << " Line : " << __LINE__ << std::endl;
            break;
        }
        std::string index = std::to_string(i);
        shader->setVec3(  "lights[" + index + "].position", lightList[i].lightModel->transform.position);
        shader->setVec3(  "lights[" + index + "].diffuse", lightList[i].diffuse);
        shader->setVec3(  "lights[" + index + "].specular", lightList[i].specular);
        shader->setVec3(  "lights[" + index + "].ambient", lightList[i].ambient);
        shader->setVec3(  "lights[" + index + "].direction", lightList[i].lightModel->transform.GetForward());
        shader->setInt(   "lights[" + index + "].lightType", lightList[i].lightType);
        shader->setFloat( "lights[" + index + "].linear", lightList[i].linear);
        shader->setFloat( "lights[" + index + "].quadratic", lightList[i].quadratic);
        shader->setFloat( "lights[" + index + "].constant", lightList[i].constant);
        shader->setFloat( "lights[" + index + "].cutOff", lightList[i].cutOffAngle);
        shader->setFloat( "lights[" + index + "].outerCutOff", lightList[i].outerCutOffAngle);
        shader->setVec3(  "lights[" + index + "].color", lightList[i].color);

    }
    
}
