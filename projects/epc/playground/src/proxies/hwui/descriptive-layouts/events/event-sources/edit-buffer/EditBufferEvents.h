#pragma once
#include <presets/EditBuffer.h>
#include <parameter_declarations.h>
#include <device-settings/Setting.h>
#include "EditBufferEvent.h"

namespace DescriptiveLayouts
{
  class EditBufferTypeStringEvent : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class EditBufferName : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class EditBufferNameWithSuffix : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class CurrentVoiceGroupLabel : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class IsCurrentVGI : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class IsCurrentVGII : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class VGIMuted : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class VGIIMuted : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class VGIIsMuted : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class VGIIIsMuted : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class IsSingleSound : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class IsLayerSound : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class IsSplitSound : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class CurrentVoiceGroupText : public VoiceGroupSelectedEvent<DisplayString>
  {
   public:
    void onChange(VoiceGroup newSelection) override;
  };

  class SoundParamsButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;

   private:
    bool isChanged(const EditBuffer *eb);
  };

  class SoundVoicesButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;

   private:
    bool isChanged(const EditBuffer *eb);
  };

  class SoundMasterButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;

   private:
    bool isChanged(const EditBuffer *eb);
  };

  class SoundFxMixMasterButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer* eb) override;

   private:
    bool isChanged(const EditBuffer* eb);
  };

  class MonoButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;

   private:
    bool isChanged(const EditBuffer *eb);
  };

  class UnisonButtonText : public EditBufferEvent<DisplayString>
  {
   public:
    void onChange(const EditBuffer *eb) override;

   private:
    bool isChanged(const EditBuffer *eb);
  };

  class AnyLayerCrossFB : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class AnyLayerCrossFBFromII : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class AnyLayerCrossFBFromI : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class ToFXIUnder100 : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class ToFXIIUnder100 : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class LayerIFBFromII
  {
   public:
    static bool check(const EditBuffer *eb);
  };

  class LayerIIFBFromI
  {
   public:
    static bool check(const EditBuffer *eb);
  };

  class ToFXIOver0 : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
    static bool check(const EditBuffer *eb);
  };

  class ToFXIIOver0 : public EditBufferEvent<bool>
  {
   public:
    void onChange(const EditBuffer *eb) override;
    static bool check(const EditBuffer *eb);
  };

  class LayerFBState : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class LayerFXState : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class FX_I_ImageState : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class FX_II_ImageState : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class LayerToFXPath : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  class Serial_FX_Imagestate : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  class Layer_FX_TO_OUT_Imagestate : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  class Split_FX_TO_OUT_Imagestate : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  class Split_FX_TO_OUT_Imagestate_flipped : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  /*
   * m_map[EventSources::Split_Arrows_To_FX_L_TO_R_I] = std::make_unique<Split_Arrows_To_FX_L_TO_R_I>();
m_map[EventSources::Split_Arrows_To_FX_L_TO_R_II] = std::make_unique<Split_Arrows_To_FX_L_TO_R_II>();
m_map[EventSources::Split_Arrows_To_FX_R_TO_L_I] = std::make_unique<Split_Arrows_To_FX_R_TO_L_I>();
m_map[EventSources::Split_Arrows_To_FX_R_TO_L_II] = std::make_unique<Split_Arrows_To_FX_R_TO_L_II>();
   */

  class Split_Arrows_To_FX_L_TO_R_I : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  class Split_Arrows_To_FX_L_TO_R_II : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  class Split_Arrows_To_FX_R_TO_L_I : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  class Split_Arrows_To_FX_R_TO_L_II : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  class Split_Arrows_FX_To_I : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  class Split_Arrows_FX_To_II : public EditBufferEvent<std::string>
  {
   public:
    void onChange(const EditBuffer* eb) override;
  };

  class LayerFXOffset : public EditBufferEvent<std::pair<int, int>>
  {
   public:
    void onChange(const EditBuffer *eb) override;
  };

  class SplitPointBehaviourIsDefaultWithoutSync : public EditBufferEvent<bool>
  {
   public:
    SplitPointBehaviourIsDefaultWithoutSync();
    void onChange(const EditBuffer *eb) override;

   private:
    void onSettingChanged(const Setting *s);
  };
}
