//Autor: Milos Neskovic 

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <string>
#include <thread>     
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//stb_image.h je header-only biblioteka za ucitavanje tekstura.
//Potrebno je definisati STB_IMAGE_IMPLEMENTATION prije njenog ukljucivanja
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath); //Ucitavanje teksture, izdvojeno u funkciju

bool isTurnedOn = false;
float x = 0;
float y = 0;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    x = xpos;
    y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && x <= 1273 && x >= 1225 && y >=1000  && y <= 1048) {
        isTurnedOn = !isTurnedOn;
    }
}

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
    unsigned int wWidth = 1920;
    unsigned int wHeight = 1080;
    const char wTitle[] = "[Generic Title]";
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    window = glfwCreateWindow(wWidth, wHeight, wTitle, monitor, NULL);
    
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

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    unsigned int buttonShader = createShader("basic.vert", "button.frag");
    unsigned int crochetShader = createShader("basic.vert", "crochet.frag");
    unsigned int rightCharacterShader = createShader("rightCharacter.vert", "texture.frag");
    unsigned int leftCharacterShader = createShader("leftCharacter.vert", "texture.frag");
    unsigned int newsShader = createShader("basic.vert", "texture.frag");
    unsigned int settingsShader = createShader("basic.vert", "settings.frag");

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    float vertices[] =
    {   //X    Y      S    T 
        -0.95,  0.90,   0.0, 1.0,
        -0.95, -0.80,   0.0, 0.0,
         0.95,  0.90,   1.0, 1.0,
         0.95, -0.80,   1.0, 0.0
    };

    float crochet[] =
    {
        -0.50, 1.0, 0.0, 1.0,
         0.50, 1.0, 1.0, 1.0,
         0.0, 0.55, 0.5, 0.0
    };

    float brand[] =
    {
        -0.25, -0.85,   0.0, 1.0,
        -0.25, -0.95,   0.0, 0.0,
         0.25, -0.85,   1.0, 1.0,
         0.25, -0.95,   1.0, 0.0
    };

    float button[(30 + 2) * 2];
    float r = 0.5; //poluprecnik

    button[0] = 0; //Centar X0
    button[1] = 0; //Centar Y0
    for (int i = 0; i <= 30; i++)
    {

        button[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 360 / 30)); //Xi
        button[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 360 / 30)); //Yi
    }

    float characters[] =
    {   //X     Y       S    T 
         0.20,  0.50,   0.1, 1.0,
         0.20,  0.00,   0.1, 0.0,
         0.00,  0.50,   0.9, 1.0,
         0.00,  0.00,   0.9, 0.0,
         
         -0.20,  0.50,   0.1, 1.0,
         -0.20,  0.00,   0.1, 0.0,
         0.00,  0.50,   0.9, 1.0,
         0.00,  0.00,   0.9, 0.0
    };

    float lines[240];
    float ra = 0.5; //poluprecnik
    int j = 90;

    for (int i = 0; i < 240; i+=10)
    {
        lines[i] = 0;
        lines[i + 1] = 0;
        lines[i + 2] = 0.0;
        lines[i + 3] = 0.0;
        lines[i + 4] = 0.0;
        lines[i + 5] = ra * cos((3.141592 / 180) * j); 
        lines[i + 6] = ra * sin((3.141592 / 180) * j); 
        lines[i + 7] = 0.0;
        lines[i + 8] = 0.0;
        lines[i + 9] = 0.0;
        j -= 30;
    }

    float texts[38*16];
    int a = 0;
    for (int i = 0; i < 38*16; i+=16) {
        texts[i] = 0.95 - a * 0.05;
        texts[i + 1] = -0.70;
        texts[i + 2] = 1.0;
        texts[i + 3] = 1.0;

        texts[i + 4] = 0.95 - a * 0.05;
        texts[i + 5] = -0.80;
        texts[i + 6] = 1.0;
        texts[i + 7] = 0.0;

        texts[i + 8] = 0.95 - (a + 1) * 0.05;
        texts[i + 9] = -0.70;
        texts[i + 10] = 0.0;
        texts[i + 11] = 1.0;

        texts[i + 12] = 0.95 - (a + 1) * 0.05;
        texts[i + 13] = -0.80;
        texts[i + 14] = 0.0;
        texts[i + 15] = 0.0;

        a++;
    }

    float settings[] = {
        -0.85,  0.70,   0.0, 1.0,
        -0.85,  0.50,   0.0, 0.0,
        -0.25,  0.70,   1.0, 1.0,
        -0.25,  0.50,   1.0, 0.0,

        -0.85,  0.40,   0.0, 1.0,
        -0.85,  0.20,   0.0, 0.0,
        -0.25,  0.40,   1.0, 1.0,
        -0.25,  0.20,   1.0, 0.0,

        -0.85,  0.10,   0.0, 1.0,
        -0.85, -0.10,   0.0, 0.0,
        -0.25,  0.10,   1.0, 1.0,
        -0.25, -0.10,   1.0, 0.0,

        -0.85, -0.20,   0.0, 1.0,
        -0.85, -0.40,   0.0, 0.0,
        -0.25, -0.20,   1.0, 1.0,
        -0.25, -0.40,   1.0, 0.0,

        -0.85, -0.50,   0.0, 1.0,
        -0.85, -0.70,   0.0, 0.0,
        -0.25, -0.50,   1.0, 1.0,
        -0.25, -0.70,   1.0, 0.0
    };

    unsigned int stride = (2 + 2) * sizeof(float);

    unsigned int VAO[8];
    glGenVertexArrays(8, VAO);
    unsigned int VBO[8];
    glGenBuffers(8, VBO);

    //TV
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Heklanje
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crochet), crochet, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Brand
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(brand), brand, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Button
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(button), button, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Characters
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(characters), characters, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Time
    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (2 + 3) * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (2 + 3) * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //News texts
    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texts), texts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Settings
    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(settings), settings, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Teksture
    unsigned crochetTexture = loadImageToTexture("res/heklanje.png");
    glBindTexture(GL_TEXTURE_2D, crochetTexture);
    glGenerateMipmap(GL_TEXTURE_2D); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned brandTexture = loadImageToTexture("res/brand.png");
    glBindTexture(GL_TEXTURE_2D, brandTexture);
    glGenerateMipmap(GL_TEXTURE_2D); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned rightCharacterTexture = loadImageToTexture("res/spiderman.png");
    glBindTexture(GL_TEXTURE_2D, rightCharacterTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned leftCharacterTexture = loadImageToTexture("res/WoodyWoodpecker.png");
    glBindTexture(GL_TEXTURE_2D, leftCharacterTexture);
    glGenerateMipmap(GL_TEXTURE_2D); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned newsTexture = loadImageToTexture("res/tv.jpg");
    glBindTexture(GL_TEXTURE_2D, newsTexture);
    glGenerateMipmap(GL_TEXTURE_2D); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    map<char, unsigned> letters;

    for (char c = 'A'; c <= 'Z'; ++c) {
        string texturePath = "res/";
        texturePath += c;
        texturePath += ".png";

        unsigned texture = loadImageToTexture(texturePath.c_str());
        glBindTexture(GL_TEXTURE_2D, texture);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        letters[c] = texture;
    }

    unsigned spaceTexture = loadImageToTexture("res/SPACE.png");
    glBindTexture(GL_TEXTURE_2D, spaceTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    letters[' '] = spaceTexture;

    unsigned brightnessPTexture = loadImageToTexture("res/brightness_p.png");
    glBindTexture(GL_TEXTURE_2D, brightnessPTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned brightnessMTexture = loadImageToTexture("res/brightness_m.png");
    glBindTexture(GL_TEXTURE_2D, brightnessMTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned colorBTexture = loadImageToTexture("res/color_blue.png");
    glBindTexture(GL_TEXTURE_2D, colorBTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned colorGTexture = loadImageToTexture("res/color_gray.png");
    glBindTexture(GL_TEXTURE_2D, colorGTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned volumeTexture = loadImageToTexture("res/volume.png");
    glBindTexture(GL_TEXTURE_2D, volumeTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    float leftX = 0;
    float rightX = 0;
    float channel = 4;
    float y = 0.6;
    int activeSettings = 0;
    float clockColor[] = {
        0.5, 0.5, 0.5
    };
    float brightness = 1.0;
    int vijest = 1;
    int letterForShow = 1;

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

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && rightX > 0.01)
        {
            rightX -= 0.01;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && rightX < 0.74)
        {
            rightX += 0.01;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && leftX > -0.74)
        {
            leftX -= 0.01;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && leftX < -0.01)
        {
            leftX += 0.01;
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && isTurnedOn)
        {
            channel = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && isTurnedOn)
        {
            channel = 2;
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && isTurnedOn)
        {
            channel = 3;
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && isTurnedOn)
        {
            channel = 4;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && isTurnedOn && y < 1)
        {
            if (y > 0.7)
                y = -0.7;
            else;
            y += 0.04;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && isTurnedOn && y > -1)
        {
            if (y < -0.7)
                y = 0.7;
            else
                y -= 0.04;

        }

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && isTurnedOn && channel == 3) {
            if (activeSettings == 2 && brightness <= 1.0) {
                brightness += 0.04;
            }
            if (activeSettings == 3 && brightness >= 0.5) {
                brightness -= 0.04;
            }
            if (activeSettings == 4) {
                clockColor[0] = 0.0;
                clockColor[1] = 0.0;
                clockColor[2] = 1.0;
            }

            if (activeSettings == 5) {
                clockColor[0] = 0.5;
                clockColor[1] = 0.5;
                clockColor[2] = 0.5;
            }

        }

        glClearColor(0.4, 0.4, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(unifiedShader);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glUseProgram(crochetShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, crochetTexture);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindTexture(GL_TEXTURE_2D, brandTexture);
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glViewport(1200, 5, 100, 100);
        glUseProgram(buttonShader);
        glBindVertexArray(VAO[3]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(button) / (2 * sizeof(float)));

        if (isTurnedOn) {
            glUniform3f(glGetUniformLocation(buttonShader, "buttonCol"), 1, 0, 0.506);
            glUseProgram(unifiedShader);
            glUniform3f(glGetUniformLocation(unifiedShader, "tvCol"), 1, 1, 1);
            glViewport(0, 0, wWidth, wHeight);


            if (channel == 1)
            {
                glUniform3f(glGetUniformLocation(unifiedShader, "tvCol"), clockColor[0], clockColor[1], clockColor[2]);
                glUniform1f(glGetUniformLocation(unifiedShader, "brightness"), brightness);
                glBindVertexArray(VAO[3]);
                glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(button) / (2 * sizeof(float)));

                glUniform3f(glGetUniformLocation(unifiedShader, "tvCol"), 0, 0, 0);
                glBindVertexArray(VAO[5]);

                glLineWidth(15);
                glDrawArrays(GL_LINES, (int)glfwGetTime() % 12 * 2, 2);
            }
            if (channel == 2) {
                glUseProgram(newsShader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, newsTexture);
                glUniform1f(glGetUniformLocation(newsShader, "brightness"), brightness);
                glBindVertexArray(VAO[0]);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


                string news = (vijest == 1) ? "PARTIZAN  BOLJI U VJECITOM DERBIJU " : "VUCIC U POSJETI KINI";;

                int size = news.size();
                int let = 0;
                int loc = 0;
                for (int i = 0; i < size + 38;i++) {
                    news += ' ';
                }
                static double lastExecutionTime = 0;
                double currTime = glfwGetTime();
                if (currTime - lastExecutionTime >= 0.2) {
                    lastExecutionTime = currTime;
                    letterForShow++;
                    let = 0;

                    if (letterForShow == news.size()) {
                        let = 0;
                        loc = 0;
                        letterForShow = 1;
                        vijest = (vijest == 1) ? 2 : 1;
                    }
                }
                for (int i = 0; i < letterForShow; i++) {
                    for (int j = loc; j >= 0; j--) {
                        glBindTexture(GL_TEXTURE_2D, letters[news.at(let)]);
                        glBindVertexArray(VAO[6]);
                        glDrawArrays(GL_TRIANGLE_STRIP, j * 4, 4);
                        let++;
                    }
                    if (loc <= 37) {
                        let = 0;
                        loc++;
                    }
                    else
                    {
                        let = i - let + 1;   // za one vijesti koje su duze od 38 karaktera
                    }
                }

                glUseProgram(crochetShader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, crochetTexture);
                glBindVertexArray(VAO[1]);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            if (channel == 3) {
                glUniform3f(glGetUniformLocation(unifiedShader, "tvCol"), 0, 0, 0);
                glBindVertexArray(VAO[3]);

                glUseProgram(settingsShader);
                glActiveTexture(GL_TEXTURE0);
                glUniform1f(glGetUniformLocation(settingsShader, "brightness"), brightness);
                glUniform1f(glGetUniformLocation(settingsShader, "y"), y);

                glUniform1i(glGetUniformLocation(settingsShader, "activeRect"), 1);
                glBindTexture(GL_TEXTURE_2D, volumeTexture);
                glBindVertexArray(VAO[7]);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

                glUniform1i(glGetUniformLocation(settingsShader, "activeRect"), 2);
                glBindTexture(GL_TEXTURE_2D, brightnessPTexture);
                glBindVertexArray(VAO[7]);
                glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

                glUniform1i(glGetUniformLocation(settingsShader, "activeRect"), 3);
                glBindTexture(GL_TEXTURE_2D, brightnessMTexture);
                glBindVertexArray(VAO[7]);
                glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

                glUniform1i(glGetUniformLocation(settingsShader, "activeRect"), 4);
                glBindTexture(GL_TEXTURE_2D, colorBTexture);
                glBindVertexArray(VAO[7]);
                glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

                glUniform1i(glGetUniformLocation(settingsShader, "activeRect"), 5);
                glBindTexture(GL_TEXTURE_2D, colorGTexture);
                glBindVertexArray(VAO[7]);
                glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);

                if (y < 0.7 && y >= 0.4)
                    activeSettings = 1;
                else if (y < 0.4 && y > 0.2)
                    activeSettings = 2;
                else if (y <= 0.2 && y > -0.1)
                    activeSettings = 3;
                else if (y < -0.1 && y > -0.4)
                    activeSettings = 4;
                else if (y <= -0.4 && y > -0.7)
                    activeSettings = 5;

            }
            if (channel == 4)
            {
                glUseProgram(unifiedShader);
                glUniform3f(glGetUniformLocation(unifiedShader, "tvCol"), 1, 1, 1);
                glUniform1f(glGetUniformLocation(unifiedShader, "brightness"), brightness);

                glUseProgram(rightCharacterShader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, rightCharacterTexture);
                glUniform1f(glGetUniformLocation(rightCharacterShader, "brightness"), brightness);
                glBindVertexArray(VAO[4]);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glUniform1f(glGetUniformLocation(rightCharacterShader, "x"), rightX);

                glUseProgram(leftCharacterShader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, leftCharacterTexture);
                glUniform1f(glGetUniformLocation(leftCharacterShader, "brightness"), brightness);
                glBindVertexArray(VAO[4]);
                glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
                glUniform1f(glGetUniformLocation(leftCharacterShader, "x"), leftX);
            }

        }
        else {
            glUniform3f(glGetUniformLocation(buttonShader, "buttonCol"), abs(sin(glfwGetTime())), abs(sin(glfwGetTime())), abs(sin(glfwGetTime())));
            glUseProgram(unifiedShader);
            glUniform3f(glGetUniformLocation(unifiedShader, "tvCol"), 0, 0, 0);
            leftX = 0;
            rightX = 0;
        }
           
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &crochetTexture);
    glDeleteTextures(1, &brandTexture);
    glDeleteTextures(1, &newsTexture);
    glDeleteBuffers(8, VBO);
    glDeleteVertexArrays(8, VAO);
    glDeleteProgram(unifiedShader);
    glDeleteProgram(buttonShader);
    glDeleteProgram(crochetShader);
    glDeleteProgram(newsShader);
    glDeleteProgram(settingsShader);

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