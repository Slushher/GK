#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>

static int WIDTH = 800;
static int HEIGHT = 800;
int iterations = 5;
// Stopien samopodobienstwa
double self_similarity = 0.33;
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
    // glScalef(0.5f, 0.5f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void startup(GLFWwindow *window, int width, int height)
{
    update_viewport(window, width, height);
    glClearColor(0, 0, 0, 1.f);
}

void shutdown(GLFWwindow *window)
{
    glfwTerminate();
    glfwDestroyWindow(window);
}

void drawCube(float x, float y, float a, float b, Vertex3f color = {1.0, 1.0, 1.0})
{
    glTranslatef(-(WIDTH / 2) + x, -(HEIGHT / 2) + y, 0.0);
    glBegin(GL_TRIANGLES);
    glColor3f(color.values[0], color.values[1], color.values[2]);
    glVertex2f(-a / 2, b / 2);
    glVertex2f(a / 2, b / 2);
    glVertex2f(a / 2, -b / 2);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(color.values[0], color.values[1], color.values[2]);
    glVertex2f(a / 2, -b / 2);
    glVertex2f(-a / 2, -b / 2);
    glVertex2f(-a / 2, b / 2);
    glEnd();
    glTranslatef(WIDTH / 2 - x, HEIGHT / 2 - y, 0.0);
    glFlush();
}

void drawCarpet(double x, double y, double size, int iterations, float self_similarity)
{
    if (iterations <= 0)
    {
        return;
    }
    drawCube(x, y, size, size);

    double buff = size * self_similarity;

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0)
                continue;
            drawCarpet(x + i * buff * 3, y + j * buff * 3, buff, iterations - 1, self_similarity);
        }
    }
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawCarpet(400, 400, 50, iterations, self_similarity);
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