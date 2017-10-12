#define GLM_FORCE_RADIANS

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  
	
	virtual void keyPressEvent (QKeyEvent *e);

  private:
    void createBuffers ();
    void carregaShaders ();
	void transformaModel ();
    void transformaModelSegonT ();
    
    // attribute locations
    GLuint vertexLoc,/* colorLoc,*/ TGLoc;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO, VBO, VAO2, VBO2;
// 	GLuint VBO_COLOR;
	
	float tx;
	float ty;
	float tz;
	float angle;
    float scalex, scaley, scalez;

    float tx2;
    float ty2;
    float tz2;
    float angle2;
    float scale2x, scale2y, scale2z;
};
