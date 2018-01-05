#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <tools/FileTools.h>
#include <proxies/hwui/controls/Label.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class FileListControl : public ControlWithChildren
{
  private:
    Label* upperFile;
    Label* middleFile;
    Label* bottomFile;
    Label* bottomFile2;
    FileTools::FileList fileList;
    int selectedItem;

    std::string getFileNameAtIndex(int index);
  public:
    FileListControl(FileTools::FileList&& files);
    virtual ~FileListControl();
    void setSelection(int index);
    void changeSelection(int change);
    fs::directory_entry getSelection() const;
    int getSelectedIndex() const;
    bool redraw(FrameBuffer &fb) override;
};
