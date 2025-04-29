# Thermostat Controller Application

## Overview

This embedded C application implements a **closed-loop thermostat system** using a **BME680 temperature sensor** and a **pump** controlled through GPIO. The system automatically turns the pump ON or OFF based on ambient temperature thresholds.

---

## Author

**Bibin Alias**  
- `APP_Thermostat.c` Created on: 22-Apr-2025  
- `Dev_TempSensor.c` Created on: 18-Apr-2025  

---

## Hardware Requirements

| Component               | Description                                |
|------------------------|--------------------------------------------|
| **ZG28 Explorer Board** | Silicon Labs ZG28-based development board  |
| **BME680 Sensor Module**| Bosch BME680 sensor (I2C interface)         |
| **Pump Control Circuit**| Connected to a GPIO for ON/OFF switching    |
| **I2C Wiring**          | SDA/SCL lines connected from ZG28 to BME680|

---

## Features

- Sensor initialization, configuration, and self-test
- Temperature-based pump control
- I2C-based sensor interface
- Retrying mechanism for sensor and pump failures
- Real-time temperature display with trend symbols
- Graceful shutdown with deinitialization

---

## Application Architecture

### Files:

| File                   | Description                                              |
|------------------------|----------------------------------------------------------|
| `APP_Thermostat.c`     | Main control loop and logic                              |
| `Dev_TempSensor.c`     | BME680 sensor driver interface                           |
| `Dev_Pump.c/h`         | Hardware abstraction for pump control                    |
| `I2C_Wrapper.c/h`      | Platform-specific I2C interface                          |
| `bme68x_defs.h/.c`     | Bosch sensor definitions                                 |
| `bme68x.c/.h`          | Bosch BME68x sensor driver                               |

---

## Functional Flow

```mermaid
graph TD
    A[Start App_vThermostat()] --> B[Pump Initialization]
    B --> C[Pump Off]
    C --> D[Sensor Self-Test]
    D --> E[Sensor Initialization]
    E --> F[Enter Main Loop]
    F --> G[Get Temperature]
    G --> H{Temperature < Low Threshold?}
    H -- Yes --> I[Pump On]
    H -- No --> J{Temperature > High Threshold?}
    J -- Yes --> K[Pump Off]
    J -- No --> L[Do Nothing]
    I --> M[Print Temp + Delay]
    K --> M
    L --> M
    M --> F
