#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "base64.h"


void
assertEquals(char *expected, char *actual)
{
    assert(strcmp(expected, actual) == 0);
    free(actual);
}

unsigned char *
ReadFile(char *path, long *length)
{
    unsigned char *buffer;
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = (unsigned char *)malloc(sizeof(unsigned char) * (*length));
    if (buffer) {
        fread(buffer, 1, *length, file);
    }
    fclose(file);
    return buffer;
}

int
main(void)
{
    size_t decode_length;

    assertEquals("Zg==", base64_encode_bytes((unsigned char *)"f", 1));
    assertEquals("Zm8=", base64_encode_bytes((unsigned char *)"fo", 2));
    assertEquals("Zm9v", base64_encode_bytes((unsigned char *)"foo", 3));
    assertEquals("Zm9vYg==", base64_encode_bytes((unsigned char *)"foob", 4));
    assertEquals("Zm9vYmE=", base64_encode_bytes((unsigned char *)"fooba", 5));
    assertEquals("Zm9vYmFy", base64_encode_bytes((unsigned char *)"foobar", 6));

    assertEquals("f", (char *)base64_decode("Zg==", &decode_length));
    assertEquals("fo", (char *)base64_decode("Zm8=", &decode_length));
    assertEquals("foo", (char *)base64_decode("Zm9v", &decode_length));
    assertEquals("foob", (char *)base64_decode("Zm9vYg==", &decode_length));
    assertEquals("fooba", (char *)base64_decode("Zm9vYmE=", &decode_length));
    assertEquals("foobar", (char *)base64_decode("Zm9vYmFy", &decode_length));

    long file_size;
    unsigned char *binary_file = ReadFile("input.png", &file_size);
    char *encoded = base64_encode_bytes(binary_file, file_size);
    unsigned char *decoded  = base64_decode(encoded, &decode_length);;
    
    FILE *wp = fopen("output.png", "wb");
    fwrite(decoded, decode_length, 1, wp);
    fclose(wp);
    assert(system("diff input.png output.png") == 0);
    free(decoded);
    free(encoded);
    
    return 0;
}
