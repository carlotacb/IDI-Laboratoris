#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  tx = 0.0;
  ty = 0.0;
  tz = 0.0;
  angle = 0.0;
  scalex = 1.0f;
  scaley = 1.0f;
  scalez = 1.0f;

  tx2 = 0.0;
  ty2 = 0.0;
  tz2 = 0.0;
  angle2 = 0.0;
  scale2x = 1.0f;
  scale2y = 1.0f;
  scale2z = 1.0f;
}

void MyGLWidget::paintGL ()
{
 // Esborrem el frame-buffer i depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  // Activem el VAO per a pintar l'escena
  glBindVertexArray (VAO);
  transformaModel();
  glDrawArrays(GL_TRIANGLES, 0, 4);

  glBindVertexArray (VAO2);
  transformaModelSegonT();
  glDrawArrays(GL_TRIANGLES, 0, 4);

  glBindVertexArray (0);
}


void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}


void MyGLWidget::createBuffers ()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(0.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(-0.5, 0.0, 0.0);

  glm::vec3 Vertices2[3];
  Vertices2[0] = glm::vec3(0.0, 0.0, 0.0);
  Vertices2[1] = glm::vec3(1.0, 0.0, 0.0);
  Vertices2[2] = glm::vec3(0.5, 1.0, 0.0);


  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);

  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

/***********************************************************/
void MyGLWidget::keyPressEvent (QKeyEvent *e) { 
  
  makeCurrent();    // SEMPRE NECESARI!!    
  
  switch (e->key())  { 
    
    case Qt::Key_Left : 
      tx -= 0.1; 
      break; 
      
    case Qt::Key_Right : 
      tx += 0.1; 
      break; 
      
    case Qt::Key_Up : 
      ty += 0.1; 
      break; 
      
    case Qt::Key_Down : 
      ty -= 0.1; 
      break; 
    
	case Qt::Key_R :
	  angle += M_PI/6;
	  angle2 -= M_PI/6;
	  break;
	  
    case Qt::Key_P:
      scalex += 0.5;
	  break;
	  
    case Qt::Key_O:
      scalex -= 0.5;
	  break;

    case Qt::Key_I:
      scaley += 0.5;
      break;

    case Qt::Key_U:
      scaley -= 0.5;
      break;
	     

    case Qt::Key_A:
      tx2 -= 0.1;
      break;

    case Qt::Key_D :
      tx2 += 0.1;
      break;

    case Qt::Key_W :
      ty2 += 0.1;
      break;

    case Qt::Key_S :
      ty2 -= 0.1;
      break;

    case Qt::Key_L:
      scale2x += 0.5;
      break;

    case Qt::Key_K:
      scale2x -= 0.5;
      break;

    case Qt::Key_J:
      scale2y += 0.5;
      break;

    case Qt::Key_H:
      scale2y -= 0.5;
      break;

    default:
      e->ignore (); // propagar al pare
  }
  
  update();
}

/************************************************************/


/************************************************************/

void MyGLWidget::transformaModel () {
  glm::mat4 TG(1.0);   // Matriu de transformació,
                       // inicialment la identitat
  TG = glm::scale(TG, glm::vec3(scalex, scaley, scalez));
  TG = glm::rotate(TG, angle, glm::vec3(0,0,1));
  TG = glm::translate (TG, glm::vec3 (tx, ty, tz));
  glUniformMatrix4fv (TGLoc, 1, GL_FALSE, &TG[0][0]); 
}

void MyGLWidget::transformaModelSegonT () {
    glm::mat4 TG(1.0);

    TG = glm::scale(TG, glm::vec3(scale2x, scale2y, scale2z));
    TG = glm::rotate(TG, angle2, glm::vec3(0,0,1));
    TG = glm::translate(TG, glm::vec3(tx2, ty2, tz2));
    glUniformMatrix4fv (TGLoc, 1, GL_FALSE, &TG[0][0]);
}

/************************************************************/

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  TGLoc = glGetUniformLocation(program->programId(), "TG");
}
