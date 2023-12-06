#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>

static double WIDTH = 800;
static double HEIGHT = 800;
int iterations = 5;
double self_similarity = 0.5;
typedef struct
{
    double values[3];
} Vertex3f;

void update_viewport(GLFWwindow *window, int width, int height)
{
    if (width == 0)
        width = 1;
    if (height == 0)
        height = 1;
    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, width, height);
    glLoadIdentity();

    if (width <= height)
        glOrtho(-100.0, 100.0, -100.0 * height / width, 100.0 * height / width, 1.0, -1.0);
    else
        glOrtho(-100.0 * width / height, 100.0 * width / height, -100.0, 100.0, 1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void startup(GLFWwindow *window, int width, int height)
{
    update_viewport(window, width, height);
    glClearColor(0, 0, 0, 1.0);
}

void shutdown(GLFWwindow *window)
{
    glfwTerminate();
    glfwDestroyWindow(window);
}

void drawTriangle(float x, float y, float a, float b, Vertex3f color)
{
    glTranslatef(-(WIDTH / 2) + x, -(HEIGHT / 2) + y, 0.0);
    glBegin(GL_TRIANGLES);
    glColor3f(color.values[0], color.values[1], color.values[2]);
    glVertex2f(a, b);
    glVertex2f(a - (a / 2), b - b * sqrt(3) / 2);
    glVertex2f(a + (a / 2), b - b * sqrt(3) / 2);
    glEnd();
    glTranslatef(WIDTH / 2 - x, HEIGHT / 2 - y, 0.0);
    glFlush();
}

void drawSierpinskiTriangle(float x, float y, double size, int iterations, float self_similarity)
{
    if (iterations <= 0)
    {
        Vertex3f color = {1.0, 1.0, 1.0};
        drawTriangle(x - size / 2, y- size / 2, size, size, color);
    }
    else
    {
        double buff = size * self_similarity;
        drawSierpinskiTriangle(x - buff / 2, y - buff / 2, buff, iterations - 1, self_similarity);
        drawSierpinskiTriangle(x + buff / 2, y - buff / 2, buff, iterations - 1, self_similarity);
        drawSierpinskiTriangle(x - buff / 2, y + buff * sqrt(3) / 2, buff, iterations - 1, self_similarity);
    }
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawSierpinskiTriangle(400, 400, 100, iterations, self_similarity);
    glFlush();
}

int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "_floating_ Lab 1", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, update_viewport);
    glfwSwapInterval(1);

    startup(window, WIDTH, HEIGHT);
    while (!glfwWindowShouldClose(window))
    {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shutdown(window);
    return 0;
}