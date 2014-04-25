#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QVector3D>
#include <QMatrix4x4>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:

public slots:
    void setWireframeShading();
    void setFlatShading();
    void setGouraudShading();
    void setPhongShading();

    void setTesselation(int t);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    QVector3D mapPointToTrackball(float x, float y);

private:
    int tesselationSteps;
    std::vector<std::vector<float> > originalVertices;
    std::vector<std::vector<float> > vertices;
    std::vector<std::vector<float> > colors;
    QVector3D lastVector;
    QQuaternion currentRotation;

};

#endif // GLWIDGET_H
