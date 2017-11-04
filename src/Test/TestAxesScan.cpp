#include "AxesScan.h"
#include <iostream>
#include "Logger.h"
#include "MainWindow.h"
#include <QImage>
#include <QLineF>
#include <qmath.h>
#include <QPainter>
#include <QPixmap>
#include <QtTest/QtTest>
#include "Test/TestAxesScan.h"

QTEST_MAIN (TestAxesScan)

using namespace std;

const int ARROWDELTA = 9;
const int L = 600; // Include lots of whitespace so rotations do not go out of bounds
const int LOUT = 500;
const int MARGIN_AXES = 120;
const int MARGIN_MAIN_LABELS = 76;
const int NUMTICS = 8;
const int TICKWIDTH = 3;

TestAxesScan::TestAxesScan(QObject *parent) :
  QObject(parent)
{
}

void TestAxesScan::cleanupTestCase ()
{
}

QImage TestAxesScan::generateSample (int angleDegrees) const
{
  // We generate a test image rather than relying on an external image file that may or may not exist
  // and be where we expect it to be
  int i;
  
  QPixmap pixmap (L, L);
  pixmap.fill (QColor (Qt::white));  
  QPainter painter (&pixmap);
  painter.setBrush (QBrush (Qt::black));
  painter.setPen (QPen (Qt::black));

  // X axis
  QLineF lineX (MARGIN_AXES, invert (MARGIN_AXES), L - MARGIN_AXES, invert (MARGIN_AXES));
  painter.drawLine (lineX);
  QPolygonF arrowX;
  arrowX << QPointF (L - MARGIN_AXES + ARROWDELTA, invert (MARGIN_AXES));
  arrowX << QPointF (L - MARGIN_AXES, invert (MARGIN_AXES + ARROWDELTA));
  arrowX << QPointF (L - MARGIN_AXES, invert (MARGIN_AXES - ARROWDELTA));
  painter.drawPolygon (arrowX);

  // X tics and labels
  for (i = 0; i < NUMTICS; i++) {
    int x = MARGIN_AXES + (L - 2 * MARGIN_AXES) * (double) i / (double) NUMTICS;
    QLineF lineX (x, invert (MARGIN_AXES - TICKWIDTH), x, invert (MARGIN_AXES + TICKWIDTH));
    painter.drawLine (lineX); // Tics
    painter.drawText (x - 8, invert (MARGIN_AXES - 2 * TICKWIDTH - 15), QString::number (x)); // Labels
  }
  
  // Y axis
  QLineF lineY (MARGIN_AXES, invert (MARGIN_AXES), MARGIN_AXES, invert (L - MARGIN_AXES));
  painter.drawLine (lineY);
  QPolygonF arrowY;
  arrowY << QPointF (MARGIN_AXES, invert (L - MARGIN_AXES + ARROWDELTA));
  arrowY << QPointF (MARGIN_AXES - ARROWDELTA, invert (L - MARGIN_AXES));
  arrowY << QPointF (MARGIN_AXES + ARROWDELTA, invert (L - MARGIN_AXES));
  painter.drawPolygon (arrowY);
  
  // Y tics and labels
  for (i = 0; i < NUMTICS; i++) {
    int y = MARGIN_AXES + (L - 2 * MARGIN_AXES) * (double) i / (double) NUMTICS;
    QLineF lineY (MARGIN_AXES - TICKWIDTH, invert (y), MARGIN_AXES + TICKWIDTH, invert (y));
    painter.drawLine (lineY); // Tics
    painter.drawText (MARGIN_AXES - 2 * TICKWIDTH - 30, invert (y - 4), QString::number (y)); // Labels
  }

  // Graphs
  for (int g = 0; g < 2; g++) {
    int xLast = 0, yLast = 0;
    switch (g) {
    case 0:
      painter.setPen (QPen (QBrush (Qt::blue), 3.0));
      break;
    case 1:
      painter.setPen (QPen (QBrush (Qt::green), 3.0));
      break;
    }
    for (int x = MARGIN_AXES; x < L - MARGIN_AXES; x++) {
      int y = g * 40 + MARGIN_AXES + 40 + (x - MARGIN_AXES) / 2.0 + 20 * qSin (x / 30.0);
      if (x > MARGIN_AXES) {
        QLineF lineG (xLast, invert (yLast), x, invert (y));
        painter.drawLine (lineG);
      }
      xLast = x;
      yLast = y;
    }
  }
  
  painter.setPen (QPen (QBrush (Qt::black), 1.0));
  painter.setFont (QFont ("Arial", 20));
  
  // Title
  painter.drawText (L / 2 - 60, invert (L - MARGIN_MAIN_LABELS), QString ("Sample Plot"));

  // X legend
  painter.drawText (L / 2 - 2, invert (MARGIN_MAIN_LABELS), QString ("Time"));

  // Y legend
  int R = L / 2;
  painter.rotate (-90); // Makes text vertical along +x axis
  painter.translate (-R, -R + MARGIN_MAIN_LABELS);
  painter.drawText (0, invert (R), QString ("Y Axis"));

  QTransform trans;
  QTransform rotateTransform = trans.rotate (-1.0 * angleDegrees);

  // Rotate the pixmap, convert to QImage and then remove alpha layer
  QPixmap rotated;
  rotated = pixmap.transformed (rotateTransform);
  
  QImage imageCornersAreBlack = rotated.toImage ();

  // Need to remove black corners
  return imageCornersAreBlack.copy (QRect (QPoint ((L - LOUT) / 2,
                                                   invert ((L + LOUT) / 2)),
                                           QPoint ((L + LOUT) / 2,
                                                   invert ((L - LOUT) / 2))));
}

void TestAxesScan::initTestCase ()
{
  const QString NO_ERROR_REPORT_LOG_FILE;
  const QString NO_REGRESSION_OPEN_FILE;
  const bool NO_GNUPLOT_LOG_FILES = false;
  const bool NO_REGRESSION_IMPORT = false;
  const bool NO_RESET = false;
  const bool DEBUG_FLAG = false;
  const QStringList NO_LOAD_STARTUP_FILES;

  initializeLogging ("engauge_test",
                     "engauge_test.log",
                     DEBUG_FLAG);

  MainWindow w (NO_ERROR_REPORT_LOG_FILE,
                NO_REGRESSION_OPEN_FILE,
                NO_GNUPLOT_LOG_FILES,
                NO_RESET,
                NO_REGRESSION_IMPORT,
                NO_LOAD_STARTUP_FILES);
  w.show ();
}

int TestAxesScan::invert (int y) const
{
  return L - y;
}

void TestAxesScan::testShear ()
{
  for (int angleDegrees = -4; angleDegrees <= +4; angleDegrees += 1) {

    QImage image = generateSample (angleDegrees);
 
    AxesScan axesScan (image);

    double kX = axesScan.shearX ();
    double kY = axesScan.shearY ();

    cerr << " kX=" << kX << " kY=" << kY << endl;
    
    QImage imageFixed (image.size (), QImage::Format_RGB32);

    imageFixed.fill (QColor (Qt::white).rgb ());

    for (int rowFrom = 0; rowFrom < image.height (); rowFrom++) {
      for (int colFrom = 0; colFrom < image.width (); colFrom++) {

        // (rowTo) = (1  ky) (rowFrom)
        // (colTo)   (kx  1) (colFrom)
        
        int rowTo = rowFrom - kY * colFrom;
        int colTo = - kX * rowFrom + colFrom;

        if ((0 <= rowTo) &&
            (0 <= colTo) &&
            (rowTo < image.height ()) &&
            (colTo < image.width ())) {

          imageFixed.setPixel (colTo,
                               rowTo,
                               image.pixel (colFrom,
                                            rowFrom));
        }
      }
    }

    QString filename = QString ("shear_%1.png").arg (angleDegrees);
    imageFixed.save (filename);
    
    QVERIFY ((true));
  }
}
