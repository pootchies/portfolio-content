# CAN\_Core



### Overview

This library links up all CAN libraries and provides basic CAN functionality for the Robomaster Dev board type C



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



### Functions

##### void CAN\_LinkLib(...)

###### Params:

&#x09;CAN\_LibraryInfo ...

&#x09;	(generally a set of functions with names the same as the library file being linked. 

###### Returns:

&#x09;void

&#x09;



This function links general purpose CAN libraries into the CAN receiving trigger. It requires an object of type CAN\_LibraryInfo to be passed in, that contains the pointer to the receive function that is to be executed at every CAN receive trigger. The function must be in the format: "void CAN\_Lib\_Recieve(CAN\_HandleTypeDef \*hcan, uint32\_t StdId,uint32\_t ExtId, uint8\_t rx\_data\[8])"



Up to 63 parameters may be passed in, in a comma separated list. 



This function overwrites previously linked libraries each time it is called.







##### void CAN\_Init\_Buses(...)

###### Params:

&#x09;int bus ...

&#x09;	(number of all buses to be initialized on c\_board in a comma seperated list. Numbers can be substituted for descriptors such as "BUS1" "BUS2")



###### Returns:

&#x09;void



This function is short form for CAN\_Init, allowing for quick user setup with minimal effort. This however has to be rewritten to be board specific if ever ported to new chipset.



##### void CAN\_Init(CAN\_HandleTypeDef \*hcanx, uint32\_t filterBank, uint32\_t startSlaveBank)

###### Params:

&#x09;CAN\_HandleTypeDef \*hcanx

&#x09;	(pointer to the can instance to initialize)



&#x09;uint32\_t filterBank

&#x09;	(filter bank to use for this instance)



&#x09;uint32\_t startSlaveBank

&#x09;	(set the cutoff between bank regions to use for different can instances) NOTE: Currently ineffective



###### Returns:

&#x09;void



##### void CAN\_Transmit(CAN\_HandleTypeDef \*hcanx, uint32\_t stdId, uint8\_t data\[8])

###### Params:

&#x09;CAN\_HandleTypeDef \*hcanx

&#x09;	(pointer to can instance to send over)

&#x09;

&#x09;uint32\_t stdId

&#x09;	(standard length CAN header)



&#x09;uint8\_t data\[8]

&#x09;	(pointer to array containing 8 bytes to send)

###### Returns:

&#x09;void





##### void CAN\_Transmit\_Ex(CAN\_HandleTypeDef \*hcanx, uint32\_t extId, uint8\_t data\[8])

###### Params:

&#x09;CAN\_HandleTypeDef \*hcanx

&#x09;	(pointer to can instance to send over)

&#x09;

&#x09;uint32\_t extId

&#x09;	(extended length CAN header)



&#x09;uint8\_t data\[8]

&#x09;	(pointer to array containing 8 bytes to send)

###### Returns:

&#x09;void









