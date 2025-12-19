#include "tm4c123gh6pm.h"   // Device header: register definitions for TM4C123 microcontroller
#include "ws2812_pwm.h"     // WS2812 LED driver header (defines ws2812_pixel_t, NUM_PIXELS, prototypes)

ws2812_pixel_t pixels[NUM_PIXELS];   // Pixel buffer: holds RGB values for each LED in the strip

// --- UART bit encoding for WS2812B at 3.2 Mbps ---
// Each WS2812 data bit is sent as one UART byte.
// Encoding scheme (timing matched to WS2812 protocol):
//   0-bit = 0x08 (short high pulse, long low pulse)
//   1-bit = 0x0E (long high pulse, short low pulse)

static inline void send_bit(int bit) {
    while (UART1_FR_R & 0x20);       // Wait until UART1 TX FIFO is not full (bit 5 = TXFF)
    UART1_DR_R = bit ? 0x0E : 0x08;  // Send encoded byte for '1' or '0'
}

static void send_color(uint8_t color) {
    for (int i = 0; i < 8; i++) {    // Loop through 8 bits (MSB first)
        // Mask current bit: (0x80 << i) shifts MSB down each iteration
        send_bit(color & (0x80 << i ));
    }
}

// --- Reset gap: force line low for >50 µs ---
// Required by WS2812 to latch data after transmission
static void ws2812_reset_gap(void) {
    UART1_CTL_R &= ~(unsigned int)0x01;   // Disable UART1 temporarily
    GPIO_PORTB_AFSEL_R &= ~(unsigned int)0x02; // Disable alternate function on PB1
    GPIO_PORTB_DIR_R   |= 0x02;           // Set PB1 as GPIO output
    GPIO_PORTB_DATA_R  &= ~(unsigned int)0x02; // Drive PB1 low
    for (volatile int d = 0; d < 40000; d++);  // Delay ~300 µs at 80 MHz
    GPIO_PORTB_AFSEL_R |= 0x02;           // Re-enable UART function on PB1
    UART1_CTL_R |= 0x301;                 // Re-enable UART1 (TX, RX, UARTEN)
}

// --- Flush UART before reset ---
// Ensures all bytes have been transmitted before issuing reset gap
static inline void uart_flush(void) {
    while (UART1_FR_R & 0x08);   // Wait until UART1 is not busy (bit 3 = BUSY)
}

// --- Public API ---
// Initialize UART1 on PB1 for WS2812 communication
void WS2812_Init(void) {
    SYSCTL_RCGCUART_R |= 0x02;   // Enable clock for UART1
    SYSCTL_RCGCGPIO_R |= 0x02;   // Enable clock for GPIOB
    while((SYSCTL_PRGPIO_R & 0x02) == 0); // Wait until GPIOB is ready

    UART1_CTL_R &= ~(unsigned int)0x01;   // Disable UART1 during config
    UART1_IBRD_R = 1;            // Integer baud divisor (3.2 Mbps @ 80 MHz)
    UART1_FBRD_R = 17;           // Fractional baud divisor
    UART1_LCRH_R = 0x70;         // Line control: 8-bit, no parity, 1 stop bit (8N1)
    UART1_CC_R   = 0x0;          // Use system clock

    GPIO_PORTB_AFSEL_R |= 0x02;  // Enable alternate function on PB1
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~(unsigned int)0x000000F0) | 0x00000010; // PB1 = U1TX
    GPIO_PORTB_DEN_R |= 0x02;    // Enable digital function on PB1

    UART1_CTL_R |= 0x301;        // Enable UART1 (TXE, RXE, UARTEN)
}

// Set a single pixel in the buffer
void WS2812_SetPixel(int index, ws2812_pixel_t c) {
    if (index < 0 || index >= NUM_PIXELS) return; // Bounds check
    pixels[index] = c;                            // Store RGB values
}

// Send pixel buffer to LED strip
void WS2812_Show(void) {
    ws2812_reset_gap();           // Issue reset gap before transmission
    for (int i = 0; i < NUM_PIXELS; i++) {
        send_color(pixels[i].red);   // Send red byte
        send_color(pixels[i].blue);  // Send blue byte
        send_color(pixels[i].green); // Send green byte
        // Note: This strip expects RGB order (tested empirically)
    }
    uart_flush();                 // Ensure last byte fully transmitted
    ws2812_reset_gap();           // Issue reset gap to latch data
}

// Clear all pixels (set to black/off)
void WS2812_Clear(void) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels[i].red   = 0;
        pixels[i].green = 0;
        pixels[i].blue  = 0;
    }
    WS2812_Show();                // Update strip
}

// Fill all pixels with a single color
void WS2812_Fill(ws2812_pixel_t c) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels[i] = c;            // Assign same color to all pixels
    }
    WS2812_Show();                // Update strip
}
