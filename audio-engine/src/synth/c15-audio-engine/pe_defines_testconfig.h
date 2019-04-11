/* configuration of the test project */

#pragma once

#include <stdint.h>
#include "pe_defines_config.h"
#include "pe_defines_labels.h"

/*
 * Test 3 requires a reMote keyboard with the correct template (64: MSE 2) -> see Linuy Engine - Test 3 for details on handling
 */

/* Preset Definition */

#define testRecallSequences 8
#define testMidiModes 2

const int32_t testPresetData[testRecallSequences][lst_recall_length] = {
    /* preset 0 - default */
    {
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env A
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env B
        0,     9440,  8000,  12640, 0,     8480,  7680,  0,     0,     0,     800,   0,     0,                          // Env C
        8000,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc A
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp A
        8000,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc B
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp B
        0,     6000,  10000, 0,     0,     5280,  0,     14000, 16000, 0,     10560, 10000, 16000, 0,     0,     0,     // Comb
        0,     0,     6000,  16000, 0,     1200,  0,     8000,  0,     0,     0,     0,     0,                          // SVF
        0,     0,     0,     8000,  0,     8000,  0,     0,     6080,                                                   // FB Mix
        0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,  0,     6080,  0,                          // Out Mix
        4000,  4000,  4000,  -1600, 10000, 2000,  7200,  0,                                                             // Cabinet
        4800,  0,     1200,  8000,  0,     0,                                                                           // Gap Filter
        0,     7200,  5072,  3963,  0,     0,     4000,  12000, 0,     0,     0,     14000,                             // Flanger
        6928,  0,     8000,  8000,  12000, 0,     16000,                                                                // Echo
        5280,  5280,  8000,  4000,  0,     16000,                                                                       // Reverb
        8000,  0,                                                                                                       // Master
        1,     0,     0,     0                                                                                          // Unison
    },
    /* preset 1 - default tone */
    {
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env A
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env B
        0,     9440,  8000,  12640, 0,     8480,  7680,  0,     0,     0,     800,   0,     0,                          // Env C
        8000,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc A
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp A
        8000,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc B
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp B
        0,     6000,  10000, 0,     0,     5280,  0,     14000, 16000, 0,     10560, 10000, 16000, 0,     0,     0,     // Comb
        0,     0,     6000,  16000, 0,     1200,  0,     8000,  0,     0,     0,     0,     0,                          // SVF
        0,     0,     0,     8000,  0,     8000,  0,     0,     6080,                                                   // FB Mix
        2000,  0,     0,     0,     0,     0,     0,     0,     0,     8000,  0,     6080,  0,                          // Out Mix
        4000,  4000,  4000,  -1600, 10000, 2000,  7200,  0,                                                             // Cabinet
        4800,  0,     1200,  8000,  0,     0,                                                                           // Gap Filter
        0,     7200,  5072,  3963,  0,     0,     4000,  12000, 0,     0,     0,     14000,                             // Flanger
        6928,  0,     8000,  8000,  12000, 0,     16000,                                                                // Echo
        5280,  5280,  8000,  4000,  0,     16000,                                                                       // Reverb
        8000,  0,                                                                                                       // Master
        1,     0,     0,     0                                                                                          // Unison
    },
    /* preset 2 - testing comb filter */
    {
        0,     0,     8000,  8000,  0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     // Env A
        0,     0,     8000,  8000,  0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     // Env B
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,                          // Env C
        8000,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc A
        0,     0,     0,     0,     0,     0,     0,     0,                                                             // Shp A
        9200,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc B
        0,     0,     0,     0,     0,     0,     0,     0,                                                             // Shp B
        0,     6000,  10000, 0,     0,     0,     0,     14000, 16000, 0,     10560, 10000, 16000, 0,     0,     0,     // Comb
        0,     0,     6000,  16000, 0,     1200,  0,     8000,  0,     0,     0,     0,     0,                          // SVF
        0,     0,     0,     8000,  0,     8000,  0,     0,     6080,                                                   // FB Mix
        0,     0,     0,     0,     2000,  0,     0,     0,     0,     8000,  0,     6080,  0,                          // Out Mix
        4000,  4000,  4000,  -1600, 10000, 2000,  7200,  0,                                                             // Cabinet
        4800,  0,     1200,  8000,  0,     0,                                                                           // Gap Filter
        0,     7200,  5072,  3963,  0,     0,     4000,  12000, 0,     0,     0,     14000,                             // Flanger
        6928,  0,     8000,  8000,  12000, 0,     16000,                                                                // Echo
        5280,  5280,  8000,  4000,  0,     16000,                                                                       // Reverb
        8000,  0,                                                                                                       // Master
        1,     0,     0,     0                                                                                          // Unison
    },
    /* preset 3 - testing state variable filter */
    {
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env A
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env B
        0,     9440,  8000,  12640, 0,     8480,  7680,  0,     0,     0,     800,   0,     0,                          // Env C
        8000,  10000, 0,     0,     0,     8000,  16000, 0,     0,     0,     0,     0,     0,     0,     8000,         // Osc A
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp A
        9200,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc B
        4000,  0,     8000,  0,     8000,  0,     0,     0,                                                             // Shp B
        0,     6000,  10000, 0,     0,     5280,  0,     14000, 16000, 0,     10560, 10000, 16000, 0,     0,     0,     // Comb
        0,     0,     6000,  16000, 0,     1200,  0,     8000,  0,     0,     0,     0,     0,                          // SVF
        0,     0,     0,     8000,  0,     8000,  0,     0,     6080,                                                   // FB Mix
        0,     0,     0,     0,     0,     0,     2000,  0,     0,     8000,  0,     6080,  0,                          // Out Mix
        4000,  4000,  4000,  -1600, 10000, 2000,  7200,  0,                                                             // Cabinet
        4800,  0,     1200,  8000,  0,     0,                                                                           // Gap Filter
        0,     7200,  5072,  3963,  0,     0,     4000,  12000, 0,     0,     0,     14000,                             // Flanger
        6928,  0,     8000,  8000,  12000, 0,     16000,                                                                // Echo
        5280,  5280,  8000,  4000,  0,     16000,                                                                       // Reverb
        8000,  0,                                                                                                       // Master
        1,     0,     0,     0                                                                                          // Unison
    },
    /* preset 4 - testing envelope a/b split */
    {
        0,     9440,  8000,  12640, 0,     8480,  0,     0,     0,     0,     0,     0,     0,     800,   0,     0,     // Env A
        0,     9440,  8000,  12640, 0,     8480,  0,     0,     0,     0,     0,     0,     0,     800,   0,     0,     // Env B
        0,     9440,  8000,  12640, 0,     8480,  7680,  0,     0,     0,     800,   0,     0,                          // Env C
        8000,  10000, 0,     0,     0,     8000,  16000, 0,     0,     0,     0,     0,     0,     0,     8000,         // Osc A
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp A
        9200,  10000, 0,     0,     0,     8000,  16000, 0,     0,     0,     0,     0,     0,     0,     8000,         // Osc B
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp B
        0,     6000,  10000, 0,     0,     5280,  0,     14000, 16000, 0,     10560, 10000, 16000, 0,     0,     0,     // Comb
        0,     0,     6000,  16000, 0,     1200,  0,     8000,  0,     0,     0,     0,     0,                          // SVF
        0,     0,     0,     8000,  0,     8000,  0,     0,     6080,                                                   // FB Mix
        2000,  0,     2000,  0,     0,     0,     0,     0,     0,     8000,  0,     6080,  0,                          // Out Mix
        4000,  4000,  4000,  -1600, 10000, 2000,  7200,  0,                                                             // Cabinet
        4800,  0,     1200,  8000,  0,     0,                                                                           // Gap Filter
        0,     7200,  5072,  3963,  0,     0,     4000,  12000, 0,     0,     0,     14000,                             // Flanger
        6928,  0,     8000,  8000,  12000, 0,     16000,                                                                // Echo
        5280,  5280,  8000,  4000,  0,     16000,                                                                       // Reverb
        8000,  0,                                                                                                       // Master
        1,     0,     0,     0                                                                                          // Unison
    },
    /* preset 5 - testing envelope c retrigger (use one voice polyphony for testing) */
    {
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env A
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env B
        12000, 9440,  8000,  12640, 4000,  14000, 0,     0,     0,     0,     800,   0,     0,                          // Env C
        8000,  10000, 4000,  0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc A
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp A
        8000,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc B
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp B
        0,     6000,  10000, 0,     0,     5280,  0,     14000, 16000, 0,     10560, 10000, 16000, 0,     0,     0,     // Comb
        0,     0,     6000,  16000, 0,     1200,  0,     8000,  0,     0,     0,     0,     0,                          // SVF
        0,     0,     0,     8000,  0,     8000,  0,     0,     6080,                                                   // FB Mix
        2000,  0,     0,     0,     0,     0,     0,     0,     0,     8000,  0,     6080,  0,                          // Out Mix
        4000,  4000,  4000,  -1600, 10000, 2000,  7200,  0,                                                             // Cabinet
        4800,  0,     1200,  8000,  0,     0,                                                                           // Gap Filter
        0,     7200,  5072,  3963,  0,     0,     4000,  12000, 0,     0,     0,     14000,                             // Flanger
        6928,  0,     8000,  8000,  12000, 0,     16000,                                                                // Echo
        5280,  5280,  8000,  4000,  0,     16000,                                                                       // Reverb
        8000,  0,                                                                                                       // Master
        1,     0,     0,     0                                                                                          // Unison
    },
    /* preset 6 - SVF NaN issue */
    {
        0,     9440,  8000,  12640, 0,     8480,  0,     0,     0,     0,     0,     0,     0,     800,   0,     0,     // Env A
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env B
        0,     9440,  8000,  12640, 0,     8480,  7680,  0,     0,     0,     800,   0,     0,                          // Env C
        8000,  10000, 0,     0,     0,     8000,  16000, 0,     0,     0,     0,     0,     0,     0,     8000,         // Osc A
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp A
        9200,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc B
        4000,  0,     8000,  0,     8000,  0,     0,     0,                                                             // Shp B
        0,     6000,  10000, 0,     0,     5280,  0,     14000, 16000, 0,     10560, 10000, 16000, 0,     0,     0,     // Comb
        0,     0,     12000, 16000, 8000,  -6000, 8000,  16000, 0,     0,     0,     0,     0,                          // SVF
        0,     0,     0,     8000,  0,     8000,  0,     0,     6080,                                                   // FB Mix
        0,     0,     0,     0,     0,     0,     2000,  0,     0,     8000,  0,     6080,  0,                          // Out Mix
        4000,  4000,  4000,  -1600, 10000, 2000,  7200,  0,                                                             // Cabinet
        4800,  0,     1200,  8000,  0,     0,                                                                           // Gap Filter
        0,     7200,  5072,  3963,  0,     0,     4000,  12000, 0,     0,     0,     14000,                             // Flanger
        6928,  0,     8000,  8000,  12000, 0,     16000,                                                                // Echo
        5280,  5280,  8000,  4000,  0,     16000,                                                                       // Reverb
        8000,  0,                                                                                                       // Master
        1,     0,     0,     0                                                                                          // Unison
    },
#if 0
    /* preset 7 - SVF NAN (handmade copy of "Linux Engine NAN" Bank) */
    {
        0,     9440,  8000,  12640, 0,     8560,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env A
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env B
        0,     9440,  8000,  12640, 0,     8480,  7680,  0,     0,     0,     800,   0,     0,                          // Env C
        8000,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc A
        2000,  0,     8000,  0,     8000,  0,     0,     0,                                                             // Shp A
        8000,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc B
        2000,  0,     8000,  0,     0,     0,     0,     0,                                                             // Shp B
        0,     6000,  10000, 0,     0,     5280,  0,     14000, 16000, 0,     10560, 10000, 16000, 0,     0,     0,     // Comb
        0,     0,     6400,  16000, 0,     4900,  -8000, 12800, 0,     0,     4640,  -2560, 16000,                      // SVF
        0,     0,     0,     8000,  0,     8000,  0,     0,     6080,                                                   // FB Mix
        0,     0,     0,     0,     0,     0,     4320,  0,     0,     8000,  0,     6080,  0,                          // Out Mix
        4000,  4000,  4000,  -1600, 10000, 2000,  7200,  0,                                                             // Cabinet
        4800,  0,     1200,  8000,  0,     0,                                                                           // Gap Filter
        0,     7200,  5072,  3963,  0,     0,     4000,  12000, 0,     0,     0,     14000,                             // Flanger
        6928,  0,     8000,  8000,  12000, 0,     16000,                                                                // Echo
        5280,  5280,  8000,  4000,  0,     16000,                                                                       // Reverb
        8000,  0,                                                                                                       // Master
        1,     0,     0,     0                                                                                          // Unison
    },
#endif
    /* preset 7 - feedback mixer asym issue */
    {
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env A
        0,     9440,  8000,  12640, 0,     8480,  0,     7680,  0,     0,     0,     0,     0,     800,   0,     0,     // Env B
        0,     9440,  8000,  12640, 0,     8480,  7680,  0,     0,     0,     800,   0,     0,                          // Env C
        8000,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc A
        0,     0,     0,     0,     0,     0,     0,     0,                                                             // Shp A
        8000,  10000, 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     8000,         // Osc B
        0,     0,     0,     0,     0,     16000, 0,     0,                                                             // Shp B
        0,     6000,  10000, 0,     0,     5280,  0,     14000, 16000, 0,     10560, 10000, 16000, 0,     0,     0,     // Comb
        0,     0,     6000,  16000, 0,     1200,  0,     8000,  0,     0,     0,     0,     0,                          // SVF
        4000,  0,     0,     8000,  0,     0,     0,     0,     6080,                                                   // FB Mix
        0,     0,     4000,  0,     0,     0,     0,     0,     0,     0,     0,     6080,  0,                          // Out Mix
        4000,  4000,  4000,  -1600, 10000, 2000,  7200,  0,                                                             // Cabinet
        4800,  0,     1200,  8000,  0,     0,                                                                           // Gap Filter
        0,     7200,  5072,  3963,  0,     0,     4000,  12000, 0,     0,     0,     14000,                             // Flanger
        6928,  0,     8000,  8000,  12000, 0,     16000,                                                                // Echo
        5280,  5280,  8000,  4000,  0,     16000,                                                                       // Reverb
        8000,  0,                                                                                                       // Master
        1,     0,     0,     0                                                                                          // Unison
    },
};

const uint32_t testMidiMapping[testMidiModes][128] = {
    /* - - - - - - - Mode 1 - Group Select - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    {
    /*  0      1      2      3      4      5      6      7      8      9     */
        0,     33,    34,    35,    36,    37,    38,    39,    40,    0,
        0,     41,    42,    43,    44,    45,    46,    47,    48,    0,
        0,     49,    50,    51,    52,    53,    54,    55,    56,    0,
        0,     1,     2,     3,     4,     5,     0,     0,     0,     0,
        0,     65,    66,    67,    68,    69,    70,    71,    72,    0,
        0,     73,    74,    75,    76,    77,    78,    79,    80,    0,
        0,     81,    82,    83,    84,    85,    86,    87,    88,    0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
    },
    /* - - - - - - - Mode 2 - Global Stuff - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    {
    /*  0      1      2      3      4      5      6      7      8      9     */
        0,     6,     18,    19,    20,    0,     0,     0,     0,     0,
        0,     7,     16,    17,    0,     0,     0,     0,     0,     0,
        0,     8,     9,     10,    11,    12,    13,    14,    15,    0,
        0,     1,     2,     3,     4,     5,     0,     0,     0,     0,
        0,     65,    67,    68,    0,     0,     0,     0,     0,     0,
        0,     66,    0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
        0,     0,     0,     0,     0,     0,     0,     0,
    },
};

const int32_t testParamRouting[25][24] = {
    /* IGNORE group */
    {
    /*  0             1             2             3             4             5             6             7            */
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* ENV A */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_EA_ATT,     P_EA_DEC1,    ParameterLabel::P_EA_BP,      P_EA_DEC2,    ParameterLabel::P_EA_SUS,     ParameterLabel::P_EA_REL,     ParameterLabel::P_EA_GAIN,    ParameterLabel::P_EA_LV,
        ParameterLabel::P_EA_AV,      ParameterLabel::P_EA_AC,      P_EA_D1V,     P_EA_D2V,     ParameterLabel::P_EA_SPL,     ParameterLabel::P_EA_RV,      ParameterLabel::P_EA_LKT,     ParameterLabel::P_EA_TKT,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* ENV B */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_EB_ATT,     P_EB_DEC1,    ParameterLabel::P_EB_BP,      P_EB_DEC2,    ParameterLabel::P_EB_SUS,     ParameterLabel::P_EB_REL,     ParameterLabel::P_EB_GAIN,    ParameterLabel::P_EB_LV,
        ParameterLabel::P_EB_AV,      ParameterLabel::P_EB_AC,      P_EB_D1V,     P_EB_D2V,     ParameterLabel::P_EB_SPL,     ParameterLabel::P_EB_RV,      ParameterLabel::P_EB_LKT,     ParameterLabel::P_EB_TKT,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* ENV C */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_EC_ATT,     P_EC_DEC1,    ParameterLabel::P_EC_BP,      P_EC_DEC2,    ParameterLabel::P_EC_SUS,     ParameterLabel::P_EC_REL,     -1,           ParameterLabel::P_EC_LV,
        ParameterLabel::P_EC_AV,      ParameterLabel::P_EC_AC,      -1,           -1,           ParameterLabel::P_EC_RH,      ParameterLabel::P_EC_RV,      ParameterLabel::P_EC_LKT,     ParameterLabel::P_EC_TKT,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* OSC A */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_OA_P,       ParameterLabel::P_OA_PKT,     ParameterLabel::P_OA_PEC,     ParameterLabel::P_OA_F,       ParameterLabel::P_OA_FEC,     ParameterLabel::P_OA_PMS,     ParameterLabel::P_OA_PMSEA,   ParameterLabel::P_OA_PMSSH,
        -1,           ParameterLabel::P_OA_PHS,     ParameterLabel::P_OA_CHI,     ParameterLabel::P_OA_PMF,     ParameterLabel::P_OA_PMFEC,   ParameterLabel::P_OA_PMB,     ParameterLabel::P_OA_PMBEB,   ParameterLabel::P_OA_PMBSH,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* SHP A */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_SA_DRV,     ParameterLabel::P_SA_FLD,     ParameterLabel::P_SA_ASM,     ParameterLabel::P_SA_MIX,     ParameterLabel::P_SA_FBM,     ParameterLabel::P_SA_RM,      -1,           -1,
        ParameterLabel::P_SA_DEA,     -1,           -1,           -1,           ParameterLabel::P_SA_FBEC,    -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* OSC B */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_OB_P,       ParameterLabel::P_OB_PKT,     ParameterLabel::P_OB_PEC,     ParameterLabel::P_OB_F,       ParameterLabel::P_OB_FEC,     ParameterLabel::P_OB_PMS,     ParameterLabel::P_OB_PMSEB,   ParameterLabel::P_OB_PMSSH,
        -1,           ParameterLabel::P_OB_PHS,     ParameterLabel::P_OB_CHI,     ParameterLabel::P_OB_PMF,     ParameterLabel::P_OB_PMFEC,   ParameterLabel::P_OB_PMA,     ParameterLabel::P_OB_PMAEA,   ParameterLabel::P_OB_PMASH,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* SHP B */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_SB_DRV,     ParameterLabel::P_SB_FLD,     ParameterLabel::P_SB_ASM,     ParameterLabel::P_SB_MIX,     ParameterLabel::P_SB_FBM,     ParameterLabel::P_SB_RM,      -1,           -1,
        ParameterLabel::P_SB_DEB,     -1,           -1,           -1,           ParameterLabel::P_SB_FBEC,    -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* OUT MIX */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_OM_AL,      ParameterLabel::P_OM_BL,      ParameterLabel::P_OM_CL,      ParameterLabel::P_OM_SL,      ParameterLabel::P_OM_LVL,     ParameterLabel::P_OM_DRV,     ParameterLabel::P_OM_FLD,     ParameterLabel::P_OM_ASM,
        ParameterLabel::P_OM_AP,      ParameterLabel::P_OM_BP,      ParameterLabel::P_OM_CP,      ParameterLabel::P_OM_SP,      ParameterLabel::P_OM_KP,      -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* COMB FILTER */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_CMB_AB,     ParameterLabel::P_CMB_P,      ParameterLabel::P_CMB_PKT,    ParameterLabel::P_CMB_PEC,    ParameterLabel::P_CMB_D,      ParameterLabel::P_CMB_DKT,    ParameterLabel::P_CMB_DG,     ParameterLabel::P_CMB_PM,
        ParameterLabel::P_CMB_APT,    ParameterLabel::P_CMB_APKT,   ParameterLabel::P_CMB_APEC,   ParameterLabel::P_CMB_APR,    ParameterLabel::P_CMB_LP,     ParameterLabel::P_CMB_LPKT,   ParameterLabel::P_CMB_LPEC,   ParameterLabel::P_CMB_PMAB,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* STATE VARIABLE FILTER */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_SVF_AB,     ParameterLabel::P_SVF_CUT,    ParameterLabel::P_SVF_CKT,    ParameterLabel::P_SVF_CEC,    -1,           ParameterLabel::P_SVF_SPR,    ParameterLabel::P_SVF_LBH,    ParameterLabel::P_SVF_FM,
        ParameterLabel::P_SVF_CMB,    ParameterLabel::P_SVF_RES,    ParameterLabel::P_SVF_RKT,    ParameterLabel::P_SVF_REC,    -1,           -1,           ParameterLabel::P_SVF_PAR,    ParameterLabel::P_SVF_FMAB,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* CABINET,     GAP FILTER */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_CAB_DRV,    ParameterLabel::P_CAB_FLD,    ParameterLabel::P_CAB_ASM,    ParameterLabel::P_CAB_TILT,   ParameterLabel::P_CAB_LPF,    ParameterLabel::P_CAB_HPF,    ParameterLabel::P_CAB_LVL,    ParameterLabel::P_CAB_MIX,
        ParameterLabel::P_GAP_CNT,    ParameterLabel::P_GAP_STE,    ParameterLabel::P_GAP_GAP,    ParameterLabel::P_GAP_RES,    ParameterLabel::P_GAP_BAL,    ParameterLabel::P_GAP_MIX,    -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* FLANGER */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_FLA_TMOD,   ParameterLabel::P_FLA_PHS,    ParameterLabel::P_FLA_RTE,    ParameterLabel::P_FLA_TIME,   ParameterLabel::P_FLA_STE,    ParameterLabel::P_FLA_FB,     ParameterLabel::P_FLA_CFB,    ParameterLabel::P_FLA_MIX,
        ParameterLabel::P_FLA_ENV,    ParameterLabel::P_FLA_APM,    ParameterLabel::P_FLA_APT,    ParameterLabel::P_FLA_LPF,    -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* ECHO,     REVERB */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_DLY_SND,    ParameterLabel::P_DLY_TIME,   ParameterLabel::P_DLY_STE,    ParameterLabel::P_DLY_FB,     ParameterLabel::P_DLY_CFB,    ParameterLabel::P_DLY_LPF,    ParameterLabel::P_DLY_MIX,          -1,
        ParameterLabel::P_REV_SND,    ParameterLabel::P_REV_SIZE,   ParameterLabel::P_REV_PRE,    ParameterLabel::P_REV_COL,    ParameterLabel::P_REV_CHO,    ParameterLabel::P_REV_MIX,    -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* (none) */
    {
    /*  0             1             2             3             4             5             6             7            */
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* (none) */
    {
    /*  0             1             2             3             4             5             6             7            */
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* FEEDBACK MIX */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_FBM_CMB,    ParameterLabel::P_FBM_SVF,    ParameterLabel::P_FBM_FX,     ParameterLabel::P_FBM_REV,    ParameterLabel::P_FBM_DRV,    ParameterLabel::P_FBM_FLD,    ParameterLabel::P_FBM_ASM,    ParameterLabel::P_FBM_LVL,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           ParameterLabel::P_FBM_LKT,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* MASTER,     UNISON */
    {
    /*  0             1             2             3             4             5             6             7            */
        ParameterLabel::P_MA_V,       ParameterLabel::P_MA_T,       ParameterLabel::P_MA_SH,      -1,           -1,           -1,           -1,           -1,
        ParameterLabel::P_UN_V,       ParameterLabel::P_UN_DET,     ParameterLabel::P_UN_PHS,     ParameterLabel::P_UN_PAN,     -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* (none) */
    {
    /*  0             1             2             3             4             5             6             7            */
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* (none) */
    {
    /*  0             1             2             3             4             5             6             7            */
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* (none) */
    {
    /*  0             1             2             3             4             5             6             7            */
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* (none) */
    {
    /*  0             1             2             3             4             5             6             7            */
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* (none) */
    {
    /*  0             1             2             3             4             5             6             7            */
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* (none) */
    {
    /*  0             1             2             3             4             5             6             7            */
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
    /* (none) */
    {
    /*  0             1             2             3             4             5             6             7            */
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
        -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
    },
};
