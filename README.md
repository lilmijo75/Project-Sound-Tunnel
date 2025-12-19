Introduction

Project Sound Tunnel is a real-time audio visualization system built on the TM4C123GH6PM microcontroller. The system samples audio, performs a 256‑point FFT, extracts frequency‑band energy, and drives WS2812B LEDs to create dynamic lighting patterns synchronized to music. This project demonstrates embedded DSP, hardware interfacing, and real-time system design.

Functional Block Diagram

<img width="775" height="691" alt="image" src="https://github.com/user-attachments/assets/ac0f3587-3cdc-4203-9350-4bf5438cc478" />

<img width="822" height="570" alt="image" src="https://github.com/user-attachments/assets/bca8458c-70a3-4642-ab76-d1f3e63eeb08" />

Results and Video Demonstration Links

Voltage Divider Circuit Testing (Multisim)

<img width="655" height="502" alt="image" src="https://github.com/user-attachments/assets/2c083e03-e0fd-4861-9286-1c3daf262b42" />

Before implementing the audio input stage on hardware, the DC‑biasing network was simulated in Multisim to verify that the analog front‑end would correctly bias the incoming audio signal for the TM4C123GXL’s ADC. The circuit consisted of two 4.7 kΩ resistors forming a voltage divider from 3.3V to ground, with a 0.1 µF capacitor placed in parallel with the lower resistor.

Simulation Results:
The voltage divider produced a stable 1.65V DC bias, exactly half of the 3.3V supply.
The parallel capacitor held the bias node steady and reduced ripple.
When an AC signal was injected (via the coupling capacitor), the waveform correctly oscillated around the 1.65V midpoint.

Demo of Project Sound Tunnel

https://youtube.com/shorts/HX7q9RPgeGg?feature=shared

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

Table of Components Used

<img width="795" height="433" alt="image" src="https://github.com/user-attachments/assets/75c006d7-9351-4d81-ba0e-e075760db22f" />
<img width="813" height="808" alt="image" src="https://github.com/user-attachments/assets/e08d3527-2e02-420b-bc38-87cb306b7666" />

Table of Pins Used

<img width="847" height="675" alt="image" src="https://github.com/user-attachments/assets/32152930-9e3f-4bd0-b90d-f702872a8bc6" />
<img width="876" height="387" alt="image" src="https://github.com/user-attachments/assets/c0aaa332-0429-4817-b67d-31f2bf834092" />
<img width="837" height="422" alt="image" src="https://github.com/user-attachments/assets/618c4f02-12cf-46e0-a69c-96525b5c827e" />
