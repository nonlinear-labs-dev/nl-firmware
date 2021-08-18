#pragma once

#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/Oleds.h>
#include <tools/ExceptionTools.h>

enum class FontDecoration
{
  Regular,
  Bold
};

enum class FontSize
{
  Size7 = 7, //This is very ugly, but would be very useful too :/
  Size8 = 8,
  Size9 = 9
};

class LabelStyle
{
 public:
  FontSize size = FontSize::Size8;
  FontDecoration decoration = FontDecoration::Regular;
  Font::Justification justification = Font::Justification::Center;

  bool operator!=(LabelStyle& other) const {
    return size != other.size || decoration != other.decoration || justification != other.justification;
  }
};

class LabelStyleable : public Label
{
 public:
  explicit LabelStyleable(const Rect& pos);
  LabelStyleable(const StringAndSuffix& text, const Rect& pos);
  LabelStyleable(const StringAndSuffix& text, const Rect& pos, LabelStyle style);
  LabelStyleable(const Rect& pos, LabelStyle style);

  void setLabelStyle(LabelStyle style);
  LabelStyle getCurrentStyle() const;

 public:
  std::shared_ptr<Font> getFont() const override;

 protected:
  Font::Justification getJustification() const override;

 private:
  LabelStyle m_styleSheet;
};
