/******************************************************************************/
/** @file       pe_defines_lists.h
    @date       2018-03-13
    @version    1.0
    @author     Matthias Seeber
    @brief      predefined id list declaration (recall, key event)
                ..
    @todo
*******************************************************************************/

#pragma once

#include <stdint.h>
#include "pe_defines_config.h"
#include "pe_defines_labels.h"

// consecutive paramIds for MONO recall update (defining tcd order of destinations) - listId = 1
// NOTE:    - recall param id order by PLAYGROUND, list mechanism fragile/risky in this case -> discard

/* later: define "official" parameter order (playground, ...) in order to transmit preset lists -> also affects testconfig preset data! */
const ParameterLabel paramIds_recall[lst_recall_length] = {

  // ENVELOPE A
  ParameterLabel::P_EA_ATT, ParameterLabel::P_EA_DEC1, ParameterLabel::P_EA_BP, ParameterLabel::P_EA_DEC2,
  ParameterLabel::P_EA_SUS, ParameterLabel::P_EA_REL, ParameterLabel::P_EA_GAIN, ParameterLabel::P_EA_LV,
  ParameterLabel::P_EA_AV, ParameterLabel::P_EA_D1V, ParameterLabel::P_EA_D2V, ParameterLabel::P_EA_RV,
  ParameterLabel::P_EA_LKT, ParameterLabel::P_EA_TKT, ParameterLabel::P_EA_AC, ParameterLabel::P_EA_SPL,

  // ENVELOPE B
  ParameterLabel::P_EB_ATT, ParameterLabel::P_EB_DEC1, ParameterLabel::P_EB_BP, ParameterLabel::P_EB_DEC2,
  ParameterLabel::P_EB_SUS, ParameterLabel::P_EB_REL, ParameterLabel::P_EB_GAIN, ParameterLabel::P_EB_LV,
  ParameterLabel::P_EB_AV, ParameterLabel::P_EB_D1V, ParameterLabel::P_EB_D2V, ParameterLabel::P_EB_RV,
  ParameterLabel::P_EB_LKT, ParameterLabel::P_EB_TKT, ParameterLabel::P_EB_AC, ParameterLabel::P_EB_SPL,

  // ENVELOPE C
  ParameterLabel::P_EC_ATT, ParameterLabel::P_EC_DEC1, ParameterLabel::P_EC_BP, ParameterLabel::P_EC_DEC2,
  ParameterLabel::P_EC_SUS, ParameterLabel::P_EC_REL, ParameterLabel::P_EC_LV, ParameterLabel::P_EC_AV,
  ParameterLabel::P_EC_RV, ParameterLabel::P_EC_LKT, ParameterLabel::P_EC_TKT, ParameterLabel::P_EC_AC,
  ParameterLabel::P_EC_RH,

  // OSCILLATOR A
  ParameterLabel::P_OA_P, ParameterLabel::P_OA_PKT, ParameterLabel::P_OA_PEC, ParameterLabel::P_OA_F,
  ParameterLabel::P_OA_FEC, ParameterLabel::P_OA_PMS, ParameterLabel::P_OA_PMSEA, ParameterLabel::P_OA_PMSSH,
  ParameterLabel::P_OA_PMB, ParameterLabel::P_OA_PMBEB, ParameterLabel::P_OA_PMBSH, ParameterLabel::P_OA_PMF,
  ParameterLabel::P_OA_PMFEC, ParameterLabel::P_OA_PHS, ParameterLabel::P_OA_CHI,

  // SHAPER A
  ParameterLabel::P_SA_DRV, ParameterLabel::P_SA_DEA, ParameterLabel::P_SA_FLD, ParameterLabel::P_SA_ASM,
  ParameterLabel::P_SA_MIX, ParameterLabel::P_SA_FBM, ParameterLabel::P_SA_FBEC, ParameterLabel::P_SA_RM,

  // OSCILLATOR B
  ParameterLabel::P_OB_P, ParameterLabel::P_OB_PKT, ParameterLabel::P_OB_PEC, ParameterLabel::P_OB_F,
  ParameterLabel::P_OB_FEC, ParameterLabel::P_OB_PMS, ParameterLabel::P_OB_PMSEB, ParameterLabel::P_OB_PMSSH,
  ParameterLabel::P_OB_PMA, ParameterLabel::P_OB_PMAEA, ParameterLabel::P_OB_PMASH, ParameterLabel::P_OB_PMF,
  ParameterLabel::P_OB_PMFEC, ParameterLabel::P_OB_PHS, ParameterLabel::P_OB_CHI,

  // SHAPER B
  ParameterLabel::P_SB_DRV, ParameterLabel::P_SB_DEB, ParameterLabel::P_SB_FLD, ParameterLabel::P_SB_ASM,
  ParameterLabel::P_SB_MIX, ParameterLabel::P_SB_FBM, ParameterLabel::P_SB_FBEC, ParameterLabel::P_SB_RM,

  // COMB FILTER
  ParameterLabel::P_CMB_AB, ParameterLabel::P_CMB_P, ParameterLabel::P_CMB_PKT, ParameterLabel::P_CMB_PEC,
  ParameterLabel::P_CMB_D, ParameterLabel::P_CMB_DKT, ParameterLabel::P_CMB_DG, ParameterLabel::P_CMB_APT,
  ParameterLabel::P_CMB_APKT, ParameterLabel::P_CMB_APEC, ParameterLabel::P_CMB_APR, ParameterLabel::P_CMB_LP,
  ParameterLabel::P_CMB_LPKT, ParameterLabel::P_CMB_LPEC, ParameterLabel::P_CMB_PM, ParameterLabel::P_CMB_PMAB,

  // STATE VARIABLE FILTER
  ParameterLabel::P_SVF_AB, ParameterLabel::P_SVF_CMB, ParameterLabel::P_SVF_CUT, ParameterLabel::P_SVF_CKT,
  ParameterLabel::P_SVF_CEC, ParameterLabel::P_SVF_SPR, ParameterLabel::P_SVF_FM, ParameterLabel::P_SVF_RES,
  ParameterLabel::P_SVF_RKT, ParameterLabel::P_SVF_REC, ParameterLabel::P_SVF_LBH, ParameterLabel::P_SVF_PAR,
  ParameterLabel::P_SVF_FMAB,

  // FEEDBACK MIXER
  ParameterLabel::P_FBM_CMB, ParameterLabel::P_FBM_SVF, ParameterLabel::P_FBM_FX, ParameterLabel::P_FBM_REV,
  ParameterLabel::P_FBM_DRV, ParameterLabel::P_FBM_FLD, ParameterLabel::P_FBM_ASM, ParameterLabel::P_FBM_LKT,
  ParameterLabel::P_FBM_LVL,

  // OUTPUT MIXER
  ParameterLabel::P_OM_AL, ParameterLabel::P_OM_AP, ParameterLabel::P_OM_BL, ParameterLabel::P_OM_BP,
  ParameterLabel::P_OM_CL, ParameterLabel::P_OM_CP, ParameterLabel::P_OM_SL, ParameterLabel::P_OM_SP,
  ParameterLabel::P_OM_DRV, ParameterLabel::P_OM_FLD, ParameterLabel::P_OM_ASM, ParameterLabel::P_OM_LVL,
  ParameterLabel::P_OM_KP,

  // CABINET
  ParameterLabel::P_CAB_DRV, ParameterLabel::P_CAB_FLD, ParameterLabel::P_CAB_ASM, ParameterLabel::P_CAB_TILT,
  ParameterLabel::P_CAB_LPF, ParameterLabel::P_CAB_HPF, ParameterLabel::P_CAB_LVL, ParameterLabel::P_CAB_MIX,

  // GAP FILTER
  ParameterLabel::P_GAP_CNT, ParameterLabel::P_GAP_STE, ParameterLabel::P_GAP_GAP, ParameterLabel::P_GAP_RES,
  ParameterLabel::P_GAP_BAL, ParameterLabel::P_GAP_MIX,

  // FLANGER
  ParameterLabel::P_FLA_TMOD, ParameterLabel::P_FLA_PHS, ParameterLabel::P_FLA_RTE, ParameterLabel::P_FLA_TIME,
  ParameterLabel::P_FLA_STE, ParameterLabel::P_FLA_FB, ParameterLabel::P_FLA_CFB, ParameterLabel::P_FLA_LPF,
  ParameterLabel::P_FLA_MIX, ParameterLabel::P_FLA_ENV, ParameterLabel::P_FLA_APM, ParameterLabel::P_FLA_APT,

  // ECHO
  ParameterLabel::P_DLY_TIME, ParameterLabel::P_DLY_STE, ParameterLabel::P_DLY_FB, ParameterLabel::P_DLY_CFB,
  ParameterLabel::P_DLY_LPF, ParameterLabel::P_DLY_MIX, ParameterLabel::P_DLY_SND,

  // REVERB
  ParameterLabel::P_REV_SIZE, ParameterLabel::P_REV_PRE, ParameterLabel::P_REV_COL, ParameterLabel::P_REV_CHO,
  ParameterLabel::P_REV_MIX, ParameterLabel::P_REV_SND,

  // MASTER
  ParameterLabel::P_MA_V, ParameterLabel::P_MA_T,

  // UNISON
  ParameterLabel::P_UN_V, ParameterLabel::P_UN_DET, ParameterLabel::P_UN_PHS, ParameterLabel::P_UN_PAN

};

// consecutive paramIds for POLY key event update (defining tcd order of destinations) - listId = 2
// NOTE:    - keyEvent param id order by LPC, list mechanism usable -> keep

#if test_milestone == 150

const ParameterLabel paramIds_keyEvent[lst_keyEvent_length]
    = { ParameterLabel::P_KEY_PH, ParameterLabel::P_KEY_NP, ParameterLabel::P_KEY_VP, ParameterLabel::P_KEY_VS };

#elif test_milestone == 155

const ParameterLabel paramIds_keyEvent[lst_keyEvent_length]
    = { ParameterLabel::P_KEY_BP, ParameterLabel::P_KEY_IDX, ParameterLabel::P_KEY_STL };

#elif test_milestone == 156

const ParameterLabel paramIds_keyEvent[lst_keyEvent_length] = { ParameterLabel::P_KEY_BP };

#endif
