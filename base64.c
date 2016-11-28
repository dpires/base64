#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "base64.h"


/*
output will always be 4 bytes
encoding:
    for each 3 byte block (3*8bits = 24bits):
        split block into 4 6bit blocks
        ( << shift each byte 8 bits AND(|) next 8bits) create 6bit mask an AND
        for each 6bit block, get base64((int)6bit)):

            if (block < 3 bytes, pad block with 0 (= character)
                    if 2 bytes missing, last 2 bytes will be ==
                    if 1 byte missing, last byte will be =

input  h | e | l | l | o
       ---------------------------------------------------------------------
ASCII  104 | 101 | 108 | 108 | 111 | 0
       ---------------------------------------------------------------------
BIN    01101000 | 01100101 | 01101100 | 01101100 | 01101111 | 00000000
       ---------------------------------------------------------------------
       011010 | 000110 | 010101 | 101100 | 011011 | 000110 | 111100 | 000000
       ---------------------------------------------------------------------
DEC    26     | 6      | 21     | 44     | 27     | 6      | 60     | 0
       ---------------------------------------------------------------------
Base64 a      | G      | V      | s      | b      | G      | 8      | =

*/


char *
base64_encode_bytes(unsigned char *input, size_t size)
{
    int len = 4 * size / 3;
    while (len < 4 || len % 4 !=0) {
        len++;
    }
    char *output = calloc(len + 1, sizeof(char));
    int block_size = 24;
    int i;
    int p;
    int block;
    unsigned int last_block;

    for (p=0,i=0; i+3<=size;i+=3,p+=3) {
        block = (((input[i] << 8) | input[i+1])  << 8) | input[i+2];
        output[p] = base64[((block >> (block_size - 6))) & 0x3F];
        output[p+1] = base64[((block >> (block_size - 12))) & 0x3F];
        output[p+2] = base64[((block >> (block_size - 18))) & 0x3F];
        output[p+3] = base64[((block >> (block_size - 24))) & 0x3F];
        p++;
    }

    int remaining = 3 - (size -i);

    if (remaining == 1) {
        last_block  = (((input[i] << 8) | input[i+1])  << 8) | 0;
        output[p] = base64[((last_block >> (block_size - 6))) & 0x3F];
        output[p+1] = base64[((last_block >> (block_size - 12))) & 0x3F];
        output[p+2] = base64[((last_block >> (block_size - 18))) & 0x3F];
        output[p+3] = '=';
    } else if (remaining ==2){
        last_block = (((input[i] << 8) | 0)  << 8) | 0;
        output[p] = base64[((last_block >> (block_size - 6))) & 0x3F];
        output[p+1] = base64[((last_block >> (block_size - 12))) & 0x3F];
        output[p+2] = '=';
        output[p+3] = '=';
    }

    output[len] = '\0';

    return output;
}

int
pos(char c)
{
    char *p;
    for(p = base64; *p; p++) {
        if (*p == c) {
            return p - base64;
        }
    }
    return -1;
}

unsigned char *
base64_decode(char *input, size_t *decoded_length)
{
    size_t length = strlen(input);
    size_t paddingIndex;
    size_t size;

    char *firstPadding = strchr(input, '=');

    if (firstPadding != NULL) {
        paddingIndex = (size_t)(firstPadding - input);
        paddingIndex = length - paddingIndex;
        size = ((length*3)/4) - paddingIndex;
    } else {
        size = ((length*3)/4);
    }

    *decoded_length = size;

    int bitBlock;
    int bytes[4];
    int i = 0;
    int j = 0;
    unsigned char *decoded = calloc(size, sizeof(char));

    for (i=i; i<length; i+=4) {
        bytes[0] = pos(input[i]);
        bytes[1] = pos(input[i+1]);
        bytes[2] = pos(input[i+2]);
        bytes[3] = pos(input[i+3]);


        unsigned int block24 = bytes[0]  << 6 | bytes[1];

        block24  = block24 << 6 | bytes[2];
        block24  = block24 << 6 | bytes[3];

        decoded[j++] = ((block24 >> 16) & 0xFF);

        if (bytes[2] < 64) {
            decoded[j++] = ((block24 >> 8) & 0xFF);
        }

        if (bytes[3] < 64) {
            decoded[j++] = ((block24) & 0xFF);
        }
    }

    return decoded;
}
