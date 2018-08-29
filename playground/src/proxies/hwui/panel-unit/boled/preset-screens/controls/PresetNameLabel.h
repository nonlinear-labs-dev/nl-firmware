#pragma once

#include "PresetLabel.h"

class PresetBank;
class Preset;

class PresetNameLabel : public PresetLabel
{
  private:
    typedef PresetLabel super;

  public:
  explicit PresetNameLabel (const Rect &pos);

  ~PresetNameLabel () = default;

    void drawBackground (FrameBuffer &fb) override;
    void update(const Glib::ustring &presetName, bool selected, bool loaded);

  protected:
    Font::Justification getJustification () const override;
    int getXOffset () const override;
    int getRightMargin() const override;
    Glib::ustring shortenStringIfNeccessary (shared_ptr<Font> font, const Glib::ustring &text) const override;

  Glib::ustring preserveEndShorten(shared_ptr<Font> font, const ustring &text) const;

  Glib::ustring edgeCases(int min, const ustring::size_type len, const ustring &ustring) const;

  Glib::ustring
  spaceBasedCropping(const ustring &text, shared_ptr<Font> font, double maxWidth) const;

  Glib::ustring rigidSplit(const shared_ptr<Font> &font, const ustring &text, int maxWidth) const;
};

