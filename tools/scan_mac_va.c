// Prints which 4 GB slots of 0x70_0000_0000..0x80_0000_0000 can be reserved on this Mac (F = free, x = busy).
// All 16 free is what the default macOS layout (runtime/include/nx/runtime.h) needs.
// Build/run: clang tools/scan_mac_va.c -o /tmp/scan_mac_va && /tmp/scan_mac_va
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
int main(void) {
    int bad = 0;
    for (int i = 0; i < 16; i++) {
        uint64_t a = 0x7000000000ull + (uint64_t)i * 0x100000000ull;
        void* p = mmap((void*)a, 0x100000000ull, PROT_NONE, MAP_PRIVATE | MAP_ANON | MAP_NORESERVE, -1, 0);
        int ok = p != MAP_FAILED && (uint64_t)p == a;
        if (p != MAP_FAILED) munmap(p, 0x100000000ull);
        printf("%llx %c\n", (unsigned long long)a, ok ? 'F' : 'x');
        bad += !ok;
    }
    printf(bad ? "Some slots are busy: pick another NX_BASE/layout (see docs/BUILD.md).\n" : "All free: default layout OK.\n");
    return bad != 0;
}
