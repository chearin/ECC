#include <stdint.h>
#include <intrin.h>

unsigned long long cpucycles() {
	return __rdtsc();
}