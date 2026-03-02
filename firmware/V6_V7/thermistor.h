/*
* FW-Dyson-BMS	-	(unofficial) Firmware Upgrade for Dyson BMS - V6/V7 Vacuums
* Copyright (C) 2022 tinfever
* 
* This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
* 
* The author can be contacted at tinfever6@(insert-everyone's-favorite-google-email-domain).com
* 
* NOTE: As an addendum to the GNU General Public License, any hardware using code or information from this project must also make publicly available complete electrical schematics and a bill of materials for such hardware.
*/


#ifndef THERMISTOR_H
#define	THERMISTOR_H

#include <stdint.h>
#include "main.h"

enum {
    voltage = 0,
    temp = 1,
};

/* First value in array is voltage at a given temperature, in 10mV steps. Ex. value of 24 = 240mV
 Second value is temperature in Celsius
 Data was computed in Google Sheets with the following layout and formulas:
 * Column A is temperature in Celsius in 1 degree steps from 0-99C
 * Column B is resistance of thermistor at temp with formula =$L$2/EXP(($L$3*((A2+273.15)-($L$1+273.15)))/((A2+273.15)*($L$1+273.15)))
 * Column C is voltage output (mV) of the resistor + thermistor voltage divider with formula =1000*$L$5*B2/(B2+$L$6)
 * Voltage values in mV were then divided by 10 and rounded to nearest whole number. Duplicate voltage values were removed, retaining highest temp duplicate.
 * Lines containing voltage values greater than 255 were discarded.
 * Variables were:
 * Cell L1 = 25 = Thermistor rating temp in C
 * Cell L2 = 10000 = Thermistor rated resistance at rating temp in Ohms
 * Cell L3 = 3500 = beta of thermistor
 * Cell L5 = 3.3 = Vin for resistor divider calc
 * Cell L6 = 23700 = Thermistor series resistor / voltage divider resistance in Ohms
 */

#define SV09_LUT_SIZE_DEF 93
#define SV11_LUT_SIZE_DEF 84

extern uint8_t const SV09_thermistor_LUT[SV09_LUT_SIZE_DEF][2];
extern uint8_t const SV11_thermistor_LUT[SV11_LUT_SIZE_DEF][2];
extern const modelnum_t LUT_SIZE[NUM_OF_MODELS];
extern uint8_t const (*ThermistorLUT[2])[2];

uint8_t getThermistorTemp (modelnum_t modelnum);














#endif	/* THERMISTOR_H */

