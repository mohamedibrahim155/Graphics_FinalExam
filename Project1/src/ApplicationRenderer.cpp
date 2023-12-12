#include"ApplicationRenderer.h"



ApplicationRenderer::ApplicationRenderer()
{
}

ApplicationRenderer::~ApplicationRenderer()
{
}



void ApplicationRenderer::WindowInitialize(int width, int height,  std::string windowName)
{
    windowWidth = width;
    WindowHeight = height;
    lastX = windowWidth / 2.0f;
    lastY= WindowHeight / 2.0f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int x, int y)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(w))->SetViewPort(w, x, y);
        });

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(window))->KeyCallBack(window, key, scancode, action, mods);
        });


    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xposIn, double yposIn)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(window))->MouseCallBack(window, xposIn, yposIn);
        });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            static_cast<ApplicationRenderer*>(glfwGetWindowUserPointer(window))->MouseScroll(window, xoffset, yoffset);
        });
   
    
    //Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }




  
    defaultShader = new Shader("Shaders/Light_VertexShader.vert", "Shaders/Light_FragmentShader2.frag");
    lightShader = new Shader("Shaders/lighting.vert", "Shaders/lighting.frag", SOLID);
    StencilShader = new Shader("Shaders/StencilOutline.vert", "Shaders/StencilOutline.frag");
   
    SkyboxShader = new Shader("Shaders/SkyboxShader.vert", "Shaders/SkyboxShader.frag");


    //ScrollShader = new Shader("Shaders/ScrollTexture.vert", "Shaders/ScrollTexture.frag");
    render.AssignStencilShader(StencilShader);
   // camera.Position = glm::vec3(0, 0, - 1.0f);
}



std::vector<ModelData> ApplicationRenderer::loadModelDataFromFile(const std::string& filePath)
{

    std::ifstream file(filePath);
    std::vector<ModelData> modelData;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return modelData;
    }
    else
    {
        std::cerr << "File Opened >>>>>>>>>>>>>>>>>>>>>>>>>>>: " << filePath << std::endl;

    }

    std::string line;
    ModelData currentModel;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "ModelPath:") 
        {
            iss >> currentModel.path;
        }
        else if (token == "ModelPosition:") {
            iss >> currentModel.position.x >> currentModel.position.y >> currentModel.position.z;
        }       
        else if (token == "ModelRotation:") {

            iss >> currentModel.rotation.x >> currentModel.rotation.y >> currentModel.rotation.z;

        }
      
        else if (token == "TexturePath:") 
        {

            iss >> currentModel.texturePath;

        }
        else if (token == "Transperency:")
        {

            iss >> currentModel.isTrans;

        }
        
        else if (token == "Cutoff:")
        {

            iss >> currentModel.isCutoff;

        }
      
        else if (token == "ModelScale:") {
            iss >> currentModel.scale.x >> currentModel.scale.y >> currentModel.scale.z;
            modelData.push_back(currentModel);
        }
    }

    file.close();
    return modelData;
}


void ApplicationRenderer::Start()
{
   // GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LESS));
    GLCALL(glEnable(GL_STENCIL_TEST));
    GLCALL(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
    GLCALL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    skybox = new Skybox(); 
    
    skybox->AssignSkyboxShader(SkyboxShader);
    skybox->SkyboxPrerender();
    

    render.AssignCamera(&camera);

    defaultBox = new Model("Models/Box/DefaultCube.fbx");
    render.SetDefaultCube(defaultBox);

    render.SetLightShader(lightShader);

    Model* Sphere = new Model((char*)"Models/DefaultSphere/Sphere_1_unit_Radius.ply", true);


    //render.AddModelsAndShader(CamPlaceholder, defaultShader);
     Sphere->transform.position.x += 2;

    


     Model* directionLightModel = new Model(*Sphere);
     directionLightModel->transform.SetPosition(glm::vec3(1.0f, 3.0f, 0.0f));
     directionLightModel->transform.SetRotation(glm::vec3(-60, 0, 0));
     directionLightModel->transform.SetScale(glm::vec3(0.1f));




/////////////////////////////////////////////////////////////////////////////////////////////

     modelData = loadModelDataFromFile("Model.txt");
     CityModel = new Model("Models/White//White.obj",false);
     CityModel->transform.SetPosition(glm::vec3(0,-15,0));
     CityModel->transform.SetRotation(glm::vec3(0,0,0));
     CityModel->transform.SetScale(glm::vec3(0.1f));
     render.AddModelsAndShader(CityModel, defaultShader);

     cityPhysics = new PhysicsObject(CityModel);
     cityPhysics->Initialize(MESH_TRIANGLES,false,STATIC);
     PhysicsEngine.AddPhysicsObjects(cityPhysics);






   //  DrawDebugModelAABB(cityPhysics->GetModelAABB());


     //////////////////////////////////////////////////////////
     //////SPACE SHIP ENTITY
     spaceshipEntity = new SpaceShip(render, defaultShader, PhysicsEngine,camera);
     spaceshipEntity->LoadModel();


#pragma region Lights

Light directionLight;
directionLight.lightType = LightType::DIRECTION_LIGHT;
directionLight.lightModel = directionLightModel;
directionLight.ambient =  glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
directionLight.diffuse =  glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
directionLight.specular = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
directionLight.intensity = 0.5f;






#pragma endregion


     render.selectedModel = nullptr;

     render.AddModelsAndShader(directionLightModel, lightShader);


     

     //LightRenderer
     lightManager.AddNewLight(directionLight);

     lightManager.SetUniforms(defaultShader->ID);
   

     defaultShader->Bind();
     defaultShader->setInt("skybox", 0);

     moveCam.AssignCam(&camera);

    
}

void ApplicationRenderer::PreRender()
{

}

void ApplicationRenderer::Render()
{
    Start();
    Material material(128.0f);
    glm::vec3 pos(5, 0, 0);
   // glEnable(GL_BLEND);
  //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window))
    {
        Clear();


        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        scrollTime += deltaTime;

        ProcessInput(window);

        glm::mat4 _projection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)WindowHeight, 0.1f, 100.0f);
        glm::mat4 _view = camera.GetViewMatrix();
        glm::mat4 _skyboxview = glm::mat4(glm::mat3(camera.GetViewMatrix()));


        PreRender(); //Update call BEFORE  DRAW

        glDepthFunc(GL_LEQUAL);
        SkyboxShader->Bind();
        SkyboxShader->setMat4("view", _skyboxview);
        SkyboxShader->setMat4("projection", _projection);

       // skybox->Skyboxrender();
        glDepthFunc(GL_LESS); 


        defaultShader->Bind();
       // material.SetMaterialProperties(*defaultShader);
     //   lightManager.UpdateUniformValuesToShader(defaultShader);
        lightManager.UpdateUniformValues(defaultShader->ID);
       

         defaultShader->setMat4("projection", _projection);
         defaultShader->setMat4("view", _view);
         defaultShader->setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
         defaultShader->setFloat("time", scrollTime);
         defaultShader->setBool("isDepthBuffer", false);

         lightShader->Bind();
         lightShader->setVec3("objectColor", glm::vec3(1, 1, 1));
         lightShader->setMat4("projection", _projection);
         lightShader->setMat4("view", _view);

         StencilShader->Bind();
         StencilShader->setMat4("projection", _projection);
         StencilShader->setMat4("view", _view);

        /* ScrollShader->Bind();
         ScrollShader->setMat4("ProjectionMatrix", _projection);*/
        

       

         
  
         
         // make models that it should not write in the stencil buffer
         render.Draw();
       

         if (cameraMoveToTarget)
         {
             camera.UpdateCameraPosition(deltaTime);

         }
       
    
        
      
     

         PostRender(); // Update Call AFTER  DRAW

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void ApplicationRenderer::PostRender()
{
   // glDisable(GL_BLEND);

    PhysicsEngine.Update(deltaTime);

    spaceshipEntity->Update(deltaTime);
 
  //  DrawDebugModelAABB(spaceshipEntity->SpaceShipPhysics->UpdateAABB());
}

void ApplicationRenderer::Clear()
{
    GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 0.1f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void ApplicationRenderer::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed=2;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
       // camera.ProcessKeyboard(FORWARD, deltaTime * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
     //   camera.ProcessKeyboard(BACKWARD, deltaTime * cameraSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
      //  camera.ProcessKeyboard(LEFT, deltaTime * cameraSpeed);

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
      //  camera.ProcessKeyboard(RIGHT, deltaTime * cameraSpeed);

    }

   // spaceshipEntity->SpaceShipInputs(window,deltaTime);


}

void ApplicationRenderer::DrawDebugModelAABB( const cAABB& aabb)
{
    
  
        glm::vec3 targetExtents = 0.5f * (aabb.maxV - aabb.minV);
        glm::vec3 center = 0.5f * (aabb.minV + aabb.maxV);

        Model* debugCube = new Model(*defaultBox);
        debugCube->transform.SetPosition(center);
        debugCube->transform.SetRotation(glm::vec3(0));
        debugCube->transform.SetScale(targetExtents);
        //render.AddModelsAndShader(debugCube, defaultShader);
        debugCube->meshes[0]->isWireFrame = true;
        debugCube->Draw(*lightShader);



    
}

void ApplicationRenderer::DrawDebugBvhNodeAABB(BvhNode* node)
{
    if (node ==nullptr)
    {
        return;
    }
    //if (node->nodeIndex == recusiveCount)
    //{
    //    DrawDebugModelAABB(node->GetModelAABB());
    //   return;
    //}

    if (node->trianglesIndex.size() != 0)
    {
        DrawDebugModelAABB(node->UpdateAABB());
    }

    if (node->leftChild == nullptr) return;

    DrawDebugBvhNodeAABB(node->leftChild);
    DrawDebugBvhNodeAABB(node->rightChild);


}




 void ApplicationRenderer::SetViewPort(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

 void ApplicationRenderer::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
 {  


     if (action == GLFW_PRESS)
     {
         InputManager::GetInstance().OnKeyPressed(key);
     }
     else if (action == GLFW_RELEASE)
     {
         InputManager::GetInstance().OnKeyReleased(key);
     }
     else if (action == GLFW_REPEAT)
     {
         InputManager::GetInstance().OnKeyHeld(key);
     }

         /*if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
         {
             recusiveCount--;
         }
         if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
         {
             recusiveCount++;
         }
         if (key == GLFW_KEY_R && action == GLFW_PRESS)
         {
             
            spaceshipEntity->model->transform.SetRotation(glm::vec3(spaceshipEntity->model->transform.rotation.x, spaceshipEntity->model->transform.rotation.y + 20, spaceshipEntity->model->transform.rotation.z));
             
         }
         if (key == GLFW_KEY_T && action == GLFW_PRESS)
         {

             spaceshipEntity->model->transform.SetRotation(glm::vec3(spaceshipEntity->model->transform.rotation.x, spaceshipEntity->model->transform.rotation.y - 20, spaceshipEntity->model->transform.rotation.z));

         }
         if (key == GLFW_KEY_Y && action == GLFW_PRESS)
         {

             spaceshipEntity->model->transform.SetRotation(glm::vec3(spaceshipEntity->model->transform.rotation.x + 20, spaceshipEntity->model->transform.rotation.y , spaceshipEntity->model->transform.rotation.z));

         }
         if (key == GLFW_KEY_U && action == GLFW_PRESS)
         {

             spaceshipEntity->model->transform.SetRotation(glm::vec3(spaceshipEntity->model->transform.rotation.x - 20, spaceshipEntity->model->transform.rotation.y , spaceshipEntity->model->transform.rotation.z));

         }

         if (key == GLFW_KEY_I && action == GLFW_PRESS)
         {

             spaceshipEntity->model->transform.SetRotation(glm::vec3(spaceshipEntity->model->transform.rotation.x , spaceshipEntity->model->transform.rotation.y, spaceshipEntity->model->transform.rotation.z + 20));

         }
         if (key == GLFW_KEY_O && action == GLFW_PRESS)
         {

             spaceshipEntity->model->transform.SetRotation(glm::vec3(spaceshipEntity->model->transform.rotation.x , spaceshipEntity->model->transform.rotation.y, spaceshipEntity->model->transform.rotation.z - 20));

         }*/
         
 }

 void ApplicationRenderer::MouseCallBack(GLFWwindow* window, double xposIn, double yposIn)
 {

    float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);
     
        if (firstMouse)
        {

        }

         if (firstMouse)
         {
             lastX = xpos;
             lastY = ypos;
             firstMouse = false;
         }
     
         float xoffset = xpos - lastX;
         float yoffset = lastY - ypos;
     
         lastX = xpos;
         lastY = ypos;
     
         if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
         {
             camera.ProcessMouseMovement(xoffset, yoffset);
         }
 }

 void ApplicationRenderer::MouseScroll(GLFWwindow* window, double xoffset, double yoffset)
 {
     camera.ProcessMouseScroll(static_cast<float>(yoffset));
 }
