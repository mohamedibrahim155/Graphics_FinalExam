#pragma once

#include <string>
#include "Renderer.h"
#include <vector>



#include <iostream>


class Texture
{
public:
	Texture() {}
	Texture(std::string& filePath);
	~Texture() {}
	unsigned int id;
	std::string path;
	std::string type;
	void LoadTexture(const std::string& filePath, const std::string& _type);

	unsigned int TextureFromFile(const char* path, const std::string& directory);

	unsigned int loadCubemap(std::vector<std::string> faces);

	void Bind();
	void Unbind();

private:
	unsigned int loadTexture(char const* path);
};