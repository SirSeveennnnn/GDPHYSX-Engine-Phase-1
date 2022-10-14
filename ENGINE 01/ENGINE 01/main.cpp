#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Particle.h"
#include"Fireworks.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include"tiny_obj_loader.h"

#include<string>
#include<iostream>

#include"MyShaders.h"
#include"Camera.h"



using namespace std;

//Init callbacks
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);


//Init Particle and Fireworks pool
vector<Particle*> particles;
vector<Fireworks*> fireworks;

//Particle Counter and Pool Size Limit
int particleLimit = 30;
int particleCounter = 1; // index 0 will be a place holder

//Firwork Counter and Pool Size Limit
int fireworksLimit = 30;
int fireworksCounter = 0;

int main(void)
{
    // width and height of the screen
    float width = 1080;
    float height = 720;
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Jae Macuha", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, width, height);

    //Init mouse and key callbacks
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetKeyCallback(window, KeyCallBack);

    //Create Shaders
    MyShaders* shader = new MyShaders("Shaders/shader.vert", "Shaders/shader.frag");

    // Delta Time
    float lastTime = glfwGetTime();

    //create camera
    Camera camera(width,height);

    //Create Particles
    for (int i = 0; i < particleLimit; i++)
    {
        particles.push_back(new Particle());
    }
    

    //Create Fireworks
    for (int i = 0; i < fireworksLimit; i++)
    {
        fireworks.push_back(new Fireworks());
    }


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Delta Time
        //current time
        GLfloat currTime = glfwGetTime();
        float deltaTime = currTime - lastTime;

        cout << "Firework counter: " << fireworks.size() << endl;
        //Physics Updates go here
        for (int i = 0; i < particles.size(); i++)
        {
            particles[i]->Update(deltaTime);
        }

        for (int i = 0; i < fireworks.size(); i++)
        {
            fireworks[i]->Update(deltaTime, fireworks, fireworksCounter, fireworksLimit);
        }

        //Draw Models
        for (int i = 0; i < particles.size(); i++)
        {
            particles[i]->Draw(shader->GetShaderProgram(), camera);
        }

        for (int i = 0; i < fireworks.size(); i++)
        {
            fireworks[i]->Draw(shader->GetShaderProgram(), camera);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        lastTime = currTime; // delta time
    }
    //Free Memory
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i]->Destroy();
    }

    glfwTerminate();
    return 0;

}


// current ballistic selected
BallisticType currentBallistic = Pistol;

//Callback for Mouse Inputs
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (currentBallistic != FireworkEnum)
        {
            particles[particleCounter]->FireBallistic(currentBallistic);
            particleCounter++;
            if (particleCounter > (particleLimit - 1)) 
            {
                particleCounter = 1; //We use 1 because the first particle is a placeholder
            }
        }
        else
        {
            fireworks[fireworksCounter]->FireFireworks(Parent, 0, nullptr);
            fireworksCounter++;
            if (fireworksCounter > fireworksLimit - 1)
            {
                fireworksCounter = 0;
            }
        }
        
    }

}

//Callback for Keyboard Inputs
void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        currentBallistic = Pistol;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        currentBallistic = Artillery;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        currentBallistic = Fireball;
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        currentBallistic = Laser;
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        currentBallistic = FireworkEnum;
    }
        
}
