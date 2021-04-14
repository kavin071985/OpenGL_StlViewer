
#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include<gl/glu.h>
#include "glwidget.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>

struct vertex
{
    GLdouble x, y, z;
	vertex():x(0),y(0),z(0){}
	vertex(double _x,double _y, double _z):x(_x),y(_y),z(_z){}

};

static std::vector<vertex> normal;
static std::vector<vertex> vertices;
static int nl = 0, vx = 0;



GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;

    qtGreen = QColor::fromRgbF(0,1,0,1);
    qtPurple = QColor::fromRgbF(1,0,0.5,1);
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
void GLWidget::stlpaser()
{
	std::string line, facet, norm;
    GLdouble x = 0, y = 0, z = 0;
    std::ifstream ifs("aneurysm.ascii.stl");
    if (ifs.is_open())
        std::cout << "File is Opened" << std::endl;

    while (!ifs.eof())
    {
        getline(ifs, line);

         if (line.find("vertex") != std::string::npos)
        {
            std::istringstream is(line);
            is >> norm >> x >> y >> z;
            vertices.push_back(vertex( x,y,z ));

            ++vx;
        }
	}

	 for (int i = 0; i < vertices.size(); )
    {
        GLdouble a[3] = { vertices[i + 1].x - vertices[i].x ,vertices[i + 1].y - vertices[i].y,vertices[i + 1].z - vertices[i].z };

        GLdouble b[3] = { vertices[i + 2].x - vertices[i].x ,vertices[i + 2].y - vertices[i].y,vertices[i + 2].z - vertices[i].z };

        GLdouble v[3] = { a[1] * b[2] - a[2] * b[1], -(a[0] * b[2] - a[2] * b[0]), a[0] * b[1] - a[1] * b[0] };

        GLdouble length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        for (int j = 0; j < 3; j++)
        {
            v[j] = v[j] / length;
        }
        
        normal.push_back(vertex(v[0], v[1], v[2]));
		nl++;
        i = i + 3;
	 }
}


void GLWidget::initializeGL()
{
	stlpaser();
    qglClearColor(qtPurple.dark());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
	
}
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
}

void GLWidget::draw()
{
 GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
 GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
 GLfloat materialColor[] = {0.0f, 1.0f, 0.0f, 1.0f};
 GLfloat shininess = 20;
 
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslatef(0.0, 0.0, -15.0);
 int sl=45; glScalef(sl,sl,sl);
 glRotatef(xRot, 1.0, 0.0, 0.0);
 glRotatef(yRot, 0.0, 1.0, 0.0);
 glRotatef(zRot, 0.0, 0.0, 1.0);

 glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
 glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
 glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialColor);
 glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess); 

 glBegin(GL_TRIANGLES);          

    for (int n = 0,v = 0; n < nl;)
  {     
      glNormal3d(normal[n].x, normal[n].y, normal[n].z); n++;
      glVertex3d(vertices[v].x, vertices[v].y, vertices[v].z); v++;
      glVertex3d(vertices[v].x, vertices[v].y, vertices[v].z); v++;
      glVertex3d(vertices[v].x, vertices[v].y, vertices[v].z); v++;
      glNormal3d(normal[n].x, normal[n].y, normal[n].z); n++;
      glVertex3d(vertices[v].x, vertices[v].y, vertices[v].z); v++;
      glVertex3d(vertices[v].x, vertices[v].y, vertices[v].z); v++;
      glVertex3d(vertices[v].x, vertices[v].y, vertices[v].z); v++;
  }
    glEnd();
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 GLfloat x = (GLfloat)width / height;
 glFrustum(-x, x, -1.0, 1.0, 4.0, 25.0);
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
        setXRotation(xRot +  dy);
        setYRotation(yRot +  dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot +  dy);
        setZRotation(zRot +  dx);
    }
    lastPos = event->pos();
}