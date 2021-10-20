#pragma once

#include "BitStream.h"
#include "Haffman.h"
#include <memory>

namespace Haffman {
    class HaffmanTree {
    private:
        struct Node;
    public:
        HaffmanTree(Letter letter);

        bool operator<(const HaffmanTree &a) const;

        HaffmanTree();

        HaffmanTree(const HaffmanTree &tree);

        HaffmanTree(const std::vector<HaffmanCode> &characters_cannonical_code);

        static HaffmanTree Merge(HaffmanTree &left, HaffmanTree &right);

        void AddLetter(const Letter &letter, const HaffmanCode &code);

        Letter GetNextLetter(BitStream::Reader &reader) const;

        std::vector<std::pair<uint8_t, Letter>> GetLengthsWithLetters() const;

        void DFS(std::shared_ptr<Node> node, std::vector<std::pair<uint8_t, Letter>> &ret, uint8_t depth = 0) const;

    private:
        struct Node {
            Letter letter = {0};
            std::shared_ptr<Node> left = nullptr;
            std::shared_ptr<Node> right = nullptr;

            Node(Letter letter);

            Node() {};
        };

        std::shared_ptr<Node> root_ = nullptr;
    };

    HaffmanTree GetHaffmanTree(const std::unordered_map<size_t, size_t> &characters_count);
}
