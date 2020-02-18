#pragma once

#include "Label.h"

class SymbolLabel : public Label
{
 private:
  typedef Label super;

 public:
  SymbolLabel(const Glib::ustring& text, const Rect& pos);
  virtual ~SymbolLabel();

 protected:
  virtual std::shared_ptr<Font> getFont() const override;
  virtual int getFontHeight() const override;

 private:
  SymbolLabel(const SymbolLabel& other);
  SymbolLabel& operator=(const SymbolLabel&);
};
