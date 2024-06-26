/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
 */

#include <tcMenu.h>
#include "PWM_arduino_menu.h"

// Global variable declarations
const  ConnectorLocalInfo applicationInfo = { "Detector Mu", "34d14ed3-dd03-4183-8a95-5b41584616ee" };

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
LiquidCrystalRenderer renderer(lcd, 16, 2);

// Global Menu Item declarations
const AnalogMenuInfo minfoVoltajeDeSalida = { "Voltaje de salida", 1, 4, 5000, NO_CALLBACK, 0, 1, "V" };
AnalogMenuItem menuVoltajeDeSalida(&minfoVoltajeDeSalida, 0, nullptr, INFO_LOCATION_PGM);

void setupMenu() {
    // First we set up eeprom and authentication (if needed).
    setSizeBasedEEPROMStorageEnabled(true);
    // Now add any readonly, non-remote and visible flags.
    menuVoltajeDeSalida.setLocalOnly(true);

    // Code generated by plugins and new operators.
    lcd.begin(16, 2);
    renderer.setUpdatesPerSecond(2);
    lcd.configureBacklightPin(10);
    lcd.backlight();
    pinMode(A0, INPUT);
    switches.initialise(inputFromDfRobotShield(), false);
    menuMgr.initFor4WayJoystick(&renderer, &menuVoltajeDeSalida, DF_KEY_DOWN, DF_KEY_UP, DF_KEY_LEFT, DF_KEY_RIGHT, DF_KEY_SELECT, 20);
}

