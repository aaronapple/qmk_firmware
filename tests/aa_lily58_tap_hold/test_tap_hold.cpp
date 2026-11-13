// SPDX-License-Identifier: GPL-2.0-or-later
#include "keyboard_report_util.hpp"
#include "test_common.hpp"
#include "test_fixture.hpp"
#include "test_keymap_key.hpp"

using testing::InSequence;
using testing::_;
class Lily58TapHold : public TestFixture {};

TEST_F(Lily58TapHold, FastSameHandKeyUsesThumbLayerImmediately) {
    TestDriver driver;
    InSequence sequence;
    auto thumb = KeymapKey(0, 0, 3, LT(1, KC_TAB));
    auto base_c = KeymapKey(0, 2, 2, KC_C);
    auto layer_key = KeymapKey(1, 2, 2, KC_F24);
    set_keymap({thumb, base_c, layer_key});

    EXPECT_NO_REPORT(driver);
    thumb.press();
    run_one_scan_loop();
    idle_for(10);
    VERIFY_AND_CLEAR(driver);

    // No timeout or release of the second key is needed to enter the layer.
    EXPECT_REPORT(driver, (KC_F24));
    layer_key.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_EMPTY_REPORT(driver);
    layer_key.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
    EXPECT_NO_REPORT(driver);
    thumb.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Lily58TapHold, ThumbTapStillSendsTab) {
    TestDriver driver;
    InSequence sequence;
    auto thumb = KeymapKey(0, 0, 3, LT(1, KC_TAB));
    set_keymap({thumb});
    EXPECT_REPORT(driver, (KC_TAB));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(thumb);
    VERIFY_AND_CLEAR(driver);
}

TEST_F(Lily58TapHold, SameHandHomeRowRollStillTaps) {
    TestDriver driver;
    InSequence sequence;
    auto home = KeymapKey(0, 1, 1, LCTL_T(KC_S));
    auto letter = KeymapKey(0, 2, 2, KC_C);
    set_keymap({home, letter});
    EXPECT_NO_REPORT(driver);
    home.press();
    run_one_scan_loop();
    idle_for(10);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_S));
    EXPECT_REPORT(driver, (KC_S, KC_C));
    letter.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_C));
    home.release();
    run_one_scan_loop();
    EXPECT_EMPTY_REPORT(driver);
    letter.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}
