#pragma once

#include "playground.h"
#include <string>

class Uuid
{
 public:
  Uuid();
  Uuid(const Uuid &other);
  Uuid(Uuid &&other);

  Uuid(const std::string &id);
  Uuid(const Glib::ustring &id);
  Uuid(const char *id);

  virtual ~Uuid();

  void generate();
  Uuid &operator=(const Uuid &other);
  bool operator==(const Uuid &other) const;
  bool operator!=(const Uuid &other) const;
  bool operator<(const Uuid &other) const;

  const std::string &raw() const;
  std::string &raw();
  bool empty() const;

  static Uuid none();
  static Uuid init();
  static Uuid converted();

 private:
  std::string id;
};
