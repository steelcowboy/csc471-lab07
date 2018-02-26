#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

using namespace std;
using namespace glm;

class Application : public EventCallbacks
{

    public:

        WindowManager * windowManager = nullptr;

        // Our shader program
        std::shared_ptr<Program> rot_light;
        
        // Our shader program
        std::shared_ptr<Program> fixed_light;

        // Shape to be used (from obj file)
        shared_ptr<Shape> shape;

        // Contains vertex information for OpenGL
        GLuint VertexArrayID;

        // Data necessary to give our triangle to OpenGL
        GLuint VertexBufferID;

        static const int zBack = -5;
        static const int xMov = 2;

        float y_rot = 0;
        int style = 0;

        void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS) {
                switch (key) {
                    case GLFW_KEY_ESCAPE:
                        glfwSetWindowShouldClose(window, GL_TRUE);
                        break;
                    case GLFW_KEY_A:
                        style = (style == 1) ? 0:1;
                        break;
                }
            }
        }

        void mouseCallback(GLFWwindow *window, int button, int action, int mods)
        {
            double posX, posY;

            if (action == GLFW_PRESS)
            {
                glfwGetCursorPos(window, &posX, &posY);
                cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
            }
        }

        void resizeCallback(GLFWwindow *window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }

        void init(const std::string& resourceDirectory)
        {
            GLSL::checkVersion();

            // Set background color.
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            // Enable z-buffer test.
            glEnable(GL_DEPTH_TEST);

            // Initialize the GLSL program.
            rot_light = make_shared<Program>();
            rot_light->setVerbose(true);
            rot_light->setShaderNames(resourceDirectory + "/rot_vert.glsl", resourceDirectory + "/rot_frag.glsl");
            if (rot_light->init()) {
                //exit(1);
            }
            rot_light->addUniform("P");
            rot_light->addUniform("V");
            rot_light->addUniform("M");
            rot_light->addUniform("uStyle");
            rot_light->addAttribute("vertPos");
            rot_light->addAttribute("vertNor");
            
            // Initialize the GLSL program.
            fixed_light = make_shared<Program>();
            fixed_light->setVerbose(true);
            fixed_light->setShaderNames(resourceDirectory + "/fixed_vert.glsl", resourceDirectory + "/fixed_frag.glsl");
            if (fixed_light->init()) {
                //exit(1);
            }
            fixed_light->addUniform("P");
            fixed_light->addUniform("V");
            fixed_light->addUniform("M");
            fixed_light->addUniform("uStyle");
            fixed_light->addAttribute("vertPos");
            fixed_light->addAttribute("vertNor");
        }

        void initGeom(const std::string& resourceDirectory)
        {
            // Initialize mesh.
            shape = make_shared<Shape>();
            //shape->loadMesh(resourceDirectory + "/cube.obj");
            shape->loadMesh(resourceDirectory + "/sphere.obj");
            shape->resize();
            shape->init();
        }

        void render()
        {
            // Get current frame buffer size.
            int width, height;
            glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
            glViewport(0, 0, width, height);

            // Clear framebuffer.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Use the matrix stack for Lab 6
            float aspect = width/(float)height;

            // Create the matrix stacks - please leave these alone for now
            auto Projection = make_shared<MatrixStack>();
            auto View = make_shared<MatrixStack>();
            auto Model = make_shared<MatrixStack>();

            // Apply perspective projection.
            Projection->pushMatrix();
            Projection->perspective(45.0f, aspect, 0.01f, 100.0f);

            // View is identity - for now
            View->pushMatrix();

            rot_light->bind();
            glUniformMatrix4fv(rot_light->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
            glUniformMatrix4fv(rot_light->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
            glUniform1i(rot_light->getUniform("uStyle"), style);

            // draw sphere 
            Model->pushMatrix();
            {
                Model->translate(vec3(xMov, 0, zBack));
                Model->rotate(y_rot, vec3(0, 1, 0));
                glUniformMatrix4fv(rot_light->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
                shape->draw(rot_light);
            }
            Model->popMatrix();

            rot_light->unbind();
            fixed_light->bind();
            glUniformMatrix4fv(fixed_light->getUniform("P"), 1, GL_FALSE, value_ptr(Projection->topMatrix()));
            glUniformMatrix4fv(fixed_light->getUniform("V"), 1, GL_FALSE, value_ptr(View->topMatrix()));
            glUniform1i(fixed_light->getUniform("uStyle"), style);

            // draw sphere 
            Model->pushMatrix();
            {
                Model->translate(vec3(-xMov, 0, zBack));
                Model->rotate(y_rot, vec3(0, 1, 0));
                glUniformMatrix4fv(fixed_light->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
                shape->draw(fixed_light);
            }
            Model->popMatrix();

            fixed_light->unbind();

            // Pop matrix stacks.
            Projection->popMatrix();
            View->popMatrix();

            y_rot += 0.01;
            //std::cout << y_rot << std::endl;
        }
};

int main(int argc, char *argv[])
{
    // Where the resources are loaded from
    std::string resourceDir = "../resources";

    if (argc >= 2)
    {
        resourceDir = argv[1];
    }

    Application *application = new Application();

    // Your main will always include a similar set up to establish your window
    // and GL context, etc.

    WindowManager *windowManager = new WindowManager();
    windowManager->init(640, 480);
    windowManager->setEventCallbacks(application);
    application->windowManager = windowManager;

    // This is the code that will likely change program to program as you
    // may need to initialize or set up different data and state

    application->init(resourceDir);
    application->initGeom(resourceDir);

    // Loop until the user closes the window.
    while (! glfwWindowShouldClose(windowManager->getHandle()))
    {
        // Render scene.
        application->render();

        // Swap front and back buffers.
        glfwSwapBuffers(windowManager->getHandle());
        // Poll for and process events.
        glfwPollEvents();
    }

    // Quit program.
    windowManager->shutdown();
    return 0;
}
