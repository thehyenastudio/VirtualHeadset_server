#pragma once
#include <vector>

using byte = unsigned char;

struct FrameBGRA {
    std::vector<byte> bytes;
    int width = 0;
    int height = 0;
    int rowPitch = 0;
    bool empty() const { return bytes.empty(); }
};