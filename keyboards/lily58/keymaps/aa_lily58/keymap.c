#include QMK_KEYBOARD_H

enum layer_number {
  _COLEMAK = 0,
  _QWERTY = 1,
  _NUM = 2,
  _NAV = 3,
  _SYM = 4,
  _FUN = 5,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* COLEMAK
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | `~   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  =+  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   Y  |   U  |  ;:  |  -_  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  |   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  ' " |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * | MUTE |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,> |   .> |  /?  |  \ | |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | XXXX | TAB  | ESC  | /Space  /       \Enter \  |BackSP| DEL  | XXXX |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_COLEMAK] = LAYOUT(
  KC_GRV,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_EQL,
  KC_TAB,  KC_Q,   KC_W,    KC_F,    KC_P,    KC_B,                     KC_J,    KC_L,    KC_Y,    KC_U, KC_SCLN, KC_MINS,
  KC_ESC, MT_AGUI, MT_RALT, MT_SCTL, MT_TSFT, KC_G,                     KC_M, MT_NSFT, MT_ECTL, MT_IALT, MT_OGUI, KC_QUOT,
  KC_MUTE, KC_Z,   KC_X,    KC_C,    KC_D,    KC_V, KC_LBRC,  KC_RBRC,  KC_K,    KC_H,  KC_COMM, KC_DOT, KC_SLSH, KC_BSLS,
                        KC_NO, KC_TAB, LNV_ESC, LNM_SPC,         LSM_ENT, LFN_BSP, KC_DEL, KC_NO
),


/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LCTRL |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |Colemk| TAB  | ESC  | /Space  /       \Enter \  |BackSP| DEL  |Colemk|
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_EQL,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,   KC_MINS,
  KC_ESC, MT_AGUI, MT_SALT, MT_DCTL, MT_FSFT,  KC_G,                    KC_H, MT_JSFT, MT_KCTL, MT_LALT, MT_CGUI,   KC_QUOT,
  KC_MUTE, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC,  KC_RBRC,  KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS,
                        DF(0), KC_TAB, LNV_ESC, LNM_SPC,         LSM_ENT, LFN_BSP, KC_DEL, DF(0)
),


/* NUMPAD
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |   k  |   +  |   -  |   *  |   /  |   ^  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |   j  |   =  |   7  |   8  |   9  |   <  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|   G  |   0  |   4  |   5  |   6  |   >  |
 * |------+------+------+------+------+------|       |    | Space |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|   ,  |   .  |   1  |   2  |   3  |   %  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LOCK |      |      | /       /       \      \  |      |      | LOCK |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_NUM] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                   KC_K,   KC_PPLS, KC_PMNS, KC_PAST, KC_PSLS, KC_CIRC,
  _______, _______, _______, _______, _______, _______,                   KC_J,   KC_PEQL,  KC_P7,   KC_P8,  KC_P9, KC_LABK,
  _______, _______, _______, _______, _______, _______,                   S(KC_G),  KC_P0,  KC_P4,   KC_P5,  KC_P6, KC_RABK,
  _______, _______, _______, _______, _______, _______, _______,  KC_SPC, KC_PCMM, KC_PDOT, KC_P1,   KC_P2,  KC_P3, KC_PERC,
                        QK_LLCK, _______, _______, _______,           _______,  _______, _______, QK_LLCK
),


/* NAV
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    | REDO |  CUT | COPY |PASTE |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------| UNDO | Left | Down |  Up  |Right |      |
 * |------+------+------+------+------+------|       |    | ESC   |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      | HOME |PgDown| PgUp | END  |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LOCK |      |      | /       /       \      \  |      |      | LOCK |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_NAV] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  _______, _______, _______, _______, _______, _______,                      KC_AGIN,  KC_CUT, KC_COPY, KC_PSTE, XXXXXXX, XXXXXXX,
  _______, _______, _______, _______, _______, _______,                      KC_UNDO, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, XXXXXXX,
  _______, _______, MS_BTN3, MS_BTN1, MS_BTN2, _______,   _______,  KC_ESC,  XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END, XXXXXXX,
                            QK_LLCK, _______, _______, _______,       _______,  _______, _______, QK_LLCK
),


/* SYM
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |   <  |   >  |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |   !  |   ~  |   `  |   {  |   }  |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |   &  |   _  |   "  |   (  |   )  |-------.    ,-------|      |      |      |      |      |      |
 * |------+------+------+------+------+------| ENTER |    |       |------+------+------+------+------+------|
 * |      |   |  |   -  |   '  |   [  |   ]  |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LOCK |      |      | /       /       \      \  |      |      | LOCK |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_SYM] = LAYOUT(
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LABK, KC_RABK,                  _______, _______, _______, _______, _______, _______,
  _______, KC_EXLM, KC_TILD, KC_GRV,  KC_LCBR, KC_RCBR,                  _______, _______, _______, _______, _______, _______,
  _______, KC_AMPR, KC_UNDS, KC_DQUO, KC_LPRN, KC_RPRN,                  _______, _______, _______, _______, _______, _______,
  _______, KC_PIPE, KC_MINS, KC_QUOT, KC_LBRC, KC_RBRC, KC_ENT, _______, _______, _______, _______, _______, _______, _______,
                        QK_LLCK, _______, _______, _______,         _______,  _______, _______, QK_LLCK
  ),


/* FUN
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | FN7  | FN8  | FN9  | FN10 | FN11 | FN12 |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | FN1  | FN2  | FN3  | FN4  | FN5  | FN6  |                    |      | Sleep| Power|QWERTY|KbBoot|      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | Mic  | Mute | VolUp|VolDwn|      |-------.    ,-------|      |      |      |      |      |      |
 * |------+------+------+------+------+------| BackSP|    |       |------+------+------+------+------+------|
 * |      |      |PrtScr| ScnUp|ScnDwn|      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LOCK |      |      | /       /       \      \  |      |      | LOCK |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_FUN] = LAYOUT(
  KC_F7  ,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,                   _______, _______, _______, _______, _______, QK_BOOT,
  KC_F1  ,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                   _______,  KC_PWR, KC_SLEP,   DF(0),   DF(1), _______,
  _______,  KC_F20, KC_MUTE, KC_VOLU, KC_VOLD, _______,                   _______, _______, _______, _______, _______, _______,
  _______, _______, KC_PSCR, KC_BRIU, KC_BRID, _______, KC_BSPC, _______, _______, _______, _______, _______, _______, _______,
                        QK_LLCK, _______, _______, _______,            _______,  _______, _______, QK_LLCK
  )
};

// layer_state_t layer_state_set_user(layer_state_t state) {
//   return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
// }

//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return rotation;
}

// When you add source files to SRC in rules.mk, you can use functions.
// const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

bool oled_task_user(void) {
  if (is_keyboard_master()) {
    // If you want to change the display of OLED, you need to change here
    // oled_write_ln(read_layer_state(), false);
    oled_write_ln(read_keylog(), false);
    oled_write_ln(read_keylogs(), false);
    //oled_write_ln(read_mode_icon(keymap_config.swap_lalt_lgui), false);
    //oled_write_ln(read_host_led_state(), false);
    //oled_write_ln(read_timelog(), false);
  } else {
    oled_write(read_logo(), false);
  }
    return false;
}
#endif // OLED_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef OLED_ENABLE
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }
  return true;
}
