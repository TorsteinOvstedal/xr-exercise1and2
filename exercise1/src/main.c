#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Required when including gl/gl.h on windows.
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>


// Answere to exercise 1 (a) and (b).

void exercise1a(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    glColor3f(1.0f, 0.0f, 0.0f);

    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -8.0f);

    glBegin(GL_TRIANGLES);
        glVertex3f( 0.0f,  1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f( 1.0f, -1.0f, 0.0f);
    glEnd();

    glFinish();
}

// Possible improvements over looping with glVertex3f:
// - glVertex3fv.
// - glVertexPointer.

void exercise1b(void)
{
    const float verticies[][3] = {
        { 0.0,  2.0, 0.0},
        { 1.5,  1.5, 0.0},
        { 2.0,  0.0, 0.0},
        { 1.5, -1.5, 0.0},
        { 0.0, -2.0, 0.0},
        {-1.5, -1.5, 0.0}, 
        {-2.0,  0.0, 0.0}, 
        {-1.5,  1.5, 0.0}
    };
    
    const int nVertices = 8;

    const int drawStyles[] = {
        GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, 
        GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
        GL_QUADS, GL_QUAD_STRIP, GL_POLYGON
    };

    const int nDrawStyles = 10;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(-18.0f , 0.0f, -40.0f);            // Global translate to position the initial shape.

    glColor3f(0.0f, 1.0f, 0.0f);

    for (size_t i = 0; i < nDrawStyles; i++)
    {
      glBegin(drawStyles[i]);
        for (size_t j = 0; j < nVertices; j++)
          glVertex3f(verticies[j][0], verticies[j][1], verticies[j][2]);
      glEnd();
      
      glTranslatef(4.0f, 0.0f, 0.0f);               // Translate the next shape relative to the previous shape. 
    }

    glFinish();
}

void reshape(int width, int height)
{
    // Configure viewport transform
    glViewport(0, 0, width, height);

    // Configure projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(width / height), 0.1f, 100.0f);
    
    // Make later transforms affect model-view matrix. 
    glMatrixMode(GL_MODELVIEW);
}


// Window settings.

#define WINDOW_TITLE     "Exercise 1"
#define WINDOW_WIDTH     640
#define WINDOW_HEIGHT    480
#define WINDOW_RESIZABLE GLFW_TRUE

// Window OpenGL context.

#define GL_VERSION_MAJOR 2
#define GL_VERSION_MINOR 1
#define GL_PROFILE       GLFW_OPENGL_ANY_PROFILE

// Window callbacks.

void windowOnError(int errorCode, const char* description);
void windowOnFramebufferResized(GLFWwindow* window, int width, int height);
void windowOnKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);

// Window data.

GLFWwindow* window = NULL;


// Toggel between solution 1 (a) and 1 (b)

void (*render)(void) = exercise1a;

bool dirty = true;

void setSolution(void (*solution)(void));


int main(int arc, char** argv)
{
    int exitCode = 0;

    // Initialize windowing library.

    glfwSetErrorCallback(windowOnError);

    if (!glfwInit())
    {
        printf("%s\n", "Failed to initialize glfw.");
        exitCode = -1;
        goto fini;
    }

    // Create window with OpenGL contex.
        
    glfwWindowHint(GLFW_OPENGL_PROFILE, GL_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_RESIZABLE, WINDOW_RESIZABLE);
 
    window = glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        WINDOW_TITLE,
        NULL, 
        NULL
    );

    if (!window)
    {
        printf("%s\n", "Failed to create window.");
        exitCode = -2;
        goto fini;
    }

    glfwMakeContextCurrent(window);
    
    // Setup window specific callbacks.

    glfwSetFramebufferSizeCallback(window, windowOnFramebufferResized);
    glfwSetKeyCallback(window, windowOnKeyInput);


    // Configure OpenGL.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_FLAT);
    reshape(WINDOW_WIDTH, WINDOW_HEIGHT);


    // Main loop.

    while (!glfwWindowShouldClose(window))
    {
        if (dirty)
        {
            render();
            glfwSwapBuffers(window);
            dirty = false;
        }
        glfwWaitEvents();
    }


    // Free resources.
fini:

    if (window)
        glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}

void windowOnError(int errorCode, const char* description)
{
    printf("%s (%d)\n", description, errorCode);
}

void windowOnFramebufferResized(GLFWwindow* window, int width, int height)
{
    reshape(width, height);

    // Redraw screen.
    dirty = true;
}

void windowOnKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;

        case GLFW_KEY_A:
            if (render != exercise1a)
                setSolution(exercise1a);
            break;

        case GLFW_KEY_B:
            if (render != exercise1b)
                setSolution(exercise1b);
            break;

        default:
            break;
        }
    }
}

void setSolution(void (*solution)(void))
{
    render = solution;
    dirty = true;
}
