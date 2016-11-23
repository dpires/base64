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

int
main(void)
{
    assertEquals("Zg==", base64_encode("f", 1));
    assertEquals("Zm8=", base64_encode("fo", 2));
    assertEquals("Zm9v", base64_encode("foo", 3));
    assertEquals("Zm9vYg==", base64_encode("foob", 4));
    assertEquals("Zm9vYmE=", base64_encode("fooba", 5));
    assertEquals("Zm9vYmFy", base64_encode("foobar", 6));

    assertEquals("f", base64_decode("Zg=="));
    assertEquals("fo", base64_decode("Zm8="));
    assertEquals("foo", base64_decode("Zm9v"));
    assertEquals("foob", base64_decode("Zm9vYg=="));
    assertEquals("fooba", base64_decode("Zm9vYmE="));
    assertEquals("foobar", base64_decode("Zm9vYmFy"));

    return 0;
}
