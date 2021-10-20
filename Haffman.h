#pragma once

#include <cstddef>
#include <vector>
#include <bitset>
#include "algorithm"

namespace Haffman {
    const size_t LETTER_SIZE = 9;
    const size_t BITES_IN_BYTE = 8;
    const size_t MAX_EL_IN_9_BITS = 512;

    const uint16_t FILENAME_END = 256;
    const uint16_t ONE_MORE_FILE = 257;
    const uint16_t ARCHIVE_END = 258;


    using HaffmanCode = std::vector<bool>;
    using Letter = std::bitset<LETTER_SIZE>;

    bool operator<(const Letter &a, const Letter &b);

    bool IntLetterComparator(const std::pair<uint8_t, Letter> &a, std::pair<uint8_t, Letter> &b);

    uint16_t FromBitesToInt(const std::vector<bool> &bites, bool rev = false);

    std::vector<bool> FromIntToVectorBool(uint16_t message, uint8_t length);

    std::vector<bool> FromLetterToVectorBool(Letter letter);

    uint16_t FromLetterToInt(const Letter &num, bool rev = false);

    Letter FromIntToLetter(uint16_t num);

    HaffmanCode FromIntToHaffmanCode(uint16_t num, uint8_t length);

    std::vector<HaffmanCode>
    BuildCannonicalCodes(const std::vector<std::pair<uint8_t, Letter>> &characters_length_and_letter, bool rev = false);
}