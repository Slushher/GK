#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

static int WIDTH = 800;
static int HEIGHT = 800;

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 10.0f;

bool frustrum = false;
const int N = 50;
float vertices[N][N][3];

void update_viewport(GLFWwindow *window, int width, int height)
{
    if (width == 0)
        width = 1;
    if (height == 0)
        height = 1;
    int aspect_ratio = width / height;

    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, width, height);
    glLoadIdentity();
    if (!frustrum)
    {
        if (width <= height)
        {
            glOrtho(-10.f, 10.f, -10.f / aspect_ratio, 10.f / aspect_ratio, 10.f, -10.f);
        }
        else
        {
            glOrtho(-10.f * aspect_ratio, 10.f * aspect_ratio, -10.f, 10.f, 10.f, -10.f);
        }
    }
    else
    {
        float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
        float fov = 90.0f;
        float near_plane = 0.1f;
        float far_plane = 100.0f;

        gluPerspective(fov, aspect_ratio, near_plane, far_plane);
    }
    // glTranslatef(-cameraX, -cameraY, -cameraZ);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void startup()
{
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    for (int i = 0; i < N; ++i)
    {
        float u = static_cast<float>(i) / static_cast<float>(N - 1);
        for (int j = 0; j < N; ++j)
        {
            float v = static_cast<float>(j) / static_cast<float>(N - 1);
            vertices[i][j][0] = ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - (45 * u)) * (cos(2 * M_PI * v));
            vertices[i][j][1] = (160 * pow(u, 4)) - (320 * pow(u, 3)) + (160 * pow(u, 2));
            vertices[i][j][2] = ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - (45 * u)) * (sin(2 * M_PI * v));
        }
    }
}

void shutdown(GLFWwindow *window)
{
    glfwTerminate();
    glfwDestroyWindow(window);
}

void render(double time)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(static_cast<float>(time) * 40.f, 1, 1, 1);
    glBegin(GL_LINES);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            glVertex3fv(vertices[i][j]);
            glVertex3fv(vertices[i][j + 1]);
            glVertex3fv(vertices[i + 1][j]);
            glVertex3fv(vertices[i + 1][j]);
            glVertex3fv(vertices[i + 1][j + 1]);
            glVertex3fv(vertices[i][j + 1]);
        }
    }
    glEnd();
    glFlush();
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main()
{
    if (!glfwInit())
        return -1;
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, " _floating_ Lab 3", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, update_viewport);
    glfwSwapInterval(1);
    update_viewport(window, WIDTH, HEIGHT);
    startup();
    while (!glfwWindowShouldClose(window))
    {
        render(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shutdown(window);
    glfwTerminate();
    return 0;
}
