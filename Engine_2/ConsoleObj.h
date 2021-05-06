#ifndef CONSOLEOBJ_H
#define CONSOLEOBJ_H
#include <iostream>
#include "Renderer.h"
#include "RenderText.h"

class ConsoleObj
{
public:
	void draw(const std::string& newText, const std::string& newText2);
	void createBox();
	unsigned int guiFrameID=0;
	ConsoleObj(unsigned int width, unsigned int height, Renderer* newRender, bool& newConsoleShow);
	std::vector<RenderText*> textObjects;
private:
	
	int fontSize = 24;
	int height = 36;
	bool& consoleShow;
	int windowWidth, windowHeight;
	std::string text;
	glm::vec3 fontColor = glm::vec3(1.0f, 0.0f, 1.0f);
	Renderer* render;
	float transp = 0.2;
};
#endif

