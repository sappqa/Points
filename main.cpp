#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>



// Variables for the window's position and size.
int xPos = 500;
int yPos = 200;
int widthInPixels = 300;
int heightInPixels = 300;

// What is the max x and y value?
GLdouble xMax = 10.0;
GLdouble yMax = 10.0;


// The current x and y values of the mouse.
GLfloat currentX = 1000000.0;
GLfloat currentY = 1000000.0;

// Color of objects drawn on screen.
GLfloat r = 1.0;
GLfloat g = 1.0;
GLfloat b = 1.0;

// Bools that determine what is being drawn and how it's being drawn.
bool linesActive = false;
int pointCount = 0;
GLfloat prevX = 0.0;
GLfloat prevY = 0.0;
bool pointsActive = true;
bool trianglesActive = false;
bool rotationActive = false;




// Clears the background color of the window.
void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void reshape(int w, int h)
{
    // Clears color buffers.
    glClear(GL_COLOR_BUFFER_BIT);
    // Clears the projection matrix and sets it to the identity.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // The part of the screen that will be drawn to will be the whole screen after it is reshaped.
    glViewport(0, 0, w, h);
    // Sets up the units of the grid.
    glOrtho(-xMax, xMax, -yMax, yMax, -1.0, 1.0);
    // New width and height of the screen is recorded.
    widthInPixels = w;
    heightInPixels = h;
    /* Display will be called shortly after the window is resized so these variables have to be initialized again
       with a value that won't be visible on the window. */
    currentX = 1000000.0;
    currentY = 1000000.0;

    if (linesActive)
    {
        pointCount = 0;
        linesActive = false;
        pointsActive = true;
        printf("Drawing points\n");
    }
}

// Returns the x coordinate of the mouse (translated from pixels to grid units)
GLfloat getXCoordinate(int mouseX)
{
    GLfloat coor = 0.0;
    coor = ((mouseX * 2) - widthInPixels) / ( widthInPixels / xMax);
    return coor;
}

// Returns the y coordinate of the mouse (translated from pixels to grid units)
GLfloat getYCoordinate(int mouseY)
{
    GLfloat coor = 0.0;
    coor = -((mouseY * 2) - heightInPixels) / ( heightInPixels / yMax);
    return coor;
}

// When you click at the top left corner of the screen, x is 0 and y is 0.
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        // Right click clears the screen.
        glClear(GL_COLOR_BUFFER_BIT);
        currentX = 1000000.0;
        currentY = 1000000.0;
        if (linesActive)
        {
            pointCount = 0;
            linesActive = false;
            pointsActive = true;
            printf("Drawing points\n");
        }

    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        /* Left click draws. Since mouse click is in pixel units and drawing uses OpenGL coordinates,
           we need methods to go from pixel to OpenGL coordinates.
         */
        currentX = getXCoordinate(x);
        currentY = getYCoordinate(y);
        // Write the coordinates of the mouse.
        //std::cout << "X: " << currentX << "\tY: " << currentY << std::endl;
        glutPostRedisplay();
    }

}


// Uses the boolean values to decide what is drawn.
void handleOptions()
{
    // Lines
    if (linesActive)
    {
        if (pointCount < 1)
        {
            glBegin(GL_POINTS);
            glVertex3f(currentX, currentY, 0.0);
            prevX = currentX;
            prevY = currentY;
            pointCount++;
            //printf("Point count: 1\n");
        }
        else
        {
            glBegin(GL_LINES);
            glVertex3f(prevX, prevY, 0.0);
            glVertex3f(currentX, currentY, 0.0);
            pointCount = 0;
            //printf("Point count: 0\n");
        }
    }
    // Draw points whenever the user left clicks.
    if (pointsActive)
    {
        glBegin(GL_POINTS);
        glVertex3f(currentX, currentY, 0.0);
    }
    // Draws triangles whenever the user left clicks.
    if (trianglesActive)
    {
        // LET USER SET SIZE?
        glBegin(GL_TRIANGLES);
        glVertex3f(currentX - 1, currentY, 0.0);
        glVertex3f(currentX + 1, currentY, 0.0);
        glVertex3f(currentX, currentY + 1, 0.0);
    }
}

void display()
{
    // Default to white.
    glColor3f(r, g, b);
    handleOptions();
    glEnd();
    glFlush();
}



void keyboard(unsigned char key, int x, int y)
{
    // Change color, rotate, points, triangles
    switch (key)
    {
        case 'c':
            printf("Enter your red, green, and blue color values (0-1)\n");
            std::cin >> r;
            std::cin >> g;
            std::cin >> b;
            printf("Your color has been set.\n");
            break;
        case 'l':
            pointsActive = false;
            trianglesActive = false;
            linesActive = true;
            printf("Drawing Lines\n");
            break;
        case 'p':
            linesActive = false;
            trianglesActive = false;
            pointsActive = true;
            printf("Drawing Points\n");
            break;
        case 't':
            linesActive = false;
            pointsActive = false;
            trianglesActive = true;
            printf("Drawing Triangles\n");
            break;
    }
}

int main(int argc, char** argv)
{
    printf("Commands:\n");
    printf("(c)- Specify a custom color\n");
    printf("(l)- Draw lines\n");
    printf("(p)- Draw points\n");
    printf("(t)- Draw triangles\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(xPos, yPos);
    glutInitWindowSize(widthInPixels, heightInPixels);
    glutCreateWindow("Drawing Tool");

    init();
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
