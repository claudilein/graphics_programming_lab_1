#include "glwidget.h"
#include "GL/glu.h"
#include <iostream>
#include <math.h>

using namespace std;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    tesselationSteps = 1;

    // set cube vertices
    float x = 0.5f;

    float position0[3] = {-x, -x, +x};
    float position1[3] = {+x, -x, +x};
    float position2[3] = {+x, -x, -x};
    float position3[3] = {-x, -x, -x};
    float position4[3] = {-x, +x, +x};
    float position5[3] = {+x, +x, +x};
    float position6[3] = {+x, +x, -x};
    float position7[3] = {-x, +x, -x};

    // front
    originalVertices.push_back(std::vector<float> (position0, position0 + 3));
    originalVertices.push_back(std::vector<float> (position1, position1 + 3));
    originalVertices.push_back(std::vector<float> (position5, position5 + 3));
    originalVertices.push_back(std::vector<float> (position4, position4 + 3));

    // right
    originalVertices.push_back(std::vector<float> (position2, position2 + 3));
    originalVertices.push_back(std::vector<float> (position6, position6 + 3));
    originalVertices.push_back(std::vector<float> (position5, position5 + 3));
    originalVertices.push_back(std::vector<float> (position1, position1 + 3));

    // back
    originalVertices.push_back(std::vector<float> (position3, position3 + 3));
    originalVertices.push_back(std::vector<float> (position2, position2 + 3));
    originalVertices.push_back(std::vector<float> (position6, position6 + 3));
    originalVertices.push_back(std::vector<float> (position7, position7 + 3));

    // left
    originalVertices.push_back(std::vector<float> (position3, position3 + 3));
    originalVertices.push_back(std::vector<float> (position0, position0 + 3));
    originalVertices.push_back(std::vector<float> (position4, position4 + 3));
    originalVertices.push_back(std::vector<float> (position7, position7 + 3));

    // bottom
    originalVertices.push_back(std::vector<float> (position3, position3 + 3));
    originalVertices.push_back(std::vector<float> (position0, position0 + 3));
    originalVertices.push_back(std::vector<float> (position1, position1 + 3));
    originalVertices.push_back(std::vector<float> (position2, position2 + 3));

    // top
    originalVertices.push_back(std::vector<float> (position7, position7 + 3));
    originalVertices.push_back(std::vector<float> (position4, position4 + 3));
    originalVertices.push_back(std::vector<float> (position5, position5 + 3));
    originalVertices.push_back(std::vector<float> (position6, position6 + 3));

    vertices = originalVertices;

    // set cube colors
    float color0[3] = {0, 0, 1};
    float color1[3] = {1, 0, 0};
    float color2[3] = {1, 1, 0};
    float color3[3] = {0, 1, 1};
    float color4[3] = {1, 0, 1};
    float color5[3] = {0, 1, 0};

    colors.push_back(std::vector<float> (color0, color0 + 3));
    colors.push_back(std::vector<float> (color1, color1 + 3));
    colors.push_back(std::vector<float> (color2, color2 + 3));
    colors.push_back(std::vector<float> (color3, color3 + 3));
    colors.push_back(std::vector<float> (color4, color4 + 3));
    colors.push_back(std::vector<float> (color5, color5 + 3));

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


    int nrVerticesSameColor = 4 * tesselationSteps * tesselationSteps;

    for (int i = 0; i < vertices.size(); i++) {
        if (i % nrVerticesSameColor == 0) glColor3f(colors[i / nrVerticesSameColor][0], colors[i / nrVerticesSameColor][1], colors[i / nrVerticesSameColor][2]);
        glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
    }


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

// range of t: [0, 50] (subdivision steps)
void GLWidget::setTesselation(int t)
{
    tesselationSteps = t;

    std::vector<std::vector<float> > newVertices;

    // for each quad
    for (int quad = 0; quad < originalVertices.size() / 4; quad++) {
        // the vertices of a quads is listed counterclockwise, starting at the lower left corner
        int lowerLeftCorner = quad * 4;
        float stepSize[3] = {0, 0, 0};

        for (int d = 0; d < 3; d++) {
            if (originalVertices[lowerLeftCorner][d] != originalVertices[lowerLeftCorner + 2][d]) {
                stepSize[d] = (fabs(originalVertices[lowerLeftCorner][d]) + fabs(originalVertices[lowerLeftCorner + 2][d])) / t;
            }
        }

        std::vector<float> lowerLeftVertex = originalVertices[lowerLeftCorner];

        for (int i = 0; i < t; i++) {
            for (int j = 0; j < t; j++) {
                if (stepSize[0] == 0) {  // in case the quad is in the zy plane
                    float position0[3] = {lowerLeftVertex[0], lowerLeftVertex[1], lowerLeftVertex[2]};
                    float position1[3] = {lowerLeftVertex[0], lowerLeftVertex[1], lowerLeftVertex[2] + stepSize[2]};
                    float position2[3] = {lowerLeftVertex[0], lowerLeftVertex[1] + stepSize[1], lowerLeftVertex[2] + stepSize[2]};
                    float position3[3] = {lowerLeftVertex[0], lowerLeftVertex[1] + stepSize[1], lowerLeftVertex[2]};

                    // insert new quad into vertices list
                    newVertices.push_back(std::vector<float> (position0, position0 + 3));
                    newVertices.push_back(std::vector<float> (position1, position1 + 3));
                    newVertices.push_back(std::vector<float> (position2, position2 + 3));
                    newVertices.push_back(std::vector<float> (position3, position3 + 3));
                } else if (stepSize[1] == 0) {  // in case the quad is in the xz plane
                    float position0[3] = {lowerLeftVertex[0], lowerLeftVertex[1], lowerLeftVertex[2]};
                    float position1[3] = {lowerLeftVertex[0] + stepSize[0], lowerLeftVertex[1], lowerLeftVertex[2]};
                    float position2[3] = {lowerLeftVertex[0] + stepSize[0], lowerLeftVertex[1], lowerLeftVertex[2] + stepSize[2]};
                    float position3[3] = {lowerLeftVertex[0], lowerLeftVertex[1], lowerLeftVertex[2] + stepSize[2]};

                    // insert new quad into vertices list
                    newVertices.push_back(std::vector<float> (position0, position0 + 3));
                    newVertices.push_back(std::vector<float> (position1, position1 + 3));
                    newVertices.push_back(std::vector<float> (position2, position2 + 3));
                    newVertices.push_back(std::vector<float> (position3, position3 + 3));
                } else if (stepSize[2] == 0) {  // in case the quad is in the xy plane
                    float position0[3] = {lowerLeftVertex[0], lowerLeftVertex[1], lowerLeftVertex[2]};
                    float position1[3] = {lowerLeftVertex[0] + stepSize[0], lowerLeftVertex[1], lowerLeftVertex[2]};
                    float position2[3] = {lowerLeftVertex[0] + stepSize[0], lowerLeftVertex[1] + stepSize[1], lowerLeftVertex[2]};
                    float position3[3] = {lowerLeftVertex[0], lowerLeftVertex[1] + stepSize[1], lowerLeftVertex[2]};

                    // insert new quad into vertices list
                    newVertices.push_back(std::vector<float> (position0, position0 + 3));
                    newVertices.push_back(std::vector<float> (position1, position1 + 3));
                    newVertices.push_back(std::vector<float> (position2, position2 + 3));
                    newVertices.push_back(std::vector<float> (position3, position3 + 3));
                }
                lowerLeftVertex = newVertices[quad * (t * t * 4) + i * (t * 4) + j * 4 + 1];
            }
            lowerLeftVertex = newVertices[quad * (t * t * 4) + i * (t * 4) + 3];
        }
    }

    vertices = newVertices;

    updateGL();
}
