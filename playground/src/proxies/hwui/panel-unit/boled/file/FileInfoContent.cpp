#include <proxies/hwui/panel-unit/boled/file/FileInfoContent.h>
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <regex>
#include <chrono>
#include <tools/FileTools.h>

FileInfoContent::FileInfoContent(std::experimental::filesystem::directory_entry file) :
    super(Rect(0, 0, 0, 64)),
    m_file(file)
{
  initializeFileInfosFromFile();
}

std::string writeTimeToString(std::experimental::filesystem::file_time_type ftime)
{
  std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
  std::string stringWithTrailingNewLine(std::asctime(std::localtime(&cftime)));
  return stringWithTrailingNewLine.substr(0, stringWithTrailingNewLine.size() - 1);
}

void FileInfoContent::initializeFileInfosFromFile()
{
  static const int divider = 70;
  addControl(new LeftAlignedLabel("Filename:", Rect(0, 0, divider, 16)));
  addControl(new LeftAlignedLabel("Path:", Rect(0, 16, divider, 16)));
  addControl(new LeftAlignedLabel("Mod. Date:", Rect(0, 32, divider, 16)));
  addControl(new LeftAlignedLabel("File Size:", Rect(0, 48, divider, 16)));

  addControl(new LeftAlignedLabel(m_file.path().filename().string(), Rect(divider, 0, 256 - divider, 16)));
  addControl(
      new LeftAlignedLabel(std::regex_replace(m_file.path().string(), std::regex("/mnt/usb-stick"), ""),
          Rect(divider, 16, 256 - divider, 16)));
  addControl(
      new LeftAlignedLabel(writeTimeToString(std::experimental::filesystem::last_write_time(m_file.path())),
          Rect(divider, 32, 256 - divider, 16)));
  addControl(
      new LeftAlignedLabel(FileTools::bytesToHumanreadable(std::experimental::filesystem::file_size(m_file.path())),
          Rect(divider, 48, 256 - divider, 16)));
}

void FileInfoContent::setPosition(const Rect &rect)
{
  super::setPosition(rect);
}

Rect FileInfoContent::getScrollableAreaRect() const
{
  return Rect(0, 16, 256, 64 - 16);
}

const Rect &FileInfoContent::getPosition() const
{
  return super::getPosition();
}

void FileInfoContent::setDirty()
{
  super::setDirty();
}
