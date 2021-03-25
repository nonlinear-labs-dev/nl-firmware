#pragma once
#include <utility>

class StringAndSuffix
{
 public:
  StringAndSuffix(const Glib::ustring &first, const Glib::ustring &second);
  explicit StringAndSuffix(const Glib::ustring &first, size_t suffix = 0);
  explicit StringAndSuffix(const char *first, size_t suffix = 0);
  explicit StringAndSuffix(char c, size_t suffix = 0);

  bool operator!=(const StringAndSuffix &other) const;

  [[nodiscard]] std::pair<Glib::ustring, Glib::ustring> getSplits() const;

  Glib::ustring text;
  size_t suffix = 0;
};
