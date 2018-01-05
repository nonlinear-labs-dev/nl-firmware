#pragma once

#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/Scrollable.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <experimental/filesystem>


class FileInfoContent : public ControlWithChildren, public Scrollable
{
  private:
    using super = ControlWithChildren;
  public:
    FileInfoContent(std::experimental::filesystem::directory_entry file);
    ~FileInfoContent() = default;

    void setPosition(const Rect &rect) override;
    const Rect &getPosition() const override;
    void setDirty() override;

  protected:
    virtual Rect getScrollableAreaRect() const;
  private:
    void initializeFileInfosFromFile();
    std::experimental::filesystem::directory_entry m_file;
};
