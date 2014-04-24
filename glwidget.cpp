#include "glwidget.h"
#include "GL/glu.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    tesselationSteps = 0;

    // set cube vertices

//    vertices.push_back();

//    glVertex3f(-0.5f, -0.5f, +0.5f);
//    glVertex3f(+0.5f, -0.5f, +0.5f);
//    glVertex3f(+0.5f, +0.5f, +0.5f);
//    glVertex3f(-0.5f, +0.5f, +0.5f);

//    // right
//    glColor3f(0, 1, 0);
//    glVertex3f(+0.5f, -0.5f, +0.5f);
//    glVertex3f(+0.5f, -0.5f, -0.5f);
//    glVertex3f(+0.5f, +0.5f, -0.5f);
//    glVertex3f(+0.5f, +0.5f, +0.5f);

//    // back
//    glColor3f(1, 1, 0);
//    glVertex3f(+0.5f, -0.5f, -0.5f);
//    glVertex3f(-0.5f, -0.5f, -0.5f);
//    glVertex3f(-0.5f, +0.5f, -0.5f);
//    glVertex3f(+0.5f, +0.5f, -0.5f);

//    // left
//    glColor3f(0, 1, 1);
//    glVertex3f(-0.5f, -0.5f, -0.5f);
//    glVertex3f(-0.5f, -0.5f, +0.5f);
//    glVertex3f(-0.5f, +0.5f, +0.5f);
//    glVertex3f(-0.5f, +0.5f, -0.5f);

//    // top
//    glColor3f(1, 0, 0);
//    glVertex3f(-0.5f, +0.5f, +0.5f);
//    glVertex3f(+0.5f, +0.5f, +0.5f);
//    glVertex3f(+0.5f, +0.5f, -0.5f);
//    glVertex3f(-0.5f, +0.5f, -0.5f);

//    // bottom
//    glColor3f(1, 0, 1);
//    glVertex3f(-0.5f, -0.5f, +0.5f);
//    glVertex3f(+0.5f, -0.5f, +0.5f);
//    glVertex3f(+0.5f, -0.5f, -0.5f);
//    glVertex3f(-0.5f, -0.5f, -0.5f);
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
    glEnable(GL_COLOR_MATERIAL);

    // init shading model to flat shading
    glShadeModel(GL_FLAT);

    // enable lighting
    glEnable(GL_LIGHTING);

    // TODO directional lightsource (0.0f) or not?
    float positionLight0[4] = {0.5f, 0.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);
    glEnable(GL_LIGHT0);


}

void GLWidget::paintGL()
{
    glClearColor(0, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw a cube

    glBegin(GL_QUADS);

    // set material properties for the cube
    float diffuseReflection[4] = {0.5, 0.5, 0.5, 1.0};
    float specularReflection[4] = {0.7, 0.3, 0.7, 1.0};
    int shininess = 66.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseReflection);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularReflection);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);

    for (int i = 0; i < tesselationSteps; i++) {
        // interpoliere zwischen jeweiligen Eckpunkten

    }

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
    updateGL();
}

void GLWidget::setFlatShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);
    updateGL();
}

void GLWidget::setGouraudShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    updateGL();
}

void GLWidget::setPhongShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    updateGL();
}

// range of t: [0, 20] (subdivision steps)
void GLWidget::setTesselation(int t)
{
    tesselationSteps = t;
    updateGL();
}
