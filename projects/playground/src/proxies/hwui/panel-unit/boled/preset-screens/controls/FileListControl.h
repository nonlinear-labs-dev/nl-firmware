#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <tools/FileSystem.h>
#include <filesystem>

namespace fs = std::filesystem;

class Label;

class FileListControl : public ControlWithChildren
{
 private:
  Label* upperFile;
  Label* middleFile;
  Label* bottomFile;
  std::unique_ptr<FileSystem::FileList> fileList;
  int selectedItem;

  std::string getFileNameAtIndex(int index);

 public:
  FileListControl(const Rect& pos);
  void setFileList(FileSystem::FileList list);
  virtual ~FileListControl();
  void setSelection(int index);
  void changeSelection(int change);
  fs::directory_entry getSelection() const;
  int getSelectedIndex() const;
};
