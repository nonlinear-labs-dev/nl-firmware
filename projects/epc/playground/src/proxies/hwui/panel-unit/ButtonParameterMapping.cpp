#include <proxies/hwui/panel-unit/ButtonParameterMapping.h>
#include "parameter_declarations.h"

ButtonParameterMapping::ButtonParameterMapping()
{
  using namespace C15::PID;
  addMapping(Buttons::BUTTON_0, { Env_A_Att, Env_A_Att_Vel, Env_A_Att_Curve });
  addMapping(Buttons::BUTTON_1, { Env_C_Att, Env_C_Att_Vel, Env_C_Att_Curve, Env_C_Retr_H });
  addMapping(Buttons::BUTTON_2, { Env_C_Rel, Env_C_Rel_Vel });
  addMapping(Buttons::BUTTON_3, { Env_B_Att, Env_B_Att_Vel, Env_B_Att_Curve });
  addMapping(Buttons::BUTTON_4, { Env_A_Dec_1, Env_A_Dec_1_Vel });
  addMapping(Buttons::BUTTON_5, { Env_C_Dec_1, Env_C_Dec_1_Vel, Env_C_BP });
  addMapping(Buttons::BUTTON_6, { Env_C_Lvl_Vel });
  addMapping(Buttons::BUTTON_7, { Env_B_Dec_1, Env_B_Dec_1_Vel });
  addMapping(Buttons::BUTTON_8, { Env_A_BP });
  addMapping(Buttons::BUTTON_9, { Env_C_Dec_2, Env_C_Dec_2_Vel, Env_C_Sus, Env_C_Loop });
  addMapping(Buttons::BUTTON_10, { Env_C_Lvl_KT, Env_C_Time_KT });
  addMapping(Buttons::BUTTON_11, { Env_B_BP });
  addMapping(Buttons::BUTTON_12, { Env_A_Dec_2, Env_A_Dec_2_Vel });
  addMapping(Buttons::BUTTON_13, { Env_A_Lvl_Vel });
  addMapping(Buttons::BUTTON_14, { Env_B_Lvl_Vel });
  addMapping(Buttons::BUTTON_15, { Env_B_Dec_2, Env_B_Dec_2_Vel });
  addMapping(Buttons::BUTTON_16, { Env_A_Sus, Env_A_Elevate });
  addMapping(Buttons::BUTTON_17, { Env_A_Lvl_KT, Env_A_Time_KT });
  addMapping(Buttons::BUTTON_18, { Env_B_Lvl_KT, Env_B_Time_KT });
  addMapping(Buttons::BUTTON_19, { Env_B_Sus, Env_B_Elevate });
  addMapping(Buttons::BUTTON_20, { Env_A_Rel, Env_A_Rel_Vel });
  addMapping(Buttons::BUTTON_21, { Env_A_Gain });
  addMapping(Buttons::BUTTON_22, { Env_B_Gain });
  addMapping(Buttons::BUTTON_23, { Env_B_Rel, Env_B_Rel_Vel });
  addMapping(Buttons::BUTTON_24, { Osc_A_Pitch, Osc_A_Pitch_Env_C, Osc_A_Pitch_KT });
  addMapping(Buttons::BUTTON_25, { Shp_A_Drive, Shp_A_Drive_Env_A });
  addMapping(Buttons::BUTTON_26, { Shp_B_Drive, Shp_B_Drive_Env_B });
  addMapping(Buttons::BUTTON_27, { Osc_B_Pitch, Osc_B_Pitch_Env_C, Osc_B_Pitch_KT });
  addMapping(Buttons::BUTTON_28, { Osc_A_Fluct, Osc_A_Fluct_Env_C });
  addMapping(Buttons::BUTTON_29, { Shp_A_Fold });
  addMapping(Buttons::BUTTON_30, { Shp_B_Fold });
  addMapping(Buttons::BUTTON_31, { Osc_B_Fluct, Osc_B_Fluct_Env_C });
  addMapping(Buttons::BUTTON_32, { Osc_A_Phase, Osc_A_Reset });
  addMapping(Buttons::BUTTON_33, { Shp_A_Asym });
  addMapping(Buttons::BUTTON_34, { Shp_B_Asym });
  addMapping(Buttons::BUTTON_35, { Osc_B_Phase, Osc_B_Reset });
  addMapping(Buttons::BUTTON_36, { Osc_A_PM_Self, Osc_A_PM_Self_Env_A, Osc_A_PM_Self_Shp });
  addMapping(Buttons::BUTTON_37, { Shp_A_Mix });
  addMapping(Buttons::BUTTON_38, { Shp_B_Mix });
  addMapping(Buttons::BUTTON_39, { Osc_B_PM_Self, Osc_B_PM_Self_Env_B, Osc_B_PM_Self_Shp });
  addMapping(Buttons::BUTTON_40, { Osc_A_PM_B, Osc_A_PM_B_Env_B, Osc_A_PM_B_Shp });
  addMapping(Buttons::BUTTON_41, { Shp_A_FB_Mix, Shp_A_FB_Env_C });
  addMapping(Buttons::BUTTON_42, { Shp_B_FB_Mix, Shp_B_FB_Env_C });
  addMapping(Buttons::BUTTON_43, { Osc_B_PM_A, Osc_B_PM_A_Env_A, Osc_B_PM_A_Shp });
  addMapping(Buttons::BUTTON_44, { Osc_A_PM_FB, Osc_A_PM_FB_Env_C, Osc_A_Chirp });
  addMapping(Buttons::BUTTON_45, { Shp_A_Ring_Mod });
  addMapping(Buttons::BUTTON_46, { Shp_B_Ring_Mod });
  addMapping(Buttons::BUTTON_47, { Osc_B_PM_FB, Osc_B_PM_FB_Env_C, Osc_B_Chirp });
  addMapping(Buttons::BUTTON_48, { FB_Mix_Osc, FB_Mix_Osc_Src });
  addMapping(Buttons::BUTTON_49, { Comb_Flt_In_A_B });
  addMapping(Buttons::BUTTON_50, { SV_Flt_In_A_B });
  addMapping(Buttons::BUTTON_51, { Out_Mix_A_Lvl, Out_Mix_A_Pan });
  addMapping(Buttons::BUTTON_52, { FB_Mix_Comb, FB_Mix_Comb_Src });
  addMapping(Buttons::BUTTON_53, { Comb_Flt_Pitch, Comb_Flt_Pitch_Env_C, Comb_Flt_Pitch_KT });
  addMapping(Buttons::BUTTON_54, { SV_Flt_Comb_Mix });
  addMapping(Buttons::BUTTON_55, { Out_Mix_B_Lvl, Out_Mix_B_Pan });
  addMapping(Buttons::BUTTON_56, { FB_Mix_SVF, FB_Mix_SVF_Src });
  addMapping(Buttons::BUTTON_57, { Comb_Flt_Decay, Comb_Flt_Decay_Gate, Comb_Flt_Decay_KT });
  addMapping(Buttons::BUTTON_58, { SV_Flt_Cut, SV_Flt_Cut_Env_C, SV_Flt_Cut_KT });
  addMapping(Buttons::BUTTON_59, { Out_Mix_Comb_Lvl, Out_Mix_Comb_Pan });
  addMapping(Buttons::BUTTON_60, { FB_Mix_FX, FB_Mix_Rvb, FB_Mix_FX_Src });
  addMapping(Buttons::BUTTON_61, { Comb_Flt_AP_Tune, Comb_Flt_AP_Env_C, Comb_Flt_AP_KT, Comb_Flt_AP_Res });
  addMapping(Buttons::BUTTON_62, { SV_Flt_Res, SV_Flt_Res_Env_C, SV_Flt_Res_KT });
  addMapping(Buttons::BUTTON_63, { Out_Mix_SVF_Lvl, Out_Mix_SVF_Pan });
  addMapping(Buttons::BUTTON_64, { FB_Mix_Drive, FB_Mix_Fold, FB_Mix_Asym });
  addMapping(Buttons::BUTTON_65, { Comb_Flt_LP_Tune, Comb_Flt_LP_Env_C, Comb_Flt_LP_KT });
  addMapping(Buttons::BUTTON_66, { SV_Flt_Spread, SV_Flt_LBH, SV_Flt_Par });
  addMapping(Buttons::BUTTON_67, { Out_Mix_Drive, Out_Mix_Fold, Out_Mix_Asym });
  addMapping(Buttons::BUTTON_68, { FB_Mix_Lvl, FB_Mix_Lvl_KT });
  addMapping(Buttons::BUTTON_69, { Comb_Flt_PM, Comb_Flt_PM_A_B });
  addMapping(Buttons::BUTTON_70, { SV_Flt_FM, SV_Flt_FM_A_B });
  addMapping(Buttons::BUTTON_71, { Out_Mix_Lvl, Out_Mix_Key_Pan, Out_Mix_To_FX });

  addMapping(Buttons::BUTTON_72, { Flanger_Rate });
  addMapping(Buttons::BUTTON_73, { Cabinet_Drive, Cabinet_Fold, Cabinet_Asym });
  addMapping(Buttons::BUTTON_74, { Echo_Time, Echo_Stereo });
  addMapping(Buttons::BUTTON_75, { MC_A });

  addMapping(Buttons::BUTTON_76, { Flanger_Envelope, Flanger_Phase });
  addMapping(Buttons::BUTTON_77, { Cabinet_Tilt, Cabinet_Hi_Cut, Cabinet_Lo_Cut });
  addMapping(Buttons::BUTTON_78, { Echo_Feedback, Echo_Cross_FB, Echo_Hi_Cut });
  addMapping(Buttons::BUTTON_79, { MC_B });

  addMapping(Buttons::BUTTON_80, { Flanger_Time_Mod, Flanger_Time, Flanger_Stereo });
  addMapping(Buttons::BUTTON_81, { Cabinet_Mix, Cabinet_Cab_Lvl });
  addMapping(Buttons::BUTTON_82, { Echo_Mix, Echo_Send });
  addMapping(Buttons::BUTTON_83, { MC_C });

  addMapping(Buttons::BUTTON_84, { Flanger_AP_Mod, Flanger_AP_Tune });
  addMapping(Buttons::BUTTON_85, { Gap_Flt_Center, Gap_Flt_Stereo });
  addMapping(Buttons::BUTTON_86, { Reverb_Size, Reverb_Pre_Dly });
  addMapping(Buttons::BUTTON_87, { MC_D });

  addMapping(Buttons::BUTTON_88, { Flanger_Feedback, Flanger_Cross_FB, Flanger_Hi_Cut });
  addMapping(Buttons::BUTTON_89, { Gap_Flt_Gap, Gap_Flt_Bal, Gap_Flt_Res });
  addMapping(Buttons::BUTTON_90, { Reverb_Color, Reverb_Chorus });
  addMapping(Buttons::BUTTON_91, { MC_E });

  addMapping(Buttons::BUTTON_92, { Flanger_Mix, Flanger_Tremolo });
  addMapping(Buttons::BUTTON_93, { Gap_Flt_Mix });
  addMapping(Buttons::BUTTON_94, { Reverb_Mix, Reverb_Send });
  addMapping(Buttons::BUTTON_95, { MC_F });
}

ButtonParameterMapping::~ButtonParameterMapping() = default;

void ButtonParameterMapping::addMapping(Buttons buttonID, std::initializer_list<int> parameterIDs)
{
  for(int paramID : parameterIDs)
  {
    g_assert(m_paramIDToButtonID.find(paramID) == m_paramIDToButtonID.end());
    m_paramIDToButtonID[paramID] = buttonID;
  }

  g_assert(m_buttonIDToParam.find(buttonID) == m_buttonIDToParam.end());
  m_buttonIDToParam[buttonID] = std::list<int>(parameterIDs);
}

void ButtonParameterMapping::forEachButton(std::function<void(Buttons, const std::list<int> &)> cb)
{
  for(auto &a : m_buttonIDToParam)
    cb(a.first, a.second);
}

Buttons ButtonParameterMapping::findButton(int param) const
{
  auto it = m_paramIDToButtonID.find(param);

  if(it != m_paramIDToButtonID.end())
    return it->second;

  return Buttons::INVALID;
}

std::list<int> ButtonParameterMapping::findParameters(Buttons button) const
{
  auto it = m_buttonIDToParam.find(button);

  if(it != m_buttonIDToParam.end())
    return it->second;

  return std::list<int>();
}
