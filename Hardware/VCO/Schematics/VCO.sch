EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:fossa library
LIBS:VCO-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "VCO (GPLv3+)"
Date "2018-08-12"
Rev "2"
Comp "White Fossa aka Artyom Vetrov"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 8675 1075 0    60   ~ 0
+5V
Text Notes 8675 1200 0    60   ~ 0
GND
$Comp
L GND #PWR17
U 1 1 5B706542
P 8100 1500
F 0 "#PWR17" H 8100 1250 50  0001 C CNN
F 1 "GND" H 8100 1350 50  0000 C CNN
F 2 "" H 8100 1500 50  0001 C CNN
F 3 "" H 8100 1500 50  0001 C CNN
	1    8100 1500
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 5B70657A
P 7850 1300
F 0 "C8" H 7875 1400 50  0000 L CNN
F 1 "0.1uF" H 7875 1200 50  0000 L CNN
F 2 "" H 7888 1150 50  0001 C CNN
F 3 "" H 7850 1300 50  0001 C CNN
	1    7850 1300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR16
U 1 1 5B706662
P 7850 1500
F 0 "#PWR16" H 7850 1250 50  0001 C CNN
F 1 "GND" H 7850 1350 50  0000 C CNN
F 2 "" H 7850 1500 50  0001 C CNN
F 3 "" H 7850 1500 50  0001 C CNN
	1    7850 1500
	1    0    0    -1  
$EndComp
$Comp
L BF862 VT1
U 1 1 5B706795
P 7350 1750
F 0 "VT1" H 7600 1350 60  0000 C CNN
F 1 "BF862" H 7150 1350 60  0000 C CNN
F 2 "" H 7350 1750 60  0001 C CNN
F 3 "" H 7350 1750 60  0001 C CNN
	1    7350 1750
	1    0    0    1   
$EndComp
$Comp
L L_Core_Ferrite L2
U 1 1 5B7068F5
P 7500 2150
F 0 "L2" V 7450 2150 50  0000 C CNN
F 1 "10uH" V 7610 2150 50  0000 C CNN
F 2 "" H 7500 2150 50  0001 C CNN
F 3 "" H 7500 2150 50  0001 C CNN
	1    7500 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR14
U 1 1 5B7069A3
P 7500 2750
F 0 "#PWR14" H 7500 2500 50  0001 C CNN
F 1 "GND" H 7500 2600 50  0000 C CNN
F 2 "" H 7500 2750 50  0001 C CNN
F 3 "" H 7500 2750 50  0001 C CNN
	1    7500 2750
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5B706A22
P 7000 1750
F 0 "C4" H 7025 1850 50  0000 L CNN
F 1 "560pF" H 7025 1650 50  0000 L CNN
F 2 "" H 7038 1600 50  0001 C CNN
F 3 "" H 7000 1750 50  0001 C CNN
	1    7000 1750
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 5B706AA9
P 7000 2150
F 0 "C5" H 7025 2250 50  0000 L CNN
F 1 "560pF" H 7025 2050 50  0000 L CNN
F 2 "" H 7038 2000 50  0001 C CNN
F 3 "" H 7000 2150 50  0001 C CNN
	1    7000 2150
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 5B706C78
P 6750 1950
F 0 "R4" V 6830 1950 50  0000 C CNN
F 1 "100k" V 6750 1950 50  0000 C CNN
F 2 "" V 6680 1950 50  0001 C CNN
F 3 "" H 6750 1950 50  0001 C CNN
	1    6750 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR12
U 1 1 5B706D03
P 6750 2350
F 0 "#PWR12" H 6750 2100 50  0001 C CNN
F 1 "GND" H 6750 2200 50  0000 C CNN
F 2 "" H 6750 2350 50  0001 C CNN
F 3 "" H 6750 2350 50  0001 C CNN
	1    6750 2350
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5B707492
P 6500 1550
F 0 "C3" H 6525 1650 50  0000 L CNN
F 1 "47pF" H 6525 1450 50  0000 L CNN
F 2 "" H 6538 1400 50  0001 C CNN
F 3 "" H 6500 1550 50  0001 C CNN
	1    6500 1550
	0    1    1    0   
$EndComp
$Comp
L GND #PWR11
U 1 1 5B707677
P 6200 2350
F 0 "#PWR11" H 6200 2100 50  0001 C CNN
F 1 "GND" H 6200 2200 50  0000 C CNN
F 2 "" H 6200 2350 50  0001 C CNN
F 3 "" H 6200 2350 50  0001 C CNN
	1    6200 2350
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5B7077BD
P 5200 1800
F 0 "C1" H 5225 1900 50  0000 L CNN
F 1 "0.1uF" H 5225 1700 50  0000 L CNN
F 2 "" H 5238 1650 50  0001 C CNN
F 3 "" H 5200 1800 50  0001 C CNN
	1    5200 1800
	1    0    0    -1  
$EndComp
$Comp
L BB135 VD2
U 1 1 5B707DE6
P 5250 2350
F 0 "VD2" H 5100 2450 60  0000 C CNN
F 1 "BB135" H 5450 2450 60  0000 C CNN
F 2 "" H 5300 2400 60  0001 C CNN
F 3 "" H 5300 2400 60  0001 C CNN
	1    5250 2350
	0    1    1    0   
$EndComp
$Comp
L GND #PWR5
U 1 1 5B707F0D
P 5200 2750
F 0 "#PWR5" H 5200 2500 50  0001 C CNN
F 1 "GND" H 5200 2600 50  0000 C CNN
F 2 "" H 5200 2750 50  0001 C CNN
F 3 "" H 5200 2750 50  0001 C CNN
	1    5200 2750
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5B71ED93
P 4900 1750
F 0 "R1" V 4980 1750 50  0000 C CNN
F 1 "100k" V 4900 1750 50  0000 C CNN
F 2 "" V 4830 1750 50  0001 C CNN
F 3 "" H 4900 1750 50  0001 C CNN
	1    4900 1750
	-1   0    0    1   
$EndComp
$Comp
L BB135 VD1
U 1 1 5B71EF87
P 4950 2350
F 0 "VD1" H 4800 2450 60  0000 C CNN
F 1 "BB135" H 5150 2450 60  0000 C CNN
F 2 "" H 5000 2400 60  0001 C CNN
F 3 "" H 5000 2400 60  0001 C CNN
	1    4950 2350
	0    1    1    0   
$EndComp
$Comp
L GND #PWR4
U 1 1 5B71F01F
P 4900 2750
F 0 "#PWR4" H 4900 2500 50  0001 C CNN
F 1 "GND" H 4900 2600 50  0000 C CNN
F 2 "" H 4900 2750 50  0001 C CNN
F 3 "" H 4900 2750 50  0001 C CNN
	1    4900 2750
	1    0    0    -1  
$EndComp
$Comp
L AD8542 DA1
U 1 1 5B720144
P 4200 2000
F 0 "DA1" H 3975 2275 60  0000 C CNN
F 1 "AD8542" H 4425 2275 60  0000 C CNN
F 2 "" H 4225 2350 60  0001 C CNN
F 3 "" H 4225 2350 60  0001 C CNN
	1    4200 2000
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR15
U 1 1 5B720252
P 7850 900
F 0 "#PWR15" H 7850 750 50  0001 C CNN
F 1 "+5V" H 7850 1040 50  0000 C CNN
F 2 "" H 7850 900 50  0001 C CNN
F 3 "" H 7850 900 50  0001 C CNN
	1    7850 900 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR3
U 1 1 5B720550
P 4225 2350
F 0 "#PWR3" H 4225 2100 50  0001 C CNN
F 1 "GND" H 4225 2200 50  0000 C CNN
F 2 "" H 4225 2350 50  0001 C CNN
F 3 "" H 4225 2350 50  0001 C CNN
	1    4225 2350
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR2
U 1 1 5B72067F
P 4225 1075
F 0 "#PWR2" H 4225 925 50  0001 C CNN
F 1 "+5V" H 4225 1215 50  0000 C CNN
F 2 "" H 4225 1075 50  0001 C CNN
F 3 "" H 4225 1075 50  0001 C CNN
	1    4225 1075
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x02 J1
U 1 1 5B720BFC
P 3525 2075
F 0 "J1" H 3525 2175 50  0000 C CNN
F 1 "Conn_01x02" H 3525 1875 50  0000 C CNN
F 2 "" H 3525 2075 50  0001 C CNN
F 3 "" H 3525 2075 50  0001 C CNN
	1    3525 2075
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR1
U 1 1 5B720EBD
P 3725 2350
F 0 "#PWR1" H 3725 2100 50  0001 C CNN
F 1 "GND" H 3725 2200 50  0000 C CNN
F 2 "" H 3725 2350 50  0001 C CNN
F 3 "" H 3725 2350 50  0001 C CNN
	1    3725 2350
	1    0    0    -1  
$EndComp
Text Notes 3225 2150 0    60   ~ 0
Tune
$Comp
L Conn_01x02 J2
U 1 1 5B7237EE
P 8600 1050
F 0 "J2" H 8600 1150 50  0000 C CNN
F 1 "Conn_01x02" H 8600 850 50  0000 C CNN
F 2 "" H 8600 1050 50  0001 C CNN
F 3 "" H 8600 1050 50  0001 C CNN
	1    8600 1050
	1    0    0    -1  
$EndComp
$Comp
L AD8031 DA2
U 1 1 5B725D0B
P 8825 1950
F 0 "DA2" H 8675 2200 60  0000 C CNN
F 1 "AD8031" H 9050 2200 60  0000 C CNN
F 2 "" H 8825 2325 60  0001 C CNN
F 3 "" H 8825 2325 60  0001 C CNN
	1    8825 1950
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 5B7264A0
P 8400 2525
F 0 "R7" V 8480 2525 50  0000 C CNN
F 1 "R" V 8400 2525 50  0000 C CNN
F 2 "" V 8330 2525 50  0001 C CNN
F 3 "" H 8400 2525 50  0001 C CNN
	1    8400 2525
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 5B7266B2
P 9050 2325
F 0 "R8" V 9130 2325 50  0000 C CNN
F 1 "R" V 9050 2325 50  0000 C CNN
F 2 "" V 8980 2325 50  0001 C CNN
F 3 "" H 9050 2325 50  0001 C CNN
	1    9050 2325
	0    1    1    0   
$EndComp
$Comp
L GND #PWR19
U 1 1 5B726A3E
P 8850 2375
F 0 "#PWR19" H 8850 2125 50  0001 C CNN
F 1 "GND" H 8850 2225 50  0000 C CNN
F 2 "" H 8850 2375 50  0001 C CNN
F 3 "" H 8850 2375 50  0001 C CNN
	1    8850 2375
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR18
U 1 1 5B726D2A
P 8850 1550
F 0 "#PWR18" H 8850 1400 50  0001 C CNN
F 1 "+5V" H 8850 1690 50  0000 C CNN
F 2 "" H 8850 1550 50  0001 C CNN
F 3 "" H 8850 1550 50  0001 C CNN
	1    8850 1550
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x02 J3
U 1 1 5B726E8F
P 9600 2325
F 0 "J3" H 9600 2425 50  0000 C CNN
F 1 "Conn_01x02" H 9600 2125 50  0000 C CNN
F 2 "" H 9600 2325 50  0001 C CNN
F 3 "" H 9600 2325 50  0001 C CNN
	1    9600 2325
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR20
U 1 1 5B72709E
P 9400 2600
F 0 "#PWR20" H 9400 2350 50  0001 C CNN
F 1 "GND" H 9400 2450 50  0000 C CNN
F 2 "" H 9400 2600 50  0001 C CNN
F 3 "" H 9400 2600 50  0001 C CNN
	1    9400 2600
	1    0    0    -1  
$EndComp
Text Notes 9700 2400 0    60   ~ 0
Out
$Comp
L GND #PWR6
U 1 1 5B7892C1
P 5600 2350
F 0 "#PWR6" H 5600 2100 50  0001 C CNN
F 1 "GND" H 5600 2200 50  0000 C CNN
F 2 "" H 5600 2350 50  0001 C CNN
F 3 "" H 5600 2350 50  0001 C CNN
	1    5600 2350
	1    0    0    -1  
$EndComp
$Comp
L D D1
U 1 1 5B7896EB
P 5900 1900
F 0 "D1" H 5900 2000 50  0000 C CNN
F 1 "1N4148" H 5900 1800 50  0000 C CNN
F 2 "" H 5900 1900 50  0001 C CNN
F 3 "" H 5900 1900 50  0001 C CNN
	1    5900 1900
	0    -1   1    0   
$EndComp
$Comp
L D D2
U 1 1 5B78975C
P 6200 1900
F 0 "D2" H 6200 2000 50  0000 C CNN
F 1 "1N4148" H 6200 1800 50  0000 C CNN
F 2 "" H 6200 1900 50  0001 C CNN
F 3 "" H 6200 1900 50  0001 C CNN
	1    6200 1900
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR8
U 1 1 5B789BB4
P 5900 2350
F 0 "#PWR8" H 5900 2100 50  0001 C CNN
F 1 "GND" H 5900 2200 50  0000 C CNN
F 2 "" H 5900 2350 50  0001 C CNN
F 3 "" H 5900 2350 50  0001 C CNN
	1    5900 2350
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5B78B0BF
P 7500 2550
F 0 "R5" V 7580 2550 50  0000 C CNN
F 1 "470" V 7500 2550 50  0000 C CNN
F 2 "" V 7430 2550 50  0001 C CNN
F 3 "" H 7500 2550 50  0001 C CNN
	1    7500 2550
	-1   0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5B78B2F8
P 7800 2350
F 0 "C7" H 7825 2450 50  0000 L CNN
F 1 "0.1uF" H 7825 2250 50  0000 L CNN
F 2 "" H 7838 2200 50  0001 C CNN
F 3 "" H 7800 2350 50  0001 C CNN
	1    7800 2350
	0    -1   -1   0   
$EndComp
$Comp
L R R6
U 1 1 5B78B691
P 8050 2550
F 0 "R6" V 8130 2550 50  0000 C CNN
F 1 "47k" V 8050 2550 50  0000 C CNN
F 2 "" V 7980 2550 50  0001 C CNN
F 3 "" H 8050 2550 50  0001 C CNN
	1    8050 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 1050 8400 1050
Wire Wire Line
	7850 900  7850 1150
Wire Wire Line
	7850 1500 7850 1450
Wire Wire Line
	8400 1150 8100 1150
Wire Wire Line
	8100 1150 8100 1500
Wire Wire Line
	7500 1050 7500 1450
Connection ~ 7850 1050
Wire Wire Line
	7500 2000 7500 1650
Wire Wire Line
	7000 2000 7000 1900
Connection ~ 7500 1950
Connection ~ 7000 1950
Wire Wire Line
	7000 1600 7000 1550
Connection ~ 7000 1550
Wire Wire Line
	6750 1800 6750 1550
Connection ~ 6750 1550
Wire Wire Line
	7000 1950 7500 1950
Wire Wire Line
	6750 2100 6750 2350
Wire Wire Line
	6200 2050 6200 2350
Wire Wire Line
	5200 1550 5200 1650
Wire Wire Line
	5200 2100 5200 1950
Wire Wire Line
	5200 2750 5200 2650
Wire Wire Line
	4900 2650 4900 2750
Wire Wire Line
	4900 1900 4900 2100
Connection ~ 5200 2000
Connection ~ 4900 2000
Wire Wire Line
	4900 2000 5200 2000
Wire Wire Line
	4900 1550 4900 1600
Wire Wire Line
	4600 2000 4650 2000
Wire Wire Line
	4650 2000 4650 1550
Wire Wire Line
	3875 1550 4900 1550
Wire Wire Line
	4225 2300 4225 2350
Wire Wire Line
	4225 1075 4225 1700
Wire Wire Line
	3875 1925 3875 1550
Connection ~ 4650 1550
Wire Wire Line
	3875 2075 3725 2075
Wire Wire Line
	3725 2350 3725 2175
Wire Wire Line
	9200 2325 9400 2325
Wire Wire Line
	8900 2325 8400 2325
Connection ~ 8400 2325
Connection ~ 9300 2325
Wire Wire Line
	9400 2600 9400 2425
Wire Wire Line
	9300 2325 9300 1950
Wire Wire Line
	9300 1950 9175 1950
Wire Wire Line
	8850 2250 8850 2375
Wire Wire Line
	8850 1675 8850 1550
Wire Wire Line
	8200 2025 8500 2025
Wire Wire Line
	8500 1875 8400 1875
Wire Wire Line
	8400 1875 8400 2375
Wire Wire Line
	5600 1825 5600 1550
Connection ~ 5600 1550
Wire Wire Line
	5600 2125 5600 2350
Wire Wire Line
	6650 1550 7100 1550
Wire Wire Line
	5200 1550 6350 1550
Wire Wire Line
	5900 2350 5900 2050
Wire Wire Line
	6200 1750 6200 1550
Wire Wire Line
	5900 1750 5900 1550
Connection ~ 6200 1550
Connection ~ 5900 1550
Wire Wire Line
	7500 2750 7500 2700
Wire Wire Line
	7500 2400 7500 2300
Connection ~ 7500 2350
Wire Wire Line
	7000 2350 7650 2350
Connection ~ 8050 2350
Wire Wire Line
	8200 2025 8200 2350
Wire Wire Line
	8200 2350 7950 2350
$Comp
L L_Core_Ferrite L1
U 1 1 5B78C44C
P 5600 1975
F 0 "L1" V 5550 1975 50  0000 C CNN
F 1 "27 uH" V 5710 1975 50  0000 C CNN
F 2 "" H 5600 1975 50  0001 C CNN
F 3 "" H 5600 1975 50  0001 C CNN
	1    5600 1975
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7000 2300 7000 2350
Wire Wire Line
	8050 2400 8050 2350
$Comp
L AD8542 DA1
U 2 1 5B789D7D
P 6700 3475
F 0 "DA1" H 6475 3750 60  0000 C CNN
F 1 "AD8542" H 6925 3750 60  0000 C CNN
F 2 "" H 6725 3825 60  0001 C CNN
F 3 "" H 6725 3825 60  0001 C CNN
	2    6700 3475
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5B789E6A
P 6100 3350
F 0 "R2" V 6180 3350 50  0000 C CNN
F 1 "47k" V 6100 3350 50  0000 C CNN
F 2 "" V 6030 3350 50  0001 C CNN
F 3 "" H 6100 3350 50  0001 C CNN
	1    6100 3350
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5B789F61
P 6100 3750
F 0 "R3" V 6180 3750 50  0000 C CNN
F 1 "47k" V 6100 3750 50  0000 C CNN
F 2 "" V 6030 3750 50  0001 C CNN
F 3 "" H 6100 3750 50  0001 C CNN
	1    6100 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6375 3400 6300 3400
Wire Wire Line
	6300 3400 6300 3025
Wire Wire Line
	6300 3025 7150 3025
Wire Wire Line
	7150 3025 7150 3625
Wire Wire Line
	7100 3475 8400 3475
Wire Wire Line
	6100 3600 6100 3500
Wire Wire Line
	5800 3550 6375 3550
Connection ~ 6100 3550
$Comp
L GND #PWR10
U 1 1 5B78A2C6
P 6100 3975
F 0 "#PWR10" H 6100 3725 50  0001 C CNN
F 1 "GND" H 6100 3825 50  0000 C CNN
F 2 "" H 6100 3975 50  0001 C CNN
F 3 "" H 6100 3975 50  0001 C CNN
	1    6100 3975
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 3975 6100 3900
$Comp
L +5V #PWR9
U 1 1 5B78A378
P 6100 3100
F 0 "#PWR9" H 6100 2950 50  0001 C CNN
F 1 "+5V" H 6100 3240 50  0000 C CNN
F 2 "" H 6100 3100 50  0001 C CNN
F 3 "" H 6100 3100 50  0001 C CNN
	1    6100 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 3200 6100 3100
$Comp
L C C2
U 1 1 5B78A998
P 5800 3775
F 0 "C2" H 5825 3875 50  0000 L CNN
F 1 "0.1uF" H 5825 3675 50  0000 L CNN
F 2 "" H 5838 3625 50  0001 C CNN
F 3 "" H 5800 3775 50  0001 C CNN
	1    5800 3775
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR7
U 1 1 5B78AA1B
P 5800 3975
F 0 "#PWR7" H 5800 3725 50  0001 C CNN
F 1 "GND" H 5800 3825 50  0000 C CNN
F 2 "" H 5800 3975 50  0001 C CNN
F 3 "" H 5800 3975 50  0001 C CNN
	1    5800 3975
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3975 5800 3925
Wire Wire Line
	5800 3625 5800 3550
Wire Wire Line
	8050 3475 8050 2700
Connection ~ 7150 3475
Wire Wire Line
	8400 3475 8400 2675
Connection ~ 8050 3475
$Comp
L C C6
U 1 1 5B78B961
P 7150 3775
F 0 "C6" H 7175 3875 50  0000 L CNN
F 1 "0.1uF" H 7175 3675 50  0000 L CNN
F 2 "" H 7188 3625 50  0001 C CNN
F 3 "" H 7150 3775 50  0001 C CNN
	1    7150 3775
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR13
U 1 1 5B78B9B8
P 7150 4000
F 0 "#PWR13" H 7150 3750 50  0001 C CNN
F 1 "GND" H 7150 3850 50  0000 C CNN
F 2 "" H 7150 4000 50  0001 C CNN
F 3 "" H 7150 4000 50  0001 C CNN
	1    7150 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 4000 7150 3925
$EndSCHEMATC
