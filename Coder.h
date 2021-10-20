#pragma once

#include "BitStream.h"
#include "Haffman.h"
#include "HaffmanTree.h"
#include "PriorityQueue.h"
#include "Haffman.h"
#include <algorithm>

namespace Haffman {

    class Coder {
    public:
        explicit Coder(std::ostream &archive_stream);

        void Encode(const std::vector<std::string> &file_names);

        void Close();


    private:
        std::pair<uint16_t, std::unordered_map<size_t, size_t>> CountFrequency(const std::string &file_name);

        void WriteInfo(uint16_t number_characters,
                       std::vector<std::pair<uint8_t, Letter>> characters_length_and_letter);

        void WriteFile(const std::string &file_name, std::vector<HaffmanCode> characters_cannonical_code);

        BitStream::Writer writer_;
    };

}