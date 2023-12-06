#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <time.h>

static int WIDTH = 400;
static int HEIGHT = 400;
typedef struct {
    double values[3];
} Vertex3f;

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

void render(float x, float y, float a, float b, float d = 0.0, Vertex3f color[6] = {}){
    a=a*d;
    b=b*d;
    glClear(GL_COLOR_BUFFER_BIT);
    glTranslatef(-(WIDTH/2)+x, -(WIDTH/2)+y, 0.0);
    glBegin(GL_TRIANGLES);
    glColor3f(color[0].values[0], color[0].values[1], color[0].values[2]);
    glVertex2f(-a / 2, b / 2);
    glColor3f(color[1].values[0], color[1].values[1], color[1].values[2]);
    glVertex2f(a / 2, b / 2);
    glColor3f(color[2].values[0], color[2].values[1], color[2].values[2]);
    glVertex2f(a / 2, -b / 2);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(color[3].values[0], color[3].values[1], color[3].values[2]);
    glVertex2f(a / 2, -b / 2);
    glColor3f(color[4].values[0], color[4].values[1], color[4].values[2]);
    glVertex2f(-a / 2, -b / 2);
    glColor3f(color[5].values[0], color[5].values[1], color[5].values[2]);
    glVertex2f(-a / 2, b / 2);
    glEnd();
    glTranslatef(WIDTH/2-x, WIDTH/2-y, 0.0);

    glFlush();
}

// Zmienna losowa normalizowana
float random_normalized(){
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int main(){
    srand (static_cast <unsigned> (time(0)));
    float d = 1.f + random_normalized();
    Vertex3f color_rand[6];
    for(int i = 0; i < 6; i++){
        color_rand[i] = {random_normalized(), random_normalized(), random_normalized()};
    }
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

        render(200,200,100,50, d, color_rand);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shutdown(window);
    glfwTerminate();
    return 0;
}


