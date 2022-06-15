#include <stdint.h>

#include "decl.h"


struct __attribute__ ((__packed__)) sandio_encoded_state {
    uint8_t constant;
    uint8_t dpi;
    uint8_t top;
    uint8_t right;
    uint8_t left;
    uint8_t magic_hi;
    uint8_t magic_lo;
};

unsigned char sandio_decode_button(uint8_t encoded, uint8_t off) {
    uint8_t val = encoded - off;
    return (unsigned char)(val & 0xF0 ? 0 : val);
}

/* buttons state is sent "encrypted"
 * thanks to https://github.com/EssentialNPC/SandioKeyMapper/blob/6bf5069cb02822964217b78aaedca098281208f2/GetRIData.cpp#L454
 */
struct sandio_state sandio_decode(const void *_encoded) {
    const struct sandio_encoded_state *encoded = _encoded;
    
    uint16_t magic = ((uint16_t)encoded->magic_hi) << 8 | encoded->magic_lo;
    uint8_t off = 240 * ((21 * (uint32_t)magic + 7) % 2048) / 2048;

    struct sandio_state decoded = {
        .top   = sandio_decode_button(encoded->top, off),
        .right = sandio_decode_button(encoded->right, off),
        .left  = sandio_decode_button(encoded->left, off),
    };

    return decoded;
}
