# AltTabSusBox
## Alt Tabs my anime when someone tries to walk into my room

This is a simple project that I wanted to fix a very unique and small problem. I like watching anime, and sometimes my parents walk into my room at the worst possible time, even if the show is completely PG13. 
I decided to spend ~$120 dollars to create 2 boxes, fitted with Arduino Nanos, RF24 radio modules, LEDs, 9V Batteries, and Battery level indicator circuits. 

**Features:**
- Automatically "spacebar" and "windows + M" minimize current working tab when PID sensor is triggered on remote box.
- LED Indiciator on box connected to PC to indiciate status of connection and battery level of remote box.
  - RED when no connection available
  - WHITE/BLUE when active
  - FLASH GREEN when PID triggered
  - YELLOW when battery level low (~approx 20 minutes left, enough for 1 more episode!).
- Automatic adjustment of "shutoff" threshold, as well as auto adjust for replaced batteries.
 
 ![20220916_130249](https://user-images.githubusercontent.com/65744075/190877533-e67a6b00-2821-4c09-b25d-ff6894433eea.jpg)
