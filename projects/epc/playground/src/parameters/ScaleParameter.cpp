#include <groups/ParameterGroup.h>
#include <groups/ScaleGroup.h>
#include <libundo/undo/StringTools.h>
#include <parameters/ScaleParameter.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ScaleParameterLayout.h>
#include <xml/Writer.h>
#include "scale-converters/dimension/NoteDimension.h"
#include "parameter_declarations.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>

ScaleParameter::ScaleParameter(ParameterGroup *group, const ParameterId& id, const ScaleConverter *scaling,
                               tControlPositionValue def, tControlPositionValue coarseDenominator,
                               tControlPositionValue fineDenominator)
    : super(group, id, scaling, scaling, def, coarseDenominator, fineDenominator)
{
}

Layout *ScaleParameter::createLayout(FocusAndMode focusAndMode) const
{
  return ScaleParameterIMPL::createLayout(focusAndMode);
}

Glib::ustring ScaleParameter::getMiniParameterEditorName() const
{
  return "Scale...";
}

void ScaleParameter::writeDocProperties(Writer &writer, tUpdateID knownRevision) const
{
  ModulateableParameterWithUnusualModUnit::writeDocProperties(writer, knownRevision);

  if(!shouldWriteDocProperties(knownRevision))
  {
    writer.writeTextElement("long-name", getLongName());
  }
}

size_t ScaleParameter::getHash() const
{
  size_t hash = ModulateableParameter::getHash();
  hash_combine(hash, getLongName());
  return hash;
}

Glib::ustring ScaleParameter::getLongName() const
{
  static const auto baseKeyParameterID = ParameterId(C15::PID::Scale_Base_Key, VoiceGroup::Global);

  const auto baseKey = getParentGroup()->getParameterByID(baseKeyParameterID);
  auto offset = 0;
  if(getID().getNumber() != C15::PID::Scale_Offset_0)
    offset = getID().getNumber() - baseKeyParameterID.getNumber();

  if(getID() != baseKeyParameterID)
  {
    const auto key = static_cast<int>(baseKey->getDisplayValue() + offset);
    const auto stringizedKey = NoteDimension::get().stringize(key % 12);
    return UNDO::StringTools::buildString(super::getLongName(), " (", stringizedKey, ")");
  }
  return super::getLongName();
}

BaseScaleParameter::BaseScaleParameter(ParameterGroup *group, const ParameterId &id, const ScaleConverter *scaling,
                                       tControlPositionValue def, tControlPositionValue coarseDenom,
                                       tControlPositionValue fineDenom)
: super(group, id, scaling, def, coarseDenom, fineDenom)
{
}

Layout *BaseScaleParameter::createLayout(FocusAndMode fam) const
{
  switch(fam.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();

    case UIMode::Edit:
      return new BaseScaleParameterEditLayout(); //TODO fix UI after design-doc is done

    case UIMode::Select:
    default:
      return new BaseScaleParameterSelectLayout();
  }

  g_return_val_if_reached(nullptr);
}

Glib::ustring BaseScaleParameter::getMiniParameterEditorName() const
{
  return "Scale...";
}

void BaseScaleParameter::writeDocProperties(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  super::writeDocProperties(writer, knownRevision);

  if(!shouldWriteDocProperties(knownRevision))
  {
    writer.writeTextElement("long-name", getLongName());
  }
}

Layout *ScaleParameterIMPL::createLayout(FocusAndMode fam)
{
  switch(fam.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();

    case UIMode::Edit:
      return new ScaleParameterEditLayout(); //TODO fix UI after design-doc is done

    case UIMode::Select:
    default:
      return new ScaleParameterSelectLayout();
  }

  g_return_val_if_reached(nullptr);
}
