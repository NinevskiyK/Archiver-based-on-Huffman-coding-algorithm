#include "Decoder.h"
#include <fstream>

using namespace std::literals;
namespace Haffman {
    Decoder::Decoder(std::istream &archive_istream) : reader_(archive_istream) {
        if (!archive_istream.good()) {
            throw std::invalid_argument("Cannot get access to archive");
        }
    }

    std::vector<uint16_t> GetCharacters(BitStream::Reader &reader, uint16_t symbols_count) {
        std::vector<uint16_t> ret(symbols_count);
        for (size_t i = 0; i < symbols_count; ++i) {
            try {
                ret[i] = FromBitesToInt(reader.GetBits(9), true);
            } catch (const std::exception &e) {
                throw std::runtime_error("Problems while decoding:  "s + e.what());
            }
        }
        return ret;
    }

    std::vector<std::pair<uint8_t, Letter>>
    GetCharactersLengthAndLetter(BitStream::Reader &reader, uint16_t symbols_count,
                                 const std::vector<uint16_t> &characters) {
        std::vector<std::pair<uint8_t, Letter>> characters_length_and_letter = {};
        uint16_t cur_lenght_count = 0;
        uint8_t cur_length = 1;
        while (cur_lenght_count != symbols_count) {
            if (cur_lenght_count > symbols_count) {
                throw std::exception();
            }
            uint16_t col = FromBitesToInt(reader.GetBits(9), true);
            for (size_t i = cur_lenght_count; i < cur_lenght_count + col; ++i) {
                characters_length_and_letter.push_back({cur_length, characters[i]});
            }
            cur_lenght_count += col;
            ++cur_length;
        }
        return characters_length_and_letter;
    }

    std::string GetFileName(BitStream::Reader &reader, Haffman::HaffmanTree tree) {
        std::string ret = "";
        while (true) {
            auto letter = tree.GetNextLetter(reader);
            uint16_t letter_int = FromLetterToInt(letter, true);
            if (letter_int == FILENAME_END) {
                break;
            }
            ret += static_cast<unsigned char>(letter_int);
        }
        return ret;
    }

    void DecodeFile(BitStream::Reader &reader, const std::string &file_name, bool &is_ended,
                    HaffmanTree &haffman_tree) {
        std::ofstream file_ofstream(file_name, std::ios::binary);
        BitStream::Writer writer(file_ofstream);
        while (true) {
            auto letter = haffman_tree.GetNextLetter(reader);
            uint16_t letter_int = FromLetterToInt(letter, true);
            if (letter_int == ARCHIVE_END) {
                is_ended = true;
                break;
            } else if (letter_int == ONE_MORE_FILE) {
                is_ended = false;
                break;
            }
            auto write = FromLetterToVectorBool(letter);
            reverse(write.begin(), write.end());
            write.pop_back();
            writer.WriteBits(write, true);
        }
        writer.Close();
    }

    void Decoder::Decode() {

        bool is_ended = false;
        try {
            while (!is_ended) {
                uint16_t symbols_count = FromBitesToInt(reader_.GetBits(9), true);
                std::vector<uint16_t> characters = GetCharacters(reader_, symbols_count);
                std::vector<std::pair<uint8_t, Letter>> characters_length_and_letter = GetCharactersLengthAndLetter(
                        reader_,
                        symbols_count,
                        characters);
                std::vector<HaffmanCode> characters_cannonical_code = BuildCannonicalCodes(characters_length_and_letter,
                                                                                           true);
                HaffmanTree haffman_tree = HaffmanTree(characters_cannonical_code);
                std::string file_name = GetFileName(reader_, haffman_tree);
                DecodeFile(reader_, file_name, is_ended, haffman_tree);
            }
        } catch (const std::exception &e) {
            throw std::runtime_error("Problems while decoding:  "s + e.what());
        }
    }

    void Decoder::Close() {
    }
}
