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

#include <xc8debug.h>

#include "thermistor.h"
#include "main.h"
#include "config.h"

// Thermistor LUT definitions (declared extern in thermistor.h)
uint8_t const SV09_thermistor_LUT[SV09_LUT_SIZE_DEF][2] = {
    {45,99}, {46,98}, {47,97}, {48,96}, {49,95}, {50,94}, {51,93}, {52,92},
    {53,91}, {54,90}, {56,89}, {57,88}, {58,87}, {59,86}, {61,85}, {62,84},
    {64,83}, {65,82}, {66,81}, {68,80}, {69,79}, {71,78}, {73,77}, {74,76},
    {76,75}, {78,74}, {79,73}, {81,72}, {83,71}, {85,70}, {87,69}, {89,68},
    {91,67}, {93,66}, {95,65}, {97,64}, {99,63}, {101,62}, {103,61}, {105,60},
    {108,59}, {110,58}, {112,57}, {115,56}, {117,55}, {120,54}, {122,53}, {125,52},
    {127,51}, {130,50}, {133,49}, {135,48}, {138,47}, {141,46}, {144,45}, {146,44},
    {149,43}, {152,42}, {155,41}, {158,40}, {161,39}, {164,38}, {167,37}, {170,36},
    {173,35}, {176,34}, {179,33}, {182,32}, {185,31}, {188,30}, {191,29}, {194,28},
    {198,27}, {201,26}, {204,25}, {207,24}, {210,23}, {213,22}, {216,21}, {219,20},
    {222,19}, {225,18}, {228,17}, {231,16}, {234,15}, {236,14}, {239,13}, {242,12},
    {245,11}, {248,10}, {250,9}, {253,8}, {255,7}
};

uint8_t const SV11_thermistor_LUT[SV11_LUT_SIZE_DEF][2] = {
    {13,99}, {14,97}, {15,94}, {16,91}, {17,89}, {18,86}, {19,84}, {20,82},
    {21,80}, {22,79}, {23,77}, {24,75}, {25,74}, {26,72}, {27,71}, {28,69},
    {29,68}, {30,67}, {31,66}, {32,64}, {33,63}, {34,62}, {35,61}, {36,60},
    {37,59}, {38,58}, {39,57}, {40,56}, {42,55}, {43,54}, {44,53}, {45,52},
    {47,51}, {48,50}, {49,49}, {51,48}, {52,47}, {54,46}, {55,45}, {57,44},
    {59,43}, {60,42}, {62,41}, {64,40}, {66,39}, {68,38}, {70,37}, {72,36},
    {74,35}, {76,34}, {78,33}, {80,32}, {83,31}, {85,30}, {88,29}, {90,28},
    {93,27}, {95,26}, {98,25}, {101,24}, {103,23}, {106,22}, {109,21}, {112,20},
    {115,19}, {118,18}, {122,17}, {125,16}, {128,15}, {131,14}, {135,13}, {138,12},
    {142,11}, {145,10}, {149,9}, {152,8}, {156,7}, {160,6}, {163,5}, {167,4},
    {171,3}, {175,2}, {179,1}, {182,0}
};

const modelnum_t LUT_SIZE[NUM_OF_MODELS] = {
    SV09_LUT_SIZE_DEF,
    SV11_LUT_SIZE_DEF,
};

uint8_t const (*ThermistorLUT[2])[2] = {SV09_thermistor_LUT, SV11_thermistor_LUT};

uint8_t getThermistorTemp (modelnum_t modelnum){    //Binary search algorithm with minor tweaks and rounding
    
    uint16_t pic_thermistor = readADCmV(ADC_THERMISTOR);
    
    uint8_t iteration = 1;
    int16_t increment = LUT_SIZE[modelnum]/(1 << iteration);    //Making this larger and signed because I'm concerned about going out of bounds and being unable to check
    uint8_t index = (uint8_t) increment;   //start at midpoint of array
    
    
    
    //Loop until we find the index where the voltage at i is less than or equal to our read value, and the voltage at i+1 is greater than our read value
    //Meaning we found the two index values our read value is between
    while ( !(ThermistorLUT[modelnum][index][voltage] * 10 <= pic_thermistor) || !(ThermistorLUT[modelnum][index+1][voltage] * 10 > pic_thermistor)  ){
        iteration++;
        if (increment >= 2) increment = LUT_SIZE[modelnum]/(1 << iteration); //Keep halving the increment but make sure increment doesn't go to zero. Using bit shift to calculate power of two, diving LUT_SIZE by bit shift so we don't get successive rounding errors by just dividing the previous increment by two
        if (increment < 1) increment = 1;   //Make sure increment can't go below one
        if ( index + increment > LUT_SIZE[modelnum] - 1 || index - increment < 0){ //if the next increment step would go out of bounds, just break. Subtract one because arrays are zero indexed
            break;
        }
        
        if (ThermistorLUT[modelnum][index][voltage] * 10 < pic_thermistor){
            //Index num is too low
            index = index + (uint8_t) increment;
        }
        else if (ThermistorLUT[modelnum][index][voltage] * 10 > pic_thermistor){
            //Index is too high
            index = index - (uint8_t) increment;
        }
    }
    
    if (index == LUT_SIZE[modelnum] - 1){   //If the resulting index is the max value, reduce it by one so our distance to i calcs work and i+1 doesn't go out of bounds. Arrays are zero indexed.
        index = index - 1;
    }
    
    int16_t dist_to_i = abs((int16_t)ThermistorLUT[modelnum][index][voltage]*10 - (int16_t)pic_thermistor);
    int16_t dist_to_i_plus_one = abs((int16_t)ThermistorLUT[modelnum][index+1][voltage]*10 - (int16_t)pic_thermistor);
    
    if (dist_to_i < dist_to_i_plus_one) {               //If pic_thermistor is closer to i, return i
        return ThermistorLUT[modelnum][index][temp];
    }
    else if (dist_to_i > dist_to_i_plus_one){           //If pic thermistor is closer to i+1, return i+1
        return ThermistorLUT[modelnum][index+1][temp];
    }
    else if (dist_to_i == dist_to_i_plus_one){          //If they are equidistant, round the temperature up (use the lower index))
        return ThermistorLUT[modelnum][index][temp];
    }
    else {
        __debug_break();    //panic
        return ThermistorLUT[modelnum][index][temp]; 
    }
    
    
}
