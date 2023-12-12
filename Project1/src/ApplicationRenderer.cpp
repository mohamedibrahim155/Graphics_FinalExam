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

    Model* terrain = new Model("Models/Exam_Models/Terrain.ply");
    terrain->transform.SetPosition(glm::vec3(0, 0, 80));
    render.AddModelsAndShader(terrain, defaultShader);

    Model* Moon = new Model("Models/Exam_Models/3D_models/3D_models/CGI_Moon_Kit/UV_Sphere_Cylindrical_UIV_Projection_.ply");
    Moon->transform.SetPosition(glm::vec3(30, 200, -200));
    Moon->transform.SetScale(glm::vec3(20));
    render.AddModelsAndShader(Moon, defaultShader);




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


    offset = glm::vec3(100, 0, 31.5f);
    for (size_t i = 0; i < 7; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(i*6.25f,0.2f,0) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }


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


    offset = glm::vec3(93.5f,0, 0);

    for (size_t i = 0; i < 6; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0, -90, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }


    offset = glm::vec3(137.25f, 0, -6.25f);

    for (size_t i = 0; i < 6; i++)
    {
        Model* copyWallType3 = new Model(*WallType3);
        copyWallType3->transform.SetPosition(glm::vec3(0, 0.2f, i * 6.25f) + offset);
        copyWallType3->transform.SetRotation(glm::vec3(0, 90, 0));
        copyWallType3->transform.SetScale(glm::vec3(0.0125f));
        render.AddModelsAndShader(copyWallType3, defaultShader);
    }

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

#pragma endregion


     render.selectedModel = nullptr;

     render.AddModelsAndShader(directionLightModel, lightShader);
     //LightRenderer


     lightManager.AddNewLight(directionLight);
     lightManager.SetUniforms(defaultShader->ID);
   

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
    lightManager.UpdateUniformValues(defaultShader->ID);


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
