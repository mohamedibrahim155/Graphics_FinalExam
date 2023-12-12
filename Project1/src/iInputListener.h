#pragma once

class iInputListener
{
public:

	virtual ~iInputListener() {};

	virtual void OnKeyPressed(const int& key) = 0;
	virtual void OnKeyReleased(const int& key) = 0;
	virtual void OnKeyHeld(const int& key) = 0;
};
