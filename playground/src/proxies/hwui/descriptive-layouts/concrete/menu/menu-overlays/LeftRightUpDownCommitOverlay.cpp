#include "LeftRightUpDownCommitOverlay.h"

LeftRightUpDownCommitOverlay::LeftRightUpDownCommitOverlay(const Rect& r)
    : MenuOverlay{ r }
{
}

bool LeftRightUpDownCommitOverlay::onButton(Buttons i, bool down, ButtonModifiers mod)
{
  switch(i)
  {
    case Buttons::BUTTON_ENTER:
      onCommit(down);
      break;
    case Buttons::BUTTON_INC:
      onUp(down);
      break;
    case Buttons::BUTTON_DEC:
      onDown(down);
      break;
    case Buttons::ROTARY_MINUS:
    case Buttons::BUTTON_C:
      onLeft(down);
      break;
    case Buttons::ROTARY_PLUS:
    case Buttons::BUTTON_D:
      onRight(down);
      break;
    default:
      return false;
  }
  return true;
}
