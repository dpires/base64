#ifndef BASE64_H
#define BASE64_H

static char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

int
pos(char c);

char *
base64_encode_bytes(unsigned char *input, size_t size);

unsigned char *
base64_decode(char *input, size_t *decoded_length);

#endif
