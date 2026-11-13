#include QMK_KEYBOARD_H
#include "tap_hold.h"

#if defined(AA_OLED_DIAGNOSTIC) && defined(OLED_ENABLE) && defined(CONSOLE_ENABLE)
#    include "i2c_master.h"
#    include "print.h"

void housekeeping_task_user(void) {
  static uint32_t last_check;
  if (timer_elapsed32(last_check) < 3000) {
    return;
  }
  last_check = timer_read32();

  // SSD1306 NOP command: use writes, since OLEDs may not support read probes.
  const uint8_t nop[] = {0x00, 0xE3};
  i2c_status_t at_3c = i2c_transmit(0x3C << 1, nop, sizeof(nop), 20);
  i2c_status_t at_3d = i2c_transmit(0x3D << 1, nop, sizeof(nop), 20);
  uprintf("OLED diag: master=%u, SDA=GP2 SCL=GP3, 0x3C=%d 0x3D=%d, on=%u\n",
          (unsigned)is_keyboard_master(), (int)at_3c, (int)at_3d, (unsigned)is_oled_on());

  // Retry initialization after power has settled, using the configured address.
  if (!is_oled_on() &&
      ((OLED_DISPLAY_ADDRESS == 0x3C && at_3c == I2C_STATUS_SUCCESS) ||
       (OLED_DISPLAY_ADDRESS == 0x3D && at_3d == I2C_STATUS_SUCCESS))) {
    bool initialized = oled_init(OLED_ROTATION_0);
    uprintf("OLED diag: initialization retry=%u\n", (unsigned)initialized);
  }
}
#endif

enum layer_number {
  _COLEMAK = 0,
  _QWERTY = 1,
  _NUM = 2,
  _NAV = 3,
  _SYM = 4,
  _FUN = 5,
};

enum custom_keycodes {
  NUM_PAIR_SHIFT = SAFE_RANGE,
  SYM_PAIR_SHIFT,
  PAIR_PERCENT_CARET,
  PAIR_DECIMAL_EQUALS,
  PAIR_ADD_SUB,
  PAIR_MUL_DIV,
  PAIR_ANGLE,
  PAIR_BRACE,
  PAIR_BRACKET,
  PAIR_PAREN,
  PAIR_DASH_UNDER,
  PAIR_QUOTES,
  PAIR_END,
};

// Pair Shift selects an alternate output without sending a Shift modifier.
// Keep the number and symbol states independent when layers are combined.
static bool num_pair_shift;
static bool sym_pair_shift;

static const uint16_t PROGMEM pair_outputs[][2] = {
  {KC_PERC, KC_CIRC},
  {KC_DOT,  KC_EQL},
  {KC_PLUS, KC_MINS},
  {KC_ASTR, KC_SLSH},
  {KC_LABK, KC_RABK},
  {KC_LCBR, KC_RCBR},
  {KC_LBRC, KC_RBRC},
  {KC_LPRN, KC_RPRN},
  {KC_MINS, KC_UNDS},
  {KC_QUOT, KC_DQUO},
};

// QMK caches the source layer until release. Remember the chosen output too,
// so releasing Pair Shift or the layer thumb first cannot leave a key held.
static uint16_t held_pair_outputs[PAIR_END - PAIR_PERCENT_CARET];

static bool process_pair_key(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case NUM_PAIR_SHIFT:
      num_pair_shift = record->event.pressed && layer_state_is(_NUM);
      return false;
    case SYM_PAIR_SHIFT:
      sym_pair_shift = record->event.pressed && layer_state_is(_SYM);
      return false;
  }

  if (keycode >= PAIR_PERCENT_CARET && keycode < PAIR_END) {
    uint8_t index = keycode - PAIR_PERCENT_CARET;
    if (record->event.pressed) {
      bool alternate = keycode < PAIR_ANGLE ? num_pair_shift : sym_pair_shift;
      held_pair_outputs[index] = pgm_read_word(&pair_outputs[index][alternate]);
      register_code16(held_pair_outputs[index]);
    } else {
      unregister_code16(held_pair_outputs[index]);
      held_pair_outputs[index] = KC_NO;
    }
    return false;
  }
  return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  // Leaving a layer clears its Pair Shift state.
  // Held outputs stay registered until their own release, preserving repeat.
  if (!(state & ((layer_state_t)1 << _NUM))) {
    num_pair_shift = false;
  }
  if (!(state & ((layer_state_t)1 << _SYM))) {
    sym_pair_shift = false;
  }
  return state;
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* COLEMAK
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | `~   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  =+  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   F  |   P  |   B  |                    |   J  |   L  |   U  |   Y  |  ;:  |  -_  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  |   A  |   R  |   S  |   T  |   G  |-------.    ,-------|   M  |   N  |   E  |   I  |   O  |  ' " |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * | MUTE |   Z  |   X  |   C  |   D  |   V  |-------|    |-------|   K  |   H  |   ,> |   .> |  /?  |  \ | |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | XXXX | ESC  | TAB  | /Space  /       \Enter \  |BackSP| DEL  | XXXX |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_COLEMAK] = LAYOUT(
  KC_GRV,  KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_EQL,
  KC_TAB,  KC_Q,   KC_W,    KC_F,    KC_P,    KC_B,                     KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, KC_MINS,
  KC_ESC, MT_AGUI, MT_RALT, MT_SCTL, MT_TSFT, KC_G,                     KC_M, MT_NSFT, MT_ECTL, MT_IALT, MT_OGUI, KC_QUOT,
  KC_MUTE, KC_Z,   KC_X,    KC_C,    KC_D,    KC_V, KC_LBRC,  KC_RBRC,  KC_K,    KC_H,  KC_COMM, KC_DOT, KC_SLSH, KC_BSLS,
                        KC_NO, KC_ESC, LNM_TAB, LNV_SPC,         LSM_ENT, LFN_BSP, KC_DEL, KC_NO
),


/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | `~   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  =+  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * | MUTE |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |  \ | |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   |Colemk| ESC  | TAB  | /Space  /       \Enter \  |BackSP| DEL  |Colemk|
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_EQL,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,   KC_MINS,
  KC_ESC, MT_AGUI, MT_SALT, MT_DCTL, MT_FSFT,  KC_G,                    KC_H, MT_JSFT, MT_KCTL, MT_LALT, MT_CGUI,   KC_QUOT,
  KC_MUTE, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC,  KC_RBRC,  KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS,
                        DF(0), KC_ESC, LNM_TAB, LNV_SPC,         LSM_ENT, LFN_BSP, KC_DEL, DF(0)
),


/* NUMPAD: the five right-hand Colemak columns (j l u y ; / m n e i o / k h , . /).
 * Hold the Colemak c position for Pair Shift; x/y means normal/alternate.
 * The number row and outer punctuation keys inherit the base layout.
 *
 * Left home: A/GUI  R/Alt  S/Ctrl  T/Shift  G
 * Left bottom: Z  X  PairShift  D  V
 * Right top:     $       7       8       9       %/^
 * Right home:    0       4       5       6       +/-
 * Right bottom: ./=      1       2       3       multiply/divide
 * Right inner key: Space. Both outer thumb keys are disabled.
 */
 [_NUM] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,                   KC_DLR,  KC_7,    KC_8,    KC_9,    PAIR_PERCENT_CARET, _______,
  _______, _______, _______, _______, _______, _______,             KC_0,    KC_4,    KC_5,    KC_6,    PAIR_ADD_SUB,       _______,
  _______, _______, _______, NUM_PAIR_SHIFT, _______, _______, _______, KC_SPC, PAIR_DECIMAL_EQUALS, KC_1, KC_2, KC_3, PAIR_MUL_DIV,    _______,
                        KC_NO, _______, _______, _______,           _______, _______, _______, KC_NO
),


/* NAV: match Kanata's right-hand navigation block.
 *
 * Right number row: inherits the base layout.
 * Right top:         Redo    Home  PgDn  PgUp   End
 * Right home:        Undo    Left  Down  Up     Right
 * Right bottom: [base k]  Cut  Copy  Paste  [base /]
 * Left bottom X/C/D: middle/left/right mouse buttons, as before.
 * Right inner key: Escape. Both outer thumb keys are disabled.
 * Undo/Redo send Super+Z / Super+Shift+Z, matching Kanata.
 */
 [_NAV] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,                   G(S(KC_Z)), KC_HOME, KC_PGDN, KC_PGUP, KC_END, _______,
  _______, _______, _______, _______, _______, _______,                   G(KC_Z), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, _______,
  _______, _______, MS_BTN3, MS_BTN1, MS_BTN2, _______, _______, KC_ESC,    _______, KC_CUT, KC_COPY, KC_PSTE, _______, _______,
                        KC_NO, _______, _______, _______,           _______, _______, _______, KC_NO
),


/* SYM: the five left-hand Colemak columns (q w f p b / a r s t g / z x c d v).
 * Hold the Colemak comma position for Pair Shift; x/y means normal/alternate.
 * The number row and outer punctuation keys inherit the base layout.
 *
 * Left top:     grave     &       |       !       ?
 * Left home:      </>    {/}     [/]     (/)      @
 * Left bottom: backslash  ~      -/_     '/"      #
 * Right home: M  N/Shift  E/Ctrl  I/Alt  O/GUI
 * Right bottom: K  H  PairShift  .  /
 * Left inner key: Enter. Both outer thumb keys are disabled.
 */
 [_SYM] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  _______, KC_GRV, KC_AMPR, KC_PIPE, KC_EXLM, KC_QUES,                      _______, _______, _______, _______, _______, _______,
  _______, PAIR_ANGLE, PAIR_BRACE, PAIR_BRACKET, PAIR_PAREN, KC_AT,          _______, _______, _______, _______, _______, _______,
  _______, KC_BSLS, KC_TILD, PAIR_DASH_UNDER, PAIR_QUOTES, KC_HASH, KC_ENT, _______, _______, _______, SYM_PAIR_SHIFT, _______, _______, _______,
                        KC_NO, _______, _______, _______,           _______, _______, _______, KC_NO
),


/* FUN
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | FN1  | FN2  | FN3  | FN4  | FN5  | FN6  |                    |      |      |      |      |      |KbBoot|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | FN7  | FN8  | FN9  | FN10 | FN11 | FN12 |                    |      | Power| Sleep|Colemk|QWERTY|      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | Mic  | Mute |VolDwn| VolUp|      |-------.    ,-------|      |      |      |      |      |      |
 * |------+------+------+------+------+------| BackSP|    |       |------+------+------+------+------+------|
 * |      |      |PrtScr|BriDwn| BriUp|      |-------|    |-------|      |      |      |      |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | XXXX |      |      | /       /       \      \  |      |      | XXXX |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_FUN] = LAYOUT(
  KC_F1  ,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                   _______, _______, _______, _______, _______, QK_BOOT,
  KC_F7  ,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,                   _______,  KC_PWR, KC_SLEP,   DF(0),   DF(1), _______,
  _______,  KC_F20, KC_MUTE, KC_VOLD, KC_VOLU, _______,                   _______, _______, _______, _______, _______, _______,
  _______, _______, KC_PSCR, KC_BRID, KC_BRIU, _______, KC_BSPC, _______, _______, _______, _______, _______, _______, _______,
                        KC_NO, _______, _______, _______,            _______,  _______, _______, KC_NO
  )
};

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
#ifdef AA_OLED_DIAGNOSTIC
  oled_on();
  oled_set_cursor(0, 0);
  oled_write_ln("OLED TEST", false);
  oled_write_ln(is_keyboard_master() ? "Role: master" : "Role: peripheral", false);
  oled_write_ln("128x32 / I2C", false);
  oled_write_ln("Text rendering OK", false);
#else
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
#endif
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
  return process_pair_key(keycode, record);
}
