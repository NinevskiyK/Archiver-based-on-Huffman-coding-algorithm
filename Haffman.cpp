#include "Haffman.h"

namespace Haffman {
    bool IntLetterComparator(const std::pair<uint8_t, Letter> &a, std::pair<uint8_t, Letter> &b) {
        if (a.first == b.first) {
            return a.second < b.second;
        }
        return a.first < b.first;
    }


    bool operator<(const Letter &a, const Letter &b) {
        for (size_t i = 0; i < LETTER_SIZE; ++i) {
            if (a[i] != b[i]) {
                if (a[i] < b[i]) {
                    return true;
                } else {
                    return false;
                }
            }
        }
        return false;
    }

    std::vector<bool> FromIntToVectorBool(uint16_t message, uint8_t length) {
        std::vector<bool> ret;
        while (message > 0) {
            ret.push_back(message % 2);
            message /= 2;
        }
        while (length > ret.size()) {
            ret.push_back(0);
        }
        return ret;
    }

    uint16_t FromLetterToInt(const Letter &num, bool rev) {
        uint16_t ret = 0;
        uint16_t pow = 1;
        for (size_t i = 0; i < LETTER_SIZE; ++i) {
            if (!rev) {
                ret += pow * num[i];
            } else {
                ret += pow * num[LETTER_SIZE - i - 1];
            }
            pow *= 2;
        }
        return ret;
    }

    std::vector<HaffmanCode>
    BuildCannonicalCodes(const std::vector<std::pair<uint8_t, Letter>> &characters_length_and_letter, bool rev) {
        std::vector<HaffmanCode> characters_cannonical_code(MAX_EL_IN_9_BITS);
        uint16_t code = 0;
        uint8_t prev_lenght = 1;
        for (const auto&[length, character] : characters_length_and_letter) {
            while (prev_lenght != length) {
                code *= 2;
                ++prev_lenght;
            }
            characters_cannonical_code[FromLetterToInt(character, rev)] = FromIntToHaffmanCode(code, length);
            reverse(characters_cannonical_code[FromLetterToInt(character, rev)].begin(),
                    characters_cannonical_code[FromLetterToInt(character, rev)].end());
            ++code;
        }
        return characters_cannonical_code;
    }

    HaffmanCode FromIntToHaffmanCode(uint16_t num, uint8_t length) {
        HaffmanCode ret;
        while (num > 0) {
            ret.push_back(num % 2);
            num /= 2;
        }
        while (ret.size() < length) {
            ret.push_back(0);
        }
        return ret;
    }

    Letter FromIntToLetter(uint16_t num) {
        Letter ret = {0};
        size_t cur_pos = 0;
        while (num > 0) {
            ret[cur_pos++] = num % 2;
            num /= 2;
        }
        return ret;
    }

    uint16_t FromBitesToInt(const std::vector<bool> &bites, bool rev) {
        uint16_t ret = 0;
        if (rev) {
            for (size_t i = 0; i < bites.size(); ++i) {
                ret *= 2;
                ret += bites[bites.size() - 1 - i];
            }
        } else {
            for (auto i : bites) {
                ret *= 2;
                ret += i;
            }
        }
        return ret;
    }

    std::vector<bool> FromLetterToVectorBool(Letter letter) {
        std::vector<bool> ret;
        for (size_t i = 0; i < LETTER_SIZE; ++i) {
            ret.push_back(letter[i]);
        }
        return ret;
    }
}