// code for a huffman coder


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "huff.h"

int main(){
    // create a new huffcoder structure
    struct huffcoder * coder = huffcoder_new();

    // find character frequencies using the training file
    huffcoder_count(coder, "/Users/eli_scorpio/C/huffman/tester.txt");

    // build the Huffman tree
    huffcoder_build_tree(coder);

    // from the Huffman tree fill the table with Huffman codes
    huffcoder_tree2table(coder);

    huffcoder_encode(coder, "/Users/eli_scorpio/C/huffman/tester.txt", "/Users/eli_scorpio/C/huffman/arse.txt");

    huffcoder_decode(coder, "/Users/eli_scorpio/C/huffman/arse.txt", "/Users/eli_scorpio/C/huffman/decoded.txt");

}

