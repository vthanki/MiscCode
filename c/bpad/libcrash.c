#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
static void crash2() { volatile int* a = (int*)(NULL); *a = 1; }
static void crash1() { crash2(); }
void crash() { crash1(); }

#ifdef __cplusplus
}
#endif

