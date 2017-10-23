/******************************************************************************************************
 * (C) 2017 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "AxesScan.h"
#include "fftw3.h"
#include "Logger.h"

const int NUMBER_OF_RASTERS_IN_ONE_DIRECTION = 10;
const int NROW = NUMBER_OF_RASTERS_IN_ONE_DIRECTION; // Less cumbersome name
const int NCOL = NUMBER_OF_RASTERS_IN_ONE_DIRECTION; // Less cumbersome name

AxesScan::AxesScan (const QImage &image) :
  m_image (image),
  m_rastersHorizontal (NCOL),
  m_rastersVertical (NROW)
{
  LOG4CPP_DEBUG_S ((*mainCat)) << "AxesScan::AxesScan";

  scanHorizontal ();
  scanVertical ();
}

int AxesScan::columnFromIndex (int indexCol) const
{
  // Indexes should span the width range but not include or touch left or right
  return (int) ((indexCol + 1.0) * m_image.width () / (NCOL + 1.0));
}

int AxesScan::indexFromColumn (int col) const
{
  // Expected column values are step, 2*step,... so ranges are
  // 0.5*step to 1.5*step, 1.5*step to 2.5*step,... for index=0, 1,...
  double step = m_image.width () / (NCOL + 1.0);
  return (int) (col / step - 0.5);
}

int AxesScan::indexFromRow (int row) const
{
  // Expected column values are step, 2*step,... so ranges are
  // 0.5*step, 1.5*step, 1.5*step to 2.5*step,... for index=0, 1,...
  double step = m_image.height () / (NROW + 1.0);
  return (int) (row / step - 0.5);
}

bool AxesScan::matchAxes (double xMin,
                          double xMax,
                          double yMin,
                          double yMax) const
{
  // Vertical rasters
  fftw_complex *rowSignal = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * (2 * NCOL - 1));
  fftw_complex *rowFrequency = (fftw_complex *) fftw_malloc (sizeof (fftw_complex) * (2 * NCOL - 1));
  fftw_plan rowPlan = fftw_plan_dft_1d (2 * NCOL - 1, rowSignal, rowFrequency, FFTW_FORWARD, FFTW_ESTIMATE);
  for (int indexCol = 0; indexCol < NCOL - 1; indexCol++) {
    for (int row = 0; row < NROW; row++) {
      if (row < NROW - 1) {
        rowSignal [row] [0] = 0;
      }
      rowSignal [row + NROW - 1] [0] = m_rastersHorizontal [indexCol] [row]; // Real part
      rowSignal [row + NROW - 1] [1] = 0.0; // Imaginary part
    }
  }
  fftw_destroy_plan (rowPlan);
  fftw_free (rowSignal);
  fftw_free (rowFrequency);

  return true;
}

int AxesScan::rowFromIndex (int indexRow) const
{
  // Indexes should span the height range but not include or touch top or bottom
  return (int) ((indexRow + 1.0) * m_image.height () / (NROW + 1.0));
}

void AxesScan::scanHorizontal ()
{
  // Create a small number of horizontal scans
  for (int indexRow = 0; indexRow < NROW; indexRow++) {

    int row = rowFromIndex (indexRow);

    for (int col = 0; col < m_image.width(); col++) {
      m_rastersHorizontal [col] [indexRow] = m_image.pixel (col, row);
    }
  }
}

void AxesScan::scanVertical ()
{
  // Create a small number of vertical scans
  for (int indexCol = 0; indexCol < NCOL; indexCol++) {

    int col = columnFromIndex (indexCol);

    for (int row = 0; row < m_image.width(); row++) {
      m_rastersVertical [indexCol] [row] = m_image.pixel (col, row);
    }
  }
}
