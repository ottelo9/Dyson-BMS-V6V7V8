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

#ifndef MAIN_H
#define	MAIN_H

#include "mcc_generated_files/adc.h"

enum {
    INIT = 0,
    SLEEP,
    IDLE,
    CHARGING,
    CHARGING_WAIT,
    CELL_BALANCE,
    OUTPUT_EN,
    ERROR,
};
extern uint8_t state;

typedef enum{
    NONE = 0,       //0b00
    TRIGGER = 1,    //0b01
    CHARGER = 2     //0b10
} detect_t;

extern detect_t detect;

extern uint8_t detect_history; //Bits 0-1 = position 0; Bits 2-3 = position 1; Bits 4-5 = position 2; Bits 6-7 = position 3

extern uint16_t adc_chrg_trig_detect_voltage_history[5];

extern uint8_t pack_charge_wait_repeats_counter; // Counter for WaitCharge -> Charge repetations

extern uint8_t enable_slow_charge; // for slow charging if mincell voltage below 3000mV but no Error

typedef enum {
    SV09 = 0,
    SV11 = 1,
    NUM_OF_MODELS,
} modelnum_t;
extern modelnum_t modelnum;

extern int16_t isl_int_temp;
extern int16_t isl_int_temp_max;
extern uint8_t thermistor_temp;
extern uint8_t thermistor_temp_max;
extern bool charge_complete_flag;
extern bool full_discharge_flag;
extern uint16_t discharge_current_mA;
extern uint16_t discharge_current_mA_last_trigger;
extern uint16_t discharge_current_mA_last_trigger_eeprom;
extern uint16_t mincell_voltage_mV_last_trigger;
extern uint8_t minimum_cell_last_trigger;
extern uint32_t mincell_internal_resistence_last_charge_uOhms;
extern uint32_t mincell_internal_resistence_max;
extern uint16_t packdelta_end_of_charging_wait_mV;
extern uint16_t mincell_voltage_mV_last_trigger_eeprom;
extern uint16_t mincell_voltage_mV_last_trigger_recover_eeprom;
extern uint8_t I2C_error_counter;
extern int8_t cell_offset_voltage_1;
extern int8_t cell_offset_voltage_2;
extern int8_t cell_offset_voltage_3;
extern int8_t cell_offset_voltage_4;
extern int8_t cell_offset_voltage_5;
extern int8_t cell_offset_voltage_6;

typedef struct {
    uint16_t value;
    bool enable;
} counter_t;

typedef struct {
    uint32_t value;
    bool enable;
} big_counter_t;

extern counter_t charge_wait_counter;
extern counter_t sleep_timeout_counter;
extern counter_t nonblocking_wait_counter;
extern counter_t error_timeout_wait_counter;
extern big_counter_t charge_duration_counter;
extern counter_t LED_code_cycle_counter;
extern big_counter_t total_runtime_counter;
extern big_counter_t onetime_runtime_counter;

detect_t GetDetectHistory(uint8_t position);
bool CheckStateInDetectHistory(detect_t detect_val);

uint16_t readADCmV(adc_channel_t channel);
void Write32BitUintVariableToEEPROM(uint8_t starting_addr, uint32_t variable_to_write);
uint32_t Read32BitUintVariableFromEEPROM(uint8_t starting_addr);
void Write16BitUintVariableToEEPROM(uint8_t starting_addr, uint16_t variable_to_write);
uint16_t Read16BitUintVariableFromEEPROM(uint8_t starting_addr);
void ClearI2CBus(void);













#endif	/* MAIN_H */

