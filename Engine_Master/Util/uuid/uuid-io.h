#pragma once
#include <iosfwd>

#include "uuid.h"
#include "export.h"

namespace stud
{
  // Insert lower case string representation.
  //
  LIBSTUD_UUID_SYMEXPORT std::ostream&
  operator<< (std::ostream&, const uuid&);

  // Extract string representation (lower or upper case).
  //
  LIBSTUD_UUID_SYMEXPORT std::istream&
  operator>> (std::istream&, uuid&);
}
