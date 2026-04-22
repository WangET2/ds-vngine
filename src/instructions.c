/* C code produced by gperf version 3.0.3 */
/* Command-line: /Library/Developer/CommandLineTools/usr/bin/gperf -C -E -m -f --output-file=src/instructions.c src/instructions.gperf  */
/* Computed positions: -k'1,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "src/instructions.gperf"

#include "parser.h"
#include <string.h>
#include <stdbool.h>
#line 9 "src/instructions.gperf"
struct InstructionKeyword {
    const char *name;
    CommandType type;
    int num_args;
    bool consumes_rest;
};
/* maximum key range = 29, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned char asso_values[] =
    {
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31,  0, 10, 10, 10,
       5,  0,  5,  0, 31, 31, 15,  5,  0, 31,
       0, 31,  0,  0, 10, 15, 31,  0, 15,  0,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31
    };
  return len + asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]];
}

const struct InstructionKeyword *
in_word_set (str, len)
     register const char *str;
     register unsigned int len;
{
  enum
    {
      TOTAL_KEYWORDS = 25,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 12,
      MIN_HASH_VALUE = 2,
      MAX_HASH_VALUE = 30
    };

  static const struct InstructionKeyword wordlist[] =
    {
      {""}, {""},
#line 17 "src/instructions.gperf"
      {"BG", CMD_BG, 1, false},
#line 31 "src/instructions.gperf"
      {"SAY", CMD_SAY, 2, true},
#line 40 "src/instructions.gperf"
      {"PASS", CMD_PASS, 0, false},
#line 18 "src/instructions.gperf"
      {"BGSUB", CMD_BG_SUB, 1, false},
      {""},
#line 29 "src/instructions.gperf"
      {"IF", CMD_IF, 2, true},
#line 35 "src/instructions.gperf"
      {"BGM", CMD_BGM, 1, false},
#line 27 "src/instructions.gperf"
      {"FLAG", CMD_FLAG, 1, false},
      {""},
#line 23 "src/instructions.gperf"
      {"SHOW_CENTER", CMD_SHOW_CENTER, 2, false},
#line 19 "src/instructions.gperf"
      {"HIDE_BG", CMD_HIDE_BG, 0, false},
#line 33 "src/instructions.gperf"
      {"HIDE_SAY", CMD_HIDE_SAY, 0, false},
#line 38 "src/instructions.gperf"
      {"WAIT", CMD_WAIT, 0, false},
#line 20 "src/instructions.gperf"
      {"HIDE_BGSUB", CMD_HIDE_BG_SUB, 0, false},
#line 26 "src/instructions.gperf"
      {"HIDE_CENTER", CMD_HIDE_CENTER, 0, false},
#line 32 "src/instructions.gperf"
      {"NARRATE", CMD_NARRATE, 1, true},
#line 36 "src/instructions.gperf"
      {"SFX", CMD_SFX, 1, false},
#line 21 "src/instructions.gperf"
      {"SHOW_LEFT", CMD_SHOW_LEFT, 2, false},
#line 22 "src/instructions.gperf"
      {"SHOW_RIGHT", CMD_SHOW_RIGHT, 2, false},
#line 37 "src/instructions.gperf"
      {"ENDBGM", CMD_ENDBGM, 0, false},
      {""},
#line 41 "src/instructions.gperf"
      {"END", CMD_END, 0, false},
#line 24 "src/instructions.gperf"
      {"HIDE_LEFT", CMD_HIDE_LEFT, 0, false},
#line 25 "src/instructions.gperf"
      {"HIDE_RIGHT", CMD_HIDE_RIGHT, 0, false},
#line 30 "src/instructions.gperf"
      {"CHOICE", CMD_CHOICE, -1, false},
#line 34 "src/instructions.gperf"
      {"HIDE_NARRATE", CMD_HIDE_NARRATE, 0, false},
      {""},
#line 39 "src/instructions.gperf"
      {"LOAD", CMD_LOAD, 1, false},
#line 28 "src/instructions.gperf"
      {"UNSET", CMD_UNSET, 1, false}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
