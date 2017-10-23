/******************************************************************************************************
 * (C) 2017 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#ifndef AXES_SCAN_H
#define AXES_SCAN_H

#include <QImage>
#include <QRgb>
#include <QVector>

// One raster line scan
typedef QVector<QRgb> Raster;

// One set of horizontal or vertical raster lines
typedef QVector<Raster> Rasters;

/// Given an image, this class identifies the most probably axes.
///
/// Strategy:
/// 1) Make a subset of the horizontal lines. Not all lines are considered since only a few are
///    needed and any more just slows the algorithm down
/// 2) For each horizontal line take the fft to get the spectrum. Identify the frequency peak
///    for period ranging from 1 line to N~50 grid lines. Higher frequencies are ignored since
///    they probably correspond to noise
/// 3) Back in untransformed domain, do correlation to identify the P peaks, offset of the first
///    peak and number of peaks
/// 4) Correlate the results of all of the horizontal lines. This step is needed since Engauge
///    supports digitizing of affine-transformed graphs that are not exactly aligned with the
///    image boundaries
/// 5) Repeat previous steps but for vertical lines
class AxesScan {
 public:
  /// Single constructor
  AxesScan (const QImage &image);

  /// Try to create points using the axes matching algorithm
  bool matchAxes (double xMin,
                  double xMax,
                  double yMin,
                  double yMax) const;

 private:
  AxesScan ();

  int columnFromIndex (int index) const;
  int indexFromColumn (int col) const;
  int indexFromRow (int Row) const;
  int rowFromIndex (int index) const;
  void scanHorizontal ();
  void scanVertical ();

  // Local copy of image with direct pixel access
  const QImage m_image;

  Rasters m_rastersHorizontal;
  Rasters m_rastersVertical;
};

#endif // AXES_SCAN_H
