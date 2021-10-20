#pragma once

#include <cstdint>
#include <fstream>
#include <vector>
#include <algorithm>


namespace BitStream {
    const unsigned char BITS_IN_BYTE = 8;

    class Reader {
    public:
        explicit Reader(std::istream &in);

        std::vector<bool> GetBits(unsigned char bits_col);

        void Close();

    private:
        bool GetBit();

        std::istream &in_;
        uint16_t surplus_ = 0;
        unsigned char surplus_lenght_ = 8;
    };

    class Writer {
    public:
        explicit Writer(std::ostream &out);

        explicit Writer(std::ofstream &out);

        void WriteBit(bool bit);

        void WriteBits(const std::vector<bool> &message, bool rev = false);

        void Close();

    private:
        std::ostream &out_;
        unsigned char surplus_ = 0;
        unsigned char surplus_lenght_ = 0;
    };

}