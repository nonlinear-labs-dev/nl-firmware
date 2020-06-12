#include <proxies/hwui/panel-unit/boled/file/FileInfoContent.h>
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <regex>
#include <chrono>
#include <utility>
#include <tools/FileSystem.h>
#include <proxies/hwui/panel-unit/boled/info/MultiLineInfoContent.h>
#include <proxies/hwui/panel-unit/boled/info/InfoField.h>

FileInfoContent::FileInfoContent(std::filesystem::directory_entry file)
    : m_file(std::move(file))
{
  addInfoField("filename", "Filename:");
  addInfoField("path", "Path:", new MultiLineInfoContent());
  addInfoField("date", "Mod. Date:");
  addInfoField("size", "File Size:");

  updateContent();
}

std::string writeTimeToString(std::filesystem::file_time_type ftime)
{
  using namespace std::chrono;
  auto sctp = time_point_cast<system_clock::duration>(ftime - std::filesystem::file_time_type::clock::now()
                                                      + system_clock::now());
  std::time_t cftime = system_clock::to_time_t(sctp);
  std::string stringWithTrailingNewLine(std::asctime(std::localtime(&cftime)));
  return stringWithTrailingNewLine.substr(0, stringWithTrailingNewLine.size() - 1);
}

void FileInfoContent::fillContents()
{
  const auto filePath = std::regex_replace(m_file.path().parent_path().string(), std::regex("/mnt/usb-stick"), "");

  infoFields["filename"]->setInfo(m_file.path().filename().string());
  infoFields["path"]->setInfo(filePath.empty() ? "/" : to_string(filePath));
  infoFields["date"]->setInfo(writeTimeToString(std::filesystem::last_write_time(m_file.path())));
  infoFields["size"]->setInfo(FileSystem::bytesToHumanreadable(std::filesystem::file_size(m_file.path())));
}
