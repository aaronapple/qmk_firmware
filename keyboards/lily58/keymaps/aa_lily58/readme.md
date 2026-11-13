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

### Upstream migration (2026-11-12)

Aaron backed up the working UF2 in Downloads, confirmed the touchpad behaves
as before, and pushed the baseline at `56169250fa` on `aa_lily58`.
The migration branch `aa_lily58-upstream-2026-11-12` merges upstream master
at `3329239097`. The sole merge conflict was in `rev1/keyboard.json`:
retain the explicit RP2040 processor and bootloader rather than upstream's
stock `development_board: promicro`. Matrix GPIOs, GP1 split serial,
right-master selection, Azoteq TPS43 configuration, and I2C enablement are
preserved. Submodules are updated to upstream's pinned revisions.

The host keymap checks pass, including all 20 paired outputs. All three QMK
tapping tests also pass with the updated submodules. Aaron ran `qmk doctor`
and compiled without warnings or errors, flashed both halves, and reported
that the updated firmware worked well. The tested merge, `59772b09a0`, was
then fast-forwarded into `aa_lily58` and the migration branch was deleted.
At this handoff, the merge has not been pushed to origin.

## Hardware and build context

This checkout targets `lily58/rev1` with RP2040 controllers, a right master,
Azoteq TPS43 touchpad on the right, and one OLED on the left. Commit
`f310a4fbd6` records the original
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

## OLED troubleshooting handoff

There is only **one OLED, on the left half**. The order confirmation lists
0.91-inch, 128x32 resolution; the controller IC and actual wiring have not
been confirmed. It has never visibly lit up, including before the upstream
update. The right touchpad works, but this does not verify the left I2C bus.
Each half drives its local peripherals; the halves communicate over serial.

### What we tried

1. Confirmed OLED support was already active: `OLED_ENABLE = yes`,
   `OLED_DRIVER = ssd1306`, and the non-master rendering path calls
   `oled_write(read_logo(), false)`. The commented-out lines are optional
   display content, not the OLED enablement. QMK defaults to 128x32 and
   address `0x3C`; this board configuration defaults to SDA GP2 / SCL GP3.
2. Added `AA_OLED_DIAGNOSTIC` in `config.h`: plain text replaces the logo,
   `OLED_TIMEOUT` is zero, and rendering calls `oled_on()`. Aaron flashed
   both halves and reconnected USB to the right with TRRS attached. The
   display remained completely blank, with no visible flash or pixels.
3. Enabled `CONSOLE_ENABLE = yes` in `rules.mk` and added a diagnostic
   `housekeeping_task_user()` in `keymap.c`. Every three seconds it writes
   an SSD1306 NOP command to addresses `0x3C` and `0x3D` with a 20 ms timeout.
   These are write transactions, avoiding read probes that OLEDs may not
   support. If the configured address responds and the display is off, it
   retries OLED initialization after power has settled.
4. Aaron flashed the console diagnostic and connected **only the left half
   directly by USB, with TRRS disconnected**, then ran `qmk console`.
   The repeated output was:

   ```text
   OLED diag: master=1, SDA=GP2 SCL=GP3, 0x3C=-1 0x3D=-1, on=0
   ```

`master=1` is expected for a half connected directly by USB; `MASTER_RIGHT`
does not prevent this diagnostic connection. Both writes returned
`I2C_STATUS_ERROR` (-1), and `on=0` means the driver reports the display off.
Neither tested address communicated successfully under the current settings.
No successful initialization retry was reported. The GP2/GP3 text is a fixed
label describing the configured defaults, not a measurement of physical
wiring. These results do **not** establish that the module is defective or
distinguish missing power, a connection problem, wrong pins/address, or a
different controller. Changing display content alone has not fixed it.

The host keymap regression checks still pass after both diagnostic changes.
They do not exercise the OLED/console code; Aaron's firmware build and flash
provided the build validation. Diagnostics have since been disabled for daily
use: `AA_OLED_DIAGNOSTIC` is commented out and `CONSOLE_ENABLE = no`.
Normal logo/keylog rendering and QMK's default OLED timeout are restored;
the periodic probes and initialization retries are excluded from the build.
The optional diagnostic code and these notes remain uncommitted. Aaron will
recompile and flash both halves to replace the installed diagnostic firmware.

### Resume here when tools are available

1. Disconnect USB and leave TRRS disconnected before opening the cover or
   touching connections. Remove the left acrylic cover and take clear photos
   of the OLED, its connections, nearby PCB markings, and any module labels.
   Look for GND/VCC/SCL/SDA labels, visible loose seating, and poor solder
   joints. Do not desolder or disconnect the module just to investigate.
2. Identify the module/controller and confirm the connection pin order and
   the PCB/controller mapping for SDA and SCL. Compare the actual wiring to
   GP2/GP3 before trying different GPIO settings. Verify any required PCB
   jumpers from the matching board documentation rather than guessing.
3. If needed, use a multimeter to check continuity with power disconnected.
   Check supply voltage against the module specification only once the pins
   are identified and probes can be placed safely. Do not guess the supply
   voltage or bridge adjacent pins.
4. Correct any confirmed configuration or connection issue and repeat the
   isolated-left console test. A zero return code means the write succeeded.
   If only `0x3D` succeeds, update `OLED_DISPLAY_ADDRESS` accordingly. If
   communication succeeds but the screen stays blank, investigate the
   controller type and initialization. If wiring and power check out but
   communication still fails, contact Holykeebs with photos and these logs.
5. Once the screen works, choose useful content for the **left** half. Disable
   `AA_OLED_DIAGNOSTIC` and set `CONSOLE_ENABLE = no` after testing, then
   rebuild and flash. While diagnostics are enabled, they keep the display
   awake; do not leave a static test screen running indefinitely.

To repeat the test, uncomment `#define AA_OLED_DIAGNOSTIC` in `config.h`,
set `CONSOLE_ENABLE = yes` in `rules.mk`, rebuild, and flash the left half.
Keep only that half connected by USB and run `qmk console`. Stop it with
Ctrl+C. To resume normal keyboard use, disconnect USB, reconnect TRRS while
both halves are unpowered, and connect USB to the right.

## Next steps

- [x] Build and flash this fork and verify the revised keymap on the assembled
  keyboard, including thumb layers and Pair Shift.
- [x] Preserve a copy of the known working UF2 and explicitly verify touchpad
  operation before upstream migration.
- [x] After baseline verification, sync with upstream QMK on a separate
  branch and make the relevant compatibility updates. Preserve the RP2040
  controller and bootloader settings, matrix pins, serial pin, and I2C
  configuration. Review upstream's `development_board` changes rather than
  accepting the stock Pro Micro configuration. Rebuild and repeat hardware
  checks before adopting the updated firmware.
- [ ] Get the single left OLED working. Resume with the physical inspection
  described in the troubleshooting handoff above, then resolve communication
  and choose useful display content.
