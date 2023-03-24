#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_3d.h"

GLuint VBO;             // буфер вершин
GLuint gWorldLocation;  // указатель для доступа к всемирной матрице

// используем вершинный шейдер
// используем GLSL версии 3.3
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;   // uniform-переменная типа матрицы 4x4                       \n\
                                                                                    \n\
void main() // входная точка в шейдер                                               \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0); //Позиции вершин треугольника в буфере вершин  \n\
}";

// используем пиксельный шейдер
static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = vec4(0.4, 0.2, 0.7, 1.0); // цвет пикселя                           \n\
}";

static void RenderSceneCB()
{
    // очистка буфера кадра
    glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;

    // по сути скорость перемещения треугольника
    Scale += 0.001f;

    // матрица преобразования
    Matrix4f World;

    // заполняем матрицу перемещения
   /* World.m[0][0] = 1.0f; World.m[0][1] = 0.0f; World.m[0][2] = 0.0f; World.m[0][3] = sinf(Scale);
    World.m[1][0] = 0.0f; World.m[1][1] = 1.0f; World.m[1][2] = 0.0f; World.m[1][3] = 0.0f;
    World.m[2][0] = 0.0f; World.m[2][1] = 0.0f; World.m[2][2] = 1.0f; World.m[2][3] = 0.0f;
    World.m[3][0] = 0.0f; World.m[3][1] = 0.0f; World.m[3][2] = 0.0f; World.m[3][3] = 1.0f;*/

    // заполняем матрицу вращений
   /* World.m[0][0] = cosf(Scale); World.m[0][1] = -sinf(Scale); World.m[0][2] = 0.0f; World.m[0][3] = 0.0f;
    World.m[1][0] = sinf(Scale); World.m[1][1] = cosf(Scale);  World.m[1][2] = 0.0f; World.m[1][3] = 0.0f;
    World.m[2][0] = 0.0f;        World.m[2][1] = 0.0f;         World.m[2][2] = 1.0f; World.m[2][3] = 0.0f;
    World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f;         World.m[3][2] = 0.0f; World.m[3][3] = 1.0f;*/

    // заполняем матрицу масштабирования
    World.m[0][0] = sinf(Scale); World.m[0][1] = 0.0f;        World.m[0][2] = 0.0f;        World.m[0][3] = 0.0f;
    World.m[1][0] = 0.0f;        World.m[1][1] = fabs(sinf(Scale)); World.m[1][2] = 0.0f;        World.m[1][3] = 0.0f;
    World.m[2][0] = 0.0f;        World.m[2][1] = 0.0f;        World.m[2][2] = sinf(Scale); World.m[2][3] = 0.0f;
    World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f;        World.m[3][2] = 0.0f;        World.m[3][3] = 1.0f;

    // функция для загрузки данных в uniform-переменные шейдера
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

    glEnableVertexAttribArray(0);
    // привязываем массив вершин к VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Этот вызов говорит конвейеру как воспринимать данные внутри буфера
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // функция для отрисовки
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    // меняем фоновый буфер и буфер кадра местами
    glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
    // функция отображения на экран
    glutDisplayFunc(RenderSceneCB);
    // указываем функцию рендера в качестве ленивой
    glutIdleFunc(RenderSceneCB);
}

static void CreateVertexBuffer()
{
    // координаты точек
    Vector3f Vertices[3];
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);
  
    // функция для генерации объектов
    glGenBuffers(1, &VBO);
    // привязываем массив вершин к VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // передаем данные в массив вершин
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    // создаем шейдер
    GLuint ShaderObj = glCreateShader(ShaderType);

    // проверка
    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    // исходный код шейдера
    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);

    // компилируем шейдер
    glCompileShader(ShaderObj);

    // проверка
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    // присоединяем скомпилированный объект шейдера к объекту программы
    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    // добавляем шейдеры
    AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    // линковка программы
    glLinkProgram(ShaderProgram);
    // проверка на программые ошибки
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    // проверяем еще раз
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }
    // для использования отлинкованной программы шейдеров мы назначаем её для конвейера
    glUseProgram(ShaderProgram);

    gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
    assert(gWorldLocation != 0xFFFFFFFF);
}

int main(int argc, char** argv)
{
    // инициализируем GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // задаем параметры окна
    glutInitWindowSize(1024, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 07");


    InitializeGlutCallbacks();

    //инициализируем GLEW
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    // цвет фона
    glClearColor(100.0f, 0.0f, 50.0f, 0.0f);

    // создание массива точек
    CreateVertexBuffer();

    // компилируем шейдеры
    CompileShaders();

    // основной цикл
    glutMainLoop();

    return 0;
}
