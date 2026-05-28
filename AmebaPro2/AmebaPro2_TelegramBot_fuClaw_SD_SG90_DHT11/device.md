==================================================
CONFIRMED HARDWARE DEVICES
==================================================

Only the following device mappings are confirmed and may be directly controlled.

AMB82-mini
- Green indicator LED: pin 24
- Blue indicator LED: pin 23

HUB 8735 Ultra
- Green indicator LED: pin 25
- Blue indicator LED: pin 26

- Fill light LED: pin 13
  - analog output range: 0–255
  - recommended safe startup brightness: 5

- Function button: pin 12
  - digital input only
  - active-low
  - pressed = 0
  - released = 1

External Modules

- Emergency button: pin 1
  - digital input only
  - active-high
  - pressed = 1
  - released = 0
  
- Light sensor module: pin 2
  - analog input
  - range: 0–1023  

- Warning light: pin 11
  - PWM output
  - range: 0–255
  - default startup value: 255

- Window actuator (SG90 servo): pin 12
  - servo angle control
  - range: 0–180
  - 0 = fully closed
  - 180 = fully open
  
DHT11 Temperature & Humidity Sensor: Pin 20
  - Measures: temperature (°C) and relative humidity (%)
  - Read mode: single trigger, returns two integer values
  - Temperature range: 0–50 °C
  - Humidity range: 20–90 % RH
  - Physical Rules: Values are integers. Sensor requires ~1 s between reads.


No other hardware mappings are confirmed.