# Pomodoro Timer with Rest and Power-Saving Modes

This project implements a Pomodoro Timer using an ESP32 microcontroller. The timer is designed to help users manage their work and rest cycles effectively, following the Pomodoro Technique. It includes features such as automatic rest periods, pause functionality, and a power-saving mode.

## Features

- **Work Time (25 minutes):** The timer starts with a 25-minute work session indicated by a red LED.
- **Rest Time (5 minutes):** After each work session, the timer enters a 5-minute rest period indicated by a yellow LED.
- **Pause Functionality:** During work or rest sessions, the timer can be paused, and the red and yellow LEDs will alternate to indicate the pause state.
- **Power-Saving Mode:** The device enters power-saving mode after 3 minutes of inactivity in the initial state, turning off all LEDs to conserve energy.
- **Auditory Feedback:** The buzzer provides feedback with different beep patterns to signal state changes such as starting work, pausing, resuming, and finishing a cycle.

## Hardware Requirements

- ESP32 microcontroller or similar
- Red, Yellow, and Green LEDs
- Buzzer
- Push Button
- Resistors and wires for circuit connections

## Setup and Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/pomodoro-timer.git
   ```

2. **Connect the hardware:**
   - Connect the LEDs to the specified pins on the ESP32.
   - Connect the buzzer to the designated pin.
   - Connect the push button to an input pin and ground.

3. **Open the project in Arduino IDE:**
   - Open `pomodoro_timer.ino` in the Arduino IDE.
   - Ensure you have selected the correct board (ESP32) and COM port.

4. **Upload the code:**
   - Click the upload button in Arduino IDE to flash the code to the ESP32.

## Usage Instructions

1. **Initial State:**
   - After powering on, the green LED indicates the device is in the initial state. Press the button to start the work session.

2. **Start Work Session:**
   - Press the button once to begin the 25-minute work session. The green LED turns off, the red LED turns on, and the timer starts counting down.

3. **Pause and Resume:**
   - Press the button during a work or rest session to pause. The red and yellow LEDs will alternate to indicate the pause. Press the button again to resume.

4. **Rest Session:**
   - After the work session, the timer automatically switches to a 5-minute rest session, turning on the yellow LED.

5. **Power-Saving Mode:**
   - If the device is inactive for 3 minutes in the initial state, it will enter power-saving mode. Press the button to wake the device back to the initial state.

## Contributing

Contributions are welcome! Please open issues or submit pull requests to improve the project. For major changes, please discuss them via an issue first.

To contribute:
1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Commit your changes with a clear description.
4. Push the branch to your fork and submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details.

## Author

Developed by C-M.Wu. Feel free to contact me for any questions or suggestions.
