#include "BitStream.h"

bool BitStream::Reader::GetBit() {
    if (surplus_lenght_ == BitStream::BITS_IN_BYTE) {
        surplus_lenght_ = 0;
        surplus_ = in_.get();
        if (surplus_ == std::char_traits<wchar_t>::eof()) {
            surplus_ = 0;
            throw std::out_of_range("Reading next byte when nothing to read");
        }
    }
    return ((surplus_ & (1 << (BitStream::BITS_IN_BYTE - 1 - surplus_lenght_++))) != 0);
}

std::vector<bool> BitStream::Reader::GetBits(unsigned char bits_col) {
    std::vector<bool> ret;
    for (unsigned char i = 0; i < bits_col; ++i) {
        bool bit = 0;
        try {
            bit = GetBit();
        }
        catch (...) {
            throw std::out_of_range("Reading file when nothing to read");
        }
        ret.push_back(bit == 1);
    }
    return ret;
}

void BitStream::Reader::Close() {}

BitStream::Reader::Reader(std::istream &in) : in_(in) {}

void BitStream::Writer::WriteBit(bool bit) {
    surplus_ += bit * (1 << (BitStream::BITS_IN_BYTE - 1 - surplus_lenght_));
    ++surplus_lenght_;
    if (surplus_lenght_ == BitStream::BITS_IN_BYTE) {
        surplus_lenght_ = 0;
        out_.put(surplus_);
        surplus_ = 0;
    }
}

void BitStream::Writer::WriteBits(const std::vector<bool> &message, bool rev) {
    if (rev) {
        for (size_t i = 0; i < message.size(); ++i) {
            WriteBit(message[message.size() - i - 1]);
        }
    } else {
        for (auto bit : message) {
            WriteBit(bit);
        }
    }
}

void BitStream::Writer::Close() {
    while (surplus_lenght_ != 0) {
        WriteBit(0);
    }
    surplus_ = 0;
}

BitStream::Writer::Writer(std::ostream &out) : out_(out) {}

BitStream::Writer::Writer(std::ofstream &out) : out_(out) {}
