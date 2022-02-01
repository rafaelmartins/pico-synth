EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "pico-synth"
Date "2022-02-01"
Rev "2.1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 "Released under CERN Open Hardware Licence Version 2 - Strongly Reciprocal"
Comment4 "Designed by: Rafael G. Martins"
$EndDescr
$Comp
L Regulator_SwitchedCapacitor:LTC1044 U5
U 1 1 60D4B11D
P 5500 3100
F 0 "U5" H 5500 3667 50  0000 C CNN
F 1 "LTC1044" H 5500 3576 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 5600 3000 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/lt1044.pdf" H 5600 3000 50  0001 C CNN
	1    5500 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C7
U 1 1 60D4C66E
P 5950 3200
F 0 "C7" H 6068 3246 50  0000 L CNN
F 1 "10u" H 6068 3155 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 5988 3050 50  0001 C CNN
F 3 "~" H 5950 3200 50  0001 C CNN
	1    5950 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C9
U 1 1 60D4D02B
P 6300 3000
F 0 "C9" H 6182 2954 50  0000 R CNN
F 1 "10u" H 6182 3045 50  0000 R CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 6338 2850 50  0001 C CNN
F 3 "~" H 6300 3000 50  0001 C CNN
	1    6300 3000
	-1   0    0    1   
$EndComp
$Comp
L Device:CP C1
U 1 1 60D4FDD6
P 4900 3000
F 0 "C1" H 4782 3046 50  0000 R CNN
F 1 "10u" H 4782 2955 50  0000 R CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 4938 2850 50  0001 C CNN
F 3 "~" H 4900 3000 50  0001 C CNN
	1    4900 3000
	1    0    0    -1  
$EndComp
$Comp
L Analog_DAC:MCP4822 U4
U 1 1 60D64751
P 5450 4650
F 0 "U4" H 5750 5150 50  0000 C CNN
F 1 "MCP4822" H 5750 5050 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 6250 4350 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20002249B.pdf" H 6250 4350 50  0001 C CNN
	1    5450 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 60D74D65
P 5450 5250
F 0 "#PWR017" H 5450 5000 50  0001 C CNN
F 1 "GND" H 5455 5077 50  0000 C CNN
F 2 "" H 5450 5250 50  0001 C CNN
F 3 "" H 5450 5250 50  0001 C CNN
	1    5450 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 3350 7700 3350
$Comp
L Amplifier_Operational:TL072 U7
U 1 1 60D9EB3D
P 8050 4350
F 0 "U7" H 8050 4717 50  0000 C CNN
F 1 "TL072" H 8050 4626 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 8050 4350 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 8050 4350 50  0001 C CNN
	1    8050 4350
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL072 U7
U 2 1 60DA0A5F
P 8050 5650
F 0 "U7" H 8050 6017 50  0000 C CNN
F 1 "TL072" H 8050 5926 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 8050 5650 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 8050 5650 50  0001 C CNN
	2    8050 5650
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TL072 U7
U 3 1 60DA2783
P 9200 3100
F 0 "U7" H 9158 3146 50  0000 L CNN
F 1 "TL072" H 9158 3055 50  0000 L CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 9200 3100 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 9200 3100 50  0001 C CNN
	3    9200 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 60DC412E
P 8100 4750
F 0 "R9" V 7893 4750 50  0000 C CNN
F 1 "39k" V 7984 4750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8030 4750 50  0001 C CNN
F 3 "~" H 8100 4750 50  0001 C CNN
	1    8100 4750
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 60DCFB80
P 7500 4750
F 0 "R5" V 7385 4750 50  0000 C CNN
F 1 "56k" V 7294 4750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7430 4750 50  0001 C CNN
F 3 "~" H 7500 4750 50  0001 C CNN
	1    7500 4750
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 60E6BEC6
P 2750 5200
F 0 "R1" V 2957 5200 50  0000 C CNN
F 1 "220R" V 2866 5200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2680 5200 50  0001 C CNN
F 3 "~" H 2750 5200 50  0001 C CNN
	1    2750 5200
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 60EE9461
P 3900 5200
F 0 "R2" H 3970 5246 50  0000 L CNN
F 1 "10k" H 3970 5155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3830 5200 50  0001 C CNN
F 3 "~" H 3900 5200 50  0001 C CNN
	1    3900 5200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 60FF03CD
P 3900 6450
F 0 "J3" H 3818 6767 50  0000 C CNN
F 1 "JST XH 4P" H 3818 6676 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 3900 6450 50  0001 C CNN
F 3 "~" H 3900 6450 50  0001 C CNN
	1    3900 6450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4100 6350 4150 6350
$Comp
L power:GND #PWR09
U 1 1 61018E42
P 4150 6750
F 0 "#PWR09" H 4150 6500 50  0001 C CNN
F 1 "GND" H 4155 6577 50  0000 C CNN
F 2 "" H 4150 6750 50  0001 C CNN
F 3 "" H 4150 6750 50  0001 C CNN
	1    4150 6750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 61051D65
P 7150 4050
F 0 "R3" V 6943 4050 50  0000 C CNN
F 1 "470R" V 7034 4050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7080 4050 50  0001 C CNN
F 3 "~" H 7150 4050 50  0001 C CNN
	1    7150 4050
	0    1    1    0   
$EndComp
$Comp
L Device:C C12
U 1 1 6106D5EA
P 7350 4250
F 0 "C12" H 7465 4296 50  0000 L CNN
F 1 "4n7" H 7465 4205 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 7388 4100 50  0001 C CNN
F 3 "~" H 7350 4250 50  0001 C CNN
	1    7350 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 6104837D
P 7600 3350
F 0 "R8" H 7531 3396 50  0000 R CNN
F 1 "10k" H 7531 3305 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7530 3350 50  0001 C CNN
F 3 "~" H 7600 3350 50  0001 C CNN
	1    7600 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 61049366
P 7600 2950
F 0 "R7" H 7531 2996 50  0000 R CNN
F 1 "22k" H 7531 2905 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7530 2950 50  0001 C CNN
F 3 "~" H 7600 2950 50  0001 C CNN
	1    7600 2950
	1    0    0    -1  
$EndComp
$Comp
L Memory_EEPROM:24LC512 U3
U 1 1 610D3B82
P 5350 6150
F 0 "U3" H 5050 6550 50  0000 C CNN
F 1 "24LC512" H 5050 6450 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 5350 6150 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21754M.pdf" H 5350 6150 50  0001 C CNN
	1    5350 6150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7700 4750 7950 4750
Wire Wire Line
	7650 4750 7700 4750
Wire Wire Line
	7700 4450 7750 4450
Wire Wire Line
	8250 4750 8400 4750
Text GLabel 7300 4750 0    50   Input ~ 0
VREF
Wire Wire Line
	7700 4250 7750 4250
Wire Wire Line
	7300 4050 7350 4050
Wire Wire Line
	7350 4100 7350 4050
Connection ~ 7350 4050
$Comp
L power:GND #PWR027
U 1 1 62064C05
P 7350 4450
F 0 "#PWR027" H 7350 4200 50  0001 C CNN
F 1 "GND" H 7355 4277 50  0000 C CNN
F 2 "" H 7350 4450 50  0001 C CNN
F 3 "" H 7350 4450 50  0001 C CNN
	1    7350 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 4450 7350 4400
Wire Wire Line
	7300 4750 7350 4750
Text GLabel 6950 4050 0    50   Input ~ 0
CH1
Wire Wire Line
	7350 4050 7700 4050
Wire Wire Line
	6950 4050 7000 4050
Wire Wire Line
	7700 4050 7700 4250
Connection ~ 7700 4750
Wire Wire Line
	7700 4450 7700 4750
Wire Wire Line
	8350 4350 8400 4350
Wire Wire Line
	8400 4350 8400 4750
Connection ~ 8400 4350
Wire Wire Line
	8400 4350 8450 4350
$Comp
L Device:R R11
U 1 1 6218BE4D
P 8600 4350
F 0 "R11" V 8393 4350 50  0000 C CNN
F 1 "1k" V 8484 4350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8530 4350 50  0001 C CNN
F 3 "~" H 8600 4350 50  0001 C CNN
	1    8600 4350
	0    1    1    0   
$EndComp
$Comp
L Device:CP C14
U 1 1 6106F67D
P 8950 4350
F 0 "C14" V 9205 4350 50  0000 C CNN
F 1 "10u" V 9114 4350 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 8988 4200 50  0001 C CNN
F 3 "~" H 8950 4350 50  0001 C CNN
	1    8950 4350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9100 4350 9150 4350
Wire Wire Line
	8750 4350 8800 4350
$Comp
L Device:R R10
U 1 1 621C87A0
P 8100 6050
F 0 "R10" V 7893 6050 50  0000 C CNN
F 1 "39k" V 7984 6050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8030 6050 50  0001 C CNN
F 3 "~" H 8100 6050 50  0001 C CNN
	1    8100 6050
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 621C87A6
P 7500 6050
F 0 "R6" V 7385 6050 50  0000 C CNN
F 1 "56k" V 7294 6050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7430 6050 50  0001 C CNN
F 3 "~" H 7500 6050 50  0001 C CNN
	1    7500 6050
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R4
U 1 1 621C87AC
P 7150 5350
F 0 "R4" V 6943 5350 50  0000 C CNN
F 1 "470R" V 7034 5350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7080 5350 50  0001 C CNN
F 3 "~" H 7150 5350 50  0001 C CNN
	1    7150 5350
	0    1    1    0   
$EndComp
$Comp
L Device:C C13
U 1 1 621C87B2
P 7350 5550
F 0 "C13" H 7465 5596 50  0000 L CNN
F 1 "4n7" H 7465 5505 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 7388 5400 50  0001 C CNN
F 3 "~" H 7350 5550 50  0001 C CNN
	1    7350 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 6050 7950 6050
Wire Wire Line
	7650 6050 7700 6050
Wire Wire Line
	7700 5750 7750 5750
Wire Wire Line
	8250 6050 8400 6050
Text GLabel 7300 6050 0    50   Input ~ 0
VREF
Wire Wire Line
	7700 5550 7750 5550
Wire Wire Line
	7300 5350 7350 5350
Wire Wire Line
	7350 5400 7350 5350
Connection ~ 7350 5350
$Comp
L power:GND #PWR028
U 1 1 621C87C1
P 7350 5750
F 0 "#PWR028" H 7350 5500 50  0001 C CNN
F 1 "GND" H 7355 5577 50  0000 C CNN
F 2 "" H 7350 5750 50  0001 C CNN
F 3 "" H 7350 5750 50  0001 C CNN
	1    7350 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 5750 7350 5700
Wire Wire Line
	7300 6050 7350 6050
Text GLabel 6950 5350 0    50   Input ~ 0
CH2
Wire Wire Line
	7350 5350 7700 5350
Wire Wire Line
	6950 5350 7000 5350
Wire Wire Line
	7700 5350 7700 5550
Connection ~ 7700 6050
Wire Wire Line
	7700 5750 7700 6050
Wire Wire Line
	8350 5650 8400 5650
Wire Wire Line
	8400 5650 8400 6050
Connection ~ 8400 5650
Wire Wire Line
	8400 5650 8450 5650
$Comp
L Device:R R12
U 1 1 621C87D3
P 8600 5650
F 0 "R12" V 8393 5650 50  0000 C CNN
F 1 "1k" V 8484 5650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8530 5650 50  0001 C CNN
F 3 "~" H 8600 5650 50  0001 C CNN
	1    8600 5650
	0    1    1    0   
$EndComp
$Comp
L Device:CP C15
U 1 1 621C87D9
P 8950 5650
F 0 "C15" V 9205 5650 50  0000 C CNN
F 1 "10u" V 9114 5650 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 8988 5500 50  0001 C CNN
F 3 "~" H 8950 5650 50  0001 C CNN
	1    8950 5650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9100 5650 9150 5650
Wire Wire Line
	8750 5650 8800 5650
NoConn ~ 8050 3550
NoConn ~ 8150 3550
$Comp
L power:+5V #PWR031
U 1 1 6227D6AF
P 7950 2900
F 0 "#PWR031" H 7950 2750 50  0001 C CNN
F 1 "+5V" H 7965 3073 50  0000 C CNN
F 2 "" H 7950 2900 50  0001 C CNN
F 3 "" H 7950 2900 50  0001 C CNN
	1    7950 2900
	1    0    0    -1  
$EndComp
$Comp
L power:-5V #PWR032
U 1 1 6227DE96
P 7950 3650
F 0 "#PWR032" H 7950 3750 50  0001 C CNN
F 1 "-5V" H 7965 3823 50  0000 C CNN
F 2 "" H 7950 3650 50  0001 C CNN
F 3 "" H 7950 3650 50  0001 C CNN
	1    7950 3650
	-1   0    0    1   
$EndComp
$Comp
L Amplifier_Operational:TL071 U6
U 1 1 60D7AAAC
P 8050 3250
F 0 "U6" H 8050 3500 50  0000 L CNN
F 1 "TL071" H 8050 3400 50  0000 L CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 8100 3300 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 8200 3400 50  0001 C CNN
	1    8050 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 2950 7950 2900
Wire Wire Line
	7700 3350 7700 3600
Wire Wire Line
	7700 3600 8400 3600
Wire Wire Line
	8400 3600 8400 3250
Wire Wire Line
	8400 3250 8350 3250
Wire Wire Line
	7600 3200 7600 3150
Wire Wire Line
	7600 3150 7750 3150
Connection ~ 7600 3150
Wire Wire Line
	7600 3150 7600 3100
$Comp
L power:+3V3 #PWR029
U 1 1 622F7B81
P 7600 2750
F 0 "#PWR029" H 7600 2600 50  0001 C CNN
F 1 "+3V3" H 7615 2923 50  0000 C CNN
F 2 "" H 7600 2750 50  0001 C CNN
F 3 "" H 7600 2750 50  0001 C CNN
	1    7600 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 2800 7600 2750
Wire Wire Line
	7950 3650 7950 3550
$Comp
L power:GND #PWR030
U 1 1 62319E7B
P 7600 3550
F 0 "#PWR030" H 7600 3300 50  0001 C CNN
F 1 "GND" H 7605 3377 50  0000 C CNN
F 2 "" H 7600 3550 50  0001 C CNN
F 3 "" H 7600 3550 50  0001 C CNN
	1    7600 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 3550 7600 3500
Text GLabel 8450 3250 2    50   Output ~ 0
VREF
Wire Wire Line
	8400 3250 8450 3250
Connection ~ 8400 3250
$Comp
L power:+5V #PWR012
U 1 1 62339C35
P 5050 1250
F 0 "#PWR012" H 5050 1100 50  0001 C CNN
F 1 "+5V" H 5065 1423 50  0000 C CNN
F 2 "" H 5050 1250 50  0001 C CNN
F 3 "" H 5050 1250 50  0001 C CNN
	1    5050 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 6233C1CC
P 5150 1500
F 0 "C4" H 5265 1546 50  0000 L CNN
F 1 "100n" H 5265 1455 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 5188 1350 50  0001 C CNN
F 3 "~" H 5150 1500 50  0001 C CNN
	1    5150 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 6233EFA8
P 4950 1500
F 0 "C2" H 4835 1546 50  0000 R CNN
F 1 "100n" H 4835 1455 50  0000 R CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 4988 1350 50  0001 C CNN
F 3 "~" H 4950 1500 50  0001 C CNN
	1    4950 1500
	1    0    0    -1  
$EndComp
$Comp
L power:-5V #PWR013
U 1 1 6234B5F9
P 5050 2150
F 0 "#PWR013" H 5050 2250 50  0001 C CNN
F 1 "-5V" H 5065 2323 50  0000 C CNN
F 2 "" H 5050 2150 50  0001 C CNN
F 3 "" H 5050 2150 50  0001 C CNN
	1    5050 2150
	-1   0    0    1   
$EndComp
$Comp
L Device:C C5
U 1 1 6235D01D
P 5150 1900
F 0 "C5" H 5265 1946 50  0000 L CNN
F 1 "100n" H 5265 1855 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 5188 1750 50  0001 C CNN
F 3 "~" H 5150 1900 50  0001 C CNN
	1    5150 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 6235D023
P 4950 1900
F 0 "C3" H 4835 1946 50  0000 R CNN
F 1 "100n" H 4835 1855 50  0000 R CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 4988 1750 50  0001 C CNN
F 3 "~" H 4950 1900 50  0001 C CNN
	1    4950 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 62374CCC
P 5500 2050
F 0 "#PWR018" H 5500 1800 50  0001 C CNN
F 1 "GND" H 5505 1877 50  0000 C CNN
F 2 "" H 5500 2050 50  0001 C CNN
F 3 "" H 5500 2050 50  0001 C CNN
	1    5500 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 1750 4950 1700
Wire Wire Line
	5150 1750 5150 1700
Connection ~ 5150 1700
Wire Wire Line
	5150 1700 5150 1650
Wire Wire Line
	5150 1700 4950 1700
Connection ~ 4950 1700
Wire Wire Line
	4950 1700 4950 1650
Wire Wire Line
	4950 2050 4950 2100
Wire Wire Line
	4950 2100 5050 2100
Wire Wire Line
	5150 2100 5150 2050
Wire Wire Line
	5050 2150 5050 2100
Connection ~ 5050 2100
Wire Wire Line
	5050 2100 5150 2100
Wire Wire Line
	4950 1350 4950 1300
Wire Wire Line
	4950 1300 5050 1300
Wire Wire Line
	5150 1300 5150 1350
Wire Wire Line
	5050 1250 5050 1300
Connection ~ 5050 1300
Wire Wire Line
	5050 1300 5150 1300
Wire Wire Line
	5500 2050 5500 1700
Wire Wire Line
	5500 1700 5150 1700
$Comp
L power:+5V #PWR033
U 1 1 624FC1E1
P 9100 2750
F 0 "#PWR033" H 9100 2600 50  0001 C CNN
F 1 "+5V" H 9115 2923 50  0000 C CNN
F 2 "" H 9100 2750 50  0001 C CNN
F 3 "" H 9100 2750 50  0001 C CNN
	1    9100 2750
	1    0    0    -1  
$EndComp
$Comp
L power:-5V #PWR034
U 1 1 624FDCA4
P 9100 3450
F 0 "#PWR034" H 9100 3550 50  0001 C CNN
F 1 "-5V" H 9115 3623 50  0000 C CNN
F 2 "" H 9100 3450 50  0001 C CNN
F 3 "" H 9100 3450 50  0001 C CNN
	1    9100 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	9100 3450 9100 3400
Wire Wire Line
	9100 2800 9100 2750
$Comp
L power:+5V #PWR010
U 1 1 625B75F7
P 4900 2750
F 0 "#PWR010" H 4900 2600 50  0001 C CNN
F 1 "+5V" H 4915 2923 50  0000 C CNN
F 2 "" H 4900 2750 50  0001 C CNN
F 3 "" H 4900 2750 50  0001 C CNN
	1    4900 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 625DFCD2
P 4900 3200
F 0 "#PWR011" H 4900 2950 50  0001 C CNN
F 1 "GND" H 4905 3027 50  0000 C CNN
F 2 "" H 4900 3200 50  0001 C CNN
F 3 "" H 4900 3200 50  0001 C CNN
	1    4900 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 3200 4900 3150
$Comp
L power:GND #PWR019
U 1 1 625F3B4D
P 5500 3650
F 0 "#PWR019" H 5500 3400 50  0001 C CNN
F 1 "GND" H 5505 3477 50  0000 C CNN
F 2 "" H 5500 3650 50  0001 C CNN
F 3 "" H 5500 3650 50  0001 C CNN
	1    5500 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3650 5500 3600
NoConn ~ 5100 3100
NoConn ~ 5100 3300
Wire Wire Line
	5900 3400 5950 3400
Wire Wire Line
	5950 3400 5950 3350
Wire Wire Line
	5900 3000 5950 3000
Wire Wire Line
	5950 3000 5950 3050
Wire Wire Line
	6300 2850 6300 2800
$Comp
L power:GND #PWR024
U 1 1 626CB621
P 6300 3200
F 0 "#PWR024" H 6300 2950 50  0001 C CNN
F 1 "GND" H 6305 3027 50  0000 C CNN
F 2 "" H 6300 3200 50  0001 C CNN
F 3 "" H 6300 3200 50  0001 C CNN
	1    6300 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 3200 6300 3150
$Comp
L power:-5V #PWR023
U 1 1 626EC2B9
P 6300 2750
F 0 "#PWR023" H 6300 2850 50  0001 C CNN
F 1 "-5V" H 6315 2923 50  0000 C CNN
F 2 "" H 6300 2750 50  0001 C CNN
F 3 "" H 6300 2750 50  0001 C CNN
	1    6300 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 2900 5050 2900
Wire Wire Line
	5050 2900 5050 2800
Wire Wire Line
	5050 2800 5100 2800
$Comp
L Device:CP C6
U 1 1 62746B22
P 5700 1500
F 0 "C6" H 5818 1546 50  0000 L CNN
F 1 "10u" H 5818 1455 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 5738 1350 50  0001 C CNN
F 3 "~" H 5700 1500 50  0001 C CNN
	1    5700 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 627473AB
P 5700 1700
F 0 "#PWR021" H 5700 1450 50  0001 C CNN
F 1 "GND" H 5705 1527 50  0000 C CNN
F 2 "" H 5700 1700 50  0001 C CNN
F 3 "" H 5700 1700 50  0001 C CNN
	1    5700 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 6275C47F
P 6050 1500
F 0 "C8" H 6165 1546 50  0000 L CNN
F 1 "100n" H 6165 1455 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 6088 1350 50  0001 C CNN
F 3 "~" H 6050 1500 50  0001 C CNN
	1    6050 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 6275CF97
P 6450 1500
F 0 "C10" H 6565 1546 50  0000 L CNN
F 1 "100n" H 6565 1455 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 6488 1350 50  0001 C CNN
F 3 "~" H 6450 1500 50  0001 C CNN
	1    6450 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 627688C7
P 6850 1500
F 0 "C11" H 6965 1546 50  0000 L CNN
F 1 "100n" H 6965 1455 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 6888 1350 50  0001 C CNN
F 3 "~" H 6850 1500 50  0001 C CNN
	1    6850 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR022
U 1 1 6276A59C
P 6050 1700
F 0 "#PWR022" H 6050 1450 50  0001 C CNN
F 1 "GND" H 6055 1527 50  0000 C CNN
F 2 "" H 6050 1700 50  0001 C CNN
F 3 "" H 6050 1700 50  0001 C CNN
	1    6050 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 62773D19
P 6450 1700
F 0 "#PWR025" H 6450 1450 50  0001 C CNN
F 1 "GND" H 6455 1527 50  0000 C CNN
F 2 "" H 6450 1700 50  0001 C CNN
F 3 "" H 6450 1700 50  0001 C CNN
	1    6450 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR026
U 1 1 62774027
P 6850 1700
F 0 "#PWR026" H 6850 1450 50  0001 C CNN
F 1 "GND" H 6855 1527 50  0000 C CNN
F 2 "" H 6850 1700 50  0001 C CNN
F 3 "" H 6850 1700 50  0001 C CNN
	1    6850 1700
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR020
U 1 1 6273FD9A
P 5700 1250
F 0 "#PWR020" H 5700 1100 50  0001 C CNN
F 1 "+3V3" H 5715 1423 50  0000 C CNN
F 2 "" H 5700 1250 50  0001 C CNN
F 3 "" H 5700 1250 50  0001 C CNN
	1    5700 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 1350 5700 1300
Wire Wire Line
	5700 1700 5700 1650
Wire Wire Line
	6050 1700 6050 1650
Wire Wire Line
	6450 1700 6450 1650
Wire Wire Line
	6850 1700 6850 1650
Wire Wire Line
	5700 1300 6050 1300
Wire Wire Line
	6850 1300 6850 1350
Connection ~ 5700 1300
Wire Wire Line
	5700 1300 5700 1250
Wire Wire Line
	6450 1350 6450 1300
Connection ~ 6450 1300
Wire Wire Line
	6450 1300 6850 1300
Wire Wire Line
	6050 1350 6050 1300
Connection ~ 6050 1300
Wire Wire Line
	6050 1300 6450 1300
$Comp
L power:GND #PWR015
U 1 1 628276F1
P 5350 6550
F 0 "#PWR015" H 5350 6300 50  0001 C CNN
F 1 "GND" H 5355 6377 50  0000 C CNN
F 2 "" H 5350 6550 50  0001 C CNN
F 3 "" H 5350 6550 50  0001 C CNN
	1    5350 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 6550 5350 6500
Connection ~ 5350 6500
Wire Wire Line
	5350 6500 5350 6450
Wire Wire Line
	5750 6050 5800 6050
Wire Wire Line
	5800 6050 5800 6150
Wire Wire Line
	5350 6500 5800 6500
Wire Wire Line
	5750 6250 5800 6250
Connection ~ 5800 6250
Wire Wire Line
	5800 6250 5800 6500
Wire Wire Line
	5750 6150 5800 6150
Connection ~ 5800 6150
Wire Wire Line
	5800 6150 5800 6250
$Comp
L power:+3V3 #PWR014
U 1 1 628628E9
P 5350 5800
F 0 "#PWR014" H 5350 5650 50  0001 C CNN
F 1 "+3V3" H 5365 5973 50  0000 C CNN
F 2 "" H 5350 5800 50  0001 C CNN
F 3 "" H 5350 5800 50  0001 C CNN
	1    5350 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 5850 5350 5800
Wire Wire Line
	4950 6250 4900 6250
Wire Wire Line
	4900 6250 4900 6500
Wire Wire Line
	4900 6500 5350 6500
Text GLabel 4900 6050 0    50   BiDi ~ 0
SDA
Wire Wire Line
	4900 6050 4950 6050
Text GLabel 4900 6150 0    50   Input ~ 0
SCL
Wire Wire Line
	4900 6150 4950 6150
$Comp
L power:+3V3 #PWR016
U 1 1 628E5F4E
P 5450 4200
F 0 "#PWR016" H 5450 4050 50  0001 C CNN
F 1 "+3V3" H 5465 4373 50  0000 C CNN
F 2 "" H 5450 4200 50  0001 C CNN
F 3 "" H 5450 4200 50  0001 C CNN
	1    5450 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4250 5450 4200
Wire Wire Line
	5450 5250 5450 5200
Wire Wire Line
	4950 4650 4900 4650
Wire Wire Line
	4900 4650 4900 5200
Wire Wire Line
	4900 5200 5450 5200
Connection ~ 5450 5200
Wire Wire Line
	5450 5200 5450 5150
Text GLabel 4850 4550 0    50   Input ~ 0
SCK
Text GLabel 4850 4750 0    50   Input ~ 0
SDI
Text GLabel 4850 4850 0    50   Input ~ 0
CS
Wire Wire Line
	4850 4550 4950 4550
Wire Wire Line
	4850 4750 4950 4750
Wire Wire Line
	4850 4850 4950 4850
Text GLabel 4200 6650 2    50   Output ~ 0
SDA
Text GLabel 4200 6550 2    50   Output ~ 0
SCL
$Comp
L power:+3V3 #PWR08
U 1 1 629CFAA2
P 4150 6350
F 0 "#PWR08" H 4150 6200 50  0001 C CNN
F 1 "+3V3" V 4165 6478 50  0000 L CNN
F 2 "" H 4150 6350 50  0001 C CNN
F 3 "" H 4150 6350 50  0001 C CNN
	1    4150 6350
	0    1    1    0   
$EndComp
Wire Wire Line
	4100 6450 4150 6450
Wire Wire Line
	4100 6550 4200 6550
Wire Wire Line
	4100 6650 4200 6650
Wire Wire Line
	4150 6750 4150 6450
$Comp
L Isolator:6N137 U2
U 1 1 62AD4F10
P 3450 5400
F 0 "U2" H 3450 5867 50  0000 C CNN
F 1 "6N137" H 3450 5776 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 3450 4900 50  0001 C CNN
F 3 "https://docs.broadcom.com/docs/AV02-0940EN" H 2600 5950 50  0001 C CNN
	1    3450 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 5400 3100 5400
NoConn ~ 3750 5300
Wire Wire Line
	3750 5200 3800 5200
Wire Wire Line
	3800 5200 3800 5000
Wire Wire Line
	3800 5000 3900 5000
Wire Wire Line
	3900 5000 3900 5050
Wire Wire Line
	3750 5400 3900 5400
Wire Wire Line
	3900 5400 3900 5350
$Comp
L power:+3V3 #PWR05
U 1 1 62B140B0
P 3900 4950
F 0 "#PWR05" H 3900 4800 50  0001 C CNN
F 1 "+3V3" H 3915 5123 50  0000 C CNN
F 2 "" H 3900 4950 50  0001 C CNN
F 3 "" H 3900 4950 50  0001 C CNN
	1    3900 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 5000 3900 4950
Connection ~ 3900 5000
$Comp
L power:GND #PWR04
U 1 1 62B1DD1E
P 3800 5650
F 0 "#PWR04" H 3800 5400 50  0001 C CNN
F 1 "GND" H 3805 5477 50  0000 C CNN
F 2 "" H 3800 5650 50  0001 C CNN
F 3 "" H 3800 5650 50  0001 C CNN
	1    3800 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 5600 3800 5600
Wire Wire Line
	3800 5600 3800 5650
Text GLabel 3950 5400 2    50   Output ~ 0
MIDI_RX
Wire Wire Line
	3900 5400 3950 5400
Connection ~ 3900 5400
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 60E30A7B
P 2300 5100
F 0 "J1" H 2218 5317 50  0000 C CNN
F 1 "JST XH 2P" H 2218 5226 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 2300 5100 50  0001 C CNN
F 3 "~" H 2300 5100 50  0001 C CNN
	1    2300 5100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4900 2750 4900 2800
Wire Wire Line
	4900 2800 5050 2800
Connection ~ 4900 2800
Wire Wire Line
	4900 2800 4900 2850
Connection ~ 5050 2800
Wire Wire Line
	5900 2800 6300 2800
Wire Wire Line
	6300 2800 6300 2750
Connection ~ 6300 2800
Text GLabel 6000 4550 2    50   Output ~ 0
CH1
Text GLabel 6000 4850 2    50   Output ~ 0
CH2
Wire Wire Line
	5950 4550 6000 4550
Wire Wire Line
	5950 4850 6000 4850
$Comp
L Connector_Generic:Conn_01x03 J4
U 1 1 62C46F2C
P 9550 4950
F 0 "J4" H 9468 5267 50  0000 C CNN
F 1 "JST XH 3P" H 9468 5176 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 9550 4950 50  0001 C CNN
F 3 "~" H 9550 4950 50  0001 C CNN
	1    9550 4950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR035
U 1 1 62C485DD
P 9300 5100
F 0 "#PWR035" H 9300 4850 50  0001 C CNN
F 1 "GND" H 9305 4927 50  0000 C CNN
F 2 "" H 9300 5100 50  0001 C CNN
F 3 "" H 9300 5100 50  0001 C CNN
	1    9300 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 5100 9300 4850
Wire Wire Line
	9300 4850 9350 4850
Text Notes 4350 7000 0    50   ~ 0
OLED
Wire Notes Line
	4600 7050 3700 7050
Wire Notes Line
	3700 7050 3700 6050
Wire Notes Line
	3700 6050 4600 6050
Wire Notes Line
	4600 6050 4600 7050
Text GLabel 3250 6400 2    50   Output ~ 0
ENC_BT
Text GLabel 2500 6400 0    50   Output ~ 0
ENC_A
Text GLabel 2500 6600 0    50   Output ~ 0
ENC_B
$Comp
L Device:Rotary_Encoder_Switch SW1
U 1 1 62CCDB5C
P 2900 6500
F 0 "SW1" H 2900 6867 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 2900 6776 50  0000 C CNN
F 2 "Rotary_Encoder:RotaryEncoder_Alps_EC11E-Switch_Vertical_H20mm_CircularMountingHoles" H 2750 6660 50  0001 C CNN
F 3 "~" H 2900 6760 50  0001 C CNN
	1    2900 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 6400 2600 6400
Wire Wire Line
	2500 6600 2600 6600
$Comp
L power:GND #PWR02
U 1 1 62D000F6
P 2900 6800
F 0 "#PWR02" H 2900 6550 50  0001 C CNN
F 1 "GND" H 2905 6627 50  0000 C CNN
F 2 "" H 2900 6800 50  0001 C CNN
F 3 "" H 2900 6800 50  0001 C CNN
	1    2900 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 6800 2900 6750
Wire Wire Line
	2900 6750 2550 6750
Wire Wire Line
	2550 6750 2550 6500
Wire Wire Line
	2550 6500 2600 6500
Wire Wire Line
	2900 6750 3250 6750
Wire Wire Line
	3250 6750 3250 6600
Wire Wire Line
	3250 6600 3200 6600
Connection ~ 2900 6750
Wire Wire Line
	3200 6400 3250 6400
Wire Wire Line
	9150 5650 9150 5050
Wire Wire Line
	9150 4950 9150 4350
Wire Wire Line
	9150 4950 9350 4950
Wire Wire Line
	9150 5050 9350 5050
Wire Notes Line
	4600 1000 7200 1000
Wire Notes Line
	7200 1000 7200 2400
Wire Notes Line
	7200 2400 4600 2400
Wire Notes Line
	4600 2400 4600 1000
Text Notes 6300 2350 0    50   ~ 0
Decoupling capacitors
Wire Notes Line
	6600 2450 6600 3900
Wire Notes Line
	6600 3900 4600 3900
Wire Notes Line
	4600 3900 4600 2450
Wire Notes Line
	4600 2450 6600 2450
Text Notes 5700 3850 0    50   ~ 0
Negative power supply
Wire Notes Line
	4350 4700 4350 5950
Text Notes 4100 5900 0    50   ~ 0
MIDI
Wire Notes Line
	5900 6800 5900 5550
Wire Notes Line
	5900 5550 4650 5550
Wire Notes Line
	4650 5550 4650 6800
Wire Notes Line
	4650 6800 5900 6800
Wire Notes Line
	4600 3950 6250 3950
Wire Notes Line
	6250 3950 6250 5500
Wire Notes Line
	6250 5500 4600 5500
Wire Notes Line
	4600 5500 4600 3950
Text Notes 5550 6750 0    50   ~ 0
EEPROM
Text Notes 6050 5450 0    50   ~ 0
DAC
Text Notes 9000 6300 0    50   ~ 0
Output amplifiers
Wire Notes Line
	2150 6050 3650 6050
Wire Notes Line
	3650 6050 3650 7050
Wire Notes Line
	3650 7050 2150 7050
Wire Notes Line
	2150 7050 2150 6050
Text Notes 3300 7000 0    50   ~ 0
Encoder
Wire Notes Line
	9750 2450 9750 6350
Wire Notes Line
	6650 6350 6650 2450
Wire Notes Line
	6650 2450 9750 2450
Wire Notes Line
	6650 6350 9750 6350
$Comp
L Diode:1N4148 D1
U 1 1 62F32801
P 2950 5400
F 0 "D1" V 2904 5320 50  0000 R CNN
F 1 "1N4148" V 2995 5320 50  0000 R CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 2950 5225 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 2950 5400 50  0001 C CNN
	1    2950 5400
	0    1    1    0   
$EndComp
Wire Wire Line
	2500 5100 2550 5100
Wire Wire Line
	2550 5100 2550 5600
Wire Wire Line
	2550 5600 2950 5600
Wire Wire Line
	2500 5200 2600 5200
Wire Wire Line
	3100 5400 3100 5200
Wire Wire Line
	3100 5200 2950 5200
Wire Wire Line
	2950 5250 2950 5200
Connection ~ 2950 5200
Wire Wire Line
	2950 5200 2900 5200
Wire Wire Line
	2950 5550 2950 5600
Connection ~ 2950 5600
Wire Wire Line
	2950 5600 3150 5600
Wire Notes Line
	4350 4700 2150 4700
Wire Notes Line
	2150 4700 2150 5950
Wire Notes Line
	2150 5950 4350 5950
$Comp
L MCU_RaspberryPi_and_Boards:Pico U1
U 1 1 63007291
P 3350 2300
F 0 "U1" H 3350 3515 50  0000 C CNN
F 1 "Pico" H 3350 3424 50  0000 C CNN
F 2 "MCU_RaspberryPi_and_Boards:RPi_Pico_SMD_TH" V 3350 2300 50  0001 C CNN
F 3 "" H 3350 2300 50  0001 C CNN
	1    3350 2300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 6301CF57
P 4150 1350
F 0 "#PWR06" H 4150 1200 50  0001 C CNN
F 1 "+5V" V 4165 1478 50  0000 L CNN
F 2 "" H 4150 1350 50  0001 C CNN
F 3 "" H 4150 1350 50  0001 C CNN
	1    4150 1350
	0    1    1    0   
$EndComp
$Comp
L power:+3V3 #PWR07
U 1 1 6302A328
P 4150 1750
F 0 "#PWR07" H 4150 1600 50  0001 C CNN
F 1 "+3V3" V 4165 1878 50  0000 L CNN
F 2 "" H 4150 1750 50  0001 C CNN
F 3 "" H 4150 1750 50  0001 C CNN
	1    4150 1750
	0    1    1    0   
$EndComp
Text GLabel 2550 1650 0    50   BiDi ~ 0
SDA
Text GLabel 2550 1750 0    50   Input ~ 0
SCL
Wire Wire Line
	4050 1350 4150 1350
Wire Wire Line
	4050 1750 4150 1750
Wire Wire Line
	2550 1650 2650 1650
Wire Wire Line
	2550 1750 2650 1750
Text GLabel 2550 1950 0    50   Input ~ 0
MIDI_RX
Wire Wire Line
	2550 1950 2650 1950
Text GLabel 2550 2250 0    50   Output ~ 0
SCK
Text GLabel 2550 2150 0    50   Output ~ 0
SDI
Text GLabel 2550 2350 0    50   Output ~ 0
CS
Wire Wire Line
	2550 2150 2650 2150
Wire Wire Line
	2550 2250 2650 2250
Wire Wire Line
	2550 2350 2650 2350
Text GLabel 2550 2650 0    50   Input ~ 0
ENC_A
Text GLabel 2550 2750 0    50   Input ~ 0
ENC_B
Text GLabel 2550 2450 0    50   Input ~ 0
ENC_BT
Wire Wire Line
	2550 2450 2650 2450
$Comp
L Connector_Generic:Conn_01x05 J2
U 1 1 63153510
P 2350 4100
F 0 "J2" H 2268 4517 50  0000 C CNN
F 1 "JST XH 5P" H 2268 4426 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B5B-XH-A_1x05_P2.50mm_Vertical" H 2350 4100 50  0001 C CNN
F 3 "~" H 2350 4100 50  0001 C CNN
	1    2350 4100
	-1   0    0    -1  
$EndComp
Text GLabel 2650 3900 2    50   Input ~ 0
DBG_TX
Text GLabel 2650 4000 2    50   Output ~ 0
DBG_RX
Text GLabel 2650 4100 2    50   BiDi ~ 0
DBG_SWCLK
Text GLabel 2650 4300 2    50   BiDi ~ 0
DBG_SWDIO
$Comp
L power:GND #PWR01
U 1 1 63162C13
P 2600 4400
F 0 "#PWR01" H 2600 4150 50  0001 C CNN
F 1 "GND" H 2605 4227 50  0000 C CNN
F 2 "" H 2600 4400 50  0001 C CNN
F 3 "" H 2600 4400 50  0001 C CNN
	1    2600 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 4400 2600 4200
Wire Wire Line
	2600 4200 2550 4200
Wire Wire Line
	2550 4300 2650 4300
Wire Wire Line
	2550 4100 2650 4100
Wire Wire Line
	2550 4000 2650 4000
Wire Wire Line
	2550 3900 2650 3900
Text GLabel 3200 3500 0    50   BiDi ~ 0
DBG_SWCLK
Wire Wire Line
	3200 3500 3250 3500
Wire Wire Line
	3250 3500 3250 3450
Text GLabel 3500 3500 2    50   BiDi ~ 0
DBG_SWDIO
Wire Wire Line
	3450 3450 3450 3500
Wire Wire Line
	3450 3500 3500 3500
$Comp
L power:GND #PWR03
U 1 1 631B31BA
P 3350 3650
F 0 "#PWR03" H 3350 3400 50  0001 C CNN
F 1 "GND" H 3355 3477 50  0000 C CNN
F 2 "" H 3350 3650 50  0001 C CNN
F 3 "" H 3350 3650 50  0001 C CNN
	1    3350 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1550 2600 1550
Wire Wire Line
	2600 1550 2600 2050
Wire Wire Line
	2600 3600 3350 3600
Wire Wire Line
	3350 3600 3350 3450
Wire Wire Line
	3350 3650 3350 3600
Connection ~ 3350 3600
Wire Wire Line
	3350 3600 4100 3600
Wire Wire Line
	4100 3600 4100 3050
Wire Wire Line
	4100 1550 4050 1550
Wire Wire Line
	2600 2050 2650 2050
Connection ~ 2600 2050
Wire Wire Line
	2600 2050 2600 2550
Wire Wire Line
	2550 2650 2650 2650
Wire Wire Line
	2550 2750 2650 2750
Wire Wire Line
	2650 2550 2600 2550
Connection ~ 2600 2550
Wire Wire Line
	2600 2550 2600 3050
Wire Wire Line
	2650 3050 2600 3050
Connection ~ 2600 3050
Wire Wire Line
	2600 3050 2600 3600
Wire Wire Line
	4050 3050 4100 3050
Connection ~ 4100 3050
Wire Wire Line
	4100 3050 4100 2550
Wire Wire Line
	4050 2550 4100 2550
Connection ~ 4100 2550
Wire Wire Line
	4100 2550 4100 2050
Wire Wire Line
	4050 2050 4100 2050
Connection ~ 4100 2050
Wire Wire Line
	4100 2050 4100 1550
Text GLabel 4150 3250 2    50   Output ~ 0
DBG_TX
Text GLabel 4150 3150 2    50   Input ~ 0
DBG_RX
NoConn ~ 2650 1850
NoConn ~ 2650 2850
NoConn ~ 2650 2950
NoConn ~ 2650 3150
NoConn ~ 2650 3250
NoConn ~ 4050 2950
NoConn ~ 4050 2850
NoConn ~ 4050 2450
NoConn ~ 4050 2350
NoConn ~ 4050 2250
NoConn ~ 4050 2150
NoConn ~ 4050 1950
NoConn ~ 4050 1850
NoConn ~ 4050 1650
NoConn ~ 4050 1450
Wire Notes Line
	2150 1000 4550 1000
Wire Notes Line
	4550 1000 4550 4650
Wire Notes Line
	4550 4650 2150 4650
Wire Notes Line
	2150 4650 2150 1000
Text Notes 3900 4600 0    50   ~ 0
Microcontroller
$Comp
L Mechanical:MountingHole H1
U 1 1 6370512C
P 6300 7000
F 0 "H1" H 6400 7046 50  0000 L CNN
F 1 "MountingHole" H 6400 6955 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad_Via" H 6300 7000 50  0001 C CNN
F 3 "~" H 6300 7000 50  0001 C CNN
	1    6300 7000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 6372B199
P 6300 7200
F 0 "H2" H 6400 7246 50  0000 L CNN
F 1 "MountingHole" H 6400 7155 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad_Via" H 6300 7200 50  0001 C CNN
F 3 "~" H 6300 7200 50  0001 C CNN
	1    6300 7200
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 6372B8A4
P 6300 7400
F 0 "H3" H 6400 7446 50  0000 L CNN
F 1 "MountingHole" H 6400 7355 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad_Via" H 6300 7400 50  0001 C CNN
F 3 "~" H 6300 7400 50  0001 C CNN
	1    6300 7400
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 6372BEEF
P 6300 7600
F 0 "H4" H 6400 7646 50  0000 L CNN
F 1 "MountingHole" H 6400 7555 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad_Via" H 6300 7600 50  0001 C CNN
F 3 "~" H 6300 7600 50  0001 C CNN
	1    6300 7600
	1    0    0    -1  
$EndComp
NoConn ~ 2650 1350
NoConn ~ 2650 1450
Wire Wire Line
	4050 3150 4150 3150
Wire Wire Line
	4050 3250 4150 3250
$Comp
L Device:LED D3
U 1 1 61F9B25F
P 8050 1750
F 0 "D3" V 8089 1632 50  0000 R CNN
F 1 "LED" V 7998 1632 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 8050 1750 50  0001 C CNN
F 3 "~" H 8050 1750 50  0001 C CNN
	1    8050 1750
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 61F9C231
P 8050 1950
F 0 "#PWR0101" H 8050 1700 50  0001 C CNN
F 1 "GND" H 8055 1777 50  0000 C CNN
F 2 "" H 8050 1950 50  0001 C CNN
F 3 "" H 8050 1950 50  0001 C CNN
	1    8050 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 61FB2915
P 7600 1750
F 0 "D2" V 7639 1632 50  0000 R CNN
F 1 "LED" V 7548 1632 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 7600 1750 50  0001 C CNN
F 3 "~" H 7600 1750 50  0001 C CNN
	1    7600 1750
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 61FB291B
P 7600 1950
F 0 "#PWR0102" H 7600 1700 50  0001 C CNN
F 1 "GND" H 7605 1777 50  0000 C CNN
F 2 "" H 7600 1950 50  0001 C CNN
F 3 "" H 7600 1950 50  0001 C CNN
	1    7600 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 61FE7FDF
P 8050 1400
F 0 "R13" H 7981 1446 50  0000 R CNN
F 1 "18k" H 7981 1355 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7980 1400 50  0001 C CNN
F 3 "~" H 8050 1400 50  0001 C CNN
	1    8050 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R14
U 1 1 61FE9F6E
P 7600 1400
F 0 "R14" H 7531 1446 50  0000 R CNN
F 1 "18k" H 7531 1355 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7530 1400 50  0001 C CNN
F 3 "~" H 7600 1400 50  0001 C CNN
	1    7600 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 1600 8050 1550
Wire Wire Line
	7600 1600 7600 1550
Wire Wire Line
	8050 1950 8050 1900
Wire Wire Line
	7600 1950 7600 1900
Text GLabel 8000 1200 0    50   Input ~ 0
LED2
Text GLabel 7550 1200 0    50   Input ~ 0
LED1
Wire Wire Line
	8000 1200 8050 1200
Wire Wire Line
	8050 1200 8050 1250
Wire Notes Line
	7250 2400 7250 1000
Wire Notes Line
	7250 1000 8400 1000
Wire Notes Line
	8400 1000 8400 2400
Wire Notes Line
	8400 2400 7250 2400
Text Notes 8150 2350 0    50   ~ 0
LEDs
Wire Wire Line
	7550 1200 7600 1200
Wire Wire Line
	7600 1200 7600 1250
Text GLabel 4150 2650 2    50   Output ~ 0
LED1
Text GLabel 4150 2750 2    50   Output ~ 0
LED2
Wire Wire Line
	4050 2650 4150 2650
Wire Wire Line
	4050 2750 4150 2750
$EndSCHEMATC
