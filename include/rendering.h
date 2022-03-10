#pragma once

#include <future>

#include "exoskeleton.h"
#include "skeletal_animation.h"
#include "animated_hand.h"
#include "menu_handler.h"

#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <igl/slim.h>
#include <igl/Camera.h>

/**
 * @brief This class performs the rendering of the scene. This has been 
 * included inside a class for better organization of the code due to forking.
 */
class Rendering
{

public:
    /// Rendering constructor.
    Rendering();

private:

};




