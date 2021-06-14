#include <QPushButton>
#include <QLayout>
#include <QTimer>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QDebug>

#include <stdlib.h>

#include <TCanvas.h>
#include <TVirtualX.h>
#include <TSystem.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1.h>
#include <TF2.h>
#include <TH2D.h>
#include <TFrame.h>
#include <TTimer.h>
#include "canvas.h"

//------------------------------------------------------------------------------

//______________________________________________________________________________
QRootCanvas::QRootCanvas(QWidget *parent) : QWidget(parent, 0), fCanvas(0)
{
   // QRootCanvas constructor.

   // set options needed to properly update the canvas when resizing the widget
   // and to properly handle context menus and mouse move events
   setAttribute(Qt::WA_PaintOnScreen, false);
   setAttribute(Qt::WA_OpaquePaintEvent, true);
   setAttribute(Qt::WA_NativeWindow, true);
   setUpdatesEnabled(kFALSE);
   setMouseTracking(kTRUE);
   setMinimumSize(300, 200);

   // register the QWidget in TVirtualX, giving its native window id
   int wid = gVirtualX->AddWindow((ULong_t)winId(), width(), height());
   // create the ROOT TCanvas, giving as argument the QWidget registered id
   fCanvas = new TCanvas("Root Canvas", width(), height(), wid);
   TQObject::Connect("TGPopupMenu", "PoppedDown()", "TCanvas", fCanvas, "Update()");
}

//______________________________________________________________________________
void QRootCanvas::mouseMoveEvent(QMouseEvent *e)
{
   // Handle mouse move events.

   if (fCanvas) {
      if (e->buttons() & Qt::LeftButton) {
         fCanvas->HandleInput(kButton1Motion, e->x(), e->y());
      } else if (e->buttons() & Qt::MidButton) {
         fCanvas->HandleInput(kButton2Motion, e->x(), e->y());
      } else if (e->buttons() & Qt::RightButton) {
         fCanvas->HandleInput(kButton3Motion, e->x(), e->y());
      } else {
         fCanvas->HandleInput(kMouseMotion, e->x(), e->y());
      }
   }
}

//______________________________________________________________________________
void QRootCanvas::mousePressEvent( QMouseEvent *e )
{
   // Handle mouse button press events.

   if (fCanvas) {
      switch (e->button()) {
         case Qt::LeftButton :
            fCanvas->HandleInput(kButton1Down, e->x(), e->y());
            break;
         case Qt::MidButton :
            fCanvas->HandleInput(kButton2Down, e->x(), e->y());
            break;
         case Qt::RightButton :
            // does not work properly on Linux...
            // ...adding setAttribute(Qt::WA_PaintOnScreen, true) 
            // seems to cure the problem
            fCanvas->HandleInput(kButton3Down, e->x(), e->y());
            break;
         default:
            break;
      }
   }
}

//______________________________________________________________________________
void QRootCanvas::mouseReleaseEvent( QMouseEvent *e )
{
   // Handle mouse button release events.

   if (fCanvas) {
      switch (e->button()) {
         case Qt::LeftButton :
            fCanvas->HandleInput(kButton1Up, e->x(), e->y());
            break;
         case Qt::MidButton :
            fCanvas->HandleInput(kButton2Up, e->x(), e->y());
            break;
         case Qt::RightButton :
            // does not work properly on Linux...
            // ...adding setAttribute(Qt::WA_PaintOnScreen, true) 
            // seems to cure the problem
            fCanvas->HandleInput(kButton3Up, e->x(), e->y());
            break;
         default:
            break;
      }
   }
}

//______________________________________________________________________________
void QRootCanvas::resizeEvent( QResizeEvent *event )
{
   // Handle resize events.

   if (fCanvas) {
      fCanvas->SetCanvasSize(event->size().width(), event->size().height()); 
      fCanvas->Resize();
      fCanvas->Update();
   }
}

//______________________________________________________________________________
void QRootCanvas::paintEvent( QPaintEvent * )
{
   // Handle paint events.

   if (fCanvas) {
      fCanvas->Resize();
      fCanvas->Update();
   }
}

//------------------------------------------------------------------------------

//______________________________________________________________________________
QMainCanvas::QMainCanvas(QWidget *parent) : QWidget(parent)
{
   // QMainCanvas constructor.

   QVBoxLayout *l = new QVBoxLayout(this);
   l->addWidget(canvas = new QRootCanvas(this));
   l->addWidget(b = new QPushButton("&Draw Histogram", this));
   connect(b, SIGNAL(clicked()), this, SLOT(clicked1()));
   fRootTimer = new QTimer( this );
   QObject::connect( fRootTimer, SIGNAL(timeout()), this, SLOT(handle_root_events()) );
   fRootTimer->start( 20 );
}

//______________________________________________________________________________
void QMainCanvas::clicked1()
{
//   // Handle the "Draw Histogram" button clicked() event.

//   static TH1F *h1f = 0;

//   // Create a one dimensional histogram (one float per bin)
//   // and fill it following the distribution in function sqroot.
//   canvas->getCanvas()->Clear();
//   canvas->getCanvas()->cd();
//   canvas->getCanvas()->SetBorderMode(0);
//   canvas->getCanvas()->SetFillColor(0);
//   canvas->getCanvas()->SetGrid();
//   if (h1f == 0) {
//      h1f = new TH1F("h1f","Test random numbers", 200, 0, 10);
//      new TFormula("form1","abs(sin(x)/x)");
//      TF1 *sqroot = new TF1("sqroot","x*gaus(0) + [3]*form1", 0, 10);
//      sqroot->SetParameters(10, 4, 1, 20);
//   }
//   h1f->Reset();
//   h1f->SetFillColor(kViolet + 2);
//   h1f->SetFillStyle(3001);
//   h1f->FillRandom("sqroot", 10000);
//   h1f->Draw();
//   canvas->getCanvas()->Modified();
//   canvas->getCanvas()->Update();

       // Handle the "Draw Histogram" button clicked() event.

       static TH2D *h2d = 0;
       static TF2  *f2 = 0;
       static int counts = 0;
       // Create a one dimensional histogram (one float per bin)
       // and fill it following the distribution in function sqroot.
       canvas->getCanvas()->Clear();
       canvas->getCanvas()->cd();
       canvas->getCanvas()->SetBorderMode(0);
       if (h2d == 0) {
          h2d = new TH2D("h2d","Test random numbers", 360, -180, 180, 180, -90, 90);
          f2= new TF2("f2","abs(sin(x/180*3.14)*sin(y/180*3.14)/(x/180*3.14*y/180*3.14))",-180,180,-90,90);
       }
       QElapsedTimer timer;
       timer.start();
       h2d->Reset();
       for (int i=1; i<=h2d->GetNbinsX(); i++) {
           for (int j=1; j<= h2d->GetNbinsY(); j++) {
               h2d->SetBinContent(i, j, f2->Eval(h2d->GetXaxis()->GetBinCenter((i + 108*counts )%360 + 1), h2d->GetYaxis()->GetBinCenter(j)) );
           }
       }
       counts++;
       h2d->Draw("COLZ");
       canvas->getCanvas()->Modified();
       canvas->getCanvas()->Update();

       qDebug() << timer.elapsed() << " ms";
}

//______________________________________________________________________________
void QMainCanvas::handle_root_events()
{
   //call the inner loop of ROOT
   gSystem->ProcessEvents();
}

//______________________________________________________________________________
void QMainCanvas::changeEvent(QEvent * e)
{
   if (e->type() == QEvent ::WindowStateChange) {
      QWindowStateChangeEvent * event = static_cast< QWindowStateChangeEvent * >( e );
      if (( event->oldState() & Qt::WindowMaximized ) ||
          ( event->oldState() & Qt::WindowMinimized ) ||
          ( event->oldState() == Qt::WindowNoState && 
            this->windowState() == Qt::WindowMaximized )) {
         if (canvas->getCanvas()) {
            canvas->getCanvas()->Resize();
            canvas->getCanvas()->Update();
         }
      }
   }
}
