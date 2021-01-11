# stepper-control-via-uart
# II. HARDWARE CONNECTION
## ------------------------------------
## 1. Uart Baurd Rate: 11520
## 2. Pulse/rev: 800 
## ------------------------------------
## 1. PUL       => DIGITAL 8 (Arduino)
## 2. DIR       => DIGITAL 9 (Arduino)
## 3. ENA       => DIGITAL 10(Arduino)
## 4. OPTO      => 5V        (Arduino)
## ------------------------------------
## 5. GND       = > GND  (12V Power)
## 6. 12v       = > +Vdc (12v Power)
## ------------------------------------
## 7. A+        => Blue  (Stepper)
## 8. A-        => Red   (Stepper)
## 9. B+        => Green (Stepper)
## 10. B-       => Black (Stepper)
## ------------------------------------

# II. COMMAND TO SET AND READ MOTOR STATUS

## 1. Setting command
 ### SP_100  : Set speed 100 revolutions per minute (rpm)
 ### TL_450  : Turn clockwise 450 step (will be change to degree)
 ### TR_180  : Turn counter clockwise 180 step (will be change to degree)

## 2. Reading command
 ### MT      : Read motor multi turn angle
 ### RP      : Read motor speed
 ### RE      : Read motor encoder
