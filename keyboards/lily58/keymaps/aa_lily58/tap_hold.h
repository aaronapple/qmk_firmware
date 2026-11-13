// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Thumb layers must resolve before another key is processed, including a
// same-hand Pair Shift. Otherwise a fast Numbers + c chord can emit the tap
// action followed by c, and the decimal/equals position emits its base k.
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    (void)record;
    return IS_QK_LAYER_TAP(keycode);
}

bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    (void)other_keycode;
    if (IS_QK_LAYER_TAP(tap_hold_keycode)) {
        return true;
    }
    // Keep same-hand roll protection and permissive hold for home-row mods.
    return get_chordal_hold_default(tap_hold_record, other_record);
}
