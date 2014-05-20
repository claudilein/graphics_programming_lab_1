#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSlider>
#include <glwidget.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showAboutBox();

private:
    QMenuBar *menuBar;
    QToolBar *toolBar;
    QSlider *slider;
    QStatusBar *statusBar;
    GLWidget *glWidget;


    // == FILE MENU == //
    QMenu *fileMenu;
    QAction *exitAction;    

    // == SHADING MENU == //
    QMenu *shadingMenu;
    QActionGroup *shadingGroup;

    QAction *noShadingAction;
    QAction *flatShadingAction;
    QAction *gouraudShadingAction;
    QAction *phongShadingAction;

    // == CAMERA MENU == //
    QMenu *cameraMenu;
    QAction *resetCameraAction;

    // == ABOUT MENU == //
    QMenu *aboutMenu;
    QAction *aboutAction;
};

#endif // MAINWINDOW_H
