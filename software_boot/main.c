#include <stdint.h>

// Assuming UART base address is 0x10000000 for standard 16550 UART in simulation
#define UART_THR ((volatile uint32_t*) 0x10000000)
#define UART_LSR ((volatile uint32_t*) 0x10000014)

void putc(char c) {
    // Wait until Transmit Holding Register Empty (THRE)
    while ((*UART_LSR & 0x20) == 0);
    *UART_THR = c;
}

void print(const char* str) {
    while (*str) {
        putc(*str++);
    }
}

int main() {
    print("Hello, TITAN-X! Bare-metal C boot successful.\n");
    
    // Perform a memory write test to SRAM
    volatile uint32_t* test_ptr = (volatile uint32_t*) 0x20000000;
    *test_ptr = 0xDEADBEEF;

    if (*test_ptr == 0xDEADBEEF) {
        print("Memory Write/Read Test PASSED.\n");
    } else {
        print("Memory Test FAILED.\n");
    }

    return 0;
}
