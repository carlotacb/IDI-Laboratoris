TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH += /home2/users/alumnes/1183742/dades/linux/IDI/LAB/Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp  MyForm.cpp \
        MyGLWidget.cpp /home2/users/alumnes/1183742/dades/linux/IDI/LAB/Model/model.cpp