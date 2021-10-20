#include "Coder.h"


namespace Haffman {
    void Coder::Close() {
        writer_.Close();
    }

    HaffmanTree GetHaffmanTree(const std::unordered_map<size_t, size_t> &characters_count) {
        PriorityQueue queue;
        for (const auto&[i, character_count]: characters_count) {
            queue.Push({character_count, HaffmanTree(FromIntToLetter(i))});
        }
        while (queue.Size() > 1) {
            auto smallest_node = queue.Top();
            queue.Pop();
            auto smallest_node_1 = queue.Top();
            queue.Pop();
            queue.Push({smallest_node.first + smallest_node_1.first,
                        HaffmanTree::Merge(smallest_node.second, smallest_node_1.second)});
        }
        return queue.Top().second;
    }

    std::pair<uint16_t, std::unordered_map<size_t, size_t>> Coder::CountFrequency(const std::string &file_name) {
        std::unordered_map<size_t, size_t> ret;
        auto in = std::ifstream(file_name, std::ios::binary);
        if (!in.good()) {
            throw std::invalid_argument("Cannot get access to file: " + file_name);
        }
        BitStream::Reader reader(in);
        uint16_t col = 0;
        try {
            while (true) {
                std::vector<bool> ch = reader.GetBits(BITES_IN_BYTE);
                ++ret[FromBitesToInt(ch)];
                if (ret[FromBitesToInt(ch)] == 1) {
                    ++col;
                }
            }
        } catch (...) {

        }
        for (unsigned char c : file_name) {
            ++ret[c];
            if (ret[c] == 1) {
                ++col;
            }
        }
        ++ret[FILENAME_END];
        ++ret[ONE_MORE_FILE];
        ++ret[ARCHIVE_END];
        return {col + 3, ret};
    }

    Coder::Coder(std::ostream &archive_stream) : writer_(archive_stream) {}

    void Coder::Encode(const std::vector<std::string> &file_names) {
        for (size_t i = 0; i < file_names.size(); ++i) {
            std::string file_name = file_names[i];
            try {
                std::unordered_map<size_t, size_t> characters_count;
                uint16_t number_characters = 0;
                auto ret = CountFrequency(file_name);
                number_characters = ret.first;
                characters_count = ret.second;
                auto root = GetHaffmanTree(characters_count);
                std::vector<std::pair<uint8_t, Letter>> characters_length_and_letter = root.GetLengthsWithLetters();
                std::sort(characters_length_and_letter.begin(), characters_length_and_letter.end(),
                          IntLetterComparator);
                std::vector<HaffmanCode> characters_cannonical_code = BuildCannonicalCodes(
                        characters_length_and_letter);
                WriteInfo(number_characters, characters_length_and_letter);
                for (unsigned char c : file_name) {
                    writer_.WriteBits(characters_cannonical_code[c]);
                }
                writer_.WriteBits(characters_cannonical_code[FILENAME_END]);
                WriteFile(file_name, characters_cannonical_code);
                if (i + 1 == file_names.size()) {
                    writer_.WriteBits(characters_cannonical_code[ARCHIVE_END]);
                } else {
                    writer_.WriteBits(characters_cannonical_code[ONE_MORE_FILE]);
                }
            } catch (const std::exception &e) {
                throw std::runtime_error("Problems while archiving " + file_name + " : " + e.what());
            }
        }
    }


    void
    Coder::WriteInfo(uint16_t number_characters, std::vector<std::pair<uint8_t, Letter>> characters_length_and_letter) {
        writer_.WriteBits(FromIntToVectorBool(number_characters, 9));
        std::unordered_map<size_t, size_t> length_count;
        unsigned char max_symbol_code_size = 0;
        for (const auto &[length, character] : characters_length_and_letter) {
            writer_.WriteBits(FromLetterToVectorBool(character));
            ++length_count[length];
            max_symbol_code_size = std::max(max_symbol_code_size, length);
        }
        for (int i = 1; i <= max_symbol_code_size; ++i) {
            writer_.WriteBits(FromIntToVectorBool(length_count[i], 9));
        }
    }

    void Coder::WriteFile(const std::string &file_name, std::vector<HaffmanCode> characters_cannonical_code) {
        auto in = std::ifstream(file_name, std::ios::binary);
        if (!in.good()) {
            throw std::invalid_argument("Cannot get access to file: " + file_name);
        }
        BitStream::Reader reader(in);
        try {
            while (true) {
                writer_.WriteBits(characters_cannonical_code[FromBitesToInt(reader.GetBits(8))]);
            }
        } catch (...) {
        }
        reader.Close();
    }
}