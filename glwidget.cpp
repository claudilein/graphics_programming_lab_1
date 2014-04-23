#include "glwidget.h"
#include "GL/glu.h"
#include <QMessageBox>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{

}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::initializeGL()
{
    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // init shading model to flat shading
    glShadeModel(GL_FLAT);

    // enable lighting
    glEnable(GL_LIGHTING);

    // TODO directional lightsource (0.0f) or not?
    float positionLight0[] = {0.5f, 0.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);
    glEnable(GL_LIGHT0);


}

void GLWidget::paintGL()
{
    glClearColor(0, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw a cube

    glBegin(GL_QUADS);

    // front
    glColor3f(0, 0, 1);
    glVertex3f(-0.5f, -0.5f, +0.5f);
    glVertex3f(+0.5f, -0.5f, +0.5f);
    glVertex3f(+0.5f, +0.5f, +0.5f);
    glVertex3f(-0.5f, +0.5f, +0.5f);

    // right
    glColor3f(0, 1, 0);
    glVertex3f(+0.5f, -0.5f, +0.5f);
    glVertex3f(+0.5f, -0.5f, -0.5f);
    glVertex3f(+0.5f, +0.5f, -0.5f);
    glVertex3f(+0.5f, +0.5f, +0.5f);

    // back
    glColor3f(1, 1, 0);
    glVertex3f(+0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, +0.5f, -0.5f);
    glVertex3f(+0.5f, +0.5f, -0.5f);

    // left
    glColor3f(0, 1, 1);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, +0.5f);
    glVertex3f(-0.5f, +0.5f, +0.5f);
    glVertex3f(-0.5f, +0.5f, -0.5f);

    // top
    glColor3f(1, 0, 0);
    glVertex3f(-0.5f, +0.5f, +0.5f);
    glVertex3f(+0.5f, +0.5f, +0.5f);
    glVertex3f(+0.5f, +0.5f, -0.5f);
    glVertex3f(-0.5f, +0.5f, -0.5f);

    // bottom
    glColor3f(1, 0, 1);
    glVertex3f(-0.5f, -0.5f, +0.5f);
    glVertex3f(+0.5f, -0.5f, +0.5f);
    glVertex3f(+0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    glEnd();

}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    // set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0d, ((double) width) / ((double) height), 0.01d, 10.0d);

    // set view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2, 2, 3, 0, 0, 0, 0, 1, 0);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{}

void GLWidget::wheelEvent(QWheelEvent *event)
{}


void GLWidget::setWireframeShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    paintGL();
}

void GLWidget::setFlatShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);
    paintGL();
}

void GLWidget::setGouraudShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    paintGL();
}

void GLWidget::setPhongShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    paintGL();
}
