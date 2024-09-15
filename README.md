# Stopwatch with Dual Mode (Increment and Countdown) Using ATmega32 and Seven-Segment Display.

![Screenshot 2024-09-16 021000](https://github.com/user-attachments/assets/fbeee9a2-ece6-4823-9811-812835d724a0)

# Project Overview: 
This project involves creating a versatile stopwatch that operates in two distinct modes: 
1. Increment Mode (Default): The stopwatch counts upwards from zero, displaying hours, minutes, and seconds on six seven-segment displays. 
2. Countdown Mode: The stopwatch counts downwards from a user-defined time, functioning as a countdown timer. 

# Features: 
• Reset Function: Resets the time to zero in both modes. 
• Pause Function: Pauses the time counting, allowing the stopwatch to hold its current value. 
• Resume Function: Resumes counting from the paused time. 
• Mode Toggle: Switches between increment and countdown modes.

# Hardware Components: 
• Microcontroller: ATmega32 
• Display: Six multiplexed seven-segment displays (common anode) driven by a 7447 BCD to seven-segment decoder connected to first 4 pins in PORTC. 
• Input: 10 push buttons for various functions.
• Alarm/Buzzer.
• LED Indicators.

# Operation Description: 
• Increment Mode: By default, the stopwatch starts in increment mode, where the time counts up continuously from zero. The time is displayed across six seven segment displays, showing hours, minutes, and seconds. The red LED on PD4 is turned on to indicate increment mode. 
• Countdown Mode: When the mode toggle button is pressed, the stopwatch switches to countdown mode. In this mode, the user can set the desired countdown time using the hour, minute, and second adjustment buttons. The countdown begins once the start/resume button is pressed. A yellow LED on PD5 is turned on to indicate countdown mode. An alarm is triggered when the countdown reaches zero.
