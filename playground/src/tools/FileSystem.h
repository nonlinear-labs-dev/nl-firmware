#pragma once

#include "playground.h"
#include <giomm.h>

class FileSystem
{
 public:
  static void deleteFolder(Glib::RefPtr<Gio::File> pmFolder);
  static bool isNameAUUID(const Glib::ustring &name);
};
