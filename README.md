# PomodoroTimer

Pomodoro Timer with Rest and Power-Saving Modes
Introduction
This project is a Pomodoro Timer designed to help users manage their work and rest cycles efficiently. The timer is programmed to follow the Pomodoro Technique, where you work for a set period and then take a short break. Additionally, the device enters a power-saving mode when not in use to conserve energy.

Features
Work Time: The timer is set to 25 minutes of work time, indicated by a red LED.
Rest Time: After each work session, the timer automatically enters a 5-minute rest period, indicated by a yellow LED.
Pause Functionality: During both work and rest periods, the timer can be paused. While paused, the red and yellow LEDs will alternate, indicating the paused state. The timer resumes from where it was paused.
Power-Saving Mode: If the device is idle for more than 3 minutes in the initial state, it automatically enters a power-saving mode, turning off all LEDs to conserve energy.
User Feedback: The device provides auditory feedback using a buzzer. The buzzer emits different beeps to indicate state changes such as starting work, pausing, resuming, and finishing a cycle.
Simple User Interaction: The device is controlled by a single button, making it easy to use. A long press of the button resets the device to the initial state.
How It Works
Initial State: Upon powering on, the device starts in the initial state with the green LED on, indicating it is ready to start the work session.
Start Work Session: Press the button once to begin the work session. The green LED turns off, the red LED turns on, and the timer starts counting down from 25 minutes.
Pause/Resume: During the work or rest session, pressing the button will pause the timer. The red and yellow LEDs will flash alternately to indicate the pause. Pressing the button again will resume the timer.
Rest Session: After the work session ends, the red LED turns off, the yellow LED turns on, and the timer starts counting down from 5 minutes.
Cycle Completion: After the rest session, the buzzer will emit three quick beeps, and the device will enter the finished state with the red LED flashing.
Power-Saving Mode: If no button is pressed within 3 minutes while in the initial state, the device will enter power-saving mode, turning off all LEDs. Pressing the button will wake the device up, returning it to the initial state without starting the timer.
Setup Instructions
Hardware Requirements:

ESP32 or similar microcontroller
LEDs (Red, Yellow, Green)
Buzzer
Push Button
Resistors and wires as needed
Software Requirements:

Arduino IDE for code compilation and uploading
Required libraries (if any)
Wiring Diagram:

Connect the LEDs to the respective pins as defined in the code.
Connect the button to the input pin and ground.
Connect the buzzer to the output pin and ground.
Uploading the Code:

Open the provided .ino file in the Arduino IDE.
Select the correct board and COM port in the Arduino IDE.
Upload the code to the microcontroller.
Usage
Once the device is powered and the code is uploaded:

Press the button to start a work session.
Press the button again to pause or resume the timer.
After the work session, the device will automatically enter rest mode.
The device will enter power-saving mode if left idle in the initial state for 3 minutes.
Contributing
Contributions are welcome! Please feel free to submit pull requests or open issues on GitHub. This project is open source and licensed under the MIT License.

License
This project is licensed under the MIT License. See the LICENSE file for more details.

Author
Developed by Chiming Wu, this Pomodoro Timer project is designed to help users manage their time more effectively. Feel free to fork the project and customize it to suit your needs.
