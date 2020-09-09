#pragma once

namespace controladapters
{
  void inc(ButtonMenu *b)
  {
    b->toggle();
  }

  void dec(ButtonMenu *b)
  {
    b->antiToggle();
  }

  template <typename TControl> void inc(TControl *b)
  {
    b->turn();
  }

  template <typename TControl> void dec(TControl *b)
  {
    b->antiTurn();
  }

  template <typename TControl> inline void incDec(TControl *c, int i)
  {
    while(i > 0)
    {
      inc(c);
      i--;
    }

    while(i < 0)
    {
      dec(c);
      i++;
    }
  }
}
