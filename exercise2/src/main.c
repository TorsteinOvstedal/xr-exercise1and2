#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Required when including gl/gl.h on windows.
    #include <windows.h>
#endif

#include <math.h>

#include <gl/gl.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>

// Data and helpers

// Cube verticies (from: https://learnopengl.com/code_viewer.php?code=getting-started/cube_vertices)
const float verticies[] = {
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,

     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,

    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f
};

static inline void rotateY(double p[3], double rotation, double out[3])
{
    // TODO: Account for camera.target
    double c = cos(rotation);
    double s = sin(rotation);
    out[0] = c * p[0] + s * p[2];
    out[1] = p[1];
    out[2] = -s * p[0] + c * p[2];
}

void drawCube(style)
{
    const size_t nVertices = sizeof(verticies) / sizeof(verticies[0]);

    glBegin(style);

    for (size_t i = 0; i < nVertices; i += 3)
        glVertex3fv(&verticies[i]);

    glEnd();
}

struct Camera
{
    double speed;
    double rotation;
    double position[3];
    double target[3];
};

struct Camera camera = {
    .speed    = 0.7,
    .rotation = 0.0,
    .position = {0.0, 2.5, 10.0},
};

int transfromStep = 0;

void step()
{
    transfromStep = (transfromStep + 1) % 4;
}

void update(double deltaTime)
{
    camera.rotation += deltaTime * camera.speed;
}

void drawGrid(int c, int r)
{
    glBegin(GL_LINES);
    for (int x = 0; x <= c; x++)
    {
        glVertex3f((float)(x - c / 2), -1.0f, (float)-c);
        glVertex3f((float)(x - c / 2), -1.0f, (float) c);
    }

    for (int z = 0; z <= r; z++)
    {
        glVertex3f((float) -r, -1.0f, (float)(z - r / 2));
        glVertex3f((float)  r,  -1.0f,(float)(z - r / 2));
    }
    glEnd();

}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start ModelView Transform

    glLoadIdentity();

    // Camera (View transform)

    double position[3];
    rotateY(camera.position, camera.rotation, position);

    gluLookAt(
        position[0], position[1], position[2],
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );
    

    // Draw grid to visualize the xz plane

    glColor3f(0.0f, 0.5f, 0.0f);
    drawGrid(50, 50);

    // Model 0 (Model transform and draw)

    glPushMatrix();
        // Order: Translate, Rotate, Scale
        if (transfromStep > 0)
            glTranslatef(0.5f, 0.0f, 1.0f);
        if (transfromStep > 1)
            glRotatef(45.0f, 1.0f, 1.0f, 0.0f);
        if (transfromStep > 2)
            glScalef(2.0f, 2.5f, 1.5f);
        
        glColor3f(0.0f, 1.0f, 0.0f);
        drawCube(GL_LINE_LOOP);
    glPopMatrix();

    // Model 1 (Model transform and draw)

    glPushMatrix();
        // Order: Scale, Rotate, Translate, 
        if (transfromStep > 0)
            glScalef(2.0f, 2.5f, 1.5f);
        if (transfromStep > 1)
            glRotatef(45.0f, 1.0f, 1.0f, 0.0f);
        if (transfromStep > 2)
            glTranslatef(0.5f, 0.0f, 1.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        drawCube(GL_LINE_LOOP);
    glPopMatrix();

    // Model 2 (Model transform and draw)

    glPushMatrix();
        // Order: Scale, Translate, Rotate 
        if (transfromStep > 0)
            glScalef(2.0f, 2.5f, 1.5f);
        if (transfromStep > 1)
            glTranslatef(0.5f, 0.0f, 1.0f);
        if (transfromStep > 2)
            glRotatef(45.0f,10.0f, 1.0f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        drawCube(GL_LINE_LOOP);
    glPopMatrix();

    glFinish();
}

void reshape(int width, int height)
{
    // Configure viewport transform
    glViewport(0, 0, width, height);

    // Configure projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / (float)height, 0.1f, 25.0f);
    
    // Make later transforms affect model-view matrix. 
    glMatrixMode(GL_MODELVIEW);
}


// Window settings.

#define WINDOW_TITLE     "Exercise 2"
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

    glfwSetTime(0.0);
    double t0 = 0.0;
    long tick = 0;
    while (!glfwWindowShouldClose(window))
    {
        double t1 = glfwGetTime();
        double dt = t1 - t0;
        t0 = t1;
        glfwPollEvents();
        update(dt);
        render();
        glfwSwapBuffers(window);
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
}

bool isDuplicate = false;

void windowOnKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;

        case GLFW_KEY_SPACE:
            if (!isDuplicate)
                step();
            isDuplicate = true;
            break;

        default:
            break;
        }
    }
    if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_SPACE:
            isDuplicate = false;
            break;

        default:
            break;
        }
    }
}
