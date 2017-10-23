/******************************************************************************************************
 * (C) 2017 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#ifndef AXES_RESULT_H
#define AXES_RESULT_H

/// Storage of offset, period, and gridline number
class AxesResult
{
 public:
  /// Default constructor
  AxesResult ();

  /// Nondefault constructor
  AxesResult (int offset,
              int step,
              int count);

  /// Assignment constructor
  AxesResult &operator= (const AxesResult &axesResult);
  
  /// Copy constructor
  AxesResult (const AxesResult &axesResult);
  
  /// Get count value
  int count () const;

  /// Get offset value
  int offset () const;

  /// Get step value
  int step () const;
  
 private:

  int m_offset;
  int m_step;
  int m_count;
};

#endif // AXES_RESULT_H
