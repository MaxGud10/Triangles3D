#include <GL/glut.h>
#include <cmath>

float camera_angle = 0.0f;
float camera_radius = 10.0f;
float rotation_speed = 0.5f;

void init() 
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void reshape(int width, int height) 
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width/height, 0.1f, 100.0f);
}

void drawCubes() 
{
    // Первый куб (центральный, зеленый)
    glColor3f(0.0f, 1.0f, 0.0f);
    glutWireCube(2.0f);
    
    // Второй куб (смещенный, красный)
    glPushMatrix();
    glTranslatef(3.0f, 0.0f, 0.0f);  // Смещение на 3 единицы по X
    glColor3f(1.0f, 0.0f, 0.0f);
    glutWireCube(1.5f);              // Немного меньший размер
    glPopMatrix();
}

void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    float camX = camera_radius * sin(camera_angle * M_PI / 180);
    float camZ = camera_radius * cos(camera_angle * M_PI / 180);
    
    gluLookAt(camX, 2.0f, camZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
    
    drawCubes();
    
    glutSwapBuffers();
}

void update(int value) 
{
    camera_angle += rotation_speed;
    if(camera_angle > 360) camera_angle -= 360;
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Two Cubes Orbiting Camera");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    
    glutMainLoop();
    return 0;
}