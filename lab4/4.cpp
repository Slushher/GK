#include <iostream>
#include <math.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

static int WIDTH = 800;
static int HEIGHT = 800;

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = -4.0f;
bool frustrum = true;

float delta_x = 0.f;
float delta_y = 0.f;
float mouse_x_pos_old = 0.f;
float mouse_y_pos_old = 0.f;
float theta = 0.f;
float phi = 0.f;
float pix2angle = 1.f;
bool left_mouse_button_pressed = false;
bool right_mouse_button_pressed = false;
float scale_factor = 1.0f;


double radians(double degree)
{
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}

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
        float far_plane = 25.0f;

        gluPerspective(fov, aspect_ratio, near_plane, far_plane);
    }
    glTranslatef(cameraX, cameraY, cameraZ);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void startup(){
    //glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
	glClearColor(0.1, 0.1, 0.1, 1.0);
}

void shutdown(GLFWwindow* window){
    glfwTerminate();
    glfwDestroyWindow(window);
}

void keyboard_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void mouse_motion_callback(GLFWwindow* window, double x_pos, double y_pos){
    delta_x = x_pos - mouse_x_pos_old;
    delta_y = y_pos - mouse_y_pos_old;
    mouse_x_pos_old = x_pos;
    mouse_y_pos_old = y_pos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        left_mouse_button_pressed = 1;
    else
        left_mouse_button_pressed = 0;
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        right_mouse_button_pressed = 1;
    else
        right_mouse_button_pressed = 0;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    scale_factor -= (float)yoffset/10.f;
    if (scale_factor < 0.1f)
        scale_factor = 0.1f;
    if (scale_factor > 3.0f)
        scale_factor = 3.0f;
}

void axes(){
    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-5.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, -5.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -5.0);
    glVertex3f(0.0, 0.0, 5.0);

    glEnd();
}

void example_object(){
    glColor3f(1.0, 1.0, 1.0);

    GLUquadric* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_LINE);
    glRotatef(90, 1.0, 0.0, 0.0);
    glRotatef(-90, 0.0, 1.0, 0.0);

    gluSphere(quadric, 1.5, 10, 10);

    glTranslatef(0.0, 0.0, 1.1);
    gluCylinder(quadric, 1.0, 1.5, 1.5, 10, 5);
    glTranslatef(0.0, 0.0, -1.1);

    glTranslatef(0.0, 0.0, -2.6);
    gluCylinder(quadric, 0.0, 1.0, 1.5, 10, 5);
    glTranslatef(0.0, 0.0, 2.6);

    glRotatef(90, 1.0, 0.0, 1.0);
    glTranslatef(0.0, 0.0, 1.5);
    gluCylinder(quadric, 0.1, 0.0, 1.0, 5, 5);
    glTranslatef(0.0, 0.0, -1.5);
    glRotatef(-90, 1.0, 0.0, 1.0);

    glRotatef(-90, 1.0, 0.0, 1.0);
    glTranslatef(0.0, 0.0, 1.5);
    gluCylinder(quadric, 0.1, 0.0, 1.0, 5, 5);
    glTranslatef(0.0, 0.0, -1.5);
    glRotatef(90, 1.0, 0.0, 1.0);

    glRotatef(90, 0.0, 1.0, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluDeleteQuadric(quadric);
}

void render(double time){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float xeye = cameraZ * sin(radians(theta)) * cos(radians(phi));
    float yeye = cameraZ * sin(radians(phi));
    float zeye = cameraZ * cos(radians(theta)) * cos(radians(phi));

    gluLookAt(xeye, yeye, zeye, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    if (left_mouse_button_pressed){
        theta += delta_x * pix2angle;
        phi += delta_y * pix2angle;
    }
    if(right_mouse_button_pressed){
        cameraZ += delta_y * 0.1;
    }
    //glRotatef(theta, 0.0, 1.0, 0.0);
    //glRotatef(phi, 0.0, 0.0, 1.0);
    glScalef(scale_factor, scale_factor, scale_factor);
    axes();
    example_object();

    glFlush();
}

void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
}

int main(){
	if(!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "_floating_ lab4",NULL,NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, update_viewport);
    glfwSetKeyCallback(window, keyboard_key_callback);
    glfwSetCursorPosCallback(window, mouse_motion_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
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
