/******************************************************************************************************
 * (C) 2017 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#ifndef CORRELATION_2D_H
#define CORRELATION_2D_H

#include "fftw3.h"

/// Fast cross correlation between two functions. We do not use complex.h along with fftw3.h since then the
/// complex numbers will be native, which would then require platform-dependent code
class Correlation2d
{
public:
  /// Single constructor. Slow memory allocations are done once and then reused repeatedly.
  Correlation2d(int N);
  ~Correlation2d();

  /// Return the shift in function1 that best aligns that function with function2. The functions
  /// are normalized internally. The correlations vector, as a function of shift, is returned for logging
  void correlateWithShift (int N,
                           const double function1 [],
                           const double function2 [],
                           int &binStartMaxX,
                           int &binStartMaxY,                           
                           double &corrMax,
                           double correlations []) const;

  /// Return the correlation of the two functions, without any shift. The functions
  /// are normalized internally.
  void correlateWithoutShift (int N,
                              const double function1 [],
                              const double function2 [],
                              double &corrMax) const;

  /// Convenience function that collapses two dimensional indexes into one dimension
  static int fold2dIndexes (int N,
                            int i,
                            int j);

private:
  Correlation2d();
  
  int m_N;

  fftw_complex *m_signalA;
  fftw_complex *m_signalB;
  fftw_complex *m_outShifted;
  fftw_complex *m_outA;
  fftw_complex *m_outB;
  fftw_complex *m_out;

  fftw_plan m_planA;
  fftw_plan m_planB;
  fftw_plan m_planX;
};

#endif // CORRELATION_2D_H
