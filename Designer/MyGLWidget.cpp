#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
  scalea = 1.0f;
  scaleb = 1.0f;
  tx = 1;
  ty = 0;
  tz = 1;
  
  txb = -1;
  tyb = 0;
  tzb = -1;
  
  anglea = 0.0f;
  angleax = 0.0f;
  angleaz = 0.0f;
  angleb = (float)M_PI;
  anglebx = 0.0f;
  anglebz = 0.0f;
  
  camera = 1; // ORTOGRAFICA
  legopat = true;
  //Valores iniciales de los valores de perspectiva
  
/*  fov = (float)M_PI/2.0f;
  fovi = fov;
  ra = 1.0f;
  zN = 1.0f;
  zF = 2.0f;*/

  
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

  //INICIALIZACIONES
  eixX   = glm::vec3(1.0, 0.0, 0.0);
  eixY   = glm::vec3(0.0, 1.0, 0.0);
  eixZ   = glm::vec3(0.0, 0.0, 1.0);
  theta  = 0.0;
  psi    = 0.0;
  phi    = 0.0;

  xClick     = 0;
  yClick     = 0;
  deltaA     = M_PI / 180.0;
  deltaFloat = (float)M_PI / 50.0;
  interaccio = NOINTERACCIO;
  
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  createBuffers();
  inicialitzaCamera();
}

void MyGLWidget::radiEsferaContenidora()
{
  float xmin, xmax, ymin, ymax, zmin, zmax;

   xmin = xmax = model.vertices()[0];
   ymin = ymax = model.vertices()[1];
   zmin = zmax = model.vertices()[2];
   for (unsigned int i = 3; i < model.vertices().size(); i += 3)
   {
     if (model.vertices()[i+0] < xmin)
       xmin = model.vertices()[i+0];
     if (model.vertices()[i+0] > xmax)
       xmax = model.vertices()[i+0];
     if (model.vertices()[i+1] < ymin)
       ymin = model.vertices()[i+1];
     if (model.vertices()[i+1] > ymax)
       ymax = model.vertices()[i+1];
     if (model.vertices()[i+2] < zmin)
       zmin = model.vertices()[i+2];
     if (model.vertices()[i+2] > zmax)
       zmax = model.vertices()[i+2];
   }
   //float dx  = xmax - xmin;
   //float dy  = ymax - ymin;
   //float dz  = zmax - zmin;
   radi      = sqrt(4*4 + 4*4 + 1*1)/2.0;
   scalea     = 1/(ymax-ymin);
   scaleb     = 1/(ymax-ymin);
   centre[0] = (xmax + xmin) / 2.0;
   centre[1] = (ymax + ymin) / 2.0;
   centre[2] = (zmax + zmin) / 2.0;
   
   centrebase[0] = centre[0];
   centrebase[1] = ymin;
   centrebase[2] = centre[2];
   
   left = -1.2 * radi;
   right =  1.2 * radi;
   top =  1.2 * radi;
   bottom = -1.2 * radi;
}

void MyGLWidget::inicialitzaCamera()
{

  radiEsferaContenidora();

  obs = glm::vec3(0.0, 0.0, 1.5 * radi);
  vrp = glm::vec3(0.0, 0.0, 0.0);
  vup = glm::vec3(0.0, 1.0, 0.0);


  float d = 0;
  for (int i = 0; i < 3; i += 1){
    d = d + (obs[i] - vrp[i]) * (obs[i] - vrp[i]);
  }
  d     = sqrt(d);
  //znear = (d - radi) / 2.0;
  zN    = (d - radi) / 2.0;
  //zfar  = d + radi;
  zF    = d + radi;
  fovi  = 2.0 * asin(radi / d); // (float)M_PI / 2.0f;
  fov   = fovi;
  ra    = 1.0;
  angle = 0.0;

  viewTransform ();
  projectTransform ();

}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  int dx = abs(e->x() - xClick);
  int dy = abs(e->y() - yClick);

  if (dx > dy)
  {   // gir "psi" respecte eixY
    if (e->x() > xClick)
    {
      psi  += abs(e->x() - xClick) * deltaA;
    }
    else if (e->x() < xClick)
    {
      psi  -= abs(e->x() - xClick) * deltaA;
    }
  }
  else
  {  // gir "theta" respecte eixX
    if (e->y() > yClick)
    {
      theta -= abs(e->y() - yClick) * deltaA;
    }
    else if (e->y() < yClick)
    {
      theta += abs(e->y() - yClick) * deltaA;
    }
  }
  update ();

  xClick = e->x();
  yClick = e->y();
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *)
{
  makeCurrent();
  interaccio = NOINTERACCIO;
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  makeCurrent();
  xClick = e->x();
  yClick = e->y();
  if (e->button() & Qt::LeftButton)
  {
    interaccio = ROTACIO;
  }
  else
  {
    interaccio = NOINTERACCIO;
  }
}


void MyGLWidget::paintGL ()
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  projectTransform();
  viewTransform();
  
  
  // Activem el VAO per a pintar la caseta
  glBindVertexArray (VAO_suelo);
   
  modelTransformterra();
  // pintem
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  
  // Carreguem la transformació de model
  
  
  
  // Activem el VAO per a pintar la caseta
  glBindVertexArray (VAO_Model);
  
  modelTransformPA ();
  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 3 * model.faces().size());

 
    // Activem el VAO per a pintar la caseta
  glBindVertexArray (VAO_Model);
  modelTransformPB ();
  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 3 * model.faces().size());

  glBindVertexArray (0);
  
  
  
}

void MyGLWidget::modelTransformPA ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, -glm::vec3(tx,ty,tz)); // T = Pfin - Pini
  transform = glm::rotate(transform, anglea, glm::vec3(0,1,0)); 
  transform = glm::rotate(transform, angleax, glm::vec3(1,0,0));
  transform = glm::rotate(transform, angleaz, glm::vec3(0,0,1));
  transform = glm::scale(transform, glm:: vec3(scalea));
  transform = glm::translate(transform, -centrebase); // T = Pfin - Pini
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPB ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, -glm::vec3(txb,tyb,tzb)); // T = Pfin - Pini
  transform = glm::rotate(transform, angleb , glm::vec3(0,1,0));
  transform = glm::rotate(transform, anglebx, glm::vec3(1,0,0));
  transform = glm::rotate(transform, anglebz, glm::vec3(0,0,1));
  transform = glm::scale(transform, glm:: vec3(scaleb));
  transform = glm::translate(transform, -centrebase); // T = Pfin - Pini
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformterra ()
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::resizeGL (int w, int h)
{
  // Es crida quan canvien les dimensions de la vista
  // els parametres (w, h) corresponen a la nova vista!!
  // Es fixa el ratio al nou valor i
  // S'ajusta la finestra (el fov), si cal

  float rViewport = float (w) / float (h);
  ra  = rViewport;
  if (rViewport < 1.0) {
    fov = 2.0 * atan(tan(fovi/2.0)/rViewport);
  }

  // Es conserva la vista. Mesures en pixels.
  glViewport(0, 0, w, h);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {  
  
  switch (event->key()) {
    makeCurrent();
    
    // CONTROLS PATRICIO A - ESCALA:
    
    case Qt::Key_S: { // escalar a més gran
      scalea += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scalea -= 0.05;
      break;
    }
    
    // CONTROLS PATRICIO A - MOURE: 
    
    case Qt::Key_Up: {
      ty += 0.5;
      break;
    }
    
    case Qt::Key_Down: {
      ty -= 0.5;
      break;
    }
    
    case Qt::Key_Left: {
      tx -= 0.5;
      break;
    }
    
    case Qt::Key_Right: {
      tx += 0.5;
      break;
    }
    
    case Qt::Key_M: {
      tz += 0.5;
      break;
    }
    
    case Qt::Key_N: {
      tz -= 0.5;
      break;
    }
    
    // CONTROLS PATRICIO A - ROTAR:
    
    case Qt::Key_R: {
      anglea += 45;
      break;
    }
    
    case Qt::Key_E: {
      angleax += 45;
      break;
    }
    
    case Qt::Key_W: {
      angleaz += 45;
      break;
    }
    
    // CONTROLS PATRICIO B - ESCALA:
    
    case Qt::Key_X: {
      scaleb += 0.05;
      break;
    }
    case Qt::Key_C: {
      scaleb -= 0.05;
      break;
    }
    
    // CONTROLS PATRICIO B - MOURE: 
    
    case Qt::Key_K: {
      tyb += 0.5;
      break;
    }
    case Qt::Key_I: {
      tyb -= 0.5;
      break;
    }
    case Qt::Key_L: {
      txb += 0.5;
      break;
    }
    case Qt::Key_J: {
      txb -= 0.5;
      break;
    }
    case Qt::Key_O: {
      tzb -= 0.5;
      break;
    }
    case Qt::Key_P: {
      tzb += 0.5;
      break;
    }
    
    
    // CONTROLS PATRICIO B - ROTAR:
    
    case Qt::Key_T: {
      angleb += 45;
      break;
    }
    
    case Qt::Key_Y: {
      anglebx += 45;
      break;
    }
    
    case Qt::Key_U: {
      anglebz += 45;
      break;
    }
    
    
    // CAMBIAR LA CAMARA:
    
    case Qt::Key_B: {
      camera = 0; //Prespectiva
      break;
    }
    case Qt::Key_V: {
      camera = 1; // ORTOGRAFICA
      break;
    }
    
    
    // CAMBIAR EL ANGLE DE VISIÓ EN CAS DE SER UNA OPTICA PERSPECTIVA
    
    case Qt::Key_F: { // fov a més petit
      if(fov>deltaFloat)
        fov -= deltaFloat;
      else
        fov = deltaFloat;
      break;
    }
    case Qt::Key_G: { // fov a més petit
      if(fov < (float)M_PI)
        fov += deltaFloat;
      else
        fov = (float)M_PI;
      break;
    }
    
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::createBuffers ()
{
  
  // Dades Patricio:
	if (legopat) {
		model.load("/home2/users/alumnes/1183742/dades/linux/IDI/LAB/models/Patricio.obj");
	}
	else {
		model.load("/home2/users/alumnes/1183742/dades/linux/IDI/LAB/models/legoman.obj");
	}
  
  // Dades Terra:
  
  glm::vec3 posterra[4] = {
    glm::vec3(-2.0,0.0,-2.0),
    glm::vec3(-2.0,0.0,2.0),
    glm::vec3(2.0,0.0,-2.0),
    glm::vec3(2.0,0.0,2.0)
  };
  
  glm::vec3 colorterra[4] = {
    glm::vec3(0.5,0,1),
    glm::vec3(0.5,0,1),
    glm::vec3(0.5,0,1),
    glm::vec3(0.5,0,1)
  };


  // Creació del Vertex Array Object per pintar
  
  glGenVertexArrays(1, &VAO_Model);
  glBindVertexArray(VAO_Model);

  glGenBuffers(1, &VBO_Vertex);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertex);
  glBufferData(GL_ARRAY_BUFFER,
                sizeof(GLfloat) * model.faces().size() * 3 * 3,
                model.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_Color);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Color);
  glBufferData(GL_ARRAY_BUFFER,
                sizeof(GLfloat) * model.faces().size() * 3 * 3,
                model.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
  
    
  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_suelo);
  glBindVertexArray(VAO_suelo);
    
  //CARREGA LA GEOMETRIA:
  
  glGenBuffers(1, &VBO_sueloPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_sueloPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);
  
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  // CARREGA EL COLOR:
  
  glGenBuffers(1, &VBO_suelocolor);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_suelocolor);
  glBufferData(GL_ARRAY_BUFFER,sizeof(colorterra), colorterra, GL_STATIC_DRAW);
  
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
  
  
  
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
  projectaLoc = glGetUniformLocation(program->programId(), "PR");
  viewLoc = glGetUniformLocation(program->programId(), "VW");
}

void MyGLWidget::projectTransform () {
//   // Matriu de projecciÃ³ del model
//   //glm::mat4 projecta = glm::perspective(fov, ra, znear, zfar);
// 
//   glm::mat4 projecta = glm::ortho(left, right, bottom, top, zN, zF);
//   glUniformMatrix4fv(projectaLoc, 1, GL_FALSE, &projecta[0][0]);

  // Matriu de projecciÃ³ del model
  glm::mat4 projecta;
  if (axonometrica){
    projecta = glm::ortho(left, right, bottom, top, zN, zF);
  } else {
    projecta = glm::perspective(fov, ra, zN, zF);
  }
  glUniformMatrix4fv(projectaLoc, 1, GL_FALSE, &projecta[0][0]);
}

void MyGLWidget::viewTransform()
{
  /*glm::mat4 view = glm::lookAt( glm::vec3(0.0, 0.0, 1.0),
                                glm::vec3(0.0, 0.0, 0.0),
                                glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);*/
  // Matriu de projecciÃ³ del model
       // glm::mat4 view = glm::lookAt(obs, vrp, vup);
       // VisiÃ³ amb Ã ngles d'Euler
    glm::vec3 trans = vrp - obs;
    glm::mat4 view(1.0f);
    view = glm::translate(view, trans);  // posa el zero a l'observador
    view = glm::rotate(view, -psi,  eixY);
    view = glm::rotate(view, theta, eixX);
    view = glm::rotate(view, phi,   eixZ);
    view = glm::translate(view, -vrp);   // restaura el zero
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
}

void MyGLWidget::canviCamera() {
	
	makeCurrent();
	axonometrica = !axonometrica;
		
	createBuffers();
	inicialitzaCamera();
	
	update();
	
}

void MyGLWidget::canviModel() {
	
	makeCurrent();
	legopat = !legopat;
	
	createBuffers();
	inicialitzaCamera();
	
	update();
	
}

void MyGLWidget::canviFOV(int valor) {
	
	makeCurrent();
	
	createBuffers();
	inicialitzaCamera();
	
	fov = (float)valor/50;
	
	update();
}

void MyGLWidget::canviEscala(int valor) {
	
	makeCurrent();
		
	createBuffers();
	inicialitzaCamera();
	
	scalea = (float)valor/50;
	scaleb = (float)valor/50;
	
	update();
}

void MyGLWidget::canvienX(int angle) {
	makeCurrent();
	psi = (float) angle / 180.0 * (float)M_PI;
	update ();
}

void MyGLWidget::canvienY(int angle) {
	makeCurrent();
	psi = (float) angle / 180.0 * (float)M_PI;
	update ();
}

	
