EESchema Schematic File Version 2
LIBS:power
LIBS:device
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
LIBS:VCO-testboard-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "VCO Testboard (GPLv3+)"
Date "2018-08-12"
Rev "2"
Comp "White Fossa aka Artyom Vetrov"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 8675 1075 0    60   ~ 0
+12V
Text Notes 8675 1200 0    60   ~ 0
GND
$Comp
L GND #PWR01
U 1 1 5B706542
P 8100 1500
F 0 "#PWR01" H 8100 1250 50  0001 C CNN
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
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7888 1150 50  0001 C CNN
F 3 "" H 7850 1300 50  0001 C CNN
	1    7850 1300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5B706662
P 7850 1500
F 0 "#PWR02" H 7850 1250 50  0001 C CNN
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
F 2 "TO_SOT_Packages_SMD:SOT-23" H 7350 1750 60  0001 C CNN
F 3 "" H 7350 1750 60  0001 C CNN
	1    7350 1750
	1    0    0    1   
$EndComp
$Comp
L GND #PWR03
U 1 1 5B7069A3
P 7500 2350
F 0 "#PWR03" H 7500 2100 50  0001 C CNN
F 1 "GND" H 7500 2200 50  0000 C CNN
F 2 "" H 7500 2350 50  0001 C CNN
F 3 "" H 7500 2350 50  0001 C CNN
	1    7500 2350
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5B706A22
P 7000 1750
F 0 "C4" H 7025 1850 50  0000 L CNN
F 1 "560pF" H 7025 1650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7038 1600 50  0001 C CNN
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
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7038 2000 50  0001 C CNN
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
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6680 1950 50  0001 C CNN
F 3 "" H 6750 1950 50  0001 C CNN
	1    6750 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5B706D03
P 6750 2350
F 0 "#PWR04" H 6750 2100 50  0001 C CNN
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
F 1 "100pF" H 6525 1450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 6538 1400 50  0001 C CNN
F 3 "" H 6500 1550 50  0001 C CNN
	1    6500 1550
	0    1    1    0   
$EndComp
$Comp
L GND #PWR05
U 1 1 5B707677
P 6200 2350
F 0 "#PWR05" H 6200 2100 50  0001 C CNN
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
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5238 1650 50  0001 C CNN
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
F 2 "Diodes_SMD:D_0805" H 5300 2400 60  0001 C CNN
F 3 "" H 5300 2400 60  0001 C CNN
	1    5250 2350
	0    1    1    0   
$EndComp
$Comp
L GND #PWR06
U 1 1 5B707F0D
P 5200 2750
F 0 "#PWR06" H 5200 2500 50  0001 C CNN
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
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4830 1750 50  0001 C CNN
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
F 2 "Diodes_SMD:D_0805" H 5000 2400 60  0001 C CNN
F 3 "" H 5000 2400 60  0001 C CNN
	1    4950 2350
	0    1    1    0   
$EndComp
$Comp
L GND #PWR07
U 1 1 5B71F01F
P 4900 2750
F 0 "#PWR07" H 4900 2500 50  0001 C CNN
F 1 "GND" H 4900 2600 50  0000 C CNN
F 2 "" H 4900 2750 50  0001 C CNN
F 3 "" H 4900 2750 50  0001 C CNN
	1    4900 2750
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 J1
U 1 1 5B720BFC
P 4450 1600
F 0 "J1" H 4450 1700 50  0000 C CNN
F 1 "Conn_01x02" H 4450 1400 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" H 4450 1600 50  0001 C CNN
F 3 "" H 4450 1600 50  0001 C CNN
	1    4450 1600
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 5B720EBD
P 4650 1825
F 0 "#PWR08" H 4650 1575 50  0001 C CNN
F 1 "GND" H 4650 1675 50  0000 C CNN
F 2 "" H 4650 1825 50  0001 C CNN
F 3 "" H 4650 1825 50  0001 C CNN
	1    4650 1825
	1    0    0    -1  
$EndComp
Text Notes 4150 1650 0    60   ~ 0
Tune
$Comp
L CONN_01X02 J2
U 1 1 5B7237EE
P 8600 1100
F 0 "J2" H 8600 1200 50  0000 C CNN
F 1 "Conn_01x02" H 8600 900 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" H 8600 1100 50  0001 C CNN
F 3 "" H 8600 1100 50  0001 C CNN
	1    8600 1100
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 J4
U 1 1 5B726E8F
P 9600 2375
F 0 "J4" H 9600 2475 50  0000 C CNN
F 1 "Conn_01x02" H 9600 2175 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" H 9600 2375 50  0001 C CNN
F 3 "" H 9600 2375 50  0001 C CNN
	1    9600 2375
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 5B72709E
P 9400 2600
F 0 "#PWR09" H 9400 2350 50  0001 C CNN
F 1 "GND" H 9400 2450 50  0000 C CNN
F 2 "" H 9400 2600 50  0001 C CNN
F 3 "" H 9400 2600 50  0001 C CNN
	1    9400 2600
	1    0    0    -1  
$EndComp
Text Notes 9700 2400 0    60   ~ 0
Out
$Comp
L GND #PWR010
U 1 1 5B7892C1
P 5600 2350
F 0 "#PWR010" H 5600 2100 50  0001 C CNN
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
F 2 "Diodes_SMD:D_0805" H 5900 1900 50  0001 C CNN
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
F 2 "Diodes_SMD:D_0805" H 6200 1900 50  0001 C CNN
F 3 "" H 6200 1900 50  0001 C CNN
	1    6200 1900
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR011
U 1 1 5B789BB4
P 5900 2350
F 0 "#PWR011" H 5900 2100 50  0001 C CNN
F 1 "GND" H 5900 2200 50  0000 C CNN
F 2 "" H 5900 2350 50  0001 C CNN
F 3 "" H 5900 2350 50  0001 C CNN
	1    5900 2350
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5B78B0BF
P 7500 2150
F 0 "R5" V 7580 2150 50  0000 C CNN
F 1 "470" V 7500 2150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7430 2150 50  0001 C CNN
F 3 "" H 7500 2150 50  0001 C CNN
	1    7500 2150
	-1   0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 5B78B2F8
P 7750 1950
F 0 "C7" H 7775 2050 50  0000 L CNN
F 1 "0.1uF" H 7775 1850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7788 1800 50  0001 C CNN
F 3 "" H 7750 1950 50  0001 C CNN
	1    7750 1950
	0    -1   -1   0   
$EndComp
$Comp
L L_Core_Ferrite L1
U 1 1 5B78C44C
P 5600 1975
F 0 "L1" V 5550 1975 50  0000 C CNN
F 1 "21 uH" V 5710 1975 50  0000 C CNN
F 2 "Inductors_NEOSID:Neosid_Inductor_Sd8" H 5600 1975 50  0001 C CNN
F 3 "" H 5600 1975 50  0001 C CNN
	1    5600 1975
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7500 1050 8400 1050
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
	7000 2000 7000 1900
Connection ~ 7000 1950
Wire Wire Line
	7000 1600 7000 1550
Connection ~ 7000 1550
Wire Wire Line
	6750 1800 6750 1550
Connection ~ 6750 1550
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
	9400 2600 9400 2425
Wire Wire Line
	5600 1825 5600 1550
Connection ~ 5600 1550
Wire Wire Line
	5600 2125 5600 2350
Wire Wire Line
	6650 1550 7100 1550
Wire Wire Line
	5900 2350 5900 2050
Wire Wire Line
	6200 1750 6200 1550
Wire Wire Line
	5900 1750 5900 1550
Connection ~ 6200 1550
Connection ~ 5900 1550
Wire Wire Line
	7500 2350 7500 2300
Wire Wire Line
	7000 2300 7000 2350
Wire Wire Line
	4650 1650 4650 1825
Wire Wire Line
	7850 900  7850 1150
$Comp
L +12V #PWR012
U 1 1 5BA019D0
P 7850 900
F 0 "#PWR012" H 7850 750 50  0001 C CNN
F 1 "+12V" H 7850 1040 50  0000 C CNN
F 2 "" H 7850 900 50  0000 C CNN
F 3 "" H 7850 900 50  0000 C CNN
	1    7850 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 1875 8400 1875
Wire Wire Line
	9300 1950 9175 1950
Wire Wire Line
	8850 2250 8850 2375
$Comp
L GND #PWR013
U 1 1 5B726A3E
P 8850 2375
F 0 "#PWR013" H 8850 2125 50  0001 C CNN
F 1 "GND" H 8850 2225 50  0000 C CNN
F 2 "" H 8850 2375 50  0001 C CNN
F 3 "" H 8850 2375 50  0001 C CNN
	1    8850 2375
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 5B7266B2
P 9050 2325
F 0 "R9" V 9130 2325 50  0000 C CNN
F 1 "47k" V 9050 2325 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 8980 2325 50  0001 C CNN
F 3 "" H 9050 2325 50  0001 C CNN
	1    9050 2325
	-1   0    0    1   
$EndComp
Wire Wire Line
	9300 2325 9300 1950
Wire Wire Line
	8850 1675 8850 1550
$Comp
L +12V #PWR014
U 1 1 5BA01A3C
P 8850 1550
F 0 "#PWR014" H 8850 1400 50  0001 C CNN
F 1 "+12V" H 8850 1690 50  0000 C CNN
F 2 "" H 8850 1550 50  0000 C CNN
F 3 "" H 8850 1550 50  0000 C CNN
	1    8850 1550
	1    0    0    -1  
$EndComp
$Comp
L AD8031 DA1
U 1 1 5B725D0B
P 8825 1950
F 0 "DA1" H 8675 2200 60  0000 C CNN
F 1 "AD8031" H 9050 2200 60  0000 C CNN
F 2 "SMD_Packages:SOIC-8-N" H 8825 2325 60  0001 C CNN
F 3 "" H 8825 2325 60  0001 C CNN
	1    8825 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 1875 8400 2375
$Comp
L R R7
U 1 1 5B7264A0
P 8400 2525
F 0 "R7" V 8480 2525 50  0000 C CNN
F 1 "4.7k" V 8400 2525 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 8330 2525 50  0001 C CNN
F 3 "" H 8400 2525 50  0001 C CNN
	1    8400 2525
	1    0    0    -1  
$EndComp
Connection ~ 8050 3475
Wire Wire Line
	8400 3475 8400 2675
Wire Wire Line
	8050 3475 8050 2700
$Comp
L R R6
U 1 1 5B78B691
P 8050 2550
F 0 "R6" V 8130 2550 50  0000 C CNN
F 1 "47k" V 8050 2550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7980 2550 50  0001 C CNN
F 3 "" H 8050 2550 50  0001 C CNN
	1    8050 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3975 5800 3925
Wire Wire Line
	6100 3975 6100 3900
$Comp
L GND #PWR015
U 1 1 5B78A2C6
P 6100 3975
F 0 "#PWR015" H 6100 3725 50  0001 C CNN
F 1 "GND" H 6100 3825 50  0000 C CNN
F 2 "" H 6100 3975 50  0001 C CNN
F 3 "" H 6100 3975 50  0001 C CNN
	1    6100 3975
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 5B78AA1B
P 5800 3975
F 0 "#PWR016" H 5800 3725 50  0001 C CNN
F 1 "GND" H 5800 3825 50  0000 C CNN
F 2 "" H 5800 3975 50  0001 C CNN
F 3 "" H 5800 3975 50  0001 C CNN
	1    5800 3975
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3625 5800 3550
$Comp
L C C2
U 1 1 5B78A998
P 5800 3775
F 0 "C2" H 5825 3875 50  0000 L CNN
F 1 "0.1uF" H 5825 3675 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5838 3625 50  0001 C CNN
F 3 "" H 5800 3775 50  0001 C CNN
	1    5800 3775
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 3600 6100 3500
Connection ~ 6100 3550
Wire Wire Line
	5800 3550 6325 3550
$Comp
L R R3
U 1 1 5B789F61
P 6100 3750
F 0 "R3" V 6180 3750 50  0000 C CNN
F 1 "47k" V 6100 3750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6030 3750 50  0001 C CNN
F 3 "" H 6100 3750 50  0001 C CNN
	1    6100 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 3200 6100 3100
$Comp
L R R2
U 1 1 5B789E6A
P 6100 3350
F 0 "R2" V 6180 3350 50  0000 C CNN
F 1 "47k" V 6100 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6030 3350 50  0001 C CNN
F 3 "" H 6100 3350 50  0001 C CNN
	1    6100 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6325 3550 6325 3375
Wire Wire Line
	6500 3575 6425 3575
Wire Wire Line
	6325 3375 6500 3375
Wire Wire Line
	6425 3575 6425 3025
$Comp
L +12V #PWR017
U 1 1 5BA01AA8
P 6100 3100
F 0 "#PWR017" H 6100 2950 50  0001 C CNN
F 1 "+12V" H 6100 3240 50  0000 C CNN
F 2 "" H 6100 3100 50  0000 C CNN
F 3 "" H 6100 3100 50  0000 C CNN
	1    6100 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6425 3025 7150 3025
Connection ~ 7150 3475
$Comp
L LM358 U1
U 1 1 5B9FF4A3
P 6800 3475
F 0 "U1" H 6800 3675 50  0000 L CNN
F 1 "LM358" H 6800 3275 50  0000 L CNN
F 2 "SMD_Packages:SOIC-8-N" H 6800 3475 50  0001 C CNN
F 3 "" H 6800 3475 50  0000 C CNN
	1    6800 3475
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 3475 8500 3475
Wire Wire Line
	7500 1650 7500 2000
Wire Wire Line
	7000 1950 7600 1950
Connection ~ 7500 1950
$Comp
L GND #PWR019
U 1 1 5BA6DD15
P 7000 2350
F 0 "#PWR019" H 7000 2100 50  0001 C CNN
F 1 "GND" H 7000 2200 50  0000 C CNN
F 2 "" H 7000 2350 50  0000 C CNN
F 3 "" H 7000 2350 50  0000 C CNN
	1    7000 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 2400 8050 1950
Wire Wire Line
	7900 1950 8200 1950
Wire Wire Line
	8200 1950 8200 2025
Wire Wire Line
	8200 2025 8500 2025
Connection ~ 8050 1950
Wire Wire Line
	6350 1550 5200 1550
Wire Wire Line
	4900 1550 4650 1550
$Comp
L GND #PWR020
U 1 1 5BA77999
P 8500 3675
F 0 "#PWR020" H 8500 3425 50  0001 C CNN
F 1 "GND" H 8500 3525 50  0000 C CNN
F 2 "" H 8500 3675 50  0000 C CNN
F 3 "" H 8500 3675 50  0000 C CNN
	1    8500 3675
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 3675 8500 3575
Connection ~ 8400 3475
$Comp
L CONN_01X02 J3
U 1 1 5BA77BC4
P 8700 3525
F 0 "J3" H 8700 3625 50  0000 C CNN
F 1 "Conn_01x02" H 8700 3325 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" H 8700 3525 50  0001 C CNN
F 3 "" H 8700 3525 50  0001 C CNN
	1    8700 3525
	1    0    0    -1  
$EndComp
Text Notes 9875 2425 0    60   ~ 0
1.7V peak to peak\n3.5MHz
Text Notes 8800 3550 0    60   ~ 0
Signal ground - 6V
$Comp
L GND #PWR021
U 1 1 5BB0F92B
P 6700 3975
F 0 "#PWR021" H 6700 3725 50  0001 C CNN
F 1 "GND" H 6700 3825 50  0000 C CNN
F 2 "" H 6700 3975 50  0000 C CNN
F 3 "" H 6700 3975 50  0000 C CNN
	1    6700 3975
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3775 6700 3975
$Comp
L +12V #PWR022
U 1 1 5BB0FB8C
P 6700 2950
F 0 "#PWR022" H 6700 2800 50  0001 C CNN
F 1 "+12V" H 6700 3090 50  0000 C CNN
F 2 "" H 6700 2950 50  0000 C CNN
F 3 "" H 6700 2950 50  0000 C CNN
	1    6700 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3175 6700 2950
Wire Wire Line
	9300 2325 9400 2325
Wire Wire Line
	8600 2125 8600 2600
Wire Wire Line
	8600 2125 8400 2125
Connection ~ 8400 2125
Wire Wire Line
	8600 2600 9050 2600
Wire Wire Line
	9050 2600 9050 2475
Wire Wire Line
	9050 2175 9050 2125
Wire Wire Line
	9050 2125 9300 2125
Connection ~ 9300 2125
Wire Wire Line
	7150 3025 7150 3475
Text Notes 4250 1400 0    60   ~ 0
0V - 3.115 MHz\n2.8V - 3.5 MHz\n4.3V - 3.575 MHz\n6.3V - 3.65 MHz\n12V - 3.769 MHz
$EndSCHEMATC
