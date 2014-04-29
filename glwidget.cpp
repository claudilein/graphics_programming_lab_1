#include "glwidget.h"
#include "GL/glu.h"
#include <iostream>
#include <math.h>
#include <QPointF>
#include <QVector3D>
#include <QMatrix4x4>
#include <QCoreApplication>
#include <QDir>

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

    // set cube normals
    float normal0[3] = {0, 0, 1};
    float normal1[3] = {1, 0, 0};
    float normal2[3] = {0, 0, -1};
    float normal3[3] = {-1, 0, 0};
    float normal4[3] = {0, -1, 0};
    float normal5[3] = {0, 1, 0};

    normals.push_back(std::vector<float> (normal0, normal0 + 3));
    normals.push_back(std::vector<float> (normal1, normal1 + 3));
    normals.push_back(std::vector<float> (normal2, normal2 + 3));
    normals.push_back(std::vector<float> (normal3, normal3 + 3));
    normals.push_back(std::vector<float> (normal4, normal4 + 3));
    normals.push_back(std::vector<float> (normal5, normal5 + 3));

    // set rotation, zoom and translation to default
    currentRotation = QQuaternion();
    currentZoom = -3;
    currentTranslation = QVector2D();

    // set radius of the trackball
    trackballRadius = 1.0f;

}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(600, 600);
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
    glEnable(GL_LIGHT0);

    // position lightsource
    float positionLight0[4] = {0.5f, 0.0f, 2.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);

    // set material properties for the cube
    float specularReflection[4] = {1.0, 1.0, 1.0, 1.0};
    int shininess = 120.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularReflection);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // set clear color
    glClearColor(0, 0, 0, 1);

    // compile and link shaders
    shaderProgram = new QGLShaderProgram(this);

    vertexShader = new QGLShader(QGLShader::Vertex, this);
    fragmentShader = new QGLShader(QGLShader::Fragment, this);

    vertexShader->compileSourceFile(":/shaders/vertexShader.vertexShader");
    fragmentShader->compileSourceFile(":/shaders/fragmentShader.fragmentShader");

    shaderProgram->addShader(vertexShader);
    shaderProgram->addShader(fragmentShader);
    shaderProgram->link();

}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // apply translation and zoom
    glTranslatef(currentTranslation.x(), currentTranslation.y(), currentZoom);
    // apply rotation
    QMatrix4x4 rotation;
    rotation.rotate(currentRotation);
    glMultMatrixf(rotation.constData());


    int nrVerticesSameColor = 4 * tesselationSteps * tesselationSteps;

    glBegin(GL_QUADS);

    for (uint i = 0; i < vertices.size(); i++) {
        if (i % nrVerticesSameColor == 0) {
            glColor3f(colors[i / nrVerticesSameColor][0], colors[i / nrVerticesSameColor][1], colors[i / nrVerticesSameColor][2]);
            glNormal3f(normals[i / nrVerticesSameColor][0], normals[i / nrVerticesSameColor][1], normals[i / nrVerticesSameColor][2]);
        }
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
    if (height != 0) gluPerspective(45.0d, ((double) width) / ((double) height), 0.01d, 10.0d);
    else gluPerspective(45.0d, 0.0d, 0.01d, 10.0d);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        lastTranslationPoint = QVector2D(event->x(), event->y());
    } else if (event->button() == Qt::LeftButton) {
        lastRotationPoint = mapPointToTrackball(event->x(), event->y());
    }

    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    int x = event->x();
    int y = event->y();
    bool valid = true;

    if (x > width() || y > height() || x < 0 || y < 0) valid = false;

    if ((event->buttons() & Qt::RightButton) == Qt::RightButton && valid) {

        QVector2D newPoint = QVector2D(x, y);
        float diffX = newPoint.x() - lastTranslationPoint.x();
        // Qt has an inverted y-axis compared to OpenGL
        float diffY = lastTranslationPoint.y() - newPoint.y();

        float scaleFactor = 0.01;
        currentTranslation += QVector2D(diffX * scaleFactor, diffY * scaleFactor);
        lastTranslationPoint = newPoint;

    } else if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton && valid) {

        QVector3D newPoint = mapPointToTrackball(x, y);

        // calculate direction to last point and construct vector that is perpendicular to the plane spanned by the two vectors
        QVector3D normal;
        normal = QVector3D::crossProduct(lastRotationPoint, newPoint);

        /* transform the normal with the current rotation */
        float currentModelView[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, currentModelView);
        QMatrix4x4 mv (currentModelView);
        normal = mv * normal;

        /* convert the distance between the two points to a number of degrees for the rotation */
        float degrees = acosf(QVector3D::dotProduct(newPoint, lastRotationPoint)) * 180 / M_PI;

        // create quaternion from the axis and the angle
        QQuaternion rotation (degrees, normal);
        rotation.normalize();

        // multiply with previous quaternion to add rotations
        rotation = currentRotation * rotation;
        currentRotation = rotation;

        lastRotationPoint = newPoint;
    }
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    // vertical rotation in wheel degrees
    int degrees = event->angleDelta().y();

    currentZoom += degrees * 0.001f;
    if (currentZoom < -9) currentZoom = -9;
    if (currentZoom > -2) currentZoom = -2;

    updateGL();
}



// ========================== HELPER FUNCTIONS ========================== //

QVector3D GLWidget::mapPointToTrackball(float x, float y) {

    /* map mouse point (given in xy-plane) to the range of [-1, 1] in both dimensions
     * note: (0, 0) equals the upper left corner in the mouse coordinates retrieved,
     *       but OpenGL considers (0, 0) to be the lower left corner. Thus, the y-axis
     *       is inverted here.
     */
    QPointF newPoint ((2 * x - width()) / (float) width(), (-1) * (2 * y - height()) / (float) height());

    // treat this point as point on a unit hemisphere -> calculate corresponding z-value and normalize the vector
    float squaredDistanceFromOrigin2D = newPoint.x() * newPoint.x() + newPoint.y() * newPoint.y();

    // determine whether to map z to the sphere or the hyperbolic sheet
    float z = 0.0f;
    if (squaredDistanceFromOrigin2D < trackballRadius * trackballRadius / 2) {
        z = sqrt(trackballRadius * trackballRadius - squaredDistanceFromOrigin2D);
    } else {
        z = trackballRadius * trackballRadius / 2 / sqrt(squaredDistanceFromOrigin2D);
    }

    QVector3D newPoint3D (newPoint.x(), newPoint.y(), z);
    newPoint3D.normalize();

    return newPoint3D;
}



// ========================== SETTER FUNCTIONS ========================== //


void GLWidget::setWireframeShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shaderProgram->release();
    updateGL();
}

void GLWidget::setFlatShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);
    shaderProgram->release();
    updateGL();
}

void GLWidget::setGouraudShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    shaderProgram->release();
    updateGL();
}

void GLWidget::setPhongShading()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    shaderProgram->bind();
    updateGL();
}

// range of t: [0, 50] (subdivision steps)
void GLWidget::setTesselation(int t)
{
    tesselationSteps = t;

    std::vector<std::vector<float> > newVertices;

    // for each quad
    for (uint quad = 0; quad < originalVertices.size() / 4; quad++) {
        // the vertices of a quad are listed counterclockwise, starting at the lower left corner
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


void GLWidget::resetCamera()
{
    // set rotation, zoom and translation to default
    currentRotation = QQuaternion();
    currentZoom = -3;
    currentTranslation = QVector2D();

    updateGL();
}
