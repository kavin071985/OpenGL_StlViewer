

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QWheelEvent>
#include <Qt>


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();


public :
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void stlpaser();

private:
	void draw();
    int xRot;
    int yRot;
    int zRot;
	GLdouble mRange;
	GLdouble mZoom;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;
};

#endif
