#include "HaffmanTree.h"
#include "PriorityQueue.h"
#include <iostream>

namespace Haffman {
    bool HaffmanTree::operator<(const HaffmanTree &a) const {
        return this->root_->letter < a.root_->letter;
    }

    HaffmanTree::HaffmanTree(Letter letter) {
        root_ = std::make_shared<Node>(letter);
    }

    HaffmanTree::HaffmanTree() {
        root_ = std::make_shared<Node>();
    }

    HaffmanTree HaffmanTree::Merge(HaffmanTree &left, HaffmanTree &right) {
        HaffmanTree ret = HaffmanTree();
        ret.root_->left = std::move(left.root_);
        ret.root_->right = std::move(right.root_);
        return ret;
    }

    std::vector<std::pair<uint8_t, Letter>> HaffmanTree::GetLengthsWithLetters() const {
        std::vector<std::pair<uint8_t, Letter>> ret;
        HaffmanTree::DFS(root_, ret, 0);
        return ret;
    }

    HaffmanTree::HaffmanTree(const std::vector<HaffmanCode> &characters_cannonical_code) {
        root_ = std::make_shared<Node>();
        for (uint16_t i = 0; i < MAX_EL_IN_9_BITS; ++i) {
            if (!characters_cannonical_code[i].empty()) {
                AddLetter(FromIntToLetter(i), characters_cannonical_code[i]);
            }
        }
    }


    void
    HaffmanTree::DFS(std::shared_ptr<Node> node, std::vector<std::pair<uint8_t, Letter>> &ret, uint8_t depth) const {
        if (node->left == nullptr) {
            ret.push_back({depth, node->letter});
            return;
        }
        DFS(node->left, ret, depth + 1);
        DFS(node->right, ret, depth + 1);
    }

    Letter HaffmanTree::GetNextLetter(BitStream::Reader &reader) const {
        std::shared_ptr<Node> now = root_;
        while (now->left != nullptr) {
            if (reader.GetBits(1)[0]) {
                now = now->right;
            } else {
                now = now->left;
            }
            if (now == nullptr) {
                throw std::exception();
            }
        }
        return now->letter;
    }

    void HaffmanTree::AddLetter(const Letter &letter, const HaffmanCode &code) {
        auto now = root_;
        for (const auto &i : code) {
            if (i) {
                if (now->right == nullptr) {
                    now->right = std::make_shared<Node>();
                }
                now = now->right;
            } else {
                if (now->left == nullptr) {
                    now->left = std::make_shared<Node>();
                }
                now = now->left;
            }
        }
        now->letter = letter;
    }

    HaffmanTree::HaffmanTree(const HaffmanTree &tree) {
        root_ = tree.root_;
    }


    HaffmanTree::Node::Node(Letter letter) : letter(letter) {}
}
