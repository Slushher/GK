#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

static int WIDTH = 800;
static int HEIGHT = 800;

typedef GLfloat vertex2f[2];
int iterations = 4;
vertex2f trianglePoints[] = { {-1.0, -0.58}, {1.0, -0.58}, {0.0, 1.15} };

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

void initTrianglePoint(vertex2f a, vertex2f b, vertex2f c)
{
    glVertex2fv(a);
    glVertex2fv(b);
    glVertex2fv(c);
}

void SierpinskiStage1(vertex2f a, vertex2f b, vertex2f c, int m)
{
    vertex2f v0, v1, v2;
    if (m > 0) {                                                
        for (int j = 0; j < 2; j++)                            
        {
            v0[j] = (a[j] + b[j]) / 2;
            v1[j] = (a[j] + c[j]) / 2;
            v2[j] = (b[j] + c[j]) / 2;
        }
        SierpinskiStage1(a, v0, v1, m - 1);                     
        SierpinskiStage1(b, v0, v2, m - 1);
        SierpinskiStage1(c, v1, v2, m - 1);
    }
    else {
        initTrianglePoint(a, b, c);
    }
}

void SierpinskiStage2(int m)
{
    glColor3f(0.5,0.5,0.5);
    SierpinskiStage1(trianglePoints[0], trianglePoints[1], trianglePoints[2], iterations);
}

void render(double time){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 1.0);
    glVertex2f(0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0.0, 50.0);
	glColor3f(0.0, 0.0, 1.0);
    glVertex2f(50.0, 0.0);
    glEnd();
    glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 1.0);
    glVertex2f(0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0.0, 50.0);
	glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-50.0, 0.0);
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
        render(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shutdown(window);
    glfwTerminate();
    return 0;
}


