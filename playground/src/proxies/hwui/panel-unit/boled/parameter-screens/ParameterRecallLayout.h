
#include "ParameterLayout.h"

class ParameterRecallLayout: public DFBLayout {
public:
    ParameterRecallLayout(ParameterLayout2 *parent);
    bool onButton (int i, bool down, ButtonModifiers modifiers) override;
    bool onRotary (int inc, ButtonModifiers modifiers) override;
protected:
    ParameterLayout2* m_parentLayout;
};
