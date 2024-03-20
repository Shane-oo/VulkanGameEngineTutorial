//
// Created by ShaneMonck on 20/03/2024.
//

#include "FirstApp.h"

void FirstApp::Run() {
    while(!window.ShouldClose()){
        // listen for keystrokes or close button
        glfwPollEvents();
    }
}
