TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#win32 {
#   QMAKE_CXXFLAGS += -FIw32pragma.h
#}
CONFIG += qt warn_on thread

INCLUDEPATH += $(ROOTSYS)/include
win32:LIBS += -L$(ROOTSYS)/lib -llibCore -llibRIO -llibNet \
        -llibHist -llibGraf -llibGraf3d -llibGpad -llibTree \
        -llibRint -llibPostscript -llibMatrix -llibPhysics \
        -llibGui -llibRGL 
else:LIBS += -L$$(ROOTSYS)/lib -lCore -lImt -lRIO -lNet -lHist -lGraf \
        -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix \
        -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic

HEADERS += canvas.h
SOURCES += canvas.cxx main.cxx


