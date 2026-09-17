# UART\_Core



### Overview

This library links up all UART libraries and provides basic UART functionality for the Robomaster Dev board type C. This includes printf and scanf functionality.



### IOC Setup

###### UART 1:

1. Open the .ioc editor window by double clicking on your .ioc file in the Project Explorer view
2. Click on "USART1" in the "Connectivity" drop down
3. Select the "Asynchronous" option in the "Mode" dropdown
4. In the "Parameter Settings" tab set your baud rate to the desired value
5. Go to the "DMA Settings" tab and click "Add"
6. In the "Select" dropdown click on "USART\_RX"
7. In the "Mode" dropdown select "Normal"
8. Go to the "NVIC Settings" tab and click the "Enabled" checkbox next to "USART1 global interrupt"
9. Go to the view of the processor pinout (the page with all the circles) and do the following
10. Find "PA9", click on it and set it to "USART1\_TX"



###### UART 6:

1. Open the .ioc editor window by double clicking on your .ioc file in the Project Explorer view
2. Click on "USART6" in the "Connectivity" drop down
3. Select the "Asynchronous" option in the "Mode" dropdown
4. In the "Parameter Settings" tab set your baud rate to the desired value
5. Go to the "DMA Settings" tab and click "Add"
6. In the "Select" dropdown click on "USART\_RX"
7. In the "Mode" dropdown select "Normal"
8. Go to the "NVIC Settings" tab and click the "Enabled" checkbox next to "USART6 global interrupt"





### Include Setup

1. Navigate to your main.h file

2\. Include the UART\_Core library by adding the following to the file in the "USER CODE BEGIN Includes" section:

&#x09;#define INCLUDE\_UART\_CORE

&#x09;#include "UART\_Core.h"





### Library Handler Setup

1. Navigate to the "StartLibraryHandler" task in your main.c file
2. After the "USER CODE BEGIN" comment, but before the for loop, add "UART\_Init()" to initialize the C board's uart lines





### Functions

##### void UART\_Printf(UARTX,"string",vars ...)

###### Params:

&#x09;UARTX  -   This parameter designates the uart line you wish to send over. It can either be in the form "UART1" or "UART2" (correlating to the labels on the board) (omit the quotation marks)

&#x09;	  or it can pass the direct pointer to the uart handle



&#x09;"string"  -  This parameter can be any string formatted in fmt format (the standard C printf format). For more info, go research (eg. sites like this https://www.geeksforgeeks.org/c/printf-in-c/)



&#x09;vars ...  -  This is a comma seperated list of values to be displayed according to the string formatting

###### Returns:

&#x09;void

&#x09;

###### NOTES:

This function is similar to standard C "printf" except with the addition of the selected uart line to print over tagged in front of the normal parameters.

##### 



##### void UART\_Scanf(UART)

###### Params:

&#x09;UART  -   This parameter designates the uart line you wish to read. It can either be in the form "UART1" or "UART2" (correlating to the labels on the board)

&#x09;	  or it can pass the direct pointer to the uart handle



###### Returns:

&#x09;uint8\_t\*

&#x09;

###### NOTES:

This function returns the pointer to the DMA targeted 256 byte buffer for each uart line. Effectively the last received transmission will be returned. Please note that this function is still experimental and the buffer may be actively overwritten as you are reading it, leading to corrupted readings. Implement a cecksum or other redundancy method to ensure that this does not seriously impact your projects.









##### void UART\_LinkLib(...)

###### Params:

&#x09;UART\_LibraryInfo ...   -   generally a set of functions with names the same as the library file being linked

###### Returns:

&#x09;void

&#x09;



This function links general purpose UART libraries into the UART receiving trigger. It requires an object of type UART\_LibraryInfo to be passed in, that contains the pointer to the receive function that is to be executed at every UART receive trigger.



The function format is not yet specified and this functionality is not yet fully implemented.

