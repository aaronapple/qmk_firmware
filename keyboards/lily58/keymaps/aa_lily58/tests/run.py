"""Compile and check the keymap on the host without the ARM toolchain."""

import json
import subprocess
import tempfile
from pathlib import Path

keymap = Path(__file__).resolve().parents[1]
repo = keymap.parents[3]

with tempfile.TemporaryDirectory(prefix='lily58-keymap-check-') as directory:
    tmp = Path(directory)
    layout = json.loads((repo / 'keyboards/lily58/rev1/keyboard.json').read_text())['layouts']['LAYOUT']['layout']
    names = [f'k{i}' for i in range(len(layout))]
    matrix = [['KC_NO'] * 6 for _ in range(10)]
    for name, key in zip(names, layout):
        row, col = key['matrix']
        matrix[row][col] = name
    initializer = '{' + ','.join('{' + ','.join(row) + '}' for row in matrix) + '}'
    (tmp / 'mock_keyboard.h').write_text('''#pragma once
    #include <stdbool.h>
    #include <stdint.h>
    #include "modifiers.h"
    #include "quantum_keycodes.h"
    #include "config.h"
    #define MATRIX_ROWS 10
    #define MATRIX_COLS 6
    #define PROGMEM
    #define pgm_read_word(ptr) (*(ptr))
    typedef uint32_t layer_state_t;
    typedef struct { struct { bool pressed; } event; } keyrecord_t;
    extern layer_state_t layer_state;
    #define layer_state_is(layer) ((layer_state & (1u << (layer))) != 0)
    void register_code16(uint16_t code);
    void unregister_code16(uint16_t code);
    bool get_chordal_hold_default(keyrecord_t *tap_hold_record, keyrecord_t *other_record);
    ''' + '#define LAYOUT(' + ','.join(names) + ') ' + initializer + '\n')
    subprocess.run(
        [
            'gcc', '-std=c11', '-Wall', '-Wextra', '-Werror',
            '-fsanitize=address,undefined', '-g',
            '-DQMK_KEYBOARD_H="mock_keyboard.h"',
            '-I' + str(tmp),
            '-I' + str(repo / 'quantum'),
            '-I' + str(repo / 'quantum/keymap_extras'),
            '-I' + str(repo / 'quantum/sequencer'),
            '-I' + str(keymap),
            str(keymap / 'tests/pair_shift.c'),
            '-o', str(tmp / 'check'),
        ],
        check=True,
    )
    subprocess.run([str(tmp / 'check')], check=True)
