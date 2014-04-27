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

    // constructs an identity quaternion
    currentRotation = QQuaternion();



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


    shaderProgram = new QGLShaderProgram(this);

    vertexShader = new QGLShader(QGLShader::Vertex, this);
    fragmentShader = new QGLShader(QGLShader::Fragment, this);

    vertexShader->compileSourceFile("../hellocube/shaders/vertexShader.vertexShader");
    fragmentShader->compileSourceFile("../hellocube/shaders/fragmentShader.fragmentShader");

    shaderProgram->addShader(vertexShader);
    shaderProgram->addShader(fragmentShader);
    shaderProgram->link();

}

void GLWidget::paintGL()
{
    glClearColor(0, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // set material properties for the cube
    float specularReflection[4] = {1.0, 1.0, 1.0, 1.0};
    int shininess = 66.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularReflection);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    float positionLight0[4] = {0.5f, 0.0f, 2.0f, 1.0f};
//    float currentModelView[16];
//    glGetFloatv(GL_MODELVIEW_MATRIX, currentModelView);
//    QMatrix4x4 mv (currentModelView);
//    QVector4D lightPos (0.5, 0.0, 2.0, 1.0);
//    lightPos = mv.inverted() * lightPos;
//    float blaa[4] = {lightPos.x(), lightPos.y(), lightPos.z(), lightPos.w()};

    glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);


    int nrVerticesSameColor = 4 * tesselationSteps * tesselationSteps;

    glBegin(GL_QUADS);

    for (int i = 0; i < vertices.size(); i++) {
        if (i % nrVerticesSameColor == 0) {
            glColor3f(colors[i / nrVerticesSameColor][0], colors[i / nrVerticesSameColor][1], colors[i / nrVerticesSameColor][2]);
        }
        glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
    }

    glEnd();

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    // diffuse colors
//       GLfloat colorRed[4] = {1.0, 0.0, 0.0, 1.0};
//       GLfloat colorGreen[4] = {0.0, 1.0, 0.0, 1.0};
//       GLfloat colorBlue[4] = {0.0, 0.0, 1.0, 1.0};
//       GLfloat colorCyan[4] = {0.0, 1.0, 1.0, 1.0};
//       GLfloat colorMagenta[4] = {1.0, 0.0, 1.0, 1.0};
//       GLfloat colorYellow[4] = {1.0, 1.0, 0.0, 1.0};

//       // specular color
//       GLfloat colorSpecular[4] = {1.0, 1.0, 1.0, 1.0};

//       glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorSpecular);
//       glMaterialf(GL_FRONT, GL_SHININESS, 128.0);

//    // draw a cube

//    glBegin(GL_QUADS);

//    glColor3f(1,0,0);
//    glVertex3f(0,0,0);
//    glVertex3f(1,0,0);
//    glVertex3f(1,1,0);
//    glVertex3f(0,1,0);

//glColor3f(0,1,0);
//    glVertex3f(0,0,0);
//    glVertex3f(-1,0,0);
//    glVertex3f(-1,1,0);
//    glVertex3f(0,1,0);



//    glEnd();

}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    // set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (height != 0) gluPerspective(45.0d, ((double) width) / ((double) height), 0.01d, 10.0d);
    else gluPerspective(45.0d, 0.0d, 0.01d, 10.0d);

    resetCamera();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        lastTranslationPoint = QVector2D(event->x(), event->y());
    } else if (event->button() == Qt::LeftButton) {
        lastVector = mapPointToTrackball(event->x(), event->y());
    }

    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::RightButton) == Qt::RightButton) {

        QVector2D newPoint = QVector2D(event->x(), event->y());
        float diffX = newPoint.x() - lastTranslationPoint.x();
        // Qt has an inverted y-axis compared to OpenGL
        float diffY = lastTranslationPoint.y() - newPoint.y();

        glMatrixMode(GL_PROJECTION);
        glTranslatef(diffX * 0.01 , diffY * 0.01, 0);
        lastTranslationPoint = newPoint;

    } else if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton) {

        QVector3D newVector = mapPointToTrackball(event->x(), event->y());

        cout << "x: " << newVector.x() << ", y: " << newVector.y() << ", z: " << newVector.z() <<  endl;

        // calculate direction to last point and construct vector that is perpendicular to the plane spanned by the two vectors
        QVector3D normal;
        normal = QVector3D::crossProduct(lastVector, newVector);


        float currentModelView[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, currentModelView);
        QMatrix4x4 mv (currentModelView);
        /* transform the normal with the current rotation, so that the rotation of the cube is not
         * performed in the cube's coordinate system, but instead in the standard coordinate system. */
        normal = mv * normal;

        /* convert the distance between the two points to a number of degrees for the rotation
         * The factor of 100 was chosen through testing and does not have a special meaning. */
        float degrees = acosf(QVector3D::dotProduct(newVector, lastVector)) * 180 / M_PI;

        cout << "degrees: " << degrees << endl;

        // create quaternion from the axis and the angle
        QQuaternion rotation (degrees, normal);
        rotation.normalize();

        // multiply with previous quaternion to add rotations
    //    rotation = rotation * currentRotation;
    //    currentRotation = rotation;


        // when mouse button released: transform quaternion to matrix and apply to modelview matrix
        QMatrix4x4 rotationMatrix;
        rotationMatrix.rotate(rotation);


        glMatrixMode(GL_MODELVIEW);
    //    glLoadIdentity();
    //    gluLookAt(2, 2, 3, 0, 0, 0, 0, 1, 0);

    //    glMultMatrixf(currentModelView);
        glMultMatrixf(rotationMatrix.constData());


        // set this point as last point
        lastVector = newVector;

    }

    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    // vertical rotation in wheel degrees
    int degrees = event->angleDelta().y();

    glMatrixMode(GL_PROJECTION);
    glTranslatef(0, 0, degrees * 0.001f);
    glMatrixMode(GL_MODELVIEW);

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
    float radius = 1.0f;
    if (squaredDistanceFromOrigin2D < radius * radius / 2) {
        z = sqrt(radius * radius - squaredDistanceFromOrigin2D);
    } else {
        z = radius * radius / 2 / sqrt(squaredDistanceFromOrigin2D);
    }

    QVector3D newVector (newPoint.x(), newPoint.y(), z);
    newVector.normalize();

    return newVector;
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


void GLWidget::resetCamera()
{
    // set view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

    updateGL();
}
