#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qimage.h>
#include <qtimer.h>

#include "canvas.h"

#include <stdlib.h>
#include <TApplication.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TPluginManager.h>
#include <TVirtualGL.h>
#include <TVirtualX.h>

int main(int argc, char** argv)
{
   TApplication rootapp("Simple Qt ROOT Application", &argc, argv);
#if QT_VERSION < 0x050000
   if (!gGLManager) {
      TString x = "win32";
      if (gVirtualX->InheritsFrom("TGX11"))
         x = "x11";
      else if (gVirtualX->InheritsFrom("TGCocoa"))
         x = "osx";
      TPluginHandler *ph = gROOT->GetPluginManager()->FindHandler("TGLManager", x);
      if (ph && ph->LoadPlugin() != -1)
         ph->ExecPlugin(0);
   }
   gStyle->SetCanvasPreferGL(true);
#endif
   QApplication app(argc,argv);
   QMainCanvas m(0);

   m.resize(m.sizeHint());
   m.setWindowTitle("Qt Example - Canvas");
   m.setGeometry( 100, 100, 699, 499 );
   m.show();
   m.resize(700, 500);

   QObject::connect( qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()) );

   return app.exec();
}


