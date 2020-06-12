#pragma once

#include "playground.h"

#include <filesystem>
#include <vector>

class FileSystem
{
 public:
  using FileList = std::vector<std::filesystem::directory_entry>;
  static void deleteFolder(Glib::RefPtr<Gio::File> pmFolder);
  static bool isNameAUUID(const Glib::ustring& name);
  static void rename(const Glib::ustring& oldFile, const Glib::ustring& newFile);
  static Glib::ustring findSuitableFileName(const Glib::ustring& desiredName, const Glib::ustring& directoryPath,
                                            int depth);
  static bool doesFileExist(const Glib::ustring& absolutePath);
  static bool writeToFile(const Glib::ustring& path, const char* content);
  static Glib::ustring readFromFile(const Glib::ustring& path);
  static Glib::ustring bytesToHumanreadable(size_t numBytes);
  static Glib::ustring getFullPath(const Glib::RefPtr<Gio::File>& file);

  static void syncAll();
};
