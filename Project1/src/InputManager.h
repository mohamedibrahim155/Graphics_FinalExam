#pragma once

#include "iInputListener.h"
#include <vector>

class InputManager
{
private:
	std::vector<iInputListener*> inputListeners;

	class Pimpl;
	Pimpl* pimpl;

public:
	InputManager();
	~InputManager();

	static InputManager& GetInstance();

	void AddListener(iInputListener* inputListener);
	void RemoveListener(iInputListener* inputListener);

	void OnKeyPressed(const int& key);
	void OnKeyReleased(const int& key);
	void OnKeyHeld(const int& key);
};

