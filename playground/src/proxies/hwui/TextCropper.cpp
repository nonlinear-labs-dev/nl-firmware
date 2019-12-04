#include <proxies/hwui/controls/Control.h>
#include <utility>
#include <testing/TestDriver.h>
#include <proxies/hwui/controls/Label.h>
#include "TextCropper.h"
#include "Oleds.h"
#include "CompileTimeOptions.h"

static TestDriver<TextCropper> tests;

void TextCropper::registerTests()
{
  g_test_add_func("/TextCropper/shortenStringIfNeccessary", []() {
    std::string installDir = getInstallPrefix();
    auto pathToFont = installDir + "/nonlinear/playground/resources/Emphase_9_Regular.ttf";
    auto font = std::make_shared<Font>(pathToFont, 9);

    auto testShortenStringIfNeccessary = [font](std::string in, int len, std::string expected) {
      auto shortened = TextCropper::shortenStringIfNeccessary(font, in, len);
      g_assert_cmpstr(shortened.c_str(), ==, expected.c_str());
    };

    testShortenStringIfNeccessary("", 5, "");
    testShortenStringIfNeccessary("", 0, "");
    testShortenStringIfNeccessary("", 100000, "");

    testShortenStringIfNeccessary("Fo", 5, "..");
    testShortenStringIfNeccessary("Fo", 20, "Fo");
    testShortenStringIfNeccessary("Fooo", 20, "Fooo");
    testShortenStringIfNeccessary("Foooo", 20, "F..o");
    testShortenStringIfNeccessary("Foooo 2", 20, "F..2");
    testShortenStringIfNeccessary("Foooo2", 20, "F..2");
    testShortenStringIfNeccessary("ANANAS SANANA", 20, "A..A");
    testShortenStringIfNeccessary("ANANAS SANANA", 40, "AN..NA");
    testShortenStringIfNeccessary("ANANAS SANANA 4", 40, "ANAN..4");
    testShortenStringIfNeccessary("ANANAS SANANA 4 5", 40, "ANAN..5");
    testShortenStringIfNeccessary("ANANASSANANA 2", 60, "ANANASS..2");
    testShortenStringIfNeccessary("HALLOHALLOHALLOHALLOHALLO 78", 128, "HALLOHALLOHALLOHAL..78");
    testShortenStringIfNeccessary("HALLO HALLO HALLO HALLO HALLO 78", 128, "HALLO HALLO HALLO H..78");
  });
};

Glib::ustring TextCropper::shortenStringIfNeccessary(std::shared_ptr<Font> font, const Glib::ustring &text,
                                                     double maxWidth)
{
  if(font->getStringWidth(text) > maxWidth)
    return preserveEndShorten(font, text, maxWidth);
  return text;
}

Glib::ustring TextCropper::preserveEndShorten(const std::shared_ptr<Font> &font, const Glib::ustring &text,
                                              double maxWidth)
{
  const auto textLen = text.length();
  const auto min = 0;
  auto edgeCaseString = edgeCases(min, textLen, text);
  if(!edgeCaseString.empty())
  {
    return edgeCaseString;
  }

  if(font->getStringWidth("..") > maxWidth)
    return "..";

  auto spaceSplitString = spaceBasedCropping(text, font, maxWidth * 0.95);
  if(!spaceSplitString.empty())
  {
    return spaceSplitString;
  }

  return rigidSplit(font, text, maxWidth);
}

Glib::ustring TextCropper::rigidSplit(const std::shared_ptr<Font> &font, const Glib::ustring &text,
                                      const double maxWidth)
{

  Glib::ustring front;
  Glib::ustring back;
  const Glib::ustring padding = "..";
  int i = 0;
  while(font->getStringWidth(front + padding) < maxWidth / 2)
  {
    front.push_back(text.at(static_cast<Glib::ustring::size_type>(i++)));
  }
  i = static_cast<int>(text.length() - 1);
  while(font->getStringWidth(back + padding) < maxWidth / 2)
  {
    back.insert(back.begin(), text.at(static_cast<Glib::ustring::size_type>(i--)));
  }
  return front + ".." + back;
}

Glib::ustring TextCropper::edgeCases(const int min, const Glib::ustring::size_type len, const Glib::ustring &str)
{
  if(len == 0)
    return "";
  if(len < min)
    return str;
  if((min + 1) == len)
    return str.substr(0, static_cast<Glib::ustring::size_type>(min + 1)) + "..";

  return "";
}

Glib::ustring TextCropper::spaceBasedCropping(const Glib::ustring &text, const std::shared_ptr<Font> &font,
                                              double maxWidth)
{
  const auto &c = text;
  if(maxWidth == 0)
    return "";
  auto roughTwoThirds = (text.length() / 3) * 2;
  if(auto pos = text.find_last_of(' '))
  {
    if(pos < text.length() && pos >= roughTwoThirds)
    {
      auto partAfterSpace = c.substr(pos + 1, c.length() - 1);
      Glib::ustring front = "";
      for(int i = 0; (font->getStringWidth(to_string(front + partAfterSpace + "...")) < maxWidth); i++)
      {
        front += text[i];
      }
      return front + ".." + partAfterSpace;
    }
  }
  return "";
}
