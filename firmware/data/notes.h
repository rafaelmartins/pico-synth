#pragma once

#include <note.h>

#define note_clkdiv 40.747549
#define note_last 127

static const note_t notes[] = {
    {
        .id   = 0,
        .name = "C-1",
        .step = {
            .pint  = 0,
            .pfrac = 45745,
        },
    },
    {
        .id   = 1,
        .name = "C#-1",
        .step = {
            .pint  = 0,
            .pfrac = 48430,
        },
    },
    {
        .id   = 2,
        .name = "D-1",
        .step = {
            .pint  = 0,
            .pfrac = 51338,
        },
    },
    {
        .id   = 3,
        .name = "D#-1",
        .step = {
            .pint  = 0,
            .pfrac = 54358,
        },
    },
    {
        .id   = 4,
        .name = "E-1",
        .step = {
            .pint  = 0,
            .pfrac = 57601,
        },
    },
    {
        .id   = 5,
        .name = "F-1",
        .step = {
            .pint  = 0,
            .pfrac = 61013,
        },
    },
    {
        .id   = 6,
        .name = "F#-1",
        .step = {
            .pint  = 0,
            .pfrac = 64648,
        },
    },
    {
        .id   = 7,
        .name = "G-1",
        .step = {
            .pint  = 1,
            .pfrac = 2970,
        },
    },
    {
        .id   = 8,
        .name = "G#-1",
        .step = {
            .pint  = 1,
            .pfrac = 7053,
        },
    },
    {
        .id   = 9,
        .name = "A-1",
        .step = {
            .pint  = 1,
            .pfrac = 11359,
        },
    },
    {
        .id   = 10,
        .name = "A#-1",
        .step = {
            .pint  = 1,
            .pfrac = 15945,
        },
    },
    {
        .id   = 11,
        .name = "B-1",
        .step = {
            .pint  = 1,
            .pfrac = 20754,
        },
    },
    {
        .id   = 12,
        .name = "C0",
        .step = {
            .pint  = 1,
            .pfrac = 25899,
        },
    },
    {
        .id   = 13,
        .name = "C#0",
        .step = {
            .pint  = 1,
            .pfrac = 31324,
        },
    },
    {
        .id   = 14,
        .name = "D0",
        .step = {
            .pint  = 1,
            .pfrac = 37084,
        },
    },
    {
        .id   = 15,
        .name = "D#0",
        .step = {
            .pint  = 1,
            .pfrac = 43236,
        },
    },
    {
        .id   = 16,
        .name = "E0",
        .step = {
            .pint  = 1,
            .pfrac = 49667,
        },
    },
    {
        .id   = 17,
        .name = "F0",
        .step = {
            .pint  = 1,
            .pfrac = 56546,
        },
    },
    {
        .id   = 18,
        .name = "F#0",
        .step = {
            .pint  = 1,
            .pfrac = 63760,
        },
    },
    {
        .id   = 19,
        .name = "G0",
        .step = {
            .pint  = 2,
            .pfrac = 5941,
        },
    },
    {
        .id   = 20,
        .name = "G#0",
        .step = {
            .pint  = 2,
            .pfrac = 14106,
        },
    },
    {
        .id   = 21,
        .name = "A0",
        .step = {
            .pint  = 2,
            .pfrac = 22719,
        },
    },
    {
        .id   = 22,
        .name = "A#0",
        .step = {
            .pint  = 2,
            .pfrac = 31890,
        },
    },
    {
        .id   = 23,
        .name = "B0",
        .step = {
            .pint  = 2,
            .pfrac = 41565,
        },
    },
    {
        .id   = 24,
        .name = "C1",
        .step = {
            .pint  = 2,
            .pfrac = 51799,
        },
    },
    {
        .id   = 25,
        .name = "C#1",
        .step = {
            .pint  = 2,
            .pfrac = 62704,
        },
    },
    {
        .id   = 26,
        .name = "D1",
        .step = {
            .pint  = 3,
            .pfrac = 8689,
        },
    },
    {
        .id   = 27,
        .name = "D#1",
        .step = {
            .pint  = 3,
            .pfrac = 20880,
        },
    },
    {
        .id   = 28,
        .name = "E1",
        .step = {
            .pint  = 3,
            .pfrac = 33799,
        },
    },
    {
        .id   = 29,
        .name = "F1",
        .step = {
            .pint  = 3,
            .pfrac = 47500,
        },
    },
    {
        .id   = 30,
        .name = "F#1",
        .step = {
            .pint  = 3,
            .pfrac = 62040,
        },
    },
    {
        .id   = 31,
        .name = "G1",
        .step = {
            .pint  = 4,
            .pfrac = 11883,
        },
    },
    {
        .id   = 32,
        .name = "G#1",
        .step = {
            .pint  = 4,
            .pfrac = 28157,
        },
    },
    {
        .id   = 33,
        .name = "A1",
        .step = {
            .pint  = 4,
            .pfrac = 45438,
        },
    },
    {
        .id   = 34,
        .name = "A#1",
        .step = {
            .pint  = 4,
            .pfrac = 63725,
        },
    },
    {
        .id   = 35,
        .name = "B1",
        .step = {
            .pint  = 5,
            .pfrac = 17595,
        },
    },
    {
        .id   = 36,
        .name = "C2",
        .step = {
            .pint  = 5,
            .pfrac = 38119,
        },
    },
    {
        .id   = 37,
        .name = "C#2",
        .step = {
            .pint  = 5,
            .pfrac = 59873,
        },
    },
    {
        .id   = 38,
        .name = "D2",
        .step = {
            .pint  = 6,
            .pfrac = 17378,
        },
    },
    {
        .id   = 39,
        .name = "D#2",
        .step = {
            .pint  = 6,
            .pfrac = 41761,
        },
    },
    {
        .id   = 40,
        .name = "E2",
        .step = {
            .pint  = 7,
            .pfrac = 2118,
        },
    },
    {
        .id   = 41,
        .name = "F2",
        .step = {
            .pint  = 7,
            .pfrac = 29520,
        },
    },
    {
        .id   = 42,
        .name = "F#2",
        .step = {
            .pint  = 7,
            .pfrac = 58545,
        },
    },
    {
        .id   = 43,
        .name = "G2",
        .step = {
            .pint  = 8,
            .pfrac = 23767,
        },
    },
    {
        .id   = 44,
        .name = "G#2",
        .step = {
            .pint  = 8,
            .pfrac = 56371,
        },
    },
    {
        .id   = 45,
        .name = "A2",
        .step = {
            .pint  = 9,
            .pfrac = 25340,
        },
    },
    {
        .id   = 46,
        .name = "A#2",
        .step = {
            .pint  = 9,
            .pfrac = 61914,
        },
    },
    {
        .id   = 47,
        .name = "B2",
        .step = {
            .pint  = 10,
            .pfrac = 35134,
        },
    },
    {
        .id   = 48,
        .name = "C3",
        .step = {
            .pint  = 11,
            .pfrac = 10646,
        },
    },
    {
        .id   = 49,
        .name = "C#3",
        .step = {
            .pint  = 11,
            .pfrac = 54155,
        },
    },
    {
        .id   = 50,
        .name = "D3",
        .step = {
            .pint  = 12,
            .pfrac = 34700,
        },
    },
    {
        .id   = 51,
        .name = "D#3",
        .step = {
            .pint  = 13,
            .pfrac = 17986,
        },
    },
    {
        .id   = 52,
        .name = "E3",
        .step = {
            .pint  = 14,
            .pfrac = 4180,
        },
    },
    {
        .id   = 53,
        .name = "F3",
        .step = {
            .pint  = 14,
            .pfrac = 58985,
        },
    },
    {
        .id   = 54,
        .name = "F#3",
        .step = {
            .pint  = 15,
            .pfrac = 51554,
        },
    },
    {
        .id   = 55,
        .name = "G3",
        .step = {
            .pint  = 16,
            .pfrac = 47535,
        },
    },
    {
        .id   = 56,
        .name = "G#3",
        .step = {
            .pint  = 17,
            .pfrac = 47150,
        },
    },
    {
        .id   = 57,
        .name = "A3",
        .step = {
            .pint  = 18,
            .pfrac = 50681,
        },
    },
    {
        .id   = 58,
        .name = "A#3",
        .step = {
            .pint  = 19,
            .pfrac = 58293,
        },
    },
    {
        .id   = 59,
        .name = "B3",
        .step = {
            .pint  = 21,
            .pfrac = 4732,
        },
    },
    {
        .id   = 60,
        .name = "C4",
        .step = {
            .pint  = 22,
            .pfrac = 21349,
        },
    },
    {
        .id   = 61,
        .name = "C#4",
        .step = {
            .pint  = 23,
            .pfrac = 42774,
        },
    },
    {
        .id   = 62,
        .name = "D4",
        .step = {
            .pint  = 25,
            .pfrac = 3865,
        },
    },
    {
        .id   = 63,
        .name = "D#4",
        .step = {
            .pint  = 26,
            .pfrac = 36029,
        },
    },
    {
        .id   = 64,
        .name = "E4",
        .step = {
            .pint  = 28,
            .pfrac = 8416,
        },
    },
    {
        .id   = 65,
        .name = "F4",
        .step = {
            .pint  = 29,
            .pfrac = 52491,
        },
    },
    {
        .id   = 66,
        .name = "F#4",
        .step = {
            .pint  = 31,
            .pfrac = 37518,
        },
    },
    {
        .id   = 67,
        .name = "G4",
        .step = {
            .pint  = 33,
            .pfrac = 29534,
        },
    },
    {
        .id   = 68,
        .name = "G#4",
        .step = {
            .pint  = 35,
            .pfrac = 28765,
        },
    },
    {
        .id   = 69,
        .name = "A4",
        .step = {
            .pint  = 37,
            .pfrac = 35826,
        },
    },
    {
        .id   = 70,
        .name = "A#4",
        .step = {
            .pint  = 39,
            .pfrac = 51051,
        },
    },
    {
        .id   = 71,
        .name = "B4",
        .step = {
            .pint  = 42,
            .pfrac = 9465,
        },
    },
    {
        .id   = 72,
        .name = "C5",
        .step = {
            .pint  = 44,
            .pfrac = 42642,
        },
    },
    {
        .id   = 73,
        .name = "C#5",
        .step = {
            .pint  = 47,
            .pfrac = 20069,
        },
    },
    {
        .id   = 74,
        .name = "D5",
        .step = {
            .pint  = 50,
            .pfrac = 7787,
        },
    },
    {
        .id   = 75,
        .name = "D#5",
        .step = {
            .pint  = 53,
            .pfrac = 6466,
        },
    },
    {
        .id   = 76,
        .name = "E5",
        .step = {
            .pint  = 56,
            .pfrac = 16833,
        },
    },
    {
        .id   = 77,
        .name = "F5",
        .step = {
            .pint  = 59,
            .pfrac = 39447,
        },
    },
    {
        .id   = 78,
        .name = "F#5",
        .step = {
            .pint  = 63,
            .pfrac = 9556,
        },
    },
    {
        .id   = 79,
        .name = "G5",
        .step = {
            .pint  = 66,
            .pfrac = 59013,
        },
    },
    {
        .id   = 80,
        .name = "G#5",
        .step = {
            .pint  = 70,
            .pfrac = 57587,
        },
    },
    {
        .id   = 81,
        .name = "A5",
        .step = {
            .pint  = 75,
            .pfrac = 6116,
        },
    },
    {
        .id   = 82,
        .name = "A#5",
        .step = {
            .pint  = 79,
            .pfrac = 36623,
        },
    },
    {
        .id   = 83,
        .name = "B5",
        .step = {
            .pint  = 84,
            .pfrac = 18986,
        },
    },
    {
        .id   = 84,
        .name = "C6",
        .step = {
            .pint  = 89,
            .pfrac = 19748,
        },
    },
    {
        .id   = 85,
        .name = "C#6",
        .step = {
            .pint  = 94,
            .pfrac = 40083,
        },
    },
    {
        .id   = 86,
        .name = "D6",
        .step = {
            .pint  = 100,
            .pfrac = 15574,
        },
    },
    {
        .id   = 87,
        .name = "D#6",
        .step = {
            .pint  = 106,
            .pfrac = 12988,
        },
    },
    {
        .id   = 88,
        .name = "E6",
        .step = {
            .pint  = 112,
            .pfrac = 33610,
        },
    },
    {
        .id   = 89,
        .name = "F6",
        .step = {
            .pint  = 119,
            .pfrac = 13302,
        },
    },
    {
        .id   = 90,
        .name = "F#6",
        .step = {
            .pint  = 126,
            .pfrac = 19112,
        },
    },
    {
        .id   = 91,
        .name = "G6",
        .step = {
            .pint  = 133,
            .pfrac = 52491,
        },
    },
    {
        .id   = 92,
        .name = "G#6",
        .step = {
            .pint  = 141,
            .pfrac = 49639,
        },
    },
    {
        .id   = 93,
        .name = "A6",
        .step = {
            .pint  = 150,
            .pfrac = 12233,
        },
    },
    {
        .id   = 94,
        .name = "A#6",
        .step = {
            .pint  = 159,
            .pfrac = 7710,
        },
    },
    {
        .id   = 95,
        .name = "B6",
        .step = {
            .pint  = 168,
            .pfrac = 37916,
        },
    },
    {
        .id   = 96,
        .name = "C7",
        .step = {
            .pint  = 178,
            .pfrac = 39496,
        },
    },
    {
        .id   = 97,
        .name = "C#7",
        .step = {
            .pint  = 189,
            .pfrac = 14631,
        },
    },
    {
        .id   = 98,
        .name = "D7",
        .step = {
            .pint  = 200,
            .pfrac = 31149,
        },
    },
    {
        .id   = 99,
        .name = "D#7",
        .step = {
            .pint  = 212,
            .pfrac = 25976,
        },
    },
    {
        .id   = 100,
        .name = "E7",
        .step = {
            .pint  = 225,
            .pfrac = 1684,
        },
    },
    {
        .id   = 101,
        .name = "F7",
        .step = {
            .pint  = 238,
            .pfrac = 26661,
        },
    },
    {
        .id   = 102,
        .name = "F#7",
        .step = {
            .pint  = 252,
            .pfrac = 38224,
        },
    },
    {
        .id   = 103,
        .name = "G7",
        .step = {
            .pint  = 267,
            .pfrac = 39447,
        },
    },
    {
        .id   = 104,
        .name = "G#7",
        .step = {
            .pint  = 283,
            .pfrac = 33743,
        },
    },
    {
        .id   = 105,
        .name = "A7",
        .step = {
            .pint  = 300,
            .pfrac = 24466,
        },
    },
    {
        .id   = 106,
        .name = "A#7",
        .step = {
            .pint  = 318,
            .pfrac = 15365,
        },
    },
    {
        .id   = 107,
        .name = "B7",
        .step = {
            .pint  = 337,
            .pfrac = 10352,
        },
    },
    {
        .id   = 108,
        .name = "C8",
        .step = {
            .pint  = 357,
            .pfrac = 13512,
        },
    },
    {
        .id   = 109,
        .name = "C#8",
        .step = {
            .pint  = 378,
            .pfrac = 29262,
        },
    },
    {
        .id   = 110,
        .name = "D8",
        .step = {
            .pint  = 400,
            .pfrac = 62299,
        },
    },
    {
        .id   = 111,
        .name = "D#8",
        .step = {
            .pint  = 424,
            .pfrac = 51897,
        },
    },
    {
        .id   = 112,
        .name = "E8",
        .step = {
            .pint  = 450,
            .pfrac = 3369,
        },
    },
    {
        .id   = 113,
        .name = "F8",
        .step = {
            .pint  = 476,
            .pfrac = 53267,
        },
    },
    {
        .id   = 114,
        .name = "F#8",
        .step = {
            .pint  = 505,
            .pfrac = 10856,
        },
    },
    {
        .id   = 115,
        .name = "G8",
        .step = {
            .pint  = 535,
            .pfrac = 13414,
        },
    },
    {
        .id   = 116,
        .name = "G#8",
        .step = {
            .pint  = 567,
            .pfrac = 1950,
        },
    },
    {
        .id   = 117,
        .name = "A8",
        .step = {
            .pint  = 600,
            .pfrac = 48933,
        },
    },
    {
        .id   = 118,
        .name = "A#8",
        .step = {
            .pint  = 636,
            .pfrac = 30730,
        },
    },
    {
        .id   = 119,
        .name = "B8",
        .step = {
            .pint  = 674,
            .pfrac = 20649,
        },
    },
    {
        .id   = 120,
        .name = "C9",
        .step = {
            .pint  = 714,
            .pfrac = 27025,
        },
    },
    {
        .id   = 121,
        .name = "C#9",
        .step = {
            .pint  = 756,
            .pfrac = 58524,
        },
    },
    {
        .id   = 122,
        .name = "D9",
        .step = {
            .pint  = 801,
            .pfrac = 59006,
        },
    },
    {
        .id   = 123,
        .name = "D#9",
        .step = {
            .pint  = 849,
            .pfrac = 38259,
        },
    },
    {
        .id   = 124,
        .name = "E9",
        .step = {
            .pint  = 900,
            .pfrac = 6738,
        },
    },
    {
        .id   = 125,
        .name = "F9",
        .step = {
            .pint  = 953,
            .pfrac = 40999,
        },
    },
    {
        .id   = 126,
        .name = "F#9",
        .step = {
            .pint  = 1010,
            .pfrac = 21712,
        },
    },
    {
        .id   = 127,
        .name = "G9",
        .step = {
            .pint  = 1070,
            .pfrac = 26773,
        },
    },
};
