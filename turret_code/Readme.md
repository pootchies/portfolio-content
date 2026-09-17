# RMUC 2026 Aerial Turret Firmware

STM32/FreeRTOS firmware for the 17 mm turret carried by SFU Ascension Robotics' RoboMaster 2026 quadcopter. Runs on a RoboMaster Development Board Type C and drives five DJI motors over CAN: two GM6020 gimbal axes, twin M3508 flywheels, and an M2006 indexer.

Gyro-stabilized pitch and yaw so the barrel holds an inertial heading while the airframe moves under it, plus flywheel ramping and indexer jam detection. Stick input over IBUS.

This is the turret code only — the drone's flight control runs separately on a Pixhawk 6C Mini in ArduPilot. All necessary libraries are included, internally developed by SFU Ascension Robotics.

Writeup and build details: [sites.google.com/view/danny-wang](https://sites.google.com/view/danny-wang/ascension-robotics-quadcopter)
