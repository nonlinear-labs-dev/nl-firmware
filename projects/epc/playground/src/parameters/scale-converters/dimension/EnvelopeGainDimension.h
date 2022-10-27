#pragma once

#include "Dimension.h"

class EnvelopeGainDimension : public Dimension
{
public:
 EnvelopeGainDimension();
 virtual ~EnvelopeGainDimension();

 static const EnvelopeGainDimension &get();

 [[nodiscard]] Glib::ustring getStingizerJS() const override;
 [[nodiscard]] Glib::ustring stringize(const tDisplayValue &displayValue) const override;
};
