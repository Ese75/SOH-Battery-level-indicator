# SOH-Battery-level-indicator
A more accurate determination of the state of health (SOH) of a lead-acid battery using Hall sensor.

Set LM2596 output to 5V. Connect output(+) to Arduino 5V pin and output(-) to GND.

Connect OLED SSD1306 I2C VCC -> 5V, GND -> GND, SCL -> A5 and SDA -> A4.

Connest Hall-sensor ACS758 VCC -> 5V, GND -> GND and OUT1 -> A0.

![Circuit Diagram](SOH.png)
