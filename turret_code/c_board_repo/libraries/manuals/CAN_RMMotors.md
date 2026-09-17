# CAN\_RMMotors

### Overview

This library provides a control layer for Robomaster motor controllers: C620 (M3508 Controllers), C610 (2006 Controllers), 6020 motor units



### Dependancies

``PID.h``

``CAN_Core.h``



### IOC Setup

###### Can 1:

1. Open the .ioc editor window by double clicking on your .ioc file in the Project Explorer view
2. Click on "CAN1" in the "Connectivity" drop down
3. Check the "Activated" check box
4. In the "Parameter Settings" tab do the following in order

   1. Set "Time Quanta in Bit Segment 1" to 10
   2. Then set "Time Quanta in Bit Segment 2" to 3
   3. Then set "Prescaler (for Time Quantum)" to 3
   4. To verify you did this correctly, make sure that the "Time for one Bit" is 1000ns and the "Baud Rate" is 1000000 bit/s
5. Go to the "NVIC Settings" tab and click the "Enabled" checkbox next to "CAN1 RX0 interrupts"
6. Go to the view of the processor pinout (the page with all the circles) and do the following

   1. Find "PD0", click on it and set it to "CAN1\_RX"
   2. Find "PD1" (just below PD0), click on it and set it to "CAN1\_TX"

7\. Generate the code



###### Can 2:

1. Open the .ioc editor window by double clicking on your .ioc file in the Project Explorer view
2. Click on "CAN2" in the "Connectivity" drop down
3. Check the "Activated" check box
4. In the "Parameter Settings" tab do the following in order

   1. Set "Time Quanta in Bit Segment 1" to 10
   2. Then set "Time Quanta in Bit Segment 2" to 3
   3. Then set "Prescaler (for Time Quantum)" to 3
   4. To verify you did this correctly, make sure that the "Time for one Bit" is 1000ns and the "Baud Rate" is 1000000 bit/s
5. Go to the "NVIC Settings" tab and click the "Enabled" checkbox next to "CAN2 RX0 interrupts"
6. Generate the code



###### Timer For PID:

1. Open the .ioc editor window by double clicking on your .ioc file in the Project Explorer view
2. Click on "TIM 13" in the "Timers" drop down
3. Click the checkbox next to "Activated"
4. Generate the code



### Include Setup

1. Navigate to your main.h file

2\. Include the CAN\_Core, CAN\_RMMotors and PID libraries by adding the following to the file in the "USER CODE BEGIN Includes" section:

&#x09;``#define INCLUDE_CAN_CORE``

&#x09;``#include "CAN_Core.h"``



&#x09;``#define INCLUDE_CAN_RMMOTORS``

&#x09;``#include "CAN_RMMotors.h"``



&#x09;``#define INCLUDE_PID``

&#x09;``#include "PID.h"``


### Library Handler Setup

1. Navigate to the "StartLibraryHandler" task in your main.c file
2. After the "USER CODE BEGIN" comment, but before the for loop, do the following:

   1. Make sure that ``CAN_Init_Buses(``...``);`` is present, where "..." is a comma seperated list of the bus numbers you want to initialize. Eg: ``CAN_Init_Buses(BUS1, BUS2);``
   2. Then add ``CAN_LinkLib(CAN_RMMotors());`` to link the the RM Motor library to the CAN core library. If other libraries are also used, add both libraries to the linker function as a comma seperated list. Eg: ``CAN_LinkLib(CAN_OtherLib1(),CAN_OtherLib2(),CAN_RMMotors());``
   3. Then lastly add ``CAN_RM_Init_Buses(``...``);`` in order to initialize the RM Motor control library on those buses. "..." is again a comma seperated list of the bus numbers you want to initialize. Eg. ``BUS1`` or ``BUS1, BUS2``
3. Lastly inside the infinite loop add ``CAN_RM_Update();``



### Functions

| Function | ``void setC610Pos(Bus Number, Motor ID, Target Position, PID[])`` | Sets the position of the M2006 RM motor |
| -------- | ----------------------------- | --------- |
| Parameter | Bus Number | The bus number your motor is on. Eg: "BUS1", or "BUS2". (uint8\_t) |
| Parameter | Motor ID | The ID number of the motor controller you want to target. It will be blinking in bursts equal to this number (uint8\_t) |
| Parameter | Target Position | The position you want the motor to go to. Values from 0-8191 (uint16\_t) |
| Parameter | PID\[] | An array of 3 floats, describing the P, I, and D values (respectively) for the system (float \[3]) |
| Returns | void | |
| Notes: | | <mark> Note that this function, although operational, has no functional purpose. Setting position on the 2006 BLDC and 3508 BLDC is quite useless since the encoder is on the unreduced end. <\mark> |



| Function | ``void setC610RPM(Bus Number, Motor ID, Target RPM, PID[])`` | Sets the RPM of the M2006 RM motor |
| -------- | ----------------------------- | --------- |
| Parameter | Bus Number | The bus number your motor is on. Eg: "BUS1", or "BUS2". (uint8\_t) |
| Parameter | Motor ID | The ID number of the motor controller you want to target. It will be blinking in bursts equal to this number (uint8\_t) |
| Parameter | Target RPM | The speed in RPM you want the motor to go to (uint16\_t) |
| Parameter | PID\[] | An array of 3 floats, describing the P, I, and D values (respectively) for the system (float \[3]) |
| Returns | void | |
| Notes: | | Note velocity readings come from the unreduced end. The gearbox applies a reduction of X:X, so normal values for target RPMs are around 700-8000 |



| Function | ``void setC620Pos(Bus Number, Motor ID, Target Position, PID[])`` | Sets the position of the M3508 RM motor |
| -------- | ----------------------------- | --------- |
| Parameter | Bus Number | The bus number your motor is on. Eg: "BUS1", or "BUS2". (uint8\_t) |
| Parameter | Motor ID | The ID number of the motor controller you want to target. It will be blinking in bursts equal to this number (uint8\_t) |
| Parameter | Target Position | The position you want the motor to go to. Values from 0-8191 (uint16\_t) |
| Parameter | PID\[] | An array of 3 floats, describing the P, I, and D values (respectively) for the system (float \[3]) |
| Returns | void | |
| Notes: | | <mark> Note that this function, although operational, has no functional purpose. Setting position on the 2006 BLDC and 3508 BLDC is quite useless since the encoder is on the unreduced end. <\mark> |


| Function | ``void setC620RPM(Bus Number, Motor ID, Target RPM, PID[])`` | Sets the RPM of the M3508 RM motor |
| -------- | ----------------------------- | --------- |
| Parameter | Bus Number | The bus number your motor is on. Eg: "BUS1", or "BUS2". (uint8\_t) |
| Parameter | Motor ID | The ID number of the motor controller you want to target. It will be blinking in bursts equal to this number (uint8\_t) |
| Parameter | Target RPM | The speed in RPM you want the motor to go to (uint16\_t) |
| Parameter | PID\[] | An array of 3 floats, describing the P, I, and D values (respectively) for the system (float \[3]) |
| Returns | void | |
| Notes: | | Note velocity readings come from the unreduced end. The gearbox applies a reduction of X:X, so normal values for target RPMs are around 700-8000 |


| Function | ``void set6020Pos(Bus Number, Motor ID, Target Position, PID\[])`` | Sets the Position of the GM6020 RM Motor |
| -------- | ----------------------------- | --------- |
| Parameter | Bus Number | The bus number your motor is on. Eg: "BUS1", or "BUS2". (uint8\_t) |
| Parameter | Motor ID | The ID number of the motor controller you want to target. It will be blinking in bursts equal to this number (uint8\_t) |
| Parameter | Target Position | The position you want the motor to go to. Values from 0-8191 (uint16\_t) |
| Parameter | PID\[] | An array of 3 floats, describing the P, I, and D values (respectively) for the system (float \[3]) |
| Returns | void | |
| Notes: | | None |


| Function | ``void set6020RPM(Bus Number, Motor ID, Target RPM, PID\[])`` | Sets the RPM of the GM6020 RM Motor |
| -------- | ----------------------------- | --------- |
| Parameter | Bus Number | The bus number your motor is on. Eg: "BUS1", or "BUS2". (uint8\_t) |
| Parameter | Motor ID | The ID number of the motor controller you want to target. It will be blinking in bursts equal to this number (uint8\_t) |
| Parameter | Target RPM | The speed in RPM you want the motor to go to (uint16\_t) |
| Parameter | PID\[] | An array of 3 floats, describing the P, I, and D values (respectively) for the system (float \[3]) |
| Returns | void | |
| Notes: | | Note there is no gearbox on this motor, what you target is what you get. Normal values are up to about 200 |





