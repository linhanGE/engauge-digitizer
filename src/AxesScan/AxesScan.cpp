/******************************************************************************************************
 * (C) 2017 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "AxesScan.h"
#include "fftw3.h"
#include <iostream>
#include "Logger.h"
#include <QFile>
#include <QPainter>

using namespace std;

const int GRAY_THRESHOLD = 127;

AxesScan::AxesScan (const QImage &image) :
  m_image (image.convertToFormat (QImage::Format_Mono))
{
  LOG4CPP_DEBUG_S ((*mainCat)) << "AxesScan::AxesScan";
}

double AxesScan::shearX () const
{
  bool isFirst = true;
  double kxMax = 0; // Shear value that gives maximum black in a column
  double colSumMax = 0; // Number of black pixels in column
  
  for (double kx = -0.1; kx <= 0.1; kx += 0.005) {

    for (int col = 0; col < m_image.width (); col++) {
      double colSum = 0;
      for (int row = 0; row < m_image.height (); row++) {
        int colSheared = col + kx * row;
        if (0 <= colSheared && colSheared < m_image.width ()) {
          colSum += (qGray (m_image.pixel (colSheared, row)) < GRAY_THRESHOLD ? 1 : 0);
        }
      }

      if (isFirst || colSum > colSumMax) {
        isFirst = false;
        kxMax = kx;
        colSumMax = colSum;
      }
    }
  }

  return kxMax;
}

double AxesScan::shearY () const
{
  bool isFirst = true;    
  double kyMax = 0; // Shear value that gives maximum black in a row
  double rowSumMax = 0; // Number of black pixels in row
  
  for (double ky = -0.1; ky <= 0.1; ky += 0.005) {

    for (int row = 0; row < m_image.height (); row++) {
      double rowSum = 0;
      for (int col = 0; col < m_image.width (); col++) {
        int rowSheared = row + ky * col;
        if (0 <= rowSheared && rowSheared < m_image.height ()) {
          rowSum += (qGray (m_image.pixel (col, rowSheared)) < GRAY_THRESHOLD ? 1 : 0);          
        }
      }

      if (isFirst || rowSum > rowSumMax) {
        isFirst = false;
        kyMax = ky;
        rowSumMax = rowSum;
      }
    }
  }

  return kyMax;
}
