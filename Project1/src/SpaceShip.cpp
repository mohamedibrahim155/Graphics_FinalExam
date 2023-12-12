#include"SpaceShip.h"

SpaceShip::SpaceShip(GraphicsRender& render, Shader* shader, PhysicsEngine& engine, Camera& camera)
{
	this->render = &render;
	this->defaultshader = shader;
	this->engine = &engine;
	this->camera = &camera;

	InputManager::GetInstance().AddListener(this);
}

SpaceShip::SpaceShip()
{
}

SpaceShip::~SpaceShip()
{
}

void SpaceShip:: LoadModel()
{
	model = new Model("Models/Spaceship/Ship2.obj");
	model->id = "SpaceShip";
	model->transform.SetPosition(glm::vec3(0, 5, 2));
	model->transform.SetScale(glm::vec3(0.5f));
	render->AddModelsAndShader(model, defaultshader);


	SpaceShipPhysics = new PhysicsObject(model);

	SpaceShipPhysics->Initialize(SPHERE, true, DYNAMIC);
	SpaceShipPhysics->gravityValue =0;

	SpaceShipPhysics->DoCollisionCall([this](PhysicsObject* other)
		{
			glm::vec3 collisionNormal = engine->collisionNormals[0];
			glm::vec3 velocity = SpaceShipPhysics->velocity;

			// Calculate the dot product of velocity and collision normal
			float dotProduct = glm::dot(collisionNormal, velocity);

			// If the dot product is negative, it means the velocity is in the opposite direction of the collision normal
			if (dotProduct < 0.0f)
			{
				// Project velocity onto collision normal to get the component in the direction of the collision normal
				glm::vec3 velocityComponent = dotProduct * collisionNormal;

				// Update velocity to keep only the component in the direction of the collision normal
				SpaceShipPhysics->velocity = velocityComponent;

				// Print a message indicating a collision
				std::cout << "Spaceship collided. Velocity clamped." << std::endl;
			}
		});

	engine->AddPhysicsObjects(SpaceShipPhysics);



}

void SpaceShip::Update(float deltaTime)
{

	glm::vec3 forward = model->transform.GetForward();
	camera->SetCameraPosition(model->transform.position + forward  *followDistance + glm::vec3(0, yoffset, 0));
	DrawAABBCollision(SpaceShipPhysics);
}

void SpaceShip::DrawAABBCollision(PhysicsObject* physicsObject)
{
	if (!isDebugAAABDraw)
	{
		return;
	}
	std::vector<cAABB> collisionAABBs = physicsObject->GetCollisionAABBs();

	for (size_t i = 0; i < collisionAABBs.size(); i++)
	{
		modelAABB m_Aabb = GetGraphicsAabb(collisionAABBs[i]);
		render->DrawAABB(m_Aabb);
	}
}

void SpaceShip::SpaceShipInputs(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{

		SpaceShipPhysics->velocity = -model->transform.GetForward() *speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		SpaceShipPhysics->velocity = model->transform.GetForward() * speed;
	}
	else   if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		SpaceShipPhysics->velocity = -model->transform.GetRight() * speed;
	}
	else  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{

		SpaceShipPhysics->velocity = model->transform.GetRight() * speed;
	}
	else if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		//Direction = model->transform.GetUp();
		SpaceShipPhysics->velocity = model->transform.GetUp() * speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		//Direction = -model->transform.GetUp();

		SpaceShipPhysics->velocity = -model->transform.GetUp() * speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		
		SpaceShipPhysics->velocity = glm::vec3(0);
		
	}
	

	
}

void SpaceShip::OnKeyPressed(const int& key)
{
	if (key == GLFW_KEY_W)
	{
		SpaceShipPhysics->velocity = -model->transform.GetForward() * speed;
	}
	else if(key == GLFW_KEY_S)
	{
		SpaceShipPhysics->velocity = model->transform.GetForward() * speed;
	}

	else if (key == GLFW_KEY_D)
	{
		SpaceShipPhysics->velocity = -model->transform.GetRight() * speed;
	}

	else if (key == GLFW_KEY_A)
	{
		SpaceShipPhysics->velocity = model->transform.GetRight() * speed;
	}
	else if (key == GLFW_KEY_Q)
	{
		SpaceShipPhysics->velocity = model->transform.GetUp() * speed;
	}
	else if (key == GLFW_KEY_E)
	{
		SpaceShipPhysics->velocity = -model->transform.GetUp() * speed;
	}


	else if (key == GLFW_KEY_LEFT)  //LEFT
	{
		model->transform.SetRotation(glm::vec3(model->transform.rotation.x, 
			model->transform.rotation.y + rotationAngle,
			model->transform.rotation.z));

	}

	else if (key == GLFW_KEY_RIGHT) //RIGHT
	{
		model->transform.SetRotation(glm::vec3(model->transform.rotation.x,
			model->transform.rotation.y - rotationAngle,
			model->transform.rotation.z));

	}
	else if (key == GLFW_KEY_UP) //UP
	{
		model->transform.SetRotation(glm::vec3(model->transform.rotation.x ,
			model->transform.rotation.y,
			model->transform.rotation.z - rotationAngle));

	}
	else if (key == GLFW_KEY_DOWN) //DOWN
	{
		model->transform.SetRotation(glm::vec3(model->transform.rotation.x ,
			model->transform.rotation.y,
			model->transform.rotation.z + rotationAngle));

	}

	camera->SetCameraPosition(model->transform.position + model->transform.GetForward() * followDistance + glm::vec3(0, yoffset, 0));

    if (key == GLFW_KEY_0)
	{
		isDebugAAABDraw = !isDebugAAABDraw;
	}
}

void SpaceShip::OnKeyReleased(const int& key)
{
}

void SpaceShip::OnKeyHeld(const int& key)
{
}
