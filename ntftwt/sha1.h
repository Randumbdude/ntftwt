#ifndef SHA1_H
#define SHA1_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdint>

class SHA1 {
public:
    SHA1() { reset(); }

    void update(const std::string& s) {
        for (char c : s) {
            buffer.push_back(static_cast<uint8_t>(c));
        }
    }

    std::string final() {
        uint64_t total_bits = buffer.size() * 8;
        buffer.push_back(0x80);
        while ((buffer.size() * 8) % 512 != 448) {
            buffer.push_back(0x00);
        }
        for (int i = 7; i >= 0; --i) {
            buffer.push_back(static_cast<uint8_t>(total_bits >> (i * 8)));
        }

        for (size_t chunk = 0; chunk < buffer.size(); chunk += 64) {
            uint32_t w[80];
            for (int i = 0; i < 16; ++i) {
                w[i] = (buffer[chunk + i * 4] << 24) | (buffer[chunk + i * 4 + 1] << 16) |
                    (buffer[chunk + i * 4 + 2] << 8) | buffer[chunk + i * 4 + 3];
            }
            for (int i = 16; i < 80; ++i) {
                w[i] = left_rotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
            }

            uint32_t a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];

            for (int i = 0; i < 80; ++i) {
                uint32_t f, k;
                if (i < 20) { f = (b & c) | (~b & d); k = 0x5A827999; }
                else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
                else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
                else { f = b ^ c ^ d; k = 0xCA62C1D6; }

                uint32_t temp = left_rotate(a, 5) + f + e + k + w[i];
                e = d; d = c; c = left_rotate(b, 30); b = a; a = temp;
            }

            state[0] += a; state[1] += b; state[2] += c; state[3] += d; state[4] += e;
        }

        std::stringstream ss;
        for (uint32_t i : state) {
            ss << std::hex << std::setw(8) << std::setfill('0') << i;
        }
        return ss.str();
    }

private:
    uint32_t state[5];
    std::vector<uint8_t> buffer;

    void reset() {
        state[0] = 0x67452301; state[1] = 0xEFCDAB89; state[2] = 0x98BADCFE;
        state[3] = 0x10325476; state[4] = 0xC3D2E1F0;
        buffer.clear();
    }

    uint32_t left_rotate(uint32_t value, size_t bits) {
        return (value << bits) | (value >> (32 - bits));
    }
};

#endif // SHA1_H