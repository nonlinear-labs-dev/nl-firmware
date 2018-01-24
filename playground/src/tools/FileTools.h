#pragma once

#include <vector>
#include <experimental/filesystem>
#include "SpawnCommandLine.h"
#include <device-settings/DebugLevel.h>
#include <fstream>

namespace FileTools
{
  namespace fs = std::experimental::filesystem;

  typedef std::vector<fs::directory_entry> FileList;

  static void rename(Glib::ustring oldFile, Glib::ustring newFile)
  {
    SpawnCommandLine cmd(Glib::ustring::format("mv ", oldFile, " ", newFile));
  }

  static Glib::ustring findSuitableFileName(Glib::ustring desiredName, Glib::ustring directoryPath, int depth)
  {
    auto fileName = desiredName;
    if (depth != 0)
      fileName.append(std::to_string(depth));

    for (auto& p : fs::directory_iterator(directoryPath.c_str()))
    {
      if (p.path().filename().stem().string() == fileName)
      {
        depth++;
        return findSuitableFileName(desiredName, directoryPath, depth);
      }
    }
    return fileName;
  }

  static FileList getListOfFiles(Glib::ustring dir, std::function<bool(fs::directory_entry)> filter)
  {
    FileList list;
    const std::string directory(dir);
    auto it = fs::recursive_directory_iterator(directory);

    while (it != fs::recursive_directory_iterator())
    {
	try 
	{
		Glib::ustring name(it->path().string());
		DebugLevel::warning(__FILE__, name);
		if (fs::is_directory(it->path()) && name.find("/.") != Glib::ustring::npos) 
		{
			DebugLevel::warning(__FILE__, name);
			it.disable_recursion_pending();
		} else if (!fs::is_directory(it->path()) && !filter(*it)) 
		{
			list.emplace_back(*it);
		}
	} catch(Glib::Error err) 
	{
		DebugLevel::error(err.what(), err.code());
	} catch (...) 
	{
		DebugLevel::error("non Glib::Error while traversing Thumbdrive!");
	}
      ++it;
    }

    return list;
  }

  static FileList getListOfFilesThatMatchFilter(Glib::ustring path, std::function<bool(fs::directory_entry)> filter)
  {
    return getListOfFiles(path, filter);
  }

  static bool doesFileExists(Glib::ustring absolutePath)
  {
    const auto filePath = fs::path(absolutePath.c_str());
    return fs::exists(filePath);
  }

  static bool writeToFile(Glib::ustring path, const char* content)
  {
    if (fs::exists(fs::path(path.c_str())))
    {
      std::ofstream outStream(path, std::ios::binary);
      outStream << content;
      outStream.close();
      return not outStream.fail();
    }
    return false;
  }

  static Glib::ustring readFromFile(Glib::ustring path)
  {
    if (fs::exists(fs::path(path.c_str())))
    {
      Glib::ustring ret;
      std::ifstream inStream(path, std::ios::binary);
      inStream >> ret;
      inStream.close();
      return ret;
    }
    return "";
  }

  static Glib::ustring bytesToHumanreadable(size_t numBytes)
  {
    gchar* stringPtr = g_format_size(numBytes);
    Glib::ustring ret(stringPtr);
    g_free(stringPtr);
    return ret;
  }
};

