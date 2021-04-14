
#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include<gl/glu.h>
#include "glwidget.h"



GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    qtGreen = QColor::fromRgbF(0,1,0,1);
    qtPurple = QColor::fromRgbF(0,0,0.5,1);
}

GLWidget::~GLWidget()
{
}



static void AngleRange(int &angle)
{
    while (angle < 0)
        angle += 360 ;
    while (angle > 360 )
        angle -= 360 ;
}

void GLWidget::setXRotation(int angle)
{
    AngleRange(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    AngleRange(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    AngleRange(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::initializeGL()
{
    qglClearColor(qtPurple.dark());

   

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
   
    static GLfloat lightPosition[4] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
}

void GLWidget::draw()
{
 static const GLfloat coords[6] [4] [3] = {
 { { +1.0, -1.0, +1.0 }, { +1.0, -1.0, -1.0 },
 { +1.0, +1.0, -1.0 }, { +1.0, +1.0, +1.0 } },
 { { -1.0, -1.0, -1.0 }, { -1.0, -1.0, +1.0 },
 { -1.0, +1.0, +1.0 }, { -1.0, +1.0, -1.0 } },
 { { +1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 },
 { -1.0, +1.0, -1.0 }, { +1.0, +1.0, -1.0 } },
 { { -1.0, -1.0, +1.0 }, { +1.0, -1.0, +1.0 },
 { +1.0, +1.0, +1.0 }, { -1.0, +1.0, +1.0 } },
 { { -1.0, -1.0, -1.0 }, { +1.0, -1.0, -1.0 },
 { +1.0, -1.0, +1.0 }, { -1.0, -1.0, +1.0 } },
 { { -1.0, +1.0, +1.0 }, { +1.0, +1.0, +1.0 },
 { +1.0, +1.0, -1.0 }, { -1.0, +1.0, -1.0 } }
 };

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslatef(0.0, 0.0, -10.0);
 glRotatef(xRot, 1.0, 0.0, 0.0);
 glRotatef(yRot, 0.0, 1.0, 0.0);
 glRotatef(zRot, 0.0, 0.0, 1.0);

 for (int i = 0; i < 6; ++i) {
 glLoadName(i);
 glBegin(GL_QUADS);
 qglColor(qtPurple);
 for (int j = 0; j < 4; ++j) {
 glVertex3f(coords[i] [j] [0], coords[i] [j] [1],
 coords[i] [j] [2]);
 }
 glEnd();
 }
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 GLfloat x = (GLfloat)width / height;
 glFrustum(-x, x, -1.0, 1.0, 4.0, 15.0);
 glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 0.5 * dy);
        setYRotation(yRot + 0.5 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 0.5 * dy);
        setZRotation(zRot + 0.5 * dx);
    }
    lastPos = event->pos();
}