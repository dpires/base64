#ifndef BASE64_H
#define BASE64_H

static char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

int pos(char c);

char *
base64_encode(char *input, int size);

char *
base64_decode(char *input);

#endif
