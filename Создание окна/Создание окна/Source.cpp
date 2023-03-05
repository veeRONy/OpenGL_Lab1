#include <GL/freeglut.h>

static void RenderSceneCB()
{
    // очистка буфера кадра
    glClear(GL_COLOR_BUFFER_BIT);
    // меняем фоновый буфер и буфер кадра местами
    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    // инициализируем GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // задаем размер окна
    glutInitWindowSize(900, 700);

    // задаем координаты окна
    glutInitWindowPosition(200, 50);

    // создаем окно с заголовком
    glutCreateWindow("Tutorial 01");

    // функция отображения на экран
    glutDisplayFunc(RenderSceneCB);

    // цвет фона
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // основной цикл
    glutMainLoop();

    return 0;
}