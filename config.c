#include <linux/input.h>

#include "decl.h"


#define TR 0x01
#define TF 0x02
#define TL 0x04
#define TB 0x08
#define RT (0x01 << 8)
#define RD (0x02 << 8)
#define RF (0x04 << 8)
#define RB (0x08 << 8)
#define LF (0x01 << 16)
#define LD (0x02 << 16)
#define LB (0x04 << 16)
#define LU (0x08 << 16)

const struct rule *rules = (struct rule[]){
    {TF, KEY_W},
    {TB, KEY_S},
    {TL, KEY_A},
    {TR, KEY_D},
    {RT, KEY_SPACE},
    {RD, KEY_C},
    {RF, KEY_B},
    {RB, KEY_Z},
    {LF, KEY_E},
    {LB, KEY_R},
    {LD, KEY_F},
    {LU, KEY_Q},
    {0, 0}
};
