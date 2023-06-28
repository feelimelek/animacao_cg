#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <stdio.h> 
#include <SOIL/SOIL.h> // for image loading

// Window dimensions
GLint windowWidth = 800;
GLint windowHeight = 800;

// Background properties
GLuint textureID;
GLfloat backgroundOffset = 0.0f;

// Ball properties
GLfloat ball1Radius = 0.04f;
GLfloat ball2Radius = 0.04f;

GLfloat ellipseXRadius = 0.15f;
GLfloat ellipseYRadius = 0.07f;

GLfloat backgroundColor_r = 0.0f;
GLfloat backgroundColor_g = 0.0f;
GLfloat backgroundColor_b = 0.0f;

GLfloat ball1Color_r = 1.0f;
GLfloat ball1Color_g = 0.0f;
GLfloat ball1Color_b = 0.0f;

GLfloat ball2Color_r = 0.0f;
GLfloat ball2Color_g = 0.0f;
GLfloat ball2Color_b = 255.0f;

GLfloat ball1Color_r_antiga = 256;
GLfloat ball1Color_g_antiga = 256;
GLfloat ball1Color_b_antiga = 256;

GLfloat ball1X = -0.5f; // X position of ball 1
GLfloat ball2X = 0.8f; // X position of ball 2
GLfloat ball1Y = 0.14f; // Y position of both balls
GLfloat ball2Y = 0.14f; // Y position of both balls
GLfloat ballSpeed = 0.003f; // Horizontal speed of the balls

GLint rectangleWidth = 100;
GLint rectangleHeight = 20;
GLint numRectangles = 10;

GLfloat toRadians = 3.14159 / 180.0;
GLfloat rotationAngle = 45.0f;

bool going_up = false;
bool going_down = false;


// Load texture function
GLuint LoadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    int width, height;
    unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return textureID;
}

void init() {
    // Set up the projection matrix
    // Load and set up the background texture
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    textureID = LoadTexture("sea.png");
    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_TEXTURE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

}

void changeColor(int value) {
    ball1Color_b = 255.0f;
    ball1Color_r = 0.0f;
    ball1Color_g = 0.0f;
    return;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    // Render the background quad
     // Set the desired color filter
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glColor3f(255.0f, 255.0f, 255.0f); // Modify the RGB values as desired
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f + backgroundOffset, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f + backgroundOffset, 0.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f + backgroundOffset, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f + backgroundOffset, 1.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glLoadIdentity();
    // Draw ball 1
    glTranslatef(ball1X, ball1Y, 0.0f);
    glRotatef(rotationAngle, 0, 0, 1);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < 360; i++) {
        GLfloat angle = i * 3.14159 / 180.0;
        GLfloat x = ellipseXRadius * cos(angle);
        GLfloat y = ellipseYRadius * sin(angle);
        glColor3f(ball1Color_r, ball1Color_g, ball1Color_b);
        glVertex2f(x, y);
        GLfloat xShadow = 0.98 * ellipseXRadius * cos(angle);
        GLfloat yShadow = 0.92 * ellipseYRadius * sin(angle);
        glColor3f(0.10 * ball1Color_r, ball1Color_g, ball1Color_b);
        glVertex2f(xShadow, yShadow);
        // cria o bico
        if(cos(angle) == 1) {
            glColor3f(0.05 * ball1Color_r, ball1Color_g, ball1Color_b); // Red color
            glVertex2f(x, y - ellipseYRadius/2); // First vertex
            glVertex2f(x + ellipseXRadius * 1.5, y); // Third vertex
            glVertex2f(x, y + ellipseYRadius/2); // Second vertex
        }
    }
    glEnd();

    // Draw ball 2
    glLoadIdentity();
    glTranslatef(ball2X, ball2Y, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(ball2Color_r, ball2Color_g, ball2Color_b);
    for (int i = 0; i < 360; i++) {
        GLfloat angle = i * 3.14159 / 180.0;
        GLfloat x = ball2Radius * cos(angle);
        GLfloat y = ball2Radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    glutSwapBuffers();
}

void update(int value) {
    // Update the position of both balls
    ball2X -= ballSpeed;
    backgroundOffset += 0.0009f;
    
    if ((ball1X + ellipseXRadius * 2) >= ball2X - ball1Radius) {
        ball2X = 0.8f;
   
        if (ball1Color_r < 0.0f) {
            ball1Color_r  = 0.0f;
            ball2Color_b = 0.0;

            return;
        }
        //troca a cor do passarinho
        ball1Color_r_antiga = ball1Color_r;
        ball1Color_g_antiga = ball1Color_g;
        ball1Color_b_antiga = ball1Color_b;
        glutTimerFunc(10, changeColor, 0);

        glTranslatef(ball1X, ball1Y, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(ball1Color_r, ball1Color_g, ball1Color_b);
        for (int i = 0; i < 360; i++) {
            GLfloat angle = i * 3.14159 / 180.0;
            GLfloat x = ball1Radius * cos(angle);
            GLfloat y = ball1Radius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();

        ellipseXRadius -= 0.04f;
        ellipseYRadius -= 0.02f;
        
        going_up = true;

    }

    if (going_up) {
        // glRotatef(rotationAngle*toRadians, ball1X, ball1Y, 0.0f);
        ball1Y += ballSpeed;
        // toda vez que ele sobre, muda o angulo de rotacao para ele girar
        rotationAngle += 10.0f;
    }
    if (going_down) {
        ball1Y -= ballSpeed;
        // quando está descendo, volta a cor vermelha um pouco mais clara
        ball1Color_r = ball1Color_r_antiga - 0.3f;
        ball1Color_g = ball1Color_g_antiga;
        ball1Color_b = ball1Color_b_antiga;
    }

    if (ball1Y < 0.14f) {
        going_down = false;
    }
    if (ball1Y >= 0.4f) {
        going_down = true;
        going_up = false;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Moving Balls");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}