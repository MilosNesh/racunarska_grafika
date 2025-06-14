// Autor: Milos Neskovic
// Tema: Artik

#define _CRT_SECURE_NO_WARNINGS
 
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

//GLM biblioteke
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);

int main(void)
{
    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 800;
    unsigned int wHeight = 800;
    const char wTitle[] = "[Generic Title]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    
    glfwMakeContextCurrent(window);

    
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ PROMJENLJIVE I BAFERI +++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    unsigned int nameShader = createShader("name.vert", "name.frag");


    float ocean[] =
    {
        -0.9, 0.0,-0.9,   0.0, 0.0, 0.9, 0.0,
        -0.9, 0.0, 0.9,   0.0, 0.0, 0.9, 0.0,
         0.9, 0.0, 0.9,   0.0, 0.0, 0.9, 0.0,

         0.9, 0.0, 0.9,   0.0, 0.0, 0.9, 0.0,
         0.9, 0.0,-0.9,   0.0, 0.0, 0.9, 0.0,
        -0.9, 0.0,-0.9,   0.0, 0.0, 0.9, 0.0

    };

    float iceberg[] = {
        -0.5,-0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5,-0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
         0.5, 0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5,-0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5, 0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
         0.5,-0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5,-0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
         0.5,-0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
         0.5, 0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
         0.5,-0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5,-0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5,-0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5, 0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
         0.5,-0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5,-0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5,-0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5,-0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
         0.5,-0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
         0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
         0.5,-0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
         0.5, 0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
         0.5,-0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
         0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
         0.5,-0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
         0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
         0.5, 0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5, 0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
         0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5, 0.5,-0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
         0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
        -0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0,
         0.5,-0.5, 0.5, 1.0, 1.0, 1.0, 0.0
    };

    float name[] = {
        -1.0, 1.0,   0.0, 1.0,
        -1.0, 0.9,   0.0, 0.0,
        -0.25, 1.0,   1.0, 1.0,
        -0.25, 0.9,   1.0, 0.0
    };
    unsigned int stride = (3 + 4) * sizeof(float); 
    
    unsigned int VAO[3];
    glGenVertexArrays(3, VAO);

    unsigned int VBO[3];
    glGenBuffers(3, VBO);

    //Trouglovi
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ocean), ocean, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Ocean
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(iceberg), iceberg, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Name
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(name), name, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (2 + 2) * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (2 + 2) * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++            UNIFORME            +++++++++++++++++++++++++++++++++++++++++++++++++
    glm::mat4 modelOcean = glm::mat4(1.0f); // Okean

    glm::mat4 model = glm::mat4(1.0f); //Matrica transformacija - mat4(1.0f) generise jedinicnu matricu
    unsigned int modelLoc = glGetUniformLocation(unifiedShader, "uM");
    
    glm::mat4 view; //Matrica pogleda (kamere)
    unsigned int viewLoc = glGetUniformLocation(unifiedShader, "uV");
    
    
    glm::mat4 projectionP = glm::perspective(glm::radians(90.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f); //Matrica perspektivne projekcije (FOV, Aspect Ratio, prednja ravan, zadnja ravan)
    glm::mat4 projectionO = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f); //Matrica ortogonalne projekcije (Lijeva, desna, donja, gornja, prednja i zadnja ravan)
    unsigned int projectionLoc = glGetUniformLocation(unifiedShader, "uP");


    //Teskutra
    unsigned nameTexture = loadImageToTexture("name.png");
    std::cout << "tekstura: " << nameTexture << std::endl;
    glBindTexture(GL_TEXTURE_2D, nameTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RENDER LOOP - PETLJA ZA CRTANJE +++++++++++++++++++++++++++++++++++++++++++++++++
    glUseProgram(unifiedShader); //Slanje default vrijednosti uniformi
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); //(Adresa matrice, broj matrica koje saljemo, da li treba da se transponuju, pokazivac do matrica)
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));
    glBindVertexArray(VAO[0]);

    glClearColor(0.1, 1.0, 1.0, 1.0);
    glCullFace(GL_BACK);//Biranje lica koje ce se eliminisati (tek nakon sto ukljucimo Face Culling)


    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST); 


    //Sante leda

    glm::vec3 positions[] = {
        glm::vec3(0.0f, -0.3f, 2.0f),
        glm::vec3(1.5f, -0.1f, -1.0f),
        glm::vec3(-1.5f, -0.5f, -2.0f),
        glm::vec3(0.0f, -1.0f, -1.5f),
        glm::vec3(-2.5f, -0.5f, -1.2f),
        glm::vec3(1.7f, -1.0f, 2.0f),
        glm::vec3(-5.7f, -1.0f, 2.0f)
    };

    glm::vec3 rotations[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 45.0f, 0.0f),
        glm::vec3(30.0f, 60.0f, 0.0f),
        glm::vec3(90.0f, 45.0f, 45.0f),
        glm::vec3(30.0f, 60.0f, 0.0f),
        glm::vec3(90.0f, 45.0f, 15.0f),
        glm::vec3(90.0f, 45.0f, 45.0f)
    };

    glm::vec3 scales[] = {
        glm::vec3(0.2f),
        glm::vec3(0.5f),
        glm::vec3(0.75f),
        glm::vec3(1.2f),
        glm::vec3(0.35f),
        glm::vec3(1.3f),
        glm::vec3(4.3f)
    };

    bool showIcebergs = true;
    bool spacePressedLastFrame = false; // Ako je space pritisnuto ne mjenja se showIcebergs

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // Pocetna pozicija kamere
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f); // Gdje kamera gleda
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);  // Up vektor
    float cameraSpeed = 0.05f;
    float yaw = -90.0f; // pocetna orijentacija (gleda ka -Z)
    float pitch = 0.0f;
    float rotationSpeed = 1.0f; // brzina rotacije

    double lastTime = glfwGetTime();
    double targetFrameTime = 1.0 / 60.0;

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - lastTime;

        if (elapsedTime < targetFrameTime)
        {
            glfwWaitEventsTimeout(targetFrameTime - elapsedTime);
        }

        lastTime = currentTime;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        //Mijenjanje projekcija
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));
        }

        //Kretanje kamere
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        // Rotacija kamere
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            yaw -= rotationSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            yaw += rotationSpeed;
        }

        //Prikaz santi
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressedLastFrame) 
        {
            showIcebergs = !showIcebergs;
            spacePressedLastFrame = true;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) 
        {
            spacePressedLastFrame = false;
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Osvjezavamo i Z bafer i bafer boje

        //Prikaz imena
        glUseProgram(nameShader);
        glBindTexture(GL_TEXTURE_2D, nameTexture);
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glUseProgram(unifiedShader);

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        //Crtanje santi leda
        if (showIcebergs)
        {
            for (int i = 0; i < 7; ++i)
            {
                glm::mat4 modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, positions[i]);
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rotations[i].x), glm::vec3(1.0f, 0.0f, 0.0f));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rotations[i].y), glm::vec3(0.0f, 1.0f, 0.0f));
                modelMatrix = glm::rotate(modelMatrix, glm::radians(rotations[i].z), glm::vec3(0.0f, 0.0f, 1.0f));
                modelMatrix = glm::scale(modelMatrix, scales[i]);

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
                glBindVertexArray(VAO[1]);
                glDrawArrays(GL_TRIANGLES, 0, 36); 
            }                   

        }

        //Crtanje okeana
        glm::mat4 oceanModel = glm::mat4(1.0f);
        oceanModel = glm::translate(oceanModel, glm::vec3(0.0f, -0.5f, 0.0f)); // Spustanje okenan 
        oceanModel = glm::scale(oceanModel, glm::vec3(10.0f, 1.0f, 10.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(oceanModel));
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ POSPREMANJE +++++++++++++++++++++++++++++++++++++++++++++++++


    glDeleteBuffers(3, VBO);
    glDeleteVertexArrays(3, VAO);
    glDeleteProgram(unifiedShader);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);
    
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}


static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}