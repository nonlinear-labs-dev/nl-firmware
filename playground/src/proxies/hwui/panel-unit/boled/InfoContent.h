#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/Scrollable.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <cassert>


class InfoContent : public Scrollable, public ControlWithChildren
{
  private:
    typedef ControlWithChildren super;
  public:
    struct InfoField
    {
        InfoField(Label* l, Control* c) :
            m_label(l), m_content(c)
        {
        }
        ;
        Label* m_label;
        Control* m_content;

        void setInfo(Glib::ustring text)
        {
          if (auto label = dynamic_cast<Label*>(m_content))
          {
            label->setText(text);
          }
          else if (auto multiLineLabel = dynamic_cast<MultiLineLabel*>(m_content))
          {
            multiLineLabel->setText(text, FrameBuffer::Colors::C103);
          }
          else
          {
            throw new invalid_argument(__LINE__ + __FILE__);
          }
        }
        void setInfo(Glib::ustring text, FrameBuffer::Colors c)
        {
          if (auto label = dynamic_cast<Label*>(m_content))
          {
            label->setText(text, c);
          }
          else if (auto multiLineLabel = dynamic_cast<MultiLineLabel*>(m_content))
          {
            multiLineLabel->setText(text, c);
          }
          else
          {
            assert(false);
          }
        }
    };

    InfoContent();
    virtual ~InfoContent();

    virtual void setPosition(const Rect &rect) override;
    virtual const Rect &getPosition() const override;
    virtual void setDirty() override;

    InfoField* addInfoField(std::string lineIdentifier, Label* label, Control* field);
  protected:
    std::map<std::string, InfoField*> infoFields;
};
