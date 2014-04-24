#include "mainwindow.h"
#include <QMessageBox>
#include <QToolBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    menuBar = new QMenuBar();
    setMenuBar(menuBar);

    toolBar = new QToolBar();
    addToolBar(toolBar);

    statusBar = new QStatusBar();
    setStatusBar(statusBar);

    glWidget = new GLWidget();
    setCentralWidget(glWidget);


    slider = new QSlider();
    slider->setOrientation(Qt::Horizontal);
    slider->setFixedWidth(80);
    connect(slider, SIGNAL(valueChanged(int)), glWidget, SLOT(setTesselation(int)));

    // == FILE MENU == //

    fileMenu = new QMenu("&File");
    exitAction = new QAction("E&xit", fileMenu);

    exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAction);
    menuBar->addMenu(fileMenu);

    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));


    // == SHADING MENU == //

    shadingMenu = new QMenu("&Shading");
    shadingGroup = new QActionGroup(shadingMenu);

    noShadingAction = new QAction("&None (Wireframe)", shadingMenu);
    noShadingAction->setShortcut(tr("Ctrl+1"));
    noShadingAction->setCheckable(true);
    noShadingAction->setIcon(QIcon(":/img/wireframe.png"));
    connect(noShadingAction, SIGNAL(triggered()), glWidget, SLOT(setWireframeShading()));

    flatShadingAction = new QAction("Fla&t", shadingMenu);
    flatShadingAction->setShortcut(tr("Ctrl+2"));
    flatShadingAction->setCheckable(true);
    flatShadingAction->setChecked(true);
    flatShadingAction->setIcon(QIcon(":/img/flat.png"));
    connect(flatShadingAction, SIGNAL(triggered()), glWidget, SLOT(setFlatShading()));

    gouraudShadingAction = new QAction("&Gouraud", shadingMenu);
    gouraudShadingAction->setShortcut(tr("Ctrl+3"));
    gouraudShadingAction->setCheckable(true);
    gouraudShadingAction->setIcon(QIcon(":/img/gouraud.png"));
    connect(gouraudShadingAction, SIGNAL(triggered()), glWidget, SLOT(setGouraudShading()));

    phongShadingAction = new QAction("&Phong", shadingMenu);
    phongShadingAction->setShortcut(tr("Ctrl+4"));
    phongShadingAction->setCheckable(true);
    phongShadingAction->setIcon(QIcon(":/img/phong.png"));
    connect(phongShadingAction, SIGNAL(triggered()), glWidget, SLOT(setPhongShading()));

    shadingGroup->addAction(noShadingAction);
    shadingGroup->addAction(flatShadingAction);
    shadingGroup->addAction(gouraudShadingAction);
    shadingGroup->addAction(phongShadingAction);

    shadingMenu->addAction(noShadingAction);
    shadingMenu->addAction(flatShadingAction);
    shadingMenu->addAction(gouraudShadingAction);
    shadingMenu->addAction(phongShadingAction);

    menuBar->addMenu(shadingMenu);

    // == ABOUT ACTION == //

    aboutAction = new QAction("&About", menuBar);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));

    menuBar->addAction(aboutAction);

    // == TOOL BAR == //

    toolBar->addAction(noShadingAction);
    toolBar->addAction(flatShadingAction);
    toolBar->addAction(gouraudShadingAction);
    toolBar->addAction(phongShadingAction);
    toolBar->addWidget(slider);


}



void MainWindow::showAboutBox()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Hello Cube!");
    msgBox.setText("Written by Claudia Pfreundt!");
    msgBox.exec();
}



MainWindow::~MainWindow()
{
    delete menuBar;
    delete toolBar;
    delete statusBar;
    delete fileMenu;
    delete shadingMenu;
    delete shadingGroup;

    delete exitAction;
    delete noShadingAction;
    delete flatShadingAction;
    delete gouraudShadingAction;
    delete phongShadingAction;
    delete aboutAction;

    delete glWidget;
    delete slider;
}
