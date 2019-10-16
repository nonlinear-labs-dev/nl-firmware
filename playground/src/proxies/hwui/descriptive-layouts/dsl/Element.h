#pragma once

#include <proxies/hwui/descriptive-layouts/TemplateEnums.h>
#include <proxies/hwui/HWUIEnums.h>
#include <string>
#include <vector>

namespace DescriptiveLayouts
{

  struct Element
  {
   public:
    using Strings = std::vector<std::string>;
    static constexpr int maxPos = std::numeric_limits<int16_t>::max();

    Element(Element *parent = nullptr, const std::string &n = "root");

    const Element &findChild(const Strings &path) const;
    const Element &findReference(const Strings &path) const;
    const Element &findRelativeElement() const;

    Element *parent = nullptr;

    std::string name;
    std::string refersTo;
    Rect position{ 0, 0, maxPos, maxPos };
    EventProviders eventProvider = EventProviders::None;

    Strings fires;
    Strings reactions;
    Strings inits;
    Strings conditions;

    std::vector<Element> children;

    using Color = StyleValues::Color;
    using BackgroundColor = StyleValues::Color;
    using SuffixColor = StyleValues::Color;
    using BorderColor = StyleValues::Color;
    using HighlightBackgroundColor = StyleValues::Color;
    using HighlightColor = StyleValues::Color;
    using FontSize = int;

    using Styles
        = std::tuple<Color, BackgroundColor, SuffixColor, BorderColor, HighlightBackgroundColor, HighlightColor,
                     StyleValues::Alignment, StyleValues::BorderStyle, StyleValues::Font, FontSize>;

    Styles style = undefinedStyles;

    static constexpr Styles undefinedStyles{ Color::Undefined,
                                             Color::Undefined,
                                             Color::Undefined,
                                             Color::Undefined,
                                             Color::Undefined,
                                             Color::Undefined,
                                             StyleValues::Alignment::Undefined,
                                             StyleValues::BorderStyle::Undefined,
                                             StyleValues::Font::Undefined,
                                             FontSize(-1) };
  };
}
