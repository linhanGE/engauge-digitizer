/******************************************************************************************************
 * (C) 2017 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "AxesResult.h"

AxesResult::AxesResult () :
  m_offset (0),
  m_step (0),
  m_count (0)
{
}

AxesResult::AxesResult (int offset,
                        int step,
                        int count) :
  m_offset (offset),
  m_step (step),
  m_count (count)
{
}

AxesResult &AxesResult::operator= (const AxesResult &other)
{
  m_offset = other.offset ();
  m_step = other.step ();
  m_count = other.count ();

  return *this;
}

AxesResult::AxesResult (const AxesResult &other) :
  m_offset (other.offset ()),
  m_step (other.step ()),
  m_count (other.count ())
{
}

int AxesResult::count () const
{
  return m_count;
}

int AxesResult::offset () const
{
  return m_offset;
}

int AxesResult::step () const
{
  return m_step;
}
