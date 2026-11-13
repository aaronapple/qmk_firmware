// SPDX-License-Identifier: GPL-2.0-or-later
// Host regression checks; register/unregister are mocked, not USB reports.
#include <assert.h>
#include <stdio.h>
#include "mock_keyboard.h"
layer_state_t layer_state;
static uint16_t last_pressed, last_released;
static unsigned press_count, release_count;
void register_code16(uint16_t code) { last_pressed = code; ++press_count; }
void unregister_code16(uint16_t code) { last_released = code; ++release_count; }
static bool default_chordal_hold;
bool get_chordal_hold_default(keyrecord_t *tap_hold_record, keyrecord_t *other_record) {
    (void)tap_hold_record;
    (void)other_record;
    return default_chordal_hold;
}
#include "../keymap.c"

static bool event(uint16_t code, bool pressed) {
    keyrecord_t record = {.event.pressed = pressed};
    return process_record_user(code, &record);
}
static void layers(layer_state_t state) {
    layer_state = layer_state_set_user(state);
}
int main(void) {
    keyrecord_t record = {0};
    const uint16_t thumbs[] = {LNM_TAB, LNV_SPC, LSM_ENT, LFN_BSP};
    for (unsigned i = 0; i < sizeof(thumbs) / sizeof(thumbs[0]); ++i) {
        assert(get_hold_on_other_key_press(thumbs[i], &record));
        assert(get_chordal_hold(thumbs[i], &record, KC_C, &record));
    }
    assert(!get_hold_on_other_key_press(MT_SCTL, &record));
    assert(!get_chordal_hold(MT_SCTL, &record, KC_C, &record));
    default_chordal_hold = true;
    assert(get_chordal_hold(MT_SCTL, &record, KC_K, &record));
    const uint16_t expected[][2] = {
        {S(KC_5), S(KC_6)}, {KC_DOT, KC_EQL}, {S(KC_EQL), KC_MINS}, {S(KC_8), KC_SLSH},
        {S(KC_COMM), S(KC_DOT)}, {S(KC_LBRC), S(KC_RBRC)}, {KC_LBRC, KC_RBRC},
        {S(KC_9), S(KC_0)}, {KC_MINS, S(KC_MINS)}, {KC_QUOT, S(KC_QUOT)}
    };
    for (unsigned i = 0; i < 10; ++i) {
        unsigned layer = i < 4 ? _NUM : _SYM;
        uint16_t shift = i < 4 ? NUM_PAIR_SHIFT : SYM_PAIR_SHIFT;
        uint16_t code = PAIR_PERCENT_CARET + i;
        for (unsigned alternate = 0; alternate < 2; ++alternate) {
            layers(1u << layer);
            if (alternate) assert(!event(shift, true));
            unsigned before = press_count;
            assert(!event(code, true));
            assert(press_count == before + 1);
            assert(last_pressed == expected[i][alternate]);
            // Shift release does not replace or release the held output.
            unsigned releases = release_count;
            assert(!event(shift, false));
            assert(release_count == releases);
            // Neither does layer exit when releasing a thumb.
            layers(0);
            assert(release_count == releases);
            assert(!event(code, false));
            assert(last_released == expected[i][alternate]);
            assert(held_pair_outputs[i] == KC_NO);
        }
        // Pressing Pair Shift after a symbol does not change its release.
        layers(1u << layer);
        event(code, true);
        event(shift, true);
        event(code, false);
        assert(last_released == expected[i][0]);
        // Leaving and reentering the layer cannot retain Pair Shift.
        layers(0);
        layers(1u << layer);
        event(code, true);
        assert(last_pressed == expected[i][0]);
        event(code, false);
        event(shift, false);
    }
    layers((1u << _NUM) | (1u << _SYM));
    event(NUM_PAIR_SHIFT, true);
    event(PAIR_BRACE, true);
    assert(last_pressed == KC_LCBR);
    event(PAIR_BRACE, false);
    event(SYM_PAIR_SHIFT, true);
    event(NUM_PAIR_SHIFT, false);
    event(PAIR_ADD_SUB, true);
    assert(last_pressed == KC_PLUS);
    event(PAIR_ADD_SUB, false);
    event(PAIR_QUOTES, true);
    assert(last_pressed == KC_DQUO);
    event(PAIR_QUOTES, false);
    layers(1u << _SYM);
    assert(!num_pair_shift && sym_pair_shift);
    layers(0);
    assert(!num_pair_shift && !sym_pair_shift);
    event(NUM_PAIR_SHIFT, true);
    event(SYM_PAIR_SHIFT, true);
    assert(!num_pair_shift && !sym_pair_shift);
    assert(event(KC_1, true));
    assert(event(KC_1, false));
    assert(keymaps[_NUM][3][3] == NUM_PAIR_SHIFT);
    assert(keymaps[_SYM][8][3] == SYM_PAIR_SHIFT);
    assert(keymaps[_NUM][2][3] == KC_TRNS); // Inherit left home-row Ctrl.
    assert(keymaps[_SYM][7][3] == KC_TRNS); // Inherit right home-row Ctrl.
    assert(keymaps[_COLEMAK][2][3] == LCTL_T(KC_S));
    assert(keymaps[_COLEMAK][7][3] == RCTL_T(KC_E));
    assert(keymaps[_NUM][6][5] == KC_DLR);
    assert(keymaps[_NUM][6][4] == KC_7);
    assert(keymaps[_NUM][7][5] == KC_0);
    assert(keymaps[_NUM][8][5] == PAIR_DECIMAL_EQUALS);
    assert(keymaps[_SYM][2][1] == PAIR_ANGLE);
    assert(keymaps[_SYM][2][4] == PAIR_PAREN);
    assert(keymaps[_NAV][6][5] == G(S(KC_Z)));
    assert(keymaps[_NAV][6][4] == KC_HOME);
    assert(keymaps[_NAV][7][5] == G(KC_Z));
    assert(keymaps[_NAV][7][4] == KC_LEFT);
    assert(keymaps[_NAV][8][4] == KC_CUT);
    assert(keymaps[_NAV][8][3] == KC_COPY);
    assert(keymaps[_NAV][8][2] == KC_PSTE);
    assert(keymaps[_NAV][5][4] == KC_TRNS);
    assert(keymaps[_NAV][5][3] == KC_TRNS);
    assert(keymaps[_NAV][5][2] == KC_TRNS);
    for (unsigned layer = _COLEMAK; layer <= _QWERTY; ++layer) {
        assert(keymaps[layer][4][2] == KC_ESC);
        assert(keymaps[layer][4][3] == LT(_NUM, KC_TAB));
        assert(keymaps[layer][4][4] == LT(_NAV, KC_SPC));
    }
    assert(keymaps[_COLEMAK][9][4] == LSM_ENT);
    assert(keymaps[_COLEMAK][9][3] == LFN_BSP);
    for (unsigned layer = _NUM; layer <= _FUN; ++layer) {
        assert(keymaps[layer][4][1] == KC_NO);
        assert(keymaps[layer][9][1] == KC_NO);
    }
    for (unsigned layer = _COLEMAK; layer <= _FUN; ++layer) {
        for (unsigned row = 0; row < MATRIX_ROWS; ++row) {
            for (unsigned col = 0; col < MATRIX_COLS; ++col) {
                assert(keymaps[layer][row][col] != QK_LLCK);
            }
        }
    }
    for (unsigned col = 0; col < 6; ++col) {
        assert(keymaps[_FUN][0][col] == KC_F1 + col);
        assert(keymaps[_FUN][1][col] == KC_F7 + col);
    }
    assert(MT_NSFT == RSFT_T(KC_N));
    assert(MT_ECTL == RCTL_T(KC_E));
    assert(MT_IALT == RALT_T(KC_I));
    assert(MT_OGUI == RGUI_T(KC_O));
    assert(press_count == release_count);
    puts("PASS: full keymap host compilation; all 20 pair outputs; shift/layer release ordering; state isolation/reset; navigation, thumbs, disabled layer locks, Ctrl access, and function order.");
}
