#include "InputManager.h"
#include <unordered_set>

class InputManager::Pimpl
{
public:
	std::unordered_set<int> keysHeld;

	void AddKeyHeld(const int& key);
	void RemoveKeyHeld(const int& key);
};

void InputManager::Pimpl::AddKeyHeld(const int& key)
{
	keysHeld.insert(key);
}

void InputManager::Pimpl::RemoveKeyHeld(const int& key)
{
	keysHeld.erase(key);
}

InputManager::InputManager() : pimpl { new Pimpl() }
{
}

InputManager::~InputManager()
{
	delete pimpl;
}

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::AddListener(iInputListener* inputListener)
{
	inputListeners.push_back(inputListener);
}

void InputManager::RemoveListener(iInputListener* inputListener)
{
	inputListeners.erase(std::remove(inputListeners.begin(), inputListeners.end(), inputListener), inputListeners.end());
}

void InputManager::OnKeyPressed(const int& key)
{
	pimpl->AddKeyHeld(key);

	for (iInputListener* listener : inputListeners)
	{
		listener->OnKeyPressed(key);
	}
}

void InputManager::OnKeyReleased(const int& key)
{
	pimpl->RemoveKeyHeld(key);

	for (iInputListener* listener : inputListeners)
	{
		listener->OnKeyReleased(key);
	}
}

void InputManager::OnKeyHeld(const int& key)
{
	for (iInputListener* listener : inputListeners)
	{
		if (pimpl->keysHeld.find(key) != pimpl->keysHeld.end())
		{
			listener->OnKeyHeld(key);
		}
	}
}


