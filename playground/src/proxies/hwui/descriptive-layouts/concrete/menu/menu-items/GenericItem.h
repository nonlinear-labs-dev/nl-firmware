#pragma once

#include <Application.h>
#include "BasicItem.h"

class GenericItem : public BasicItem
{
 public:
  template <class T, class tCB>
  GenericItem(const T& caption, const Rect& r, tCB cb)
      : BasicItem(caption, r)
      , m_cb{ cb }
  {
  }

  void doAction() override;

 protected:
  std::function<void(void)> m_cb;
};