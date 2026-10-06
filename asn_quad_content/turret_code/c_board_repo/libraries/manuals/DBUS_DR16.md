# DBUS\_DR16



### Overview

This library enables receiving DR16 inputs vie the DBUS interface. This is used for getting controls from the DT7 (old white) DJI remote controllers. This includes mouse and keyboard controls when the controller is connected to a computer running the RM Client. DBUS is just an altered version of UART, so UART setup is required.



### IOC Setup

###### UART 3 (DBUS):

1. Open the .ioc editor window by double clicking on your .ioc file in the Project Explorer view
2. Click on "USART3" in the "Connectivity" drop down
3. Select the "Asynchronous" option in the "Mode" dropdown
4. In the "Parameter Settings" tab:

   1. Set your baud rate to 100000
   2. Set your word length to "9 Bits (including Parity)"
   3. Set your "Parity" to "Even"
5. Go to the "DMA Settings" tab and:

   1. Click "Add"
   2. In the "Select" dropdown click on "USART\_RX"
   3. In the "Mode" dropdown use "Normal"
6. Go to the "NVIC Settings" tab and click the checkbox next to "USART3 global interrupt"



### Include Setup

1. Navigate to your main.h file

2\. Include the DBUS\_DR16 library by adding the following to the file in the "USER CODE BEGIN Includes" section:

&#x09;#define INCLUDE\_UART\_CORE

&#x09;#include "UART\_Core.h"

&#x09;

&#x09;#define INCLUDE\_DBUS\_DR16

&#x09;#include "DBUS\_DR16.h"





### Library Handler Setup

1. Navigate to the "StartLibraryHandler" task in your main.c file
2. After the "USER CODE BEGIN" comment, but before the for loop, ensure there is a call to "UART\_Init()" to initialize the UART\_Core library
3. AFTER UART\_Core, add UART\_LinkLib(DBUS\_DR16()); If other libraries are also used, add both libraries to the linker function as a comma seperated list. Eg: UART\_LinkLib(UART\_OtherLib1(),UART\_OtherLib2(),DBUS\_DR16());

&#x09;MAKE SURE: UART\_Init() comes before the linker function. If this is not done, UART\_Printf and UART\_Scanf may behave unexpectedly





### Functions

##### int16\_t getDR16\_CH0()

###### Params:

&#x09;None

###### Returns:

&#x09;int16\_t  -  The current value of the channel. Center is 0

&#x09;



##### int16\_t getDR16\_CH1()

###### Params:

&#x09;None

###### Returns:

&#x09;int16\_t  -  The current value of the channel. Center is 0





##### int16\_t getDR16\_CH2()

###### Params:

&#x09;None

###### Returns:

&#x09;int16\_t  -  The current value of the channel. Center is 0





##### int16\_t getDR16\_CH3()

###### Params:

&#x09;None

###### Returns:

&#x09;int16\_t  -  The current value of the channel. Center is 0





##### int16\_t getDR16\_CH4()

###### Params:

&#x09;None

###### Returns:

&#x09;int16\_t  -  The current value of the channel. Center is 0

###### NOTE:

&#x09;When last tested this funciton was non-functional. Should be remidied in future with shitch to non polling







##### uint8\_t getDR16\_S1()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  The current value of the right switch:

&#x09;

&#x09;		1: Top Position

&#x09;		3: Middle Position

&#x09;		2: Bottom Position

&#x09;		Defaults to 0 or last value



##### uint8\_t getDR16\_S2()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  The current value of the left switch:

&#x09;

&#x09;		1: Top Position

&#x09;		3: Middle Position

&#x09;		2: Bottom Position

&#x09;		Defaults to 0 or last value



##### uint8\_t getDR16\_S1UP()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when right switch is in the up position





##### uint8\_t getDR16\_S1MID()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when right switch is in the middle position





##### uint8\_t getDR16\_S1DWN()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when right switch is in the down position





##### uint8\_t getDR16\_S2UP()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when left switch is in the up position





##### uint8\_t getDR16\_S2MID()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when left switch is in the middle position





##### uint8\_t getDR16\_S2DWN()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when left switch is in the down position



### RM Client Functions



##### int16\_t getDR16\_MouseX()

###### Params:

&#x09;None

###### Returns:

&#x09;int16\_t  -  returns the velocity of the mouse in the x direction



##### int16\_t getDR16\_MouseY()

###### Params:

&#x09;None

###### Returns:

&#x09;int16\_t  -  returns the velocity of the mouse in the Y direction





##### int16\_t getDR16\_MouseZ()

###### Params:

&#x09;None

###### Returns:

&#x09;int16\_t  -  returns the velocity of the scroll wheel



##### uint8\_t getDR16\_MouseL()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 if the left mouse button is pressed



##### uint8\_t getDR16\_MouseR()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 if the right mouse button is pressed





##### uint8\_t getDR16\_W()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when W key is pressed



##### uint8\_t getDR16\_S()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when S key is pressed





##### uint8\_t getDR16\_A()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when A key is pressed





##### uint8\_t getDR16\_D()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when D key is pressed





##### uint8\_t getDR16\_Q()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when Q key is pressed



##### uint8\_t getDR16\_E()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when E key is pressed



##### uint8\_t getDR16\_Shift()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when Shift key is pressed





##### uint8\_t getDR16\_Ctrl()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when Ctrl key is pressed





##### uint8\_t getDR16\_R()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when R key is pressed





##### uint8\_t getDR16\_F()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when F key is pressed





##### uint8\_t getDR16\_G()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when G key is pressed



##### uint8\_t getDR16\_Z()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when Z key is pressed





##### uint8\_t getDR16\_X()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when X key is pressed





##### uint8\_t getDR16\_C()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when C key is pressed







##### uint8\_t getDR16\_V()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when V key is pressed





##### uint8\_t getDR16\_B()

###### Params:

&#x09;None

###### Returns:

&#x09;uint8\_t  -  returns 1 when B key is pressed







