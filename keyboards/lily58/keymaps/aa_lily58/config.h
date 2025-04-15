/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// Tapping
#define TAPPING_TERM 200
#define QUICK_TAP_TERM 0
#define PERMISSIVE_HOLD
#define CHORDAL_HOLD

// Custom
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

// Bootloader
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

// Split
#define MASTER_RIGHT
#define SPLIT_LAYER_STATE_ENABLE
#define SERIAL_USART_TX_PIN GP1
// #define SPLIT_WPM_ENABLE

// Pointing
#define AZOTEQ_IQS5XX_TPS43
#define MOUSE_EXTENDED_REPORT

// Thumb Layer Tap Keys
#define LNM_SPC LT(_NUM, KC_SPC)
#define LNV_ESC LT(_NAV, KC_ESC)
#define LSM_ENT LT(_SYM, KC_ENT)
#define LFN_BSP LT(_FUN, KC_BSPC)

// Home-Row Mods for Colemak
#define MT_AGUI LGUI_T(KC_A)
#define MT_RALT LALT_T(KC_R)
#define MT_SCTL LCTL_T(KC_S)
#define MT_TSFT LSFT_T(KC_T)

#define MT_NSFT LSFT_T(KC_N)
#define MT_ECTL LCTL_T(KC_E)
#define MT_IALT LALT_T(KC_I)
#define MT_OGUI LGUI_T(KC_O)

// Home-Row Mods for QWERTY
#define MT_AGUI LGUI_T(KC_A)
#define MT_SALT LALT_T(KC_S)
#define MT_DCTL LCTL_T(KC_D)
#define MT_FSFT LSFT_T(KC_F)

#define MT_JSFT LSFT_T(KC_J)
#define MT_KCTL LCTL_T(KC_K)
#define MT_LALT LALT_T(KC_L)
#define MT_CGUI LGUI_T(KC_SCLN)

/*
#undef RGBLIGHT_LED_COUNT
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_CHRISTMAS
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
#define RGBLIGHT_EFFECT_RGB_TEST
#define RGBLIGHT_EFFECT_ALTERNATING
#define RGBLIGHT_EFFECT_TWINKLE
#define RGBLIGHT_LED_COUNT 27
#define RGBLIGHT_LIMIT_VAL 120
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17
*/

// Underglow
/*
#undef RGBLIGHT_LED_COUNT
#define RGBLIGHT_LED_COUNT 14    // Number of LEDs
#define RGBLIGHT_SLEEP
*/
