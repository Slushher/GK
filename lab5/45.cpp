#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

static int WIDTH = 800;
static int HEIGHT = 800;

bool frustrum = true;

GLfloat camera[3] = {0.0f, 0.0f, 5.0f};
float delta_x = 0.f;
float delta_y = 0.f;
float mouse_x_pos_old = 0.f;
float mouse_y_pos_old = 0.f;
float theta = 0.f;
float phi = 0.f;
float pix2angle = 1.f;
bool left_mouse_button_pressed = false;

const GLfloat mat_ambient[4] = {1.0, 1.0, 1.0, 1.0};
const GLfloat mat_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
const GLfloat mat_specular[4] = {1.0, 1.0, 1.0, 1.0};
const GLfloat mat_shininess = 20.0;

const GLfloat light_ambient[4] = {0.0, 0.0, 0.0, 1.0};
const GLfloat light_diffuse[4] = {0.8, 0.8, 0.0, 1.0};
const GLfloat light_specular[4] = {0.0, 1.0, 0.0, 1.0};
const GLfloat light_position[4] = {-10.0, 0.0, 0.0, 1.0};
const GLfloat att_constant = 1.0;
const GLfloat att_linear = 0.05;
const GLfloat att_quadratic = 0.001;

const GLfloat specularLight1[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_position1[4] = {10.0, 0.0, 0.0, 1.0};

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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void startup(){
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glEnable(GL_LIGHT1);
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
}

void render(double time){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camera[0], camera[1], camera[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    if(left_mouse_button_pressed){
        theta += delta_x * pix2angle;
    }
    glRotatef(theta, 0.0, 1.0, 0.0);
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluSphere(quadric, 3.0, 10, 10);
    gluDeleteQuadric(quadric);
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


