#include <openenclave/enclave.h>
#include <stdarg.h>
#include <string.h>

void * __memset_chk(void * dest, int c, size_t len, size_t destlen)
{
    return memset(dest, c, len);
}
