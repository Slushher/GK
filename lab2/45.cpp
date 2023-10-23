#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

static int WIDTH = 400;
static int HEIGHT = 400;
int iterations = 2;

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

void DrawCube(float x, float y, float a_1, float b_1, double d = 0.0, double w = 0.0){
    float a = a_1;
    float b = b_1;
    a+=d/2.f;
    b+=d/2.f;
    glBegin(GL_TRIANGLES);
    glColor3f(1*w,1*w,1*w);
    glVertex2f(-a/2.f,b/2.f);
    glVertex2f(a/2.f,b/2.f);
    glVertex2f(a/2.f,-b/2.f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(1*w,1*w,1*w);
    glVertex2f(a/2.f,-b/2.f);
    glVertex2f(-a/2.f,-b/2.f);
    glVertex2f(-a/2.f,b/2.f);
    glEnd();
}

void SierpinskiStage1(int x, int y, double w, int iterations){
    double color = sin(iterations)*0.5;
    DrawCube(x,y,0,0,w, color);
}

void SierpinskiStage2(int x, int y, double w, int m){
    //printf("Iteration : %i\n",m);
for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 1 && j == i)
				continue;

			int step = (int)(pow(3, m));
			int newX = x + i * step;
			int newY = y + j * step;

			if (m == 0)
			{
				GLdouble squareW = 2.0 / (int)(pow(3, m));
				GLdouble squareX = newX * squareW;
				GLdouble squareY = newY * squareW;

				SierpinskiStage1(squareX, squareY, squareW, m);
			}
			else
			{
                printf("Iteration : %i\n",m);
				SierpinskiStage2(newX, newY, w, (m - 1));
			}
		}
	}
}

void render(){
	glClear(GL_COLOR_BUFFER_BIT);
    SierpinskiStage2(0, 0, 400, iterations);
	glFlush();
}

int main(){
    srand (time(NULL));
    float d = (float)(rand() % 100 + 1)/1.f;
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
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shutdown(window);
    glfwTerminate();
    return 0;
}


