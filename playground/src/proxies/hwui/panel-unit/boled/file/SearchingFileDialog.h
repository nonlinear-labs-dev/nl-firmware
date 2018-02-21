#pragma once
#include <proxies/hwui/DFBLayout.h>


class SearchingFileDialog : public DFBLayout {
public:
    SearchingFileDialog(std::string title);
    bool onButton(int i, bool down, ButtonModifiers modifiers) override;
protected:
    ///void searchForB
};
