#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);


  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void modelTransformTerra ();
    void projectTransform ();
    void viewTransform ();
	void EsferaContenedora ();
	void inicamera ();

    // Interaccio amb cursor
    typedef enum {NOINTERACCIO, ROTACIO} Interaccio;
    Interaccio interaccio;
    int        xClick, yClick;

    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO i VBO names
    GLuint VAO, VBO_CasaPos, VBO_CasaCol, VAO_Terra, VBO_Posicio, VBO_Color;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
	
	// Per camera:
    float FOV, FOVi, ra, znear, zfar, radi;
	
	// Per transformacions:
	float scale, anglex, angley, tx, ty, tz;
	float theta, psi, phi;

    glm::vec3 pos;
	
	glm::vec3 obs, vrp, up, centre;
	
	glm::vec3 eixX, eixY, eixZ;

    Model m;

};

