#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <time.h>

static int WIDTH = 800;
static int HEIGHT = 800;

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 15.0f;

bool frustrum = true;
const int N = 200;
float vertices[N][N][3];

typedef struct
{
    double values[3];
} Vertex3f;

Vertex3f color_rand[N][N][6];

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
            glOrtho(-20.f, 20.f, -20.f / aspect_ratio, 20.f / aspect_ratio, 20.f, -20.f);
        }
        else
        {
            glOrtho(-20.f * aspect_ratio, 20.f * aspect_ratio, -20.f, 20.f, 20.f, -20.f);
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
    glTranslatef(-cameraX, -cameraY, -cameraZ);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void startup()
{
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.2, 0.2, 0.2, 1.0);
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
    for (int i = 0; i < N - 1; ++i)
    {
        for (int j = 0; j < N - 1; ++j)
        {
            glBegin(GL_TRIANGLES);
            glColor3f(color_rand[i][j][0].values[0], color_rand[i][j][0].values[1], color_rand[i][j][0].values[2]);
            glVertex3fv(vertices[i][j]);

            glColor3f(color_rand[i][j][1].values[0], color_rand[i][j][1].values[1], color_rand[i][j][1].values[2]);
            glVertex3fv(vertices[i][j + 1]);

            glColor3f(color_rand[i][j][2].values[0], color_rand[i][j][2].values[1], color_rand[i][j][2].values[2]);
            glVertex3fv(vertices[i + 1][j]);
            glEnd();
            glBegin(GL_TRIANGLES);
            glColor3f(color_rand[i][j][3].values[0], color_rand[i][j][3].values[1], color_rand[i][j][3].values[2]);
            glVertex3fv(vertices[i + 1][j]);

            glColor3f(color_rand[i][j][4].values[0], color_rand[i][j][4].values[1], color_rand[i][j][4].values[2]);
            glVertex3fv(vertices[i + 1][j + 1]);

            glColor3f(color_rand[i][j][5].values[0], color_rand[i][j][5].values[1], color_rand[i][j][5].values[2]);
            glVertex3fv(vertices[i][j+1]);
            glEnd();
        }
    }
    glEnd();
}

// Zmienna losowa normalizowana
float random_normalized()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
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
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < 6; k++)
            {
                color_rand[i][j][k] = {random_normalized(), random_normalized(), random_normalized()};
            }
        }
    }
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
