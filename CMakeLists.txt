cmake_minimum_required(VERSION 3.17)
project(huffman C)

set(CMAKE_C_STANDARD 11)

add_executable(huffman huff.c huff.h main.c tester.txt)