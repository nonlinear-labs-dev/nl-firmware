#pragma once

#include "playground.h"
#include <proxies/hwui/HWUIEnums.h>
#include <nltools/threading/Throttler.h>
#include <map>

class OLEDProxy;
class Font;

class Fonts
{
 public:
  typedef std::shared_ptr<Font> tFont;
  static Fonts& get();
  tFont getFont(const Glib::ustring &name, int height);

 private:
  Fonts() = default;

  typedef std::pair<Glib::ustring, int> tKey;
  std::map<tKey, tFont> m_fonts;
};

class Oleds
{
 public:
  Oleds();

  virtual ~Oleds();

  void registerProxy(OLEDProxy *proxy);
  void syncRedraw(bool force = false);
  void setDirty();
  [[nodiscard]] bool isDirty() const;

  void deInit();

 private:
  Oleds(const Oleds &other);
  Oleds &operator=(const Oleds &);

  std::list<OLEDProxy *> m_proxies;
  Throttler m_throttler;
};
