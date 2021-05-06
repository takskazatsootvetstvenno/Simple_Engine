#include "GL/glew.h"
#include "ConsoleObj.h"

void ConsoleObj::draw(const std::string& newText, const std::string& newText2 = "")
{
	glBindFramebuffer(GL_FRAMEBUFFER, render->Frames[guiFrameID].framebuffer);
	textObjects[0]->RenderTextOnScreen(newText, 120.0f, static_cast<float>(windowHeight - height), 1.0f, fontColor);
	textObjects[1]->RenderTextOnScreen(newText2, 120.0f, static_cast<float>(windowHeight - height)-35, 1.0f, fontColor);
}
void ConsoleObj::createBox()
{
	guiFrameID = render->CreateFrame(windowWidth, windowHeight,false,0.f,0.8f,1.f,0.2f);
	render->createRectangle(120.f, windowHeight*0.11, windowWidth*0.04, windowHeight*0.07, glm::vec4(0.0, 0.5, 0.5, 0.8),guiFrameID, &consoleShow);
    render->createRectangle(0, 0, windowWidth, windowHeight*0.05, glm::vec4(0.0, 0.1, 0.1, 0.8),guiFrameID, &consoleShow);
	textObjects[2]->RenderTextOnScreen("Simple Engine,  Ver: Alpha 0.0.3", windowWidth-0.28*windowWidth, static_cast<float>(windowHeight - height), 1.0f, fontColor);
	textObjects[2]->RenderTextOnScreen("FPS:", 25.0f, static_cast<float>(windowHeight - height), 1.0f, fontColor);
	textObjects[1]->RenderTextOnScreen("Node ID:", 25.0f, static_cast<float>(windowHeight - height)-35, 1.0f, fontColor);//render->createRectangle(-0.5f, -0.5f, 0.25f, 0.25f, glm::vec4(1.0, 0.0, 1.0, 0.3),guiFrameID, &consoleShow);
}
ConsoleObj::ConsoleObj(unsigned int width, unsigned int height, Renderer* newRender,bool& newConsoleShow )
	:render(newRender),consoleShow(newConsoleShow)
{
	windowWidth = width;
	windowHeight = height;
	textObjects.push_back(new RenderText(width, height));
	textObjects[0]->Load("fonts/arial.ttf", 24);
	textObjects.push_back(new RenderText(width, height));
	textObjects[1]->Load("fonts/arial.ttf", 24);
	textObjects.push_back(new RenderText(width, height));
	textObjects[2]->Load("fonts/arial.ttf", 24);
}