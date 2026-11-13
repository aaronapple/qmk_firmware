# Aaron's Lily58

Colemak with home-row modifiers and utility layers aligned with the laptop's
Kanata configuration. The Lily keeps its thumb layer taps:

| Thumb | Tap | Hold |
| --- | --- | --- |
| Left Tab (next to Space) | Tab | Numbers |
| Left Space | Space | Navigation |
| Right Enter | Enter | Coding symbols |
| Right Backspace | Backspace | Functions |

The next left thumb key outward from Tab sends plain Escape.

On Numbers, hold the Colemak **c** position for Pair Shift. On Coding, hold
the Colemak **comma** position. Pair Shift selects alternate punctuation without
sending a real Shift modifier. The layer comments in `keymap.c` show the
normal and alternate outputs. Both home-row Ctrl keys remain available.
Layer locking is disabled; the two outer thumb keys do nothing on utility layers.

Navigation has Home/Page Down/Page Up/End above the arrows. Cut/Copy/Paste
are on the right bottom-row **h / comma / period** positions. Undo/Redo send Super+Z and
Super+Shift+Z, matching Kanata.

On Functions, F1–F6 run left to right across the top left row, and F7–F12
across the second left row.

Thumb layer taps activate their layer as soon as another key is pressed,
including keys on the same hand. This lets Numbers + c Pair Shift work
without waiting 200 ms and prevents the thumb from accidentally sending
its tap action before c/k reaches an editor. A thumb pressed and released alone
still sends its tap action. Home-row modifiers retain Chordal Hold and
Permissive Hold for typing rolls.

## Validation

Run the host regression checks with Python 3 and GCC:

```sh
python3 keyboards/lily58/keymaps/aa_lily58/tests/run.py
make test:aa_lily58_tap_hold
```

The checks compile the actual keymap using QMK's keycode headers and the
matrix from `lily58/rev1/keyboard.json`, with keyboard I/O mocked. They cover
all paired outputs, release ordering, state reset, navigation, thumb access,
and modifier assignments. The QMK tapping tests additionally exercise fast
same-hand thumb chords, standalone thumb taps, and home-row typing rolls
through QMK's actual tapping engine. They do not replace an ARM firmware build
or hardware checks.

Aaron compiled and flashed both halves and confirmed the revised keymap works
on hardware, including the thumb timing fix, Pair Shift, and final Tab/Escape
swap. OLED bring-up remains a separate task.

## Hardware and build context

This checkout targets `lily58/rev1` with RP2040 controllers, a right master,
Azoteq TPS43 touchpad, and OLED. Commit `f310a4fbd6` records the original
Holykeebs-derived hardware configuration. Preserve the local matrix pins,
serial pin, and I2C settings when building.

Configure an installed QMK CLI to use this checkout and keymap:

```sh
cd ~/projects/qmk_firmware
qmk config user.qmk_home="$PWD"
qmk config user.keyboard=lily58/rev1 user.keymap=aa_lily58
git submodule update --init --recursive
qmk doctor
```

QMK uses the Git branch currently checked out in this directory. The CLI is
separate from the compiler and firmware dependencies checked by `qmk doctor`.
Aaron manages installation, compilation, and flashing; Codex provides guidance.

Compile without flashing:

```sh
qmk compile -kb lily58/rev1 -km aa_lily58
```

This target successfully built with ARM GCC 15.2.0 against this fork's pinned
submodules. Keep a copy of the working UF2 before updating upstream.

### Flashing

```sh
qmk flash -kb lily58/rev1 -km aa_lily58
```

This command rebuilds and then waits for the bootloader drive. Flash each half
separately, using the same command and firmware image; this configuration does
not need `SIDE` or split-handedness flags.

1. Disconnect USB before inserting or removing TRRS. Leave TRRS disconnected
   while flashing halves individually.
2. Connect one half by USB, run the flash command, and enter its bootloader.
   On the right half with this keymap, hold the Backspace thumb key to activate
   Functions, then press the top-right `= / +` key (`QK_BOOT`). Double-tapping
   the board's Reset button is another method when supported by its firmware.
3. Wait for successful deployment before disconnecting USB. Repeat for the
   other half.
4. With both halves unpowered, insert TRRS fully at both ends, then connect
   USB to the right half (`MASTER_RIGHT`) for normal use.

The bootloader appears as `RP2 Boot` in `lsusb` and exposes an `RPI-RP2`
storage volume. The CLI deploys the UF2 through that volume; copying the UF2
to it manually is an alternative. If the volume appears but the CLI keeps
waiting, open it in the file manager to mount it.

### Troubleshooting learned during bring-up

- **RP2 Boot appears in USB listings but no storage drive appears:** after an
  Arch kernel upgrade, the running kernel may no longer have matching modules
  on disk. During this session, rebooting resolved a missing `usb_storage`
  module and allowed flashing. Check `uname -r`, `/usr/lib/modules`, and
  `lsblk` before assuming bootloader entry failed.
- **The assembled split is unresponsive:** power off and check that TRRS is
  fully seated at both ends. A partially inserted cable caused the connection
  problem during this session.
- **Fast Numbers + Pair Shift sends editor commands:** thumb layers must be
  exempt from Chordal Hold's same-hand tap rule and use hold-on-other-key-press.
  Without this, the former Escape/Numbers thumb could send `Escape`, `c`, `k`,
  which Neovim interprets as a change command across two lines. `tap_hold.h`
  implements the fix for all four thumb layers while retaining the home-row
  modifier rules.
- **Suspected double remapping:** the laptop's Kanata configuration explicitly
  selects its internal i8042 keyboard. It does not select the Lily58; preserve
  that device selection when changing the laptop configuration.

Holykeebs' [current firmware guide](https://docs.holykeebs.com/firmware/)
uses the standard QMK development environment with their `hk-master` firmware
forks and a separate userspace overlay. Their current Lily58 target is
`holykeebs/lily58`; that is a different source layout from this checkout.
The guide does not specify a special compiler version. Migrating to that
layout is separate from updating this keymap.

## Next steps

- [x] Build and flash this fork and verify the revised keymap on the assembled
  keyboard, including thumb layers and Pair Shift.
- [ ] Preserve a copy of the known working UF2 and explicitly verify touchpad
  operation before upstream migration.
- [ ] After baseline verification, sync with upstream QMK on a separate
  branch and make the relevant compatibility updates. Preserve the RP2040
  controller and bootloader settings, matrix pins, serial pin, and I2C
  configuration. Review upstream's `development_board` changes rather than
  accepting the stock Pro Micro configuration. Rebuild and repeat hardware
  checks before adopting the updated firmware.
- [ ] Get the OLED screens working. OLED support is configured, but the
  displays have not yet been brought up or verified. Check initialization,
  I2C communication, and rendering on both halves, then choose useful
  display content. Track this separately from baseline keymap verification.
