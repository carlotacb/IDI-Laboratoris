TEMPLATE    = app
QT         += opengl

INCLUDEPATH += ../glm
INCLUDEPATH += ../Model

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h \
    model.h \

SOURCES += main.cpp  MyForm.cpp \
        MyGLWidget.cpp ../Model/model.cpp \

OTHER_FILES += \
    shaders/fragshad.frag \
    shaders/vertshad.vert
