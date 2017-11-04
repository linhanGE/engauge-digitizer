#ifndef TEST_AXES_SCAN_H
#define TEST_AXES_SCAN_H

#include <QImage>
#include <QObject>

/// Unit tests of fast correlation algorithm
class TestAxesScan : public QObject
{
  Q_OBJECT
public:
  /// Single constructor.
  explicit TestAxesScan(QObject *parent = 0);

signals:

private slots:
  void cleanupTestCase ();
  void initTestCase ();

  void testOffset ();
  void testShearMinus1 ();
  void testShearMinus2 ();
  void testShearMinus3 ();
  void testShearMinus4 ();
  void testShearPlus1 ();
  void testShearPlus2 ();
  void testShearPlus3 ();
  void testShearPlus4 ();
  void testShearZero ();

private:
  QImage generateSample (double angleDegrees,
                         int xOffset,
                         int yOffset) const;
  int invert (int y) const;
  void saveResultsFile (const QString &filename,
                        const QImage &image,
                        double kX,
                        double kY) const;
  void testShear (double angleDegrees) const;
};

#endif // TEST_AXES_SCAN_H
