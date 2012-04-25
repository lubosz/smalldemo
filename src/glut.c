#include <GL/glut.h>

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow (argv[0]);
    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}
