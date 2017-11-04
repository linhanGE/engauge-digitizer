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
const int ZERO_ANGLE = 0;
const int ZERO_OFFSET = 0;

TestAxesScan::TestAxesScan(QObject *parent) :
  QObject(parent)
{
}

void TestAxesScan::cleanupTestCase ()
{
}

QImage TestAxesScan::generateSample (double angleDegrees,
                                     int xOffset,
                                     int yOffset) const
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
  QLineF lineX (xOffset + MARGIN_AXES,
                yOffset + invert (MARGIN_AXES),
                xOffset + L - MARGIN_AXES,
                yOffset + invert (MARGIN_AXES));
  painter.drawLine (lineX);
  QPolygonF arrowX;
  arrowX << QPointF (xOffset + L - MARGIN_AXES + ARROWDELTA, invert (yOffset + MARGIN_AXES));
  arrowX << QPointF (xOffset + L - MARGIN_AXES,              invert (yOffset + MARGIN_AXES + ARROWDELTA));
  arrowX << QPointF (xOffset + L - MARGIN_AXES,              invert (yOffset + MARGIN_AXES - ARROWDELTA));
  painter.drawPolygon (arrowX);

  // X tics and labels
  for (i = 0; i < NUMTICS; i++) {
    int x = MARGIN_AXES + (L - 2 * MARGIN_AXES) * (double) i / (double) NUMTICS;
    QLineF lineX (xOffset + x,
                  invert (yOffset + MARGIN_AXES - TICKWIDTH),
                  xOffset + x,
                  invert (yOffset + MARGIN_AXES + TICKWIDTH));
    painter.drawLine (lineX); // Tics
    painter.drawText (xOffset + x - 8,
                      invert (yOffset + MARGIN_AXES - 2 * TICKWIDTH - 15),
                      QString::number (x)); // Labels
  }
  
  // Y axis
  QLineF lineY (xOffset + MARGIN_AXES,
                invert (yOffset + MARGIN_AXES),
                xOffset + MARGIN_AXES,
                invert (yOffset + L - MARGIN_AXES));
  painter.drawLine (lineY);
  QPolygonF arrowY;
  arrowY << QPointF (xOffset + MARGIN_AXES,              invert (yOffset + L - MARGIN_AXES + ARROWDELTA));
  arrowY << QPointF (xOffset + MARGIN_AXES - ARROWDELTA, invert (yOffset + L - MARGIN_AXES));
  arrowY << QPointF (xOffset + MARGIN_AXES + ARROWDELTA, invert (yOffset + L - MARGIN_AXES));
  painter.drawPolygon (arrowY);
  
  // Y tics and labels
  for (i = 0; i < NUMTICS; i++) {
    int y = MARGIN_AXES + (L - 2 * MARGIN_AXES) * (double) i / (double) NUMTICS;
    QLineF lineY (xOffset + MARGIN_AXES - TICKWIDTH,
                  invert (yOffset + y),
                  xOffset + MARGIN_AXES + TICKWIDTH,
                  invert (yOffset + y));
    painter.drawLine (lineY); // Tics
    painter.drawText (xOffset + MARGIN_AXES - 2 * TICKWIDTH - 30,
                      invert (yOffset + y - 4),
                      QString::number (y)); // Labels
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
        QLineF lineG (xOffset + xLast,
                      invert (yOffset + yLast),
                      xOffset + x,
                      invert (yOffset + y));
        painter.drawLine (lineG);
      }
      xLast = x;
      yLast = y;
    }
  }
  
  painter.setPen (QPen (QBrush (Qt::black), 1.0));
  painter.setFont (QFont ("Arial", 20));
  
  // Title
  painter.drawText (xOffset + L / 2 - 60,
                    invert (yOffset + L - MARGIN_MAIN_LABELS),
                    QString ("Sample Plot"));

  // X legend
  painter.drawText (xOffset + L / 2 - 2,
                    invert (yOffset + MARGIN_MAIN_LABELS),
                    QString ("Time"));

  // Y legend
  int R = L / 2;
  painter.rotate (-90); // Makes text vertical along +x axis
  painter.translate (-R, -R + MARGIN_MAIN_LABELS);
  painter.drawText (xOffset + 0, invert (R), QString ("Y Axis"));

  QTransform trans;
  QTransform rotateTransform = trans.rotate (-1.0 * angleDegrees);

  // Rotate the pixmap, convert to QImage and then remove alpha layer
  QPixmap rotated;
  rotated = pixmap.transformed (rotateTransform);
  
  QImage imageCornersAreBlack = rotated.toImage ();

  // Need to remove black corners
  return imageCornersAreBlack.copy (QRect (QPoint (xOffset + (L - LOUT) / 2,
                                                   invert (yOffset + (L + LOUT) / 2)),
                                           QPoint (xOffset + (L + LOUT) / 2,
                                                   invert (yOffset + (L - LOUT) / 2))));
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

#ifdef SAVE_RESULTS
void TestAxesScan::saveResultsFile (const QString &filename,
#else
void TestAxesScan::saveResultsFile (const QString & /* filename */,
#endif
                                    const QImage &image,
                                    double kX,
                                    double kY) const
{
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

#ifdef SAVE_RESULTS
  imageFixed.save (filename);
#endif
}

void TestAxesScan::testOffset (int xOffsetExpected,
                               int yOffsetExpected) const
{
  QImage image = generateSample (ZERO_ANGLE, xOffsetExpected, yOffsetExpected);
  QImage imageReference = generateSample (ZERO_ANGLE, ZERO_OFFSET, ZERO_OFFSET);
  AxesScan axesScan (image);

  int xOffsetGot, yOffsetGot;
  axesScan.offsets (imageReference, xOffsetGot, yOffsetGot);

  QVERIFY ((xOffsetExpected == xOffsetGot && yOffsetExpected == yOffsetGot));
}

void TestAxesScan::testOffset0_0 ()
{
  testOffset (0, 0);
}

void TestAxesScan::testShear (double angleDegrees) const
{
  QImage image = generateSample (angleDegrees, ZERO_OFFSET, ZERO_OFFSET);

  AxesScan axesScan (image);

  double kXGot = axesScan.shearX ();
  double kYGot = axesScan.shearY ();

  QString filename = QString ("shear_%1.png").arg (angleDegrees);
  saveResultsFile (filename,
                   image,
                   kXGot,
                   kYGot);

  // Pattern experimentally determined from data is that best kx=sin(angleDegrees) and ky=sin(angleDegrees)
  double kXWanted = qSin (qDegreesToRadians (angleDegrees));
  double kYWanted = -1.0 * kXWanted;

  const double EPSILON = 0.01;

  QVERIFY ((qAbs (kXGot - kXWanted) < EPSILON && qAbs (kYGot - kYWanted) < EPSILON));
}

void TestAxesScan::testShearMinus1 ()
{
  testShear (-1);
}

void TestAxesScan::testShearMinus2 ()
{
  testShear (-2);
}

void TestAxesScan::testShearMinus3 ()
{
  testShear (-3);
}

void TestAxesScan::testShearMinus4 ()
{
  testShear (-4);
}

void TestAxesScan::testShearPlus1 ()
{
  testShear (1);
}

void TestAxesScan::testShearPlus2 ()
{
  testShear (2);
}

void TestAxesScan::testShearPlus3 ()
{
  testShear (3);
}

void TestAxesScan::testShearPlus4 ()
{
  testShear (4);
}

void TestAxesScan::testShearZero ()
{
  testShear (0);
}
