#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

static int WIDTH = 800;
static int HEIGHT = 800;

bool frustrum = true;

const int N = 100;

float vertices[N][N][3];

void update_viewport(GLFWwindow* window, int width, int height){
    if(width == 0)
        width = 1;
    if (height == 0)
        height = 1;
    int aspect_ratio = width / height;

    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, width, height);
    glLoadIdentity();
	if(frustrum){
		//glFrustum(-10, 10, 10, -10, 0.1, 10);
		 if (width <= height)
        glOrtho(-15.f, 15.f, -15.f / aspect_ratio, 15.f / aspect_ratio, 15.f, -15.f);
    else
        glOrtho(-15.f * aspect_ratio, 15.f * aspect_ratio, -15.f, 15.f, 15.f, -15.f);
    }
	else{
		if (width <= height)
				glOrtho(-100.0, 100.0, -100.0 / aspect_ratio, 100.0 / aspect_ratio, 1.0, -1.0);
		else
				glOrtho(-100.0 * aspect_ratio, 100.0 * aspect_ratio, -100.0, 100.0, 1.0, -1.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void startup() {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
        	float u = static_cast<float>(i) / static_cast<float>(N);
            float v = static_cast<float>(j) / static_cast<float>(N);
            vertices[i][j][0] = ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - (45 * u)) * (cos(M_PI * v));
			vertices[i][j][1] = (160 * pow(u, 4)) - (320 * pow(u, 3)) + (160 * pow(u, 2));
			vertices[i][j][2] = ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - (45 * u)) * (sin(M_PI * v));
        }
    }
}

void shutdown(GLFWwindow* window){
    glfwTerminate();
    glfwDestroyWindow(window);
}

void render(double time){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            glVertex3fv(vertices[i][j]);
            glVertex3fv(vertices[i][j+1]);
            glVertex3fv(vertices[i+1][j]);
        }
    }
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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, " _floating_ Lab 3",NULL,NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, update_viewport);
    glfwSwapInterval(1);
	update_viewport(window, WIDTH, HEIGHT);
    startup();
    while(!glfwWindowShouldClose(window)){
        render(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shutdown(window);
    glfwTerminate();
    return 0;
}


