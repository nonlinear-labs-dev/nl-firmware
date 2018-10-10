#pragma once

#include "bbbb.h"
#include <giomm/file.h>

class Options
{
 public:
  Options(int &argc, char **&argv);
  virtual ~Options();

  bool doTimeStamps() const;

 private:
  bool m_doTimeStamps = false;
};
