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

    // mouse events
    virtual void mousePressEvent   (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent    (QMouseEvent *event);

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransformPA ();
    void modelTransformPB ();
    void modelTransformterra ();
    void projectTransform ();
    void viewTransform();
    void radiEsferaContenidora();
    void inicialitzaCamera();

    // Interaccio amb cursor
    typedef enum {NOINTERACCIO, ROTACIO} Interaccio;
    Interaccio interaccio;
    int        xClick, yClick;

    // attribute locations
    GLuint vertexLoc, colorLoc, projectaLoc, viewLoc;
    // uniform locations
    GLuint transLoc;
    // VAO i VBO names
    //GLuint VAO_Casa, VBO_CasaPos, VBO_CasaCol;
    GLuint VAO_Model, VBO_Vertex, VBO_Color;
    GLuint VAO_suelo, VBO_sueloPos, VBO_suelocolor; 
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scalea, scaleb, tx, ty, tz, txb, tyb, tzb;

    //Valores de la perspectiva
    float fov;
    float ra;
    float zN;
    float zF;
    float fovi;
    float deltaFloat;
    
    float bottom;
    float top;
    float right;
    float left;
	bool axonometrica, legopat;

    //Valores de la esfera
    float radi;
    //float centre [3] = {0.0f,0.0f,0.0f};
    float angle, anglea, angleb, angleax, angleaz, anglebx, anglebz;

    float theta;
    float psi;
    float phi;
    float deltaA;
    
    float camera;

    glm::vec3 centre, centrebase;
    glm::vec3 pos;
    glm::vec3 obs;
    glm::vec3 vrp;
    glm::vec3 vup;
    glm::vec3 eixX;
    glm::vec3 eixY;
    glm::vec3 eixZ;

    Model model;
	
	public slots:
		void canviCamera();
		void canviModel();
		void canviFOV(int valor);
		void canviEscala(int valor);
		void canvienX(int angle);
		void canvienY(int angle);
};
