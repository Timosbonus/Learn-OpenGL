#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Fenstergröße
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

// Farbstruktur für den Hintergrund
struct color {
    GLfloat r = 0.f;
    GLfloat g = 0.f;
    GLfloat b = 0.f;
} backgroundColor;

// Enum zur Auswahl der Hintergrundfarben
enum colorSwap {
    RED,
    BLUE,
    GREEN,
};

// Funktionsprototypen
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void changeColor(colorSwap newColor);

int main()
{
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW initialisieren und konfigurieren
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW-Fenster erstellen
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD initialisieren, um OpenGL-Funktionen zu laden
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Eckpunkte für linkesDreieck
    float verticesLeft[] = {
        -1.0f, -0.5f, 0.f, // linkes Dreieck, unten links
        -0.5f, 0.5f, 0.f,  // linkes Dreieck, oben
        0.0f, -0.5f, 0.f,  // linkes Dreieck, unten rechts
    };

    // Eckpunkte für rechtesDreieck
    float verticesRight[] = {
            0.0f, -0.5f, 0.f,  // linkes Dreieck, unten rechts
            0.5f, 0.5f, 0.f,   // rechtes Dreieck, oben
            1.0f, -0.5f, 0.f   // rechtes Dreieck, unten rechts
        };

    // Indizes zur Reihenfolge der Eckpunkte
    /*
    unsigned int indices[] = {
        0, 1, 2, // erstes Dreieck
        2, 3, 4  // zweites Dreieck
    };
    */

    // Vertex Array Object (VAO) erstellen und binden
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO (Vertex Buffer Object) zur Speicherung der Eckpunkte
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLeft), verticesLeft, GL_STATIC_DRAW);

    // Vertex-Attributzeiger für linkes Dreieck setzen
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // VAO für rechtes Dreieck
    unsigned int VAOright;
    glGenVertexArrays(1, &VAOright);
    glBindVertexArray(VAOright);

    // VBO für rechtes Dreieck
    unsigned int VBOright;
    glGenBuffers(1, &VBOright);
    glBindBuffer(GL_ARRAY_BUFFER, VBOright);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRight), verticesRight, GL_STATIC_DRAW);

    // Vertex-Attributzeiger setzen (Position der Eckpunkte)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // EBO (Element Buffer Object) zur Speicherung der Indizes
    /*
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    */

    // Vertex-Shader-Quellcode
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    // Vertex-Shader erstellen und kompilieren
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Überprüfen, ob der Vertex-Shader kompiliert wurde
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment-Shader-Quellcode
    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.f, 0.5f, 0.2f, 1.f);\n"
        "}\0";

    // Fragment-Shader erstellen und kompilieren
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Shader-Programm erstellen und verlinken
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);



    // Fragment-Shader-Quellcode
    const char *yellowFragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.f, 1.0f, 0.0f, 1.f);\n"
        "}\0";

    // Fragment-Shader erstellen und kompilieren
    unsigned int yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, nullptr);
    glCompileShader(yellowFragmentShader);

    // yellow Shader-Programm erstellen und verlinken
    unsigned int yellowShaderProgram = glCreateProgram();
    glAttachShader(yellowShaderProgram, vertexShader);
    glAttachShader(yellowShaderProgram, yellowFragmentShader);
    glLinkProgram(yellowShaderProgram);



    // Überprüfen, ob das Shader-Programm verlinkt wurde
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Shader löschen, da sie nun im Programm verlinkt sind
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(yellowFragmentShader);


    // Haupt-Schleife
    while (!glfwWindowShouldClose(window))
    {
        // Eingaben verarbeiten
        processInput(window);

        // Hintergrundfarbe setzen und Bildschirm löschen
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Shader-Programm aktivieren
        glUseProgram(shaderProgram);

        // Linkes Dreieck zeichnen
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(yellowShaderProgram);

        // Rechtes Dreieck zeichnen
        glBindVertexArray(VAOright);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Vertex Array deaktivieren
        glBindVertexArray(0);

        // Buffers tauschen und IO-Ereignisse abfragen
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Resourcen freigeben
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAOright);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBOright);
    // glDeleteBuffers(1, &EBO);

    // GLFW beenden
    glfwTerminate();
    return 0;
}

// Hintergrundfarbe ändern je nach ausgewählter Farbe
void changeColor(const colorSwap newColor)
{
    backgroundColor = {0.f, 0.f, 0.f}; // Standardfarbe auf Schwarz setzen

    switch (newColor) {
        case RED:
            backgroundColor.r = 1.f; // Rot setzen
            break;
        case BLUE:
            backgroundColor.b = 1.f; // Blau setzen
            break;
        case GREEN:
            backgroundColor.g = 1.f; // Grün setzen
            break;
    }
}

// Eingaben verarbeiten: Escape zum Schließen, R/B/G zur Farbänderung
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        changeColor(RED);
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        changeColor(BLUE);
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        changeColor(GREEN);
}

// Callback-Funktion zum Anpassen der Viewport-Größe bei Fensteränderung
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
