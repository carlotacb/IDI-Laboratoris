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

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  
  inicamera ();
  
  // Inicialitzacions:
  txa = 1.0;
  tya = 0.0;
  tza = 1.0;
  txb = -1.0;
  tyb = 0.0;
  tzb = -1.0;
  
  perspectiva = true;
  
  eixX   = glm::vec3(1.0, 0.0, 0.0);
  eixY   = glm::vec3(0.0, 1.0, 0.0);
  eixZ   = glm::vec3(0.0, 0.0, 1.0);
  theta  = 0.0;
  psi    = 0.0;
  phi    = 0.0;
  xClick     = 0;
  yClick     = 0;
  deltaA     = M_PI / 180.0;
  interaccio = NOINTERACCIO; 
  
  glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model

  viewTransform();
  projectTransform();

  // Activem el VAO per a pintar la caseta 
  glBindVertexArray (VAO);
  modelTransformPA();
  // pintem
  glDrawArrays(GL_TRIANGLES, 0,  3 * m.faces().size());
  
  glBindVertexArray(VAO);
  modelTransformPB();
  glDrawArrays(GL_TRIANGLES, 0,  3 * m.faces().size());
  


  glBindVertexArray (VAO_Terra);
  modelTransformTerra();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::inicamera () 
{
  EsferaContenedora();

  obs = glm::vec3(0.0, 0.0, 1.5 * radi);
  vrp = glm::vec3(0.0, 0.0, 0.0);
  up = glm::vec3(0.0, 1.0, 0.0);

  float d = 0;
  for (int i = 0; i < 3; i += 1) {
    d = d + (obs[i] - vrp[i]) * (obs[i] - vrp[i]);
  }
  
  d     = sqrt(d);
  znear = (d - radi) / 2.0;
  zfar  = d + radi;
  
  FOVi 	 = 2.0 * asin(radi / d);;
  FOV 	 = FOVi;
  ra 	 = 1.0f;
  anglex = 0;
  angley = 0;

  viewTransform ();
  projectTransform ();

}

void MyGLWidget::EsferaContenedora () 
{
	float xmin, xmax, ymin, ymax, zmin, zmax;
  
  xmin = xmax = m.vertices()[0];
  ymin = ymax = m.vertices()[1];
  zmin = zmax = m.vertices()[2];
  
  for (unsigned int i = 3; i < m.vertices().size(); i += 3) {
	  
    if (m.vertices()[i+0] < xmin) {
		xmin = m.vertices()[i+0];
	}
	
    if (m.vertices()[i+0] > xmax) {
		xmax = m.vertices()[i+0];
	}
	
    if (m.vertices()[i+1] < ymin) {
		ymin = m.vertices()[i+1];
	}
	
    if (m.vertices()[i+1] > ymax) {
		ymax = m.vertices()[i+1];
	}
	
    if (m.vertices()[i+2] < zmin) {
		zmin = m.vertices()[i+2];
	}
	
    if (m.vertices()[i+2] > zmax) {
		zmax = m.vertices()[i+2];
	}
  }
  
  radi      = sqrt(4 * 4 + 4 * 4 + 1 * 1)/2.0;
  scale     = 1/(ymax-ymin);
  centre[0] = (xmax + xmin) / 2.0;
  centre[1] = (ymax + ymin) / 2.0;
  centre[2] = (zmax + zmin) / 2.0; 
  
  centrebase[0] = centre[0]; 
  centrebase[1] = ymin;
  centrebase[2] = centre[2];
  
  left = -1.2 * radi;
  right = 1.2 * radi;
  bottom = -1.2 * radi;
  top = 1.2 * radi;
}

void MyGLWidget::calculEscena () {
	escenaMax[0] = -2.0;
	escenaMax[1] = 0.0;
	escenaMax[2] = -2.0;
	
	escenaMin[0] = 2.0;
	escenaMin[1] = 1.0;
	escenaMin[2] = 2.0;
	
	centreEsc = glm::vec3((escenaMax[0] + escenaMin[0])/2, (escenaMax[1] + escenaMin[1])/2, (escenaMax[2] + escenaMin[2])/2);
	
	float mx = pow(escenaMax[0] - centreEsc[0], 2);
	float my = pow(escenaMax[1] - centreEsc[1], 2);
	float mz = pow(escenaMax[2] - centreEsc[2], 2);
	
	radiEsc = sqrt(mx + my + mz);
}


void MyGLWidget::modelTransformPA () {
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::translate(transform, glm::vec3(txb,tyb,tzb));
  transform = glm::rotate(transform, float(M_PI), glm::vec3(0,1,0));
  transform = glm::rotate(transform, anglex, glm::vec3(1,0,0));
  transform = glm::rotate(transform, angley, glm::vec3(0,1,0));
  transform = glm::translate(transform, -centrebase);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPB () {
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, glm::vec3(txa,tya,tza));
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, anglex, glm::vec3(1,0,0));
  transform = glm::rotate(transform, angley, glm::vec3(0,1,0));
  transform = glm::translate(transform, -centrebase);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra() {
    glm::mat4 transform(1.0f);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform() {
    //prespective(FOV, raw, Znear, Zfar)

    glm::mat4 projecta;
	
	if (perspectiva){
		projecta = glm::perspective(FOV, ra, znear, zfar);
	} 
	
	else {
		projecta = glm::ortho(left, right, bottom, top, znear, zfar);
	}
	
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projecta[0][0]);
}

void MyGLWidget::viewTransform() {
    //lookAt(OBS, VRP, up)
	
	glm::vec3 trans = vrp - obs;
	glm::mat4 view(1.0f);
	view = glm::translate(view, trans);  // posa el zero a l'observador
	view = glm::rotate(view, -psi,  eixY);
	view = glm::rotate(view, theta, eixX);
	view = glm::rotate(view, phi,   eixZ);
	view = glm::translate(view, -vrp);   // restaura el zero
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}
	
void MyGLWidget::resizeGL (int w, int h) 
{
    // Es crida quan canvien les dimensions de la vista
    // els parametres (w, h) corresponen a la nova vista!!
    // Es fixa el ratio al nou valor i
    // S'ajusta la finestra (el fov), si cal

    glViewport(0, 0, w, h);

    ra  = float(w)/h;
    if (ra < 1.0) {
      FOV = 2.0 * glm::atan(glm::tan(FOVi/2.0)/ra);
	  left   = 
	  right  = 
	  top    = 
	  bottom = 
    }

    projectTransform();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  switch (event->key()) {
    makeCurrent();
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: {
        anglex += M_PI/4;
        break;
    }

    case Qt::Key_T: {
        angley += M_PI/4;
        break;
    }
    
	case Qt::Key_X: {
		FOV += 0.1f;
		break;
	}
	
	case Qt::Key_Z: {
		FOV -= 0.1f;
		break;
	}
	
	case Qt::Key_C: {
		perspectiva = !perspectiva;
		break;
	}
		
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e) {
  makeCurrent();
  xClick = e->x();
  yClick = e->y();
  if (e->button() & Qt::LeftButton) {
    interaccio = ROTACIO;
  }else{
    interaccio = NOINTERACCIO;
  }
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *) {
  makeCurrent();
  interaccio = NOINTERACCIO;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
  makeCurrent();
  int dx = abs(e->x() - xClick);
  int dy = abs(e->y() - yClick);

  if (dx > dy){   // gir "psi" respecte eixY
    if (e->x() > xClick) {
      psi  += abs(e->x() - xClick) * deltaA;
    } 
    
    else if (e->x() < xClick) {
      psi  -= abs(e->x() - xClick) * deltaA;
    }
  } 
  
  else {  // gir "theta" respecte eixX
    if (e->y() > yClick) {
      theta -= abs(e->y() - yClick) * deltaA;
    }
    else if (e->y() < yClick) {
      theta += abs(e->y() - yClick) * deltaA;
    }
  }
  update ();

  xClick = e->x();
  yClick = e->y();
  
}

void MyGLWidget::createBuffers () 
{
  // Dades de la caseta
  // Dos VBOs, un amb posició i l'altre amb color

  m.load("../../../../models/Patricio.obj");

  glm::vec3 posicio[4] = {
    glm::vec3(-2.0, 0.0, -2.0),
    glm::vec3(2.0, 0.0, -2.0),
    glm::vec3(-2.0, 0.0, 2.0),
    glm::vec3(2.0, 0.0, 2.0)
  };

  glm::vec3 color[4] = {
    glm::vec3(0.5,0,1),
    glm::vec3(0.5,0,1),
    glm::vec3(0.5,0,1),
    glm::vec3(0.5,0,1)
  };

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO_CasaPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_CasaPos);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(GLfloat) * m.faces().size() * 3 * 3,
               m.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_CasaCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_CasaCol);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(GLfloat) * m.faces().size() * 3 * 3,
               m.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);

  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_Posicio);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Posicio);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_Color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Color);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}

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
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}
