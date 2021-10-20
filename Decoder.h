#pragma once

#include <fstream>
#include "Haffman.h"
#include "BitStream.h"
#include "HaffmanTree.h"

namespace Haffman {
    class Decoder {
    public:
        explicit Decoder(std::istream &archive_istream);

        void Decode();

        void Close();

    private:
        BitStream::Reader reader_;
    };
}