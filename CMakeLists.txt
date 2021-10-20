cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

project(archiver)
project(archiver1)
add_executable(archiver
        main.cpp
        BitStream.h BitStream.cpp Coder.cpp Coder.h HaffmanTree.h PriorityQueue.cpp PriorityQueue.h HaffmanTree.cpp Decoder.cpp Decoder.h Haffman.cpp Haffman.h)

project(test)

add_executable(test
        test.cpp
        BitStream.h BitStream.cpp Coder.cpp Coder.h HaffmanTree.h PriorityQueue.cpp PriorityQueue.h HaffmanTree.cpp Decoder.cpp Decoder.h Haffman.cpp Haffman.h)

