Introduction



Project Sound Tunnel is a real-time audio visualization system built on the TM4C123GH6PM microcontroller. The system samples audio, performs a 256‑point FFT, extracts frequency‑band energy, and drives WS2812B LEDs to create dynamic lighting patterns synchronized to music. This project demonstrates embedded DSP, hardware interfacing, and real-time system design.



Functional Block Diagram







Results and Video Demonstration Links







Background and Methodology



\### Embedded Systems Concepts Applied

\- ADC configuration and continuous sampling  

\- Timer-based periodic interrupts  

\- UART-based WS2812B timing emulation  

\- Fixed-point DSP and FFT implementation  

\- Real-time processing loop design  

\- Modular C programming and hardware abstraction  



\### How the Project Goals Were Achieved

\- Implemented ADC0 to sample audio at 8 kHz  

\- Performed a 256‑point FFT to extract frequency magnitudes  

\- Grouped FFT bins into bass/mid/treble bands  

\- Mapped normalized band energy to LED colors  

\- Used UART at 3.2 Mbps to generate WS2812B timing  

\- Integrated all modules into a real-time main loop  

