#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

static int WIDTH = 400;
static int HEIGHT = 400;

void update_viewport(GLFWwindow* window, int width, int height){
    if(width == 0)
        width = 1;
    if (height == 0)
        height = 1;
    int aspect_ratio = width / height;

    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, width, height);
    glLoadIdentity();

    if (width <= height)
        glOrtho(-100.0, 100.0, -100.0 / aspect_ratio, 100.0 / aspect_ratio,
                1.0, -1.0);
    else
        glOrtho(-100.0 * aspect_ratio, 100.0 * aspect_ratio, -100.0, 100.0,
                1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void startup(){
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

void shutdown(GLFWwindow* window){
    glfwTerminate();
    glfwDestroyWindow(window);
}

void render(float x, float y, float a, float b){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1,1,0);
    glVertex2f(-a/2,b/2);
    glVertex2f(a/2,b/2);
    glVertex2f(a/2,-b/2);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0,1,1);
    glVertex2f(a/2,-b/2);
    glVertex2f(-a/2,-b/2);
    glVertex2f(-a/2,b/2);
    glEnd();

    glFlush();
}

void render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main(){
	if(!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(400, 400, "Lab 1",NULL,NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, update_viewport);
    glfwSwapInterval(1);

    startup();
    while(!glfwWindowShouldClose(window)){
        render(0,0,100,50);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shutdown(window);
    glfwTerminate();
    return 0;
}


