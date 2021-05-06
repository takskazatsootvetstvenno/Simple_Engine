#include <GL/glew.h>
#include "MainDrawingLoop.h"

#include <gtc/type_ptr.hpp>



void MainDrawingLoop::drawScreen() {
    glDisable(GL_DEPTH_TEST);
   
    //glClearColor(1.0f, 0.0f, 1.0f, 0.5f);
    //glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(app.render.shaders[1].ID);

    
    glBindVertexArray(app.render.screenVAO);
    glBindTexture(GL_TEXTURE_2D,
                  app.render.texColorBuffer);  // используем прикрепленную цветовую текстуру в качестве
                                               // текстуры для прямоугольника
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
    
    if (app.showConsole) {
        //glBindVertexArray(app.render.screenVAO);
        glBindVertexArray(curRender->Frames[tinyConsole.guiFrameID].frameVAO);

        glBindTexture(GL_TEXTURE_2D,
            curRender->Frames[tinyConsole.guiFrameID].texColorBuffer);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
}
void MainDrawingLoop::updateProjViewUBO() {
    model = glm::mat4(1.0f);
    view = app.getCamera()->GetViewMatrix();
    glm::mat4 projection = glm::perspectiveFov(glm::radians(app.getCamera()->GetZoom()), app.getWindowWidth(),
                                               app.getWindowHeight(), 0.1f, 100.f);
    curRender->updateViewMatrix(app.render.UBO[0], view);
    if (lastProjection != projection) {
        curRender->updateProjectionMatrix(app.render.UBO[0], projection);
        lastProjection = projection;
    }
    glUseProgram(curRender->shaders[WATER_SHADER].ID);
    curRender->shaders[WATER_SHADER].setVec3("u_cameraPosition", app.getCamera()->getCameraPosition());
    glUseProgram(0);
    /*
    glBindBuffer(GL_UNIFORM_BUFFER, app.render.UBO[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3),
                    glm::value_ptr(app.getCamera()->Position));
    */
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void MainDrawingLoop::deltaTimeAndFPS() {
    currentTime = glfwGetTime();
    deltaTime = currentTime - previousTime;
    glUseProgram(curRender->shaders[WATER_SHADER].ID);
    static float moveFactor;
    moveFactor +=((int)(currentTime*100.0)%10) * 0.00001f;

   // moveFactor = (int)moveFactor % 1;
    curRender->shaders[WATER_SHADER].setFloat("moveFactor",moveFactor );
    previousTime = currentTime;
    frame_counter++;
    if (frame_counter >= 100) {
        memset(str, 0, sizeof(str));
        int num = int(100.0 / (currentTime - previous));
        sprintf_s(str, "%d", num);
        previous = currentTime;
        frame_counter = 0;
    }
}
//void MainDrawingLoop::
void MainDrawingLoop::Loop() {
      tinyConsole.createBox();
      
    //glBindFramebuffer(GL_FRAMEBUFFER, app.render.Frames[1].framebuffer);
    //view = app.getCamera()->GetViewMatrix();
   // glm::mat4 projection = glm::perspectiveFov(glm::radians(app.getCamera()->GetZoom()), app.getWindowWidth(),
   //                                            app.getWindowHeight(), 0.1f, 100.f);
    glm::mat4 projection = glm::ortho(0.0f, curRender->width, 0.0f, curRender->height*0.2f);//glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glUseProgram(app.render.shaders[5].ID);
    /*if (lastProjection != projection) {
        curRender->updateProjectionMatrix(*(tinyConsole.textObjects[0]->TextShader->UBO), projection);
        lastProjection = projection;
    }*/
     app.render.shaders[5].setMat4("u_projection", projection);
    /*
    glBindBuffer(GL_UNIFORM_BUFFER, app.render.UBO[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3),
                    glm::value_ptr(app.getCamera()->Position));
    */
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
  


    while (!glfwWindowShouldClose(curRender->window)) {
        glfwPollEvents();
    if (app.getUserMovement()->checkAndGetLastMousePos(xpos, ypos)) {
        app.ClickMousePosX = xpos;
        app.ClickMousePosY = ypos;
        app.drawCheckedNode();
       // float x, y;
       // cin >> x >> y;
       // curRender->setNewResolution(x, y);
    }
    KeyStates& CurrentKeyStates = app.getUserMovement()->getAllStates();
    if (CurrentKeyStates.grave_clicked)//only get value
    {
        app.showConsole = (!app.showConsole);
        CurrentKeyStates.grave_clicked = false;//0==false
    }
    deltaTimeAndFPS();
    app.getUserMovement()->movement(static_cast<float>(deltaTime));

    glBindFramebuffer(GL_FRAMEBUFFER, app.render.framebuffer);
    //  glBindFramebuffer(GL_FRAMEBUFFER, 2);
   // glClearColor(0.2f, 0.2f, 0.2f, 1.0f);    //не очищаю буффер!!!!!!!!!!
  //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    updateProjViewUBO();

    app.drawSkybox();
   
    app.root->useMeshesRec(model);
    curRender->drawObjects();
    glBindFramebuffer(GL_FRAMEBUFFER, app.render.framebuffer);
    if(app.showConsole){
       
        char buf[10];
        sprintf_s(buf, "%d", app.clickedID);
        tinyConsole.draw(str, buf);
     }
    
    //text->RenderTextOnScreen(str + std::string("hello!хай!"), 25.0f, 50.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    drawScreen();
        glfwSwapBuffers(curRender->window);
    }
}
MainDrawingLoop::MainDrawingLoop(Application& curApp, Renderer* initRender)
    : app(curApp), curRender(initRender), tinyConsole(curApp.getWindowWidth(),curApp.getWindowHeight()*0.2, initRender, curApp.showConsole) {
}
