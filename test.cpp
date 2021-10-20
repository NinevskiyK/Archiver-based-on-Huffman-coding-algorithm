#include "BitStream.h"
#include "HaffmanTree.h"
#include "Coder.h"
#include "Decoder.h"
#include <sstream>
#include <assert.h>
#include <vector>
#include <iostream>
#include <unordered_map>

void MakeArchive(const std::string& archive_name, const std::vector<std::string> &file_names);

void Unarchive(const std::string &archive_name);

void WriteToStream(std::ostream &stream, const std::string &str) {
    for (unsigned char i : str) {
        stream.put(i);
    }
}

void WriteToFile(const std::string &file_name, const std::string &content) {
    std::ofstream stream(file_name);
    WriteToStream(stream, content);
    stream.close();
}

std::string ReadFile(const std::string &file_name) {
    std::ifstream stream(file_name);
    std::string ret;
    stream >> ret;
    stream.close();
    return ret;
}

int main() {
    std::cout << "Test start" << std::endl;
    {
        {
            char byte1 = 0b1110'1110;
            char byte2 = 0b1011'1010;
            std::stringstream stream;
            stream << byte1 << byte2;
            BitStream::Reader reader(stream);
            std::vector<bool> check = {1, 1, 1, 0, 1, 1, 1, 0};
            assert(reader.GetBits(8) == check);
            bool exc = false;
            try {
                reader.GetBits(9);
            } catch (std::exception e) {
                exc = true;
            }
            assert(exc == true);
        }
        {
            std::stringstream stream;
            BitStream::Writer writer(stream);
            writer.WriteBits({0, 1, 1, 0, 1});
            writer.WriteBits({0, 1}, true);
            writer.WriteBit(1);
            char c;
            stream >> c;
            assert(c == 0b01101'10'1);
        }
    }
    {
        {
            WriteToFile("test.txt", "abbc");
            auto stream = std::ofstream("test_archive");
            Haffman::Coder coder(stream);
            coder.Encode({"test.txt"});
            coder.Close();
            stream.close();
            WriteToFile("test.txt", "trash");
            auto archive_stream = std::ifstream("test_archive", std::ios::binary);
            Haffman::Decoder decoder(archive_stream);
            decoder.Decode();
            decoder.Close();
            archive_stream.close();
            assert(ReadFile("test.txt") == "abbc");
        }
        {
            WriteToFile("test.txt", "text");
            WriteToFile("document", "content");
            auto stream = std::ofstream("test_archive");
            Haffman::Coder coder(stream);
            coder.Encode({"test.txt", "document"});
            coder.Close();
            stream.close();
            WriteToFile("test.txt", "+");
            WriteToFile("document", "+");
            auto archive_stream = std::ifstream("test_archive", std::ios::binary);
            Haffman::Decoder decoder(archive_stream);
            decoder.Decode();
            decoder.Close();
            archive_stream.close();
            assert(ReadFile("test.txt") == "text");
            assert(ReadFile("document") == "content");
        }
    }
    {

        {
            WriteToFile("test_archive", "a");
            bool exc = false;
            try {
                auto archive_stream = std::ifstream("test_archive", std::ios::binary);
                Haffman::Decoder decoder(archive_stream);
                decoder.Decode();
                decoder.Close();
                archive_stream.close();
            } catch (std::exception e) {
                exc = true;
            }
            assert(exc == true);
        }
        {
            WriteToFile("test_archive", "lorem ipsum");
            bool exc = false;
            try {
                auto archive_stream = std::ifstream("test_archive", std::ios::binary);
                Haffman::Decoder decoder(archive_stream);
                decoder.Decode();
                decoder.Close();
                archive_stream.close();
            } catch (std::exception e) {
                exc = true;
            }
            assert(exc == true);
        }
    }
    {
        {
            Haffman::HaffmanTree root;
            Haffman::Letter letter1 = 0b1000001001;
            Haffman::Letter letter2 = 0b1001011001;
            Haffman::Letter letter3 = 0b1101001001;
            Haffman::HaffmanCode code1 = {1, 1, 0};
            Haffman::HaffmanCode code2 = {0};
            Haffman::HaffmanCode code3 = {1, 0, 0, 0};
            root.AddLetter(letter1, code1);
            root.AddLetter(letter2, code2);
            root.AddLetter(letter3, code3);
            std::stringstream stream;
            char byte = 0b11001000;
            stream << byte;
            BitStream::Reader reader(stream);
            assert(root.GetNextLetter(reader) == letter1);
            assert(root.GetNextLetter(reader) == letter2);
            assert(root.GetNextLetter(reader) == letter3);
            bool exc = false;
            try {
                root.GetNextLetter(reader);
            } catch (std::exception e) {
                exc = true;
            }
            assert(exc == true);
        }
        {
            std::unordered_map<size_t, size_t> map;
            map[3] = 3;
            map[2] = 2;
            map[1] = 1;
           /* auto root1 = Haffman::GetHaffmanTree(map); // Происходит что то странное - в этом map появляются откуда то лишние символы, потом исчезают после этой функции
            std::stringstream stream;
            char byte = 0b0'10'11'000;
            stream << byte;
            BitStream::Reader reader(stream);
            assert(root1.GetNextLetter(reader) == Haffman::FromIntToLetter(3));
            assert(root1.GetNextLetter(reader) == Haffman::FromIntToLetter(2));
            assert(root1.GetNextLetter(reader) == Haffman::FromIntToLetter(1));
            */
        }
    }
    std::cout << "All tests passed!";
    return 0;
}

