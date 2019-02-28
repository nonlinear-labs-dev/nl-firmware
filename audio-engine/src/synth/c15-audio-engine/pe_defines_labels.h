/******************************************************************************/
/** @file       pe_defines_labels.h
    @date       2018-04-20
    @version    1.0
    @author     Matthias Seeber
    @brief      c15 parameter labels (for better code readability)
                NOTE:
                The provided IDs represent the rendering items of the parameter
                engine, NOT the elements of the shared m_paramsignaldata[][] array!
                ..
    @todo
*******************************************************************************/

#pragma once

/* Envelope Parameter Labels - access parameter with (envIndex + ID) */
// NOTE: placeholders and Env Param Labels are needed until old envelopes disappear

#define E_ATT       0
#define E_DEC1      1
#define E_BP        2
#define E_DEC2      3
#define E_SUS       4
#define E_REL       5
#define E_LV        7
#define E_AV        8
#define E_D1V       9
#define E_D2V       10
#define E_RV        11
#define E_LKT       12
#define E_TKT       13
#define E_AC        14
#define E_RH        15
#define E_SPL       16

/* 'normal' Parameter Labels - access parameter with m_head[ID] - maybe, the rendering index would be much more efficient? */

#define P_EA_ATT    0
#define P_EA_DEC1   1
#define P_EA_BP     2
#define P_EA_DEC2   3
#define P_EA_SUS    4
#define P_EA_REL    5
#define P_EA_GAIN   6
#define P_EA_LV     7
#define P_EA_AV     8
#define P_EA_D1V    9
#define P_EA_D2V    10
#define P_EA_RV     11
#define P_EA_LKT    12
#define P_EA_TKT    13
#define P_EA_AC     14
#define P_EA_SPL    16

#define P_EB_ATT    17
#define P_EB_DEC1   18
#define P_EB_BP     19
#define P_EB_DEC2   20
#define P_EB_SUS    21
#define P_EB_REL    22
#define P_EB_GAIN   23
#define P_EB_LV     24
#define P_EB_AV     25
#define P_EB_D1V    26
#define P_EB_D2V    27
#define P_EB_RV     28
#define P_EB_LKT    29
#define P_EB_TKT    30
#define P_EB_AC     31
#define P_EB_SPL    33

#define P_EC_ATT    34
#define P_EC_DEC1   35
#define P_EC_BP     36
#define P_EC_DEC2   37
#define P_EC_SUS    38
#define P_EC_REL    39
#define P_EC_LV     41
#define P_EC_AV     42
#define P_EC_RV     45
#define P_EC_LKT    46
#define P_EC_TKT    47
#define P_EC_AC     48
#define P_EC_RH     49

#define P_OA_P      51
#define P_OA_PKT    52
#define P_OA_PEC    53
#define P_OA_F      54
#define P_OA_FEC    55
#define P_OA_PMS    56
#define P_OA_PMSEA  57
#define P_OA_PMSSH  58
#define P_OA_PMB    59
#define P_OA_PMBEB  60
#define P_OA_PMBSH  61
#define P_OA_PMF    62
#define P_OA_PMFEC  63
#define P_OA_PHS    64
#define P_OA_CHI    65

#define P_SA_DRV    66
#define P_SA_DEA    67
#define P_SA_FLD    68
#define P_SA_ASM    69
#define P_SA_MIX    70
#define P_SA_FBM    71
#define P_SA_FBEC   72
#define P_SA_RM     73

#define P_OB_P      74
#define P_OB_PKT    75
#define P_OB_PEC    76
#define P_OB_F      77
#define P_OB_FEC    78
#define P_OB_PMS    79
#define P_OB_PMSEB  80
#define P_OB_PMSSH  81
#define P_OB_PMA    82
#define P_OB_PMAEA  83
#define P_OB_PMASH  84
#define P_OB_PMF    85
#define P_OB_PMFEC  86
#define P_OB_PHS    87
#define P_OB_CHI    88

#define P_SB_DRV    89
#define P_SB_DEB    90
#define P_SB_FLD    91
#define P_SB_ASM    92
#define P_SB_MIX    93
#define P_SB_FBM    94
#define P_SB_FBEC   95
#define P_SB_RM     96

#define P_CMB_AB    97
#define P_CMB_P     98
#define P_CMB_PKT   99
#define P_CMB_PEC   100
#define P_CMB_D     101
#define P_CMB_DKT   102
#define P_CMB_DG    103
#define P_CMB_APT   104
#define P_CMB_APKT  105
#define P_CMB_APEC  106
#define P_CMB_APR   107
#define P_CMB_LP    108
#define P_CMB_LPKT  109
#define P_CMB_LPEC  110
#define P_CMB_PM    111
#define P_CMB_PMAB  112

#define P_SVF_AB    113
#define P_SVF_CMB   114
#define P_SVF_CUT   115
#define P_SVF_CKT   116
#define P_SVF_CEC   117
#define P_SVF_SPR   118
#define P_SVF_FM    119
#define P_SVF_RES   120
#define P_SVF_RKT   121
#define P_SVF_REC   122
#define P_SVF_LBH   123
#define P_SVF_PAR   124
#define P_SVF_FMAB  125

#define P_FBM_CMB   126
#define P_FBM_SVF   127
#define P_FBM_FX    128
#define P_FBM_REV   129
#define P_FBM_DRV   130
#define P_FBM_FLD   131
#define P_FBM_ASM   132
#define P_FBM_LKT   133
#define P_FBM_LVL   134

#define P_OM_AL     135
#define P_OM_AP     136
#define P_OM_BL     137
#define P_OM_BP     138
#define P_OM_CL     139
#define P_OM_CP     140
#define P_OM_SL     141
#define P_OM_SP     142
#define P_OM_DRV    143
#define P_OM_FLD    144
#define P_OM_ASM    145
#define P_OM_LVL    146
#define P_OM_KP     147

#define P_CAB_DRV   148
#define P_CAB_FLD   149
#define P_CAB_ASM   150
#define P_CAB_TILT  151
#define P_CAB_LPF   152
#define P_CAB_HPF   153
#define P_CAB_LVL   154
#define P_CAB_MIX   155

#define P_GAP_CNT   156
#define P_GAP_STE   157
#define P_GAP_GAP   158
#define P_GAP_RES   159
#define P_GAP_BAL   160
#define P_GAP_MIX   161

#define P_FLA_TMOD  162
#define P_FLA_PHS   163
#define P_FLA_RTE   164
#define P_FLA_TIME  165
#define P_FLA_STE   166
#define P_FLA_FB    167
#define P_FLA_CFB   168
#define P_FLA_LPF   169
#define P_FLA_MIX   170
#define P_FLA_ENV   171
#define P_FLA_APM   172
#define P_FLA_APT   173

#define P_DLY_TIME  174
#define P_DLY_STE   175
#define P_DLY_FB    176
#define P_DLY_CFB   177
#define P_DLY_LPF   178
#define P_DLY_MIX   179
#define P_DLY_SND   180

#define P_REV_SIZE  181
#define P_REV_PRE   182
#define P_REV_COL   183
#define P_REV_CHO   184
#define P_REV_MIX   185
#define P_REV_SND   186

#define P_MA_V      187
#define P_MA_T      188
#define P_MA_SH     189

#define P_UN_V      190
#define P_UN_DET    191
#define P_UN_PHS    192
#define P_UN_PAN    193

/* milestone 1.5 POLY KEY params */
#define P_KEY_PH    194
#define P_KEY_NP    195
#define P_KEY_VP    196
#define P_KEY_VS    197

/* milestone 1.55 POLY KEY params / milestone 1.56 POLY basePitch param */
#define P_KEY_BP    194
#define P_KEY_IDX   195
#define P_KEY_STL   196
