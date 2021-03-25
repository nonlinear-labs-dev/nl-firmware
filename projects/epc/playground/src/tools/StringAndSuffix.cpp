#include "StringAndSuffix.h"

StringAndSuffix::StringAndSuffix(const Glib::ustring& first, const Glib::ustring& second)
    : text(first + second)
    , suffix(second.length())
{
}

StringAndSuffix::StringAndSuffix(const Glib::ustring& first, size_t suffix)
    : text(first)
    , suffix(suffix)
{
}

StringAndSuffix::StringAndSuffix(const char* first, size_t suffix)
    : text(first)
    , suffix(suffix)
{
}

StringAndSuffix::StringAndSuffix(const char c, size_t suffix)
    : text(std::to_string(c))
    , suffix(suffix)
{
}

bool StringAndSuffix::operator!=(const StringAndSuffix& other) const
{

  return text.raw() != other.text.raw() || suffix != other.suffix;
}

std::pair<ustring, ustring> StringAndSuffix::getSplits() const
{
  return std::make_pair<Glib::ustring, Glib::ustring>(text.substr(0, text.length() - suffix),
                                                      text.substr(text.length() - suffix));
}
