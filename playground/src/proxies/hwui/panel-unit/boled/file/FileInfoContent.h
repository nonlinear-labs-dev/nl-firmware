#pragma once

#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/Scrollable.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <experimental/filesystem>
#include <proxies/hwui/panel-unit/boled/InfoContent.h>

class FileInfoContent : public InfoContent
{
 public:
  FileInfoContent(std::experimental::filesystem::directory_entry file);

  void fillContents() override;

 private:
  std::experimental::filesystem::directory_entry m_file;
};
