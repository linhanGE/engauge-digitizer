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

  void testShear ();

private:
  QImage generateSample (int angleDegrees) const;
  int invert (int y) const;
  
};

#endif // TEST_AXES_SCAN_H
