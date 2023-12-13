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
   
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
     io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
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


#pragma region MODEL_LOADING


#pragma region TERRAIN


      Model* terrain = new Model("Models/Exam_Models/Terrain.ply");
      terrain->transform.SetPosition(glm::vec3(0, 0, 80));
      render.AddModelsAndShader(terrain, defaultShader);
     

      std::string waterTerrainTexurePath = "Models/Exam_Models/Water.png";
      Texture* waterAlphaTexture = new Texture(waterTerrainTexurePath);
      Model* waterTerrain = new Model("Models/Exam_Models/Water_Terrain.ply");
     

      waterAlphaTexture->type = "opacity_Texture";
     waterTerrain->meshes[0]->meshMaterial->diffuseTexture = waterAlphaTexture;
     waterTerrain->meshes[0]->meshMaterial->alphaTexture = waterAlphaTexture;

      waterTerrain->transform.SetPosition(glm::vec3(-5.60f, 2.9f, -38.5f));
      waterTerrain->transform.SetRotation(glm::vec3(90,90,0));
      waterTerrain->transform.SetScale(glm::vec3(0.1f));
      render.AddTransparentModels(waterTerrain, defaultShader);


     
#pragma endregion

 
    #pragma region Crystals


      std::string crystaltexturePath= "Models/Exam_Models/3D_models/3D_models/Crystals/Crystal.jpg";
    
      Texture* crystalsTexture = new Texture(crystaltexturePath);


      
      Model* crystal = new Model("Models/Exam_Models/3D_models/3D_models/Crystals/SM_Env_Crystals_Cluster_Large_03.ply");

      crystalsTexture->type = "diffuse_Texture";
      crystal->meshes[0]->meshMaterial->diffuseTexture = crystalsTexture;
      crystalsTexture->type = "opacity_Texture";
      crystal->meshes[0]->meshMaterial->alphaTexture = crystalsTexture;

      crystal->transform.SetPosition(glm::vec3(0, 1, 0));
      crystal->transform.SetScale(glm::vec3(0.0125f));


      render.AddTransparentModels(crystal, defaultShader);

      Model* crystal2 = new Model(*crystal);
      crystal2->transform.SetPosition(glm::vec3(0, 1, 0));
      crystal2->transform.SetScale(glm::vec3(0.0125f));

      crystalsTexture->type = "diffuse_Texture";
      crystal2->meshes[0]->meshMaterial->diffuseTexture = crystalsTexture;
      crystalsTexture->type = "opacity_Texture";
      crystal2->meshes[0]->meshMaterial->alphaTexture = crystalsTexture;

      render.AddTransparentModels(crystal2, defaultShader);

      Model* crystal3 = new Model(*crystal);
      crystal3->transform.SetPosition(glm::vec3(0, 1, 0));
      crystal3->transform.SetScale(glm::vec3(0.0125f));

      crystalsTexture->type = "diffuse_Texture";
      crystal3->meshes[0]->meshMaterial->diffuseTexture = crystalsTexture;
      crystalsTexture->type = "opacity_Texture";
      crystal3->meshes[0]->meshMaterial->alphaTexture = crystalsTexture;

      render.AddTransparentModels(crystal3, defaultShader);

      testModel = crystal;
      isTestingModel = true;

    #pragma endregion


     #pragma region MOON




    Model* Moon = new Model("Models/Exam_Models/3D_models/3D_models/CGI_Moon_Kit/UV_Sphere_Cylindrical_UIV_Projection_.ply");
    Moon->transform.SetPosition(glm::vec3(30, 200, -200));
    Moon->transform.SetScale(glm::vec3(20));
    render.AddModelsAndShader(Moon, defaultShader);

#pragma endregion


    #pragma region Floors
    std::string dungeonTexturePath = "Models/Exam_Models/3D_models/3D_models/z_Dungeon_Textures/Dungeons_2_Texture_01_A.png";

    Model* floorType4 = new Model("Models/Exam_Models/3D_models/3D_models/Floors/SM_Env_Dwarf_Floor_08.ply");
    floorType4->meshes[0]->meshMaterial->diffuseTexture = new Texture(dungeonTexturePath);
    floorType4->transform.SetPosition(glm::vec3(0,0.1f,0));
    floorType4->transform.SetScale(glm::vec3(0.025f));
   // render.AddModelsAndShader(floorType4, defaultShader);



    float scaleOffset = 0.025f;
    // bottom  left

    for (size_t i = 0; i < 7; i++)  
    {
        for (size_t j = 0; j < 6; j++)
        {
            
            Model* copyfloorType4 = new Model(*floorType4);
            copyfloorType4->transform.SetPosition(glm::vec3(i*6.25f, 0.2f, j* 6.25f));
            copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyfloorType4, defaultShader);
           
        }
    }

    glm::vec3 offset(100, 0, 0); // bottom  right
    for (size_t i = 0; i < 7; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {

            Model* copyfloorType4 = new Model(*floorType4);
            copyfloorType4->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, j * 6.25f)+ offset);
            copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyfloorType4, defaultShader);

        }
    }


    offset = glm::vec3(100, 0, -56.25f); // top  right
    for (size_t i = 0; i < 7; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {

            Model* copyfloorType4 = new Model(*floorType4);
            copyfloorType4->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, j * 6.25f) + offset);
            copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyfloorType4, defaultShader);

        }
    }

    offset = glm::vec3(0, 0, -56.25f);  // top left
    for (size_t i = 0; i < 7; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {

            Model* copyfloorType4 = new Model(*floorType4);
            copyfloorType4->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, j * 6.25f) + offset);
            copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyfloorType4, defaultShader);

        }
    }

    for (size_t i = 1; i < 4; i++)  // left  connection to bottom to top
    {
        Model* copyfloorType4 = new Model(*floorType4);
        copyfloorType4->transform.SetPosition(glm::vec3(18.75f, 0.2f, i * -6.25f));
        copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyfloorType4, defaultShader);
    }


    offset = glm::vec3(25, 0, 0);  // Left to mmiddle connection
    for (size_t i = 0; i < 5; i++)
    {
        Model* copyfloorType4 = new Model(*floorType4);
        copyfloorType4->transform.SetPosition(glm::vec3(6.25f * i, 0.2f, -12.5f) + offset);
        copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyfloorType4, defaultShader);
    }


    offset = glm::vec3(56.25f, 0, -25); // center
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            Model* copyfloorType4 = new Model(*floorType4);
            copyfloorType4->transform.SetPosition(glm::vec3(6.25f * i, 0.2f, j * 6.25f) + offset);
            copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyfloorType4, defaultShader);
        }
    }

    offset = glm::vec3(87.5f, 0, 0);  // center to right connection

    for (size_t i = 0; i < 5; i++)
    {
        Model* copyfloorType4 = new Model(*floorType4);
        copyfloorType4->transform.SetPosition(glm::vec3(6.25f * i, 0.2f, -12.5f) + offset);
        copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyfloorType4, defaultShader);
    }


    offset = glm::vec3(118.75f, 0, 0);  // bottom right to top connection
    for (size_t i = 0; i < 4; i++)
    {
        Model* copyfloorType4 = new Model(*floorType4);
        copyfloorType4->transform.SetPosition(glm::vec3(0, 0.2f, -6.25f * i) + offset);
        copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyfloorType4, defaultShader);
    }

    offset = glm::vec3(68.75f, 0, -25);   // Center to top center connection
    Model* copyfloorType4 = new Model(*floorType4);
    copyfloorType4->transform.SetPosition(glm::vec3(0, 0.2f, -6.25f * 1) + offset);
    copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
    render.AddModelsAndShader(copyfloorType4, defaultShader);

    offset = glm::vec3(62.5f, 0, -37.5f);  // Top center
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            Model* copyfloorType4 = new Model(*floorType4);
            copyfloorType4->transform.SetPosition(glm::vec3(6.25f * i, 0.2f, j * -6.25f) + offset);
            copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyfloorType4, defaultShader);
        }
    }

    offset = glm::vec3(62.5f, 0, -37.5f);  // Top center
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            Model* copyfloorType4 = new Model(*floorType4);
            copyfloorType4->transform.SetPosition(glm::vec3(6.25f * i, 0.2f, j * -6.25f) + offset);
            copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyfloorType4, defaultShader);
        }
    }



    offset = glm::vec3(43.75f, 0, 0);  // Top left to top center connection
    for (size_t i = 0; i < 3; i++)
    {
        Model* copyfloorType4 = new Model(*floorType4);
        copyfloorType4->transform.SetPosition(glm::vec3(6.25f * i, 0.2f, -43.75f) + offset);
        copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyfloorType4, defaultShader);
    }

    offset = glm::vec3(81.25f, 0, 0);  // Top left to top center connection
    for (size_t i = 0; i < 3; i++)
    {
        Model* copyfloorType4 = new Model(*floorType4);
        copyfloorType4->transform.SetPosition(glm::vec3(6.25f * i, 0.2f, -43.75f) + offset);
        copyfloorType4->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyfloorType4, defaultShader);
    }


    //  Main Gate  Floor
    Model* mainGateFloor = new Model(*floorType4);
    mainGateFloor->transform.SetPosition(glm::vec3(68.75f,0.2f,-6.5f));
    mainGateFloor->transform.SetScale(glm::vec3(0.0125f));
    render.AddModelsAndShader(mainGateFloor, defaultShader);


#pragma endregion

    #pragma region Walls

    Model* WallType3 = new Model("Models/Exam_Models/3D_models/3D_models/Walls/SM_Env_Dwarf_Wall_03.ply");
    WallType3->meshes[0]->meshMaterial->diffuseTexture = new Texture(dungeonTexturePath);

    Model* WallArchWayType2 = new Model("Models/Exam_Models/3D_models/3D_models/Walls/SM_Env_Dwarf_Wall_Archway_02.ply");
    WallArchWayType2->meshes[0]->meshMaterial->diffuseTexture = new Texture(dungeonTexturePath);


    // Bottom left GRID (top and bottom walls)
    for (size_t i = 0; i < 7; i++)
{
    glm::vec3 position = glm::vec3(i * 6.25f, 0.2f, 0);

    if (i == 3)
    {
        
        Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
        copyWallArchWayType2->transform.SetPosition(position + glm::vec3(-6.25f, 0, -6.25f));
        copyWallArchWayType2->transform.SetRotation(glm::vec3(0, 180, 0));
        copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallArchWayType2, defaultShader);


        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(position + glm::vec3(0, 0, 31.25f));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }
    else
    {
        
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(position + glm::vec3(0, 0, 31.25f));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);

        copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(position + glm::vec3(-6.25f, 0, -6.25f));
        copyWallType3->transform.SetRotation(glm::vec3(0, 180, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }
}  

    offset = glm::vec3(-6.25f, 0, 0); // Left walls
    for (size_t i = 0; i < 6; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0,-90,0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }

    offset = glm::vec3(37.5f, 0, -6.25f); // Right walls
    for (size_t i = 0; i < 6; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0, 90, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }


    // Bottom left to top Left  walls connection
    for (size_t i = 0; i < 3; i++) 

    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 offset;

        offset = glm::vec3(12.5f, 0, -18.75f);
        position = glm::vec3(0, 0.2f, i * 6.25f) + offset;
        rotation = glm::vec3(0, -90, 0);

        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(position);
        copyWallType3->transform.SetRotation(rotation);
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);

        offset = glm::vec3(18.75f, 0, -25);
        position = glm::vec3(0, 0.2f, i * 6.25f) + offset;
        rotation = glm::vec3(0, 90, 0);

       
        if (i == 1)
        {
            Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
            copyWallArchWayType2->transform.SetPosition(position);
            copyWallArchWayType2->transform.SetRotation(rotation);
            copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {

            copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(position);
            copyWallType3->transform.SetRotation(rotation);
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }
    }



    // Top left GRID  (Down walls)
    offset = glm::vec3(0, 0, -25); 
    for (size_t i = 0; i < 7; i++)
    {
        if (i == 3)
        {
            Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
            copyWallArchWayType2->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
            copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }

    }


    offset = glm::vec3(-6.25f, 0, -62.5f);  //Up  walls
    for (size_t i = 0; i < 7; i++)
    {
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
            copyWallType3->transform.SetRotation(glm::vec3(0, 180, 0));
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
    }


    // Left and right walls
    for (size_t i = 0; i < 6; i++)
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 offset;

        
        offset = glm::vec3(-6.25f, 0, -56.25f);
        position = glm::vec3(0, 0.2f, i * 6.25f) + offset;
        rotation = glm::vec3(0, -90, 0);

        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(position);
        copyWallType3->transform.SetRotation(rotation);
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);

       
        offset = glm::vec3(37.5f, 0, -62.5f);
        position = glm::vec3(0, 0.2f, i * 6.25f) + offset;
        rotation = glm::vec3(0, 90, 0);

        if (i == 2)
        {
            Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
            copyWallArchWayType2->transform.SetPosition(position);
            copyWallArchWayType2->transform.SetRotation(rotation);
            copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {
          
            copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(position);
            copyWallType3->transform.SetRotation(rotation);
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }
    } 


    //  Top and Bottom Connection to Middle Room Walls
    for (size_t i = 0; i < 5; i++)
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 offset;


        offset = glm::vec3(25, 0, -12.5f);
        position = glm::vec3(i * 6.25f, 0.2f, 0) + offset;
        rotation = glm::vec3(0, 0, 0);  

        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(position);
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);

       
        offset = glm::vec3(18.75f, 0, -18.75f);
        position = glm::vec3(i * 6.25f, 0.2f, 0) + offset;
        rotation = glm::vec3(0, 180, 0);

        copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(position);
        copyWallType3->transform.SetRotation(rotation);
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }


    // Middle ROOM (Down and Up walls)
    for (size_t i = 0; i < 5; i++)
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 offset;

      
        offset = glm::vec3(56.25f, 0, -12.5f);
        position = glm::vec3(i * 6.25f, 0.2f, 0) + offset;
        rotation = glm::vec3(0, 0, 0);

        if (i == 2)
        {
            Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
            copyWallArchWayType2->transform.SetPosition(position);
            copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {
         
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(position);
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }

     
        offset = glm::vec3(50, 0, -31.25f);
        position = glm::vec3(i * 6.25f, 0.2f, 0) + offset;
        rotation = glm::vec3(0, 180, 0);

        
        if (i == 2)
        {
            Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
            copyWallArchWayType2->transform.SetPosition(position);
            copyWallArchWayType2->transform.SetRotation(rotation);
            copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {
            
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(position);
            copyWallType3->transform.SetRotation(rotation);
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }
    }


    // Middle ROOM (LEFT and RIGHT walls)
    for (size_t i = 0; i < 3; i++)
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 offset;

        // Case 1: offset = glm::vec3(50, 0, -25);
        offset = glm::vec3(50, 0, -25);
        position = glm::vec3(0, 0.2f, i * 6.25f) + offset;
        rotation = glm::vec3(0, -90, 0);

        // Check if it's the position for the archway
        if (i == 2)
        {
            // Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
            // copyWallArchWayType2->transform.SetPosition(position);
            // copyWallArchWayType2->transform.SetRotation(rotation);
            // copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
            // render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {
            // Regular wall
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(position);
            copyWallType3->transform.SetRotation(rotation);
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }

        // Case 2: offset = glm::vec3(81.25f, 0, -31.25f);
        offset = glm::vec3(81.25f, 0, -31.25f);
        position = glm::vec3(0, 0.2f, i * 6.25f) + offset;
        rotation = glm::vec3(0, 90, 0);

        // Check if it's the position for the archway
        if (i == 2)
        {
            // Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
            // copyWallArchWayType2->transform.SetPosition(position);
            // copyWallArchWayType2->transform.SetRotation(rotation);
            // copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
            // render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {
            // Regular wall
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(position);
            copyWallType3->transform.SetRotation(rotation);
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }
    }


    // Middle ROOM to left bottom Conenction Walls
    for (size_t i = 0; i < 5; i++)
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 offset;


        offset = glm::vec3(87.5f, 0, -12.5f);
        position = glm::vec3(i * 6.25f, 0.2f, 0) + offset;
        rotation = glm::vec3(0, 0, 0);


        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(position);
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);

        offset = glm::vec3(81.25f, 0, -18.75f);
        position = glm::vec3(i * 6.25f, 0.2f, 0) + offset;
        rotation = glm::vec3(0, 180, 0);

         copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(position);
        copyWallType3->transform.SetRotation(rotation);
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);

    }

    // Right bottom ROOM to Righ top ROOM  connection  to left bottom Conenction Walls
    for (size_t i = 0; i < 3; i++)
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 offset;

        offset = glm::vec3(118.75f, 0, -25);
        position = glm::vec3(0, 0.2f, i * 6.25f) + offset;
        rotation = glm::vec3(0, 90, 0);

        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(position);
        copyWallType3->transform.SetRotation(rotation);
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);

      
        offset = glm::vec3(112.5, 0, -18.75);
        position = glm::vec3(0, 0.2f, i * 6.25f) + offset;
        rotation = glm::vec3(0, -90, 0);

       
        if (i == 1)
        {
              Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
              copyWallArchWayType2->transform.SetPosition(position);
              copyWallArchWayType2->transform.SetRotation(rotation);
              copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
             render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(position);
            copyWallType3->transform.SetRotation(rotation);
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }
    }


    // Right Bottom Room Down wall
    offset = glm::vec3(100, 0, 31.5f);
    for (size_t i = 0; i < 7; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(i*6.25f,0.2f,0) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }


    // Right Bottom Room Up wall
    offset = glm::vec3(93.5f, 0, -6.25f);
    for (size_t i = 0; i < 7; i++)
    {
        if (i == 3)
        {
            Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
            copyWallArchWayType2->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
            copyWallArchWayType2->transform.SetRotation(glm::vec3(0, 180, 0));
            copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
            copyWallType3->transform.SetRotation(glm::vec3(0, 180, 0));
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }
       
    }


    // Right Bottom Room left wall
    offset = glm::vec3(93.5f,0, 0);

    for (size_t i = 0; i < 6; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0, -90, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }


    // Right Bottom Room Right wall
    offset = glm::vec3(137.25f, 0, -6.25f);

    for (size_t i = 0; i < 6; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0, 90, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }




    // RIGHT TOP room  (right wall) 
    offset = glm::vec3(137.25f, 0, -62.5f);

    for (size_t i = 0; i < 6; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0, 90, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }

    // RIGHT TOP room  (left wall) 
    offset = glm::vec3(93.5f, 0, -56.25);

    for (size_t i = 0; i < 6; i++)
    {
        if (i ==2)
        {
            Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
            copyWallArchWayType2->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
            copyWallArchWayType2->transform.SetRotation(glm::vec3(0, 90, 0));
            copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
            copyWallType3->transform.SetRotation(glm::vec3(0, -90, 0));
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }
        
    }


    // RIGHT TOP room  (UP wall) 
    offset = glm::vec3(93.5f, 0, -62.5f);
   
    for (size_t i = 0; i < 7; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0,180,0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }

    // RIGHT TOP room  (DOWN wall) 
    offset = glm::vec3(100, 0, -25);

    for (size_t i = 0; i < 7; i++)
    {
        if (i == 3)
        {
            Model* copyWallArchWayType2 = new Model(*WallArchWayType2);
            copyWallArchWayType2->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
            copyWallArchWayType2->transform.SetRotation(glm::vec3(0, 0, 0));
            copyWallArchWayType2->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallArchWayType2, defaultShader);
        }
        else
        {
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
            copyWallType3->transform.SetRotation(glm::vec3(0, 0, 0));
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }
       
    }




    // TOP left to Top middle connection walls up

    offset = glm::vec3(81.25f, 0, -43.75f);
    for (size_t i = 0; i < 3; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0, 0, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }

    // TOP left to Top middle connection walls DOWN
    offset = glm::vec3(75, 0, -50);
    for (size_t i = 0; i < 3; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0, 180, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }



    // TOP Right to Top middle up room connection walls up

    offset = glm::vec3(43.75f, 0, -43.75f);
    for (size_t i = 0; i < 3; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0, 0, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }

    // TOP Right to Top middle up room connection walls Down
    offset = glm::vec3(37.5f, 0, -50);
    for (size_t i = 0; i < 3; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0, 180, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }


    // Middle TopRoom LEFT
    offset = glm::vec3(56.25f, 0, -50);
    for (size_t i = 0; i < 3; i++)
    {
        if (i ==1)
        {

        }
        else
        {
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
            copyWallType3->transform.SetRotation(glm::vec3(0, -90, 0));
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }
       
    }

    // Middle top room RIGHT
    offset = glm::vec3(75, 0, -56.25f);
    for (size_t i = 0; i < 3; i++)
    {
        if (i == 1)
        {

        }
        else
        {
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
            copyWallType3->transform.SetRotation(glm::vec3(0, 90, 0));
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }

    }


    // Middle top room UP
    offset = glm::vec3(56.25f, 0, -56.25f);
    for (size_t i = 0; i < 3; i++)
    {
       
        
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
            copyWallType3->transform.SetRotation(glm::vec3(0, 180, 0));
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        

    }


    // Middle top room DOWN
    offset = glm::vec3(62.5f, 0, -37.5f);
    for (size_t i = 0; i < 3; i++)
    {
        if (i == 1)
        {

        }
        else
        {
            Model* copyWallType3 = new Model(*WallType3);
            copyWallType3->transform.SetPosition(glm::vec3(i * 6.25f, 0.2f, 0) + offset);
            copyWallType3->transform.SetRotation(glm::vec3(0, 0, 0));
            copyWallType3->transform.SetScale(glm::vec3(0.0125f));
            render.AddModelsAndShader(copyWallType3, defaultShader);
        }

    }


    
    // SINGLE path left middle center to top connection (RIGHT)

    Model* singlecopyWallType3 = new Model(*WallType3);
    singlecopyWallType3->transform.SetPosition(glm::vec3(68.75f,0.2f,-37.5f));
    singlecopyWallType3->transform.SetRotation(glm::vec3(0, 90, 0));
    singlecopyWallType3->transform.SetScale(glm::vec3(0.0125f));
    render.AddModelsAndShader(singlecopyWallType3, defaultShader);

    // SINGLE path left middle center to midle top connection (LEFT)
    singlecopyWallType3 = new Model(*WallType3);
    singlecopyWallType3->transform.SetPosition(glm::vec3(62.5f, 0.2f, -31.25f));
    singlecopyWallType3->transform.SetRotation(glm::vec3(0, -90, 0));
    singlecopyWallType3->transform.SetScale(glm::vec3(0.0125f));
    render.AddModelsAndShader(singlecopyWallType3, defaultShader);

#pragma endregion

    #pragma region Torches

    Model* plane = new Model("Models/Plane/Plane.obj");
    std::string texurepath = "Models/Plane/Fire.png";
    Texture* FireTexture = new Texture(texurepath);

    FireTexture->type = "diffuse_Texture";

    plane->meshes[0]->meshMaterial->diffuseTexture = FireTexture;
    plane->transform.SetPosition(glm::vec3(0, 1, 0));
    plane->transform.SetRotation(glm::vec3(90, 0, 0));

    render.AddModelsAndShader(plane, defaultShader);


    Model* Torch = new Model("Models/Exam_Models/3D_models/3D_models/Torches/SM_Prop_Dwarf_Torch_06.ply");
    Torch->meshes[0]->meshMaterial->diffuseTexture = new Texture(dungeonTexturePath);
    ////Light 1
    Torch->transform.SetPosition(glm::vec3(0, 0.4f, 30));
    Torch->transform.SetScale(glm::vec3(0.05f));
    render.AddModelsAndShader(Torch, defaultShader);

    Model* torch1PointLight = new Model(*Sphere);
    glm::vec3 yOffset(0, 2, 0);
    torch1PointLight->transform.SetPosition(Torch->transform.position+ yOffset);
    torch1PointLight->transform.SetScale(glm::vec3(0.5f));
    render.AddModelsAndShader(torch1PointLight, lightShader);

    Light TorchLight1;
    TorchLight1.Initialize(torch1PointLight,POINT_LIGHT, 0.5f);
    TorchLight1.ambient = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight1.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight1.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight1.quadratic = 0.01f;
    TorchLight1.SetColor(1, 0.6f, 0.2, 1);

    lightManager.AddNewLight(TorchLight1);
    int lastLightIndex= lightManager.LightLightIndex();

    FireScale* fire1 = new FireScale(render, defaultShader, FireTexture);

    fire1->LoadModel(plane, torch1PointLight->transform.position);

    ListfireBehaviour.push_back(fire1);


    ////Light 2
    Model* Torch2 = new Model(*Torch);
    Torch2->transform.SetPosition(glm::vec3(30, 0.4f, 2));
    Torch2->transform.SetScale(glm::vec3(0.05f));
    render.AddModelsAndShader(Torch2, defaultShader);

    Model* torch2DebugModel = new Model(*Sphere);
    torch2DebugModel->transform.SetPosition(Torch2->transform.position + yOffset);
    torch2DebugModel->transform.SetScale(glm::vec3(0.5f));
    render.AddModelsAndShader(torch2DebugModel, lightShader);

    Light TorchLight2;
    TorchLight2.Initialize(torch2DebugModel, POINT_LIGHT, 0.5f);
    TorchLight2.ambient = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight2.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight2.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight2.quadratic = 0.01f;
    TorchLight2.SetColor(1, 0.6f, 0.2, 1);

    TorchLight2.SetColor(glm::vec4(1,0.5f,0,1));

    lightManager.AddNewLight(TorchLight2);

    FireScale* fire2 = new FireScale(render, defaultShader, FireTexture);

    fire2->LoadModel(plane, torch2DebugModel->transform.position);

    ListfireBehaviour.push_back(fire2);

    ////Light 3

    Model* Torch3 = new Model(*Torch);
    Torch3->transform.SetPosition(glm::vec3(30, 0.4f, -60.0f));
    Torch3->transform.SetScale(glm::vec3(0.05f));
    render.AddModelsAndShader(Torch3, defaultShader);

    Model* torch3DebugModel = new Model(*Sphere);
    torch3DebugModel->transform.SetPosition(Torch3->transform.position + yOffset);
    torch3DebugModel->transform.SetScale(glm::vec3(0.5f));
    render.AddModelsAndShader(torch3DebugModel, lightShader);

    Light TorchLight3;
    TorchLight3.Initialize(torch3DebugModel, POINT_LIGHT, 0.5f);
    TorchLight3.SetColor(glm::vec4(1, 0.5f, 0, 1));
    TorchLight3.ambient = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight3.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight3.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight3.quadratic = 0.01f;
    TorchLight3.SetColor(1, 0.6f, 0.2, 1);
    lightManager.AddNewLight(TorchLight3);


    FireScale* fire3 = new FireScale(render, defaultShader, FireTexture);

    fire3->LoadModel(plane, torch3DebugModel->transform.position);

    ListfireBehaviour.push_back(fire3);


    ////Light 4
    Model* Torch4 = new Model(*Torch);
    Torch4->transform.SetPosition(glm::vec3(0, 0.4f, -27));
    Torch4->transform.SetScale(glm::vec3(0.05f));
    render.AddModelsAndShader(Torch4, defaultShader);

    Model* torch4DebugModel = new Model(*Sphere);
    torch4DebugModel->transform.SetPosition(Torch4->transform.position + yOffset);
    torch4DebugModel->transform.SetScale(glm::vec3(0.5f));
    render.AddModelsAndShader(torch4DebugModel, lightShader);

    Light TorchLight4;
    TorchLight4.Initialize(torch4DebugModel, POINT_LIGHT, 0.5f);
    TorchLight4.SetColor(glm::vec4(1, 0.5f, 0, 1));

    TorchLight4.ambient = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight4.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight4.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight4.quadratic = 0.01f;
    TorchLight4.SetColor(1, 0.6f, 0.2, 1);

    lightManager.AddNewLight(TorchLight4);

    FireScale* fire4 = new FireScale(render, defaultShader, FireTexture);

    fire4->LoadModel(plane, torch4DebugModel->transform.position);

    ListfireBehaviour.push_back(fire4);


    ////Light 5
    Model* Torch5 = new Model(*Torch);
    Torch5->transform.SetPosition(glm::vec3(60.100f, 0.4f, -52));
    Torch5->transform.SetScale(glm::vec3(0.05f));
    render.AddModelsAndShader(Torch5, defaultShader);

    Model* torch5DebugModel = new Model(*Sphere);
    torch5DebugModel->transform.SetPosition(Torch5->transform.position + yOffset);
    torch5DebugModel->transform.SetScale(glm::vec3(0.5f));
    render.AddModelsAndShader(torch5DebugModel, lightShader);

    Light TorchLight5;
    TorchLight5.Initialize(torch5DebugModel, POINT_LIGHT, 0.75f);
    TorchLight5.SetColor(glm::vec4(1, 0.5f, 0, 1));

    TorchLight5.ambient = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight5.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight5.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight5.quadratic = 0.01f;
    TorchLight5.SetColor(1, 0.6f, 0.2, 1);

    lightManager.AddNewLight(TorchLight5);


    FireScale* fire5 = new FireScale(render, defaultShader, FireTexture);

    fire5->LoadModel(plane, torch5DebugModel->transform.position);

    ListfireBehaviour.push_back(fire5);



    ////Light 6

    Model* Torch6 = new Model(*Torch);
    Torch6->transform.SetPosition(glm::vec3(72.700f, 0.4f, -41.200f));
    Torch6->transform.SetScale(glm::vec3(0.05f));
    render.AddModelsAndShader(Torch6, defaultShader);

    Model* torch6DebugModel = new Model(*Sphere);
    torch6DebugModel->transform.SetPosition(Torch6->transform.position + yOffset);
    torch6DebugModel->transform.SetScale(glm::vec3(0.5f));
    render.AddModelsAndShader(torch6DebugModel, lightShader);

    Light TorchLight6;
    TorchLight6.Initialize(torch6DebugModel, POINT_LIGHT, 0.75f);
    
    TorchLight6.SetColor(glm::vec4(1, 0.5f, 0, 1));

    TorchLight6.ambient = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight6.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight6.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight6.quadratic = 0.01f;
    TorchLight6.SetColor(1, 0.6f, 0.2, 1);

    lightManager.AddNewLight(TorchLight6);

    FireScale* fire6 = new FireScale(render, defaultShader, FireTexture);

    fire6->LoadModel(plane, torch6DebugModel->transform.position);

    ListfireBehaviour.push_back(fire6);


    Model* Torch7 = new Model(*Torch);
    Torch7->transform.SetPosition(glm::vec3(115.0f, 0.4f,-46.20f));
    Torch7->transform.SetScale(glm::vec3(0.05f));
    render.AddModelsAndShader(Torch7, defaultShader);

    Model* torch7DebugModel = new Model(*Sphere);
    torch7DebugModel->transform.SetPosition(Torch7->transform.position + yOffset);
    torch7DebugModel->transform.SetScale(glm::vec3(0.5f));
    render.AddModelsAndShader(torch7DebugModel, lightShader);

    Light TorchLight7;
    TorchLight7.Initialize(torch7DebugModel, POINT_LIGHT, 0.75f);
    TorchLight7.SetColor(glm::vec4(1, 0.5f, 0, 1));

    TorchLight7.ambient = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight7.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight7.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight7.quadratic = 0.01f;
    TorchLight7.SetColor(1, 0.6f, 0.2, 1);

    lightManager.AddNewLight(TorchLight7);

    FireScale* fire7 = new FireScale(render, defaultShader, FireTexture);

    fire7->LoadModel(plane, torch7DebugModel->transform.position);

    ListfireBehaviour.push_back(fire7);


    Model* Torch8 = new Model(*Torch);
    Torch8->transform.SetPosition(glm::vec3(130.0f, 0.4f, 13.8f));
    Torch8->transform.SetScale(glm::vec3(0.05f));
    render.AddModelsAndShader(Torch8, defaultShader);


    Model* torch8DebugModel = new Model(*Sphere);
    torch8DebugModel->transform.SetPosition(Torch8->transform.position + yOffset);
    torch8DebugModel->transform.SetScale(glm::vec3(0.5f));
    render.AddModelsAndShader(torch8DebugModel, lightShader);

    Light TorchLight8;
    TorchLight8.Initialize(torch8DebugModel, POINT_LIGHT, 0.75f);
    TorchLight8.SetColor(glm::vec4(1, 0.5f, 0, 1));


    TorchLight8.ambient = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight8.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight8.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight8.quadratic = 0.01f;
    TorchLight8.SetColor(1, 0.6f, 0.2, 1);

    lightManager.AddNewLight(TorchLight8);

    FireScale* fire8 = new FireScale(render, defaultShader, FireTexture);

    fire8->LoadModel(plane, torch8DebugModel->transform.position);

    ListfireBehaviour.push_back(fire8);


    Model* Torch9 = new Model(*Torch);
    Torch9->transform.SetPosition(glm::vec3(100.0f, 0.4f, 13.80f));
    Torch9->transform.SetScale(glm::vec3(0.05f));
    render.AddModelsAndShader(Torch9, defaultShader);

    Model* torch9DebugModel = new Model(*Sphere);
    torch9DebugModel->transform.SetPosition(Torch9->transform.position + yOffset);
    torch9DebugModel->transform.SetScale(glm::vec3(0.5f));


    render.AddModelsAndShader(torch9DebugModel, lightShader);

    Light TorchLight9;
    TorchLight9.Initialize(torch9DebugModel, POINT_LIGHT, 0.75f);
    TorchLight9.SetColor(glm::vec4(1, 0.5f, 0, 1));

    TorchLight9.ambient = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight9.diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight9.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1);
    TorchLight9.quadratic = 0.01f;
    TorchLight9.SetColor(1, 0.6f, 0.2, 1);

    lightManager.AddNewLight(TorchLight9);


    FireScale* fire9 = new FireScale(render, defaultShader, FireTexture);

    fire9->LoadModel(plane, torch8DebugModel->transform.position);

    ListfireBehaviour.push_back(fire9);

#pragma endregion


#pragma region BEHOLDER

    Beholder* beholder = new Beholder(&render, defaultShader);

    beholder->LoadBeholderModel();
#pragma endregion



#pragma endregion



   
#pragma region Lights

    Model* directionLightModel = new Model(*Sphere);
    directionLightModel->transform.SetPosition(glm::vec3(Moon->transform.position));
    directionLightModel->transform.SetRotation(glm::vec3(0, 0, 0));
    directionLightModel->transform.SetScale(glm::vec3(0.1f));

    Light directionLight;
    directionLight.lightType = LightType::POINT_LIGHT;
    directionLight.lightModel = directionLightModel;
    directionLight.ambient =  glm::vec4(50, 50, 50, 1.0f);
    directionLight.diffuse =  glm::vec4(30, 30, 30, 1.0f);
    directionLight.specular = glm::vec4(30, 30, 30, 1.0f);
    directionLight.quadratic = 0.001f;
    directionLight.intensity = 0.75f;


    Model* beholderSpotLight1Model = new Model(*Sphere);
    beholderSpotLight1Model->transform.SetPosition(glm::vec3(Moon->transform.position));
    beholderSpotLight1Model->transform.SetRotation(glm::vec3(0, 0, 0));
    beholderSpotLight1Model->transform.SetScale(glm::vec3(0.1f));

    render.AddModelsAndShader(beholderSpotLight1Model, lightShader);

    Light beholderSpotLight1;
    beholderSpotLight1.Initialize(beholderSpotLight1Model, SPOT_LIGHT, 0.75f);
    beholderSpotLight1.ambient = glm::vec4(2, 2, 2, 1.0f);
    beholderSpotLight1.diffuse = glm::vec4(2, 2, 2, 1.0f);
    beholderSpotLight1.specular = glm::vec4(2, 2, 2, 1.0f);
    beholderSpotLight1.quadratic = 0.001f;
    beholderSpotLight1.linear = 0.001f;
    beholderSpotLight1.constant = 0.001f;
    beholderSpotLight1.cutOffAngle = 12.5f;
    beholderSpotLight1.outerCutOffAngle = 15;
    
    lightManager.AddNewLight(beholderSpotLight1);
   

#pragma endregion




     render.selectedModel = nullptr;

     render.AddModelsAndShader(directionLightModel, lightShader);
     //LightRenderer


     lightManager.AddNewLight(directionLight);
     
   

     defaultShader->Bind();
     //defaultShader->setInt("skybox", 0);


    
}

void ApplicationRenderer::PreRender()
{
    glm::mat4 _projection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)WindowHeight, 0.1f, 1000.0f);
    glm::mat4 _view = camera.GetViewMatrix();
    glm::mat4 _skyboxview = glm::mat4(glm::mat3(camera.GetViewMatrix()));

    glDepthFunc(GL_LEQUAL);
    SkyboxShader->Bind();
    SkyboxShader->setMat4("view", _skyboxview);
    SkyboxShader->setMat4("projection", _projection);

     skybox->Skyboxrender();
    glDepthFunc(GL_LESS);


    defaultShader->Bind();
  //  lightManager.UpdateUniformValues(defaultShader->ID);
    lightManager.UpdateUniformValuesToShader(defaultShader);

    defaultShader->setMat4("projection", _projection);
    defaultShader->setMat4("view", _view);
    defaultShader->setVec3("viewPos", camera.transform.position.x, camera.transform.position.y, camera.transform.position.z);
    defaultShader->setFloat("time", scrollTime);
    defaultShader->setBool("isDepthBuffer", false);

    lightShader->Bind();
    lightShader->setVec3("objectColor", glm::vec3(1, 1, 1));
    lightShader->setMat4("projection", _projection);
    lightShader->setMat4("view", _view);

    StencilShader->Bind();
    StencilShader->setMat4("projection", _projection);
    StencilShader->setMat4("view", _view);
}

void ApplicationRenderer::ImGUIRender()
{
    ImGui::Begin("Media Player Lite!");
    // ImGui::SetWindowFontScale(2.0f);
    ImGui::SetWindowSize(ImVec2(800, 800));

    //add a intro text
    ImGui::Text("KAIZOKU ENGINE");
   
    ImGui::NewLine();
    ImGui::PushItemWidth(100);
    ImGui::InputFloat("X Position", &xPos, 0.1f);
    ImGui::SameLine();
    ImGui::InputFloat("Y Position", &yPos, 0.1f);
    ImGui::SameLine();
    ImGui::InputFloat("Z Position", &zPos, 0.1f);
    ImGui::Text("POSITION");
    CheckingValues(testModel, xPos, yPos, zPos);

    //framerate
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
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


        ImGUIRender();

        PreRender(); //Update call BEFORE  DRAW



        /* ScrollShader->Bind();
        // ScrollShader->setMat4("ProjectionMatrix", _projection);*/
        

         render.Draw();
       

         PostRender(); // Update Call AFTER  DRAW


         ImGui::Render();
         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    glfwTerminate();
}

void ApplicationRenderer::PostRender()
{
    for (size_t i = 0; i < ListfireBehaviour.size(); i++)
    {
        ListfireBehaviour[i]->Update(deltaTime);

        float calulateFlicker = std::abs(ListfireBehaviour[i]->CalculateFlickering(deltaTime) * 10.0f);
        lightManager.lightList[i].intensity = calulateFlicker ;
    }

    
}

void ApplicationRenderer::Clear()
{
    GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 0.1f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));


    ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
}

void ApplicationRenderer::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed=70;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime * cameraSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime * cameraSpeed);

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime * cameraSpeed);

    }

 


}

void ApplicationRenderer::CheckingValues(Model* testModel, float x, float y, float z)
{
    if (isTestingModel && testModel!=nullptr)
    {
        testModel->transform.SetPosition(glm::vec3(x,y,z));

    }
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
