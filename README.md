# Info
Project during the course ES 116 - Principals and Applications of Electrical Engineering

Instructor - [Prof. Arup Lal Chakraborty](https://iitgn.ac.in/hi/faculty/ee/fac-arup)

Full Project Report [`EE Project Report.pdf`](https://github.com/Reckadon/ES116-ResponsiveLightingSystem/blob/main/EE%20Project%20Report.pdf)

---

## Aim of the Project
The aim of the project was to make a multi-band
brightness-changing light system that responds to the amount
of low, medium and high-frequency sound waves present in
the ambient music/audio respectively. This project would come
as a luxury use and, in some engineering cases, to detect
the mechanical stability of systems(based on frequency). The
algorithm works by taking analogue sound input, digitising it,
finding its frequency spectrum and sending PWM output signals,
corresponding to the required brightness levels of the various
LED strips, through an amplification circuit to the LED lights.

## Tech Used
- arduinoFFT library
- IC-741 op-amps
- microphone module
