#pragma once

#include "UsageMode.h"
#include <tuple>
#include <Application.h>
#include <proxies/hwui/TwoStateLED.h>

class ButtonRepeat;

class TextEditUsageMode : public UsageMode
{
 private:
  typedef UsageMode super;

 public:
  TextEditUsageMode(const Glib::ustring &initalText);
  ~TextEditUsageMode();

  virtual void setup();
  virtual bool onButtonPressed(gint32 buttonID, ButtonModifiers modifiers, bool state);

  connection onTextChanged(slot<void, const ustring &> cb);
  ustring getKeyLabel(int buttonID) const;

  gunichar getCharAtRelativeToCursorPosition(int pos) const;
  bool isInserting() const;
  void moveCursor(int step);
  ustring getText() const;

  enum class Layout
  {
    Normal,
    Shift,
    Symbol,
    SymbolShift
  };

  Layout getLayout() const;

  static void registerTests();

 private:
  void replaceCharWith(gunichar newChar);
  void clampPosition();
  void insertChar(gunichar c);
  void onCharPressed(gunichar c);
  bool handleSpecialChar(gunichar c);
  ustring stringizeSpecialChar(gunichar c) const;
  gunichar getCharForButton(int buttonID) const;
  void toggleCapsLock();
  void toggleSymbol();
  void installButtonRepeat(std::function<void()> cb);

  void updateLeds();
  void chooseLayout();
  void handleShiftButton(bool state);

  ustring m_text;
  int m_position;
  Signal<void, const ustring &> m_sigTextChanged;

  Layout m_layout;

  bool m_insert = true;

  bool m_capsLock = false;
  bool m_symbolState = false;
  bool m_shiftState = false;

  typedef std::tuple<gunichar, gunichar, gunichar, gunichar> tButtonAssignment;
  std::map<int, tButtonAssignment> m_buttonMap;
  std::unique_ptr<ButtonRepeat> m_buttonRepeat;
};
