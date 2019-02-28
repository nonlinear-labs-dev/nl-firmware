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
const uint32_t paramIds_recall[lst_recall_length] = {

    // ENVELOPE A
    P_EA_ATT,   P_EA_DEC1,  P_EA_BP,    P_EA_DEC2,  P_EA_SUS,   P_EA_REL,   P_EA_GAIN,  P_EA_LV,    P_EA_AV,    P_EA_D1V,
    P_EA_D2V,   P_EA_RV,    P_EA_LKT,   P_EA_TKT,   P_EA_AC,    P_EA_SPL,

    // ENVELOPE B
    P_EB_ATT,   P_EB_DEC1,  P_EB_BP,    P_EB_DEC2,  P_EB_SUS,   P_EB_REL,   P_EB_GAIN,  P_EB_LV,    P_EB_AV,    P_EB_D1V,
    P_EB_D2V,   P_EB_RV,    P_EB_LKT,   P_EB_TKT,   P_EB_AC,    P_EB_SPL,

    // ENVELOPE C
    P_EC_ATT,   P_EC_DEC1,  P_EC_BP,    P_EC_DEC2,  P_EC_SUS,   P_EC_REL,   P_EC_LV,    P_EC_AV,    P_EC_RV,    P_EC_LKT,
    P_EC_TKT,   P_EC_AC,    P_EC_RH,

    // OSCILLATOR A
    P_OA_P,     P_OA_PKT,   P_OA_PEC,   P_OA_F,     P_OA_FEC,   P_OA_PMS,   P_OA_PMSEA, P_OA_PMSSH, P_OA_PMB,   P_OA_PMBEB,
    P_OA_PMBSH, P_OA_PMF,   P_OA_PMFEC, P_OA_PHS,   P_OA_CHI,

    // SHAPER A
    P_SA_DRV,   P_SA_DEA,   P_SA_FLD,   P_SA_ASM,   P_SA_MIX,   P_SA_FBM,   P_SA_FBEC,  P_SA_RM,

    // OSCILLATOR B
    P_OB_P,     P_OB_PKT,   P_OB_PEC,   P_OB_F,     P_OB_FEC,   P_OB_PMS,   P_OB_PMSEB, P_OB_PMSSH, P_OB_PMA,   P_OB_PMAEA,
    P_OB_PMASH, P_OB_PMF,   P_OB_PMFEC, P_OB_PHS,   P_OB_CHI,

    // SHAPER B
    P_SB_DRV,   P_SB_DEB,   P_SB_FLD,   P_SB_ASM,   P_SB_MIX,   P_SB_FBM,   P_SB_FBEC,  P_SB_RM,

    // COMB FILTER
    P_CMB_AB,   P_CMB_P,    P_CMB_PKT,  P_CMB_PEC,  P_CMB_D,    P_CMB_DKT,  P_CMB_DG,   P_CMB_APT,  P_CMB_APKT, P_CMB_APEC,
    P_CMB_APR,  P_CMB_LP,   P_CMB_LPKT, P_CMB_LPEC, P_CMB_PM,   P_CMB_PMAB,

    // STATE VARIABLE FILTER
    P_SVF_AB,   P_SVF_CMB,  P_SVF_CUT,  P_SVF_CKT,  P_SVF_CEC,  P_SVF_SPR,  P_SVF_FM,   P_SVF_RES,  P_SVF_RKT,  P_SVF_REC,
    P_SVF_LBH,  P_SVF_PAR,  P_SVF_FMAB,

    // FEEDBACK MIXER
    P_FBM_CMB,  P_FBM_SVF,  P_FBM_FX,   P_FBM_REV,  P_FBM_DRV,  P_FBM_FLD,  P_FBM_ASM,  P_FBM_LKT,  P_FBM_LVL,

    // OUTPUT MIXER
    P_OM_AL,    P_OM_AP,    P_OM_BL,    P_OM_BP,    P_OM_CL,    P_OM_CP,    P_OM_SL,    P_OM_SP,    P_OM_DRV,   P_OM_FLD,
    P_OM_ASM,   P_OM_LVL,   P_OM_KP,

    // CABINET
    P_CAB_DRV,  P_CAB_FLD,  P_CAB_ASM,  P_CAB_TILT, P_CAB_LPF,  P_CAB_HPF,  P_CAB_LVL,  P_CAB_MIX,

    // GAP FILTER
    P_GAP_CNT,  P_GAP_STE,  P_GAP_GAP,  P_GAP_RES,  P_GAP_BAL,  P_GAP_MIX,

    // FLANGER
    P_FLA_TMOD, P_FLA_PHS,  P_FLA_RTE,  P_FLA_TIME, P_FLA_STE,  P_FLA_FB,   P_FLA_CFB,  P_FLA_LPF,  P_FLA_MIX,  P_FLA_ENV,
    P_FLA_APM,  P_FLA_APT,

    // ECHO
    P_DLY_TIME, P_DLY_STE,  P_DLY_FB,   P_DLY_CFB,  P_DLY_LPF,  P_DLY_MIX,  P_DLY_SND,

    // REVERB
    P_REV_SIZE, P_REV_PRE,  P_REV_COL,  P_REV_CHO,  P_REV_MIX,  P_REV_SND,

    // MASTER
    P_MA_V,     P_MA_T,

    // UNISON
    P_UN_V,     P_UN_DET,   P_UN_PHS,   P_UN_PAN

};

// consecutive paramIds for POLY key event update (defining tcd order of destinations) - listId = 2
// NOTE:    - keyEvent param id order by LPC, list mechanism usable -> keep

#if test_milestone == 150

const uint32_t paramIds_keyEvent[lst_keyEvent_length] = {
    P_KEY_PH, P_KEY_NP, P_KEY_VP, P_KEY_VS
};

#elif test_milestone == 155

const uint32_t paramIds_keyEvent[lst_keyEvent_length] = {
    P_KEY_BP, P_KEY_IDX, P_KEY_STL
};

#elif test_milestone == 156

const uint32_t paramIds_keyEvent[lst_keyEvent_length] = {
    P_KEY_BP
};

#endif
