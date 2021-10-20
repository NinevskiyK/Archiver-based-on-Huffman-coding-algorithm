#include <exception>
#include <iostream>
#include "Coder.h"
#include "Decoder.h"

void PrintInformation() {
    std::cout
            << "To archive filename1, filename2, ... to archive_name write: \"archiver -c archive_name filename1 [filename2 ...]\""
            << std::endl;
    std::cout << "To decode archive_name, write: \"archiver -d archive_name\"" << std::endl;
    std::cout << "To get the information about using the program, write \"archiver -h\"" << std::endl;
}

enum Status {
    Encode,
    Decode,
    Help,
    Undefined
};

void MakeArchive(const std::string& archive_name, const std::vector<std::string> &file_names) {
    auto archive_stream = std::ofstream(archive_name, std::ios::binary);
    Haffman::Coder coder(archive_stream);
    coder.Encode(file_names);
    coder.Close();
    archive_stream.close();
}

void Unarchive(const std::string &archive_name) {
    auto archive_stream = std::ifstream(archive_name, std::ios::binary);
    Haffman::Decoder decoder(archive_stream);
    decoder.Decode();
    decoder.Close();
    archive_stream.close();
}


int main(int argc, const char *const argv[]) {
    Status status = Status::Undefined;
    if (argc != 0) {
        std::string first_arg = argv[1];
        if (first_arg == "-c") {
            if (argc > 2) {
                status = Encode;
            }
        } else if (first_arg == "-d") {
            if (argc == 3) {
                status = Decode;
            }
        } else if (first_arg == "-h") {
            if (argc == 1) {
                status = Help;
            }
        }
    }
    if (status == Encode) {
        std::vector<std::string> file_names;
        for (size_t i = 3; i < argc; ++i) {
            file_names.push_back(argv[i]);
        }
        try {
            MakeArchive(argv[2], file_names);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    } else if (status == Decode) {
        try {
            Unarchive(argv[2]);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    } else if (status == Help) {
        PrintInformation();
    } else {
        std::cout << "Incorrect format. Please write \"archiver -h\" to get information about using this program";
        return 1;
    }
    return 0;
}
