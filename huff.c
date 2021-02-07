// code for a huffman coder


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "huff.h"

// create a new huffcoder structure
struct huffcoder *  huffcoder_new()
{
    struct huffcoder * result = malloc(sizeof(struct huffcoder));
    result->tree = malloc(sizeof(struct huffchar));
    for(int i = 0; i < NUM_CHARS; i++){
        result->codes[i] = malloc(sizeof(char)* NUM_CHARS);
        result->freqs[i] = 0;
        result->code_lengths[i] = 0;
    }
    return result;
}

// count the frequency of characters in a file; set chars with zero
// frequency to one
void huffcoder_count(struct huffcoder * this, char * filename)
{
    FILE * file;
    file = fopen(filename, "r");
    assert( file != NULL );
    unsigned char currentChar = fgetc(file);

    while(!feof(file)){
        this->freqs[currentChar]++;
        currentChar = fgetc(file);
    }

    for(int i = 0; i < NUM_CHARS; i++){
        if(this->freqs[i] == 0)
            this->freqs[i] = 1;
    }

}

//create new compound node
struct huffchar * new_compound(struct huffchar * min1, struct huffchar * min2, int seqno){
    struct huffchar * newCompound = malloc(sizeof(struct huffchar)*1);
    newCompound->is_compound = 1;
    newCompound->freq = min1->freq + min2->freq;
    newCompound->u.compound.left = malloc(sizeof(struct huffchar));
    newCompound->u.compound.right = malloc(sizeof(struct huffchar));
    newCompound->u.compound.left = min1;
    newCompound->u.compound.right = min2;
    newCompound->seqno = seqno;
    return newCompound;
}

// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder * this){
    struct huffchar *list[NUM_CHARS];
    int seqno = 256;

    //setup list of chars
    for(int i = 0; i < NUM_CHARS; i++){
        list[i] = malloc(sizeof(struct huffchar)*1);
        list[i]->freq = this->freqs[i];
        list[i]->is_compound = 0;
        list[i]->seqno = i;
        list[i]->u.c = i;
    }

    int listSize = NUM_CHARS;
    while(listSize > 1) {
        int indexOfSmallest = find_least_freq(list, listSize);
        struct huffchar *min1 = list[indexOfSmallest];
        list[indexOfSmallest] = list[listSize - 1];
        indexOfSmallest = find_least_freq(list, listSize - 1);
        struct huffchar *min2 = list[indexOfSmallest];
        list[indexOfSmallest] = list[listSize - 2];

        //create compound char
        struct huffchar * newCompound = new_compound(min1, min2, seqno);
        seqno++;
        listSize--;
        list[listSize - 1] = newCompound;
        this->tree = list[0];
    }
}

    //find least freq char
int find_least_freq(struct huffchar ** list, int listSize){
    int min = list[0]->freq;
    int minIndex = 0;

    for(int i = 0; i < listSize; i++){
        if(list[i]->freq < min) {
            min = list[i]->freq;
            minIndex = i;
        }
        else if(list[i]->freq == min && list[i]->seqno < list[minIndex]->seqno){
            minIndex = i;
        }
    }

    return minIndex;
}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object
void huffcoder_tree2table(struct huffcoder * this){
    huffcoder_tree2table_(this, this->tree, 0, "");
}

void huffcoder_tree2table_(struct huffcoder * this, struct huffchar * currentNode, int heightOfTree, char* code){
    if(currentNode->is_compound == 1){
        char tempCode[NUM_CHARS];
        strcpy(tempCode, code);
        strcat(tempCode, "0");
        huffcoder_tree2table_(this, currentNode->u.compound.left, heightOfTree+1, tempCode);
        strcpy(tempCode, code);
        strcat(tempCode, "1");
        huffcoder_tree2table_(this, currentNode->u.compound.right, heightOfTree+1, tempCode);
    }
    else{
        this->code_lengths[currentNode->u.c] = heightOfTree;
        strcpy(this->codes[currentNode->u.c], code);
    }
}


// print the Huffman codes for each character in order;
// you should not modify this function
void huffcoder_print_codes(struct huffcoder * this)
{
  for ( int i = 0; i < NUM_CHARS; i++ ) {
    // print the code
    printf("char: %d, freq: %d, code: %s\n", i, this->freqs[i], this->codes[i]);
  }
}

// encode the input file and write the encoding to the output file
void huffcoder_encode(struct huffcoder * this, char * input_filename,
    char * output_filename)
{
    FILE * inputFile;
    inputFile = fopen(input_filename, "r");
    assert(inputFile != NULL);

    FILE * outputFile;
    outputFile = fopen(output_filename, "w");
    assert(outputFile != NULL);

    unsigned char letter = fgetc(inputFile);
    while(!feof(inputFile)) {
        fprintf(outputFile, "%s", this->codes[(int)letter]);
        letter = fgetc(inputFile);
    }

    fprintf(outputFile, "%s", this->codes[(unsigned char)4]);
    fclose(inputFile);
    fclose(outputFile);
    }


// decode the input file and write the decoding to the output file
void huffcoder_decode(struct huffcoder * this, char * input_filename,
    char * output_filename)
{
    FILE * inputFile;
    inputFile = fopen(input_filename, "r");
    assert(inputFile != NULL);

    FILE * outputFile;
    outputFile = fopen(output_filename, "w");
    assert(outputFile != NULL);

    char letterBit;
    struct huffchar * currentNode = this->tree;

    while(!feof(inputFile)){
        currentNode = this->tree;
        while(currentNode->is_compound == 1){
            letterBit = fgetc(inputFile);

            if(letterBit == '0')
                currentNode = currentNode->u.compound.left;
            else
                currentNode = currentNode->u.compound.right;
        }

        fprintf(outputFile, "%c", currentNode->u.c);
    }

    fclose(inputFile);
    fclose(outputFile);
}
