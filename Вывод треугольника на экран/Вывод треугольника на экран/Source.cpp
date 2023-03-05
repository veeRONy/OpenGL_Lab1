#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_3d.h"

GLuint VBO; // буфер вершин

static void RenderSceneCB()
{
    // очистка буфера кадра
    glClear(GL_COLOR_BUFFER_BIT);

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

static void CreateVertexBuffer()
{
    // массив точек
    Vector3f Vertices[3];

    // координаты точек
    Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
    Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

    // цвет треугольника
    glColor3f(0.5f, 1.0f, 0.5f);

    // функция для генерации объектов
    glGenBuffers(1, &VBO);

    // привязываем массив вершин к VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // передаем данные в массив вершин
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}


int main(int argc, char** argv)
{
    // инициализируем GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // задаем параметры окна
    glutInitWindowSize(900, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 03");

    // функция отображения на экран
    glutDisplayFunc(RenderSceneCB);

    //инициализируем GLEW
    GLenum res = glewInit();
    //проверяем на ошибки
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }
    // цвет фона
    glClearColor(0.4f, 0.2f, 50.0f, 0.0f);

    CreateVertexBuffer();

    // основной цикл
    glutMainLoop();

    return 0;
}