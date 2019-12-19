#pragma once

#include "playground.h"
#include <proxies/hwui/Font.h>
#include <list>
#include <memory>

class TextSplitter
{
 private:
  std::list<Glib::ustring> words;
  std::list<Glib::ustring> lines;

 public:
  TextSplitter(std::shared_ptr<Font> font, double width, const Glib::ustring &text);

  const std::list<Glib::ustring> &getLines();
  Glib::ustring eatWords(std::shared_ptr<Font> font, double width, std::list<Glib::ustring> &words);
};
