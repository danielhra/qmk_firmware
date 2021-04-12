/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it unis der the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdlib.h>
#include <time.h>
#include "frames.h"

uint16_t roll(uint16_t N);

static int num_keypresses = 0;
static int current_frame = 0;
uint8_t die_idx = 0;
uint16_t roll_result = 1;


// Layers
enum {
	_BASE,
	_SYM,
	_NUM,
	_GREEK,
	_MATH,
	_GAME,
	_WPN,
	_DICE
};

enum custom_keycodes {
	NEXT_DIE = SAFE_RANGE,
	ROLL,
};

const uint16_t dice[7] = {2, 4, 6, 8, 10, 12, 20};

#define KC_NP KC_NO // key is not present
#define KC_NA KC_NO // present but not available for use
#define KC_NU KC_NO // available but not used

// non-KC_ keycodes
#define KC_RST RESET
#define KC_TOG RGB_TOG
#define KC_MOD RGB_MOD
#define KC_HUI RGB_HUI
#define KC_SAI RGB_SAI
#define KC_VAI RGB_VAI

enum layers { BASE, MEDR, NAVR, MOUR, NSSL, NSL, FUNL, GAME };


// copied from users/manna-harbour_miryoku/manna-harbour_miryoku.c
// generated from users/manna-harbour_miryoku/miryoku.org  -*- buffer-read-only: t -*-

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT_split_3x6_3(
    KC_LALT,KC_Q,              KC_W,              KC_E,              KC_R,              KC_T,              KC_Y,              KC_U,              KC_I,              KC_O,              KC_P,TG(GAME),
    KC_LCTL,LCTL_T(KC_A),      LALT_T(KC_S),      LGUI_T(KC_D),      LSFT_T(KC_F),      KC_G,              KC_H,              LSFT_T(KC_J),      LGUI_T(KC_K),      LALT_T(KC_L),      LCTL_T(KC_QUOT),KC_LCTL,
    KC_LSFT,KC_Z,              ALGR_T(KC_X),      KC_C,              KC_V,              KC_B,              KC_N,              KC_M,              KC_COMM,           ALGR_T(KC_DOT),    KC_SLSH,KC_LSFT,
    LT(MEDR, KC_ESC),  LT(NAVR, KC_SPC),  LT(MOUR, KC_TAB),  LT(NSSL, KC_ENT),  LT(NSL, KC_BSPC),  LT(FUNL, KC_DEL)
  ),
  [NAVR] = LAYOUT_split_3x6_3(
    KC_LALT,KC_RST,  KC_NA,   KC_NA,   KC_NA,   KC_NA,   LGUI(KC_X), LGUI(KC_V), LGUI(KC_C), LGUI(KC_Z), LGUI(S(KC_Z)),TG(GAME),
    KC_LCTL,KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, KC_NA,   KC_CAPS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,KC_LCTL,
    KC_LSFT,KC_NA,   KC_ALGR, KC_NA,   KC_NA,   KC_NA,   KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,KC_LSFT,
    KC_NA,   KC_NA,   KC_NA,   KC_ENT,  KC_BSPC, KC_DEL
  ),
  [MOUR] = LAYOUT_split_3x6_3(
    KC_LALT,KC_RST,  KC_NA,   KC_NA,   KC_NA,   KC_NA,   KC_NU,   KC_NU,   KC_NU,   KC_NU,   KC_NU,TG(GAME),
    KC_LCTL,KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, KC_NA,   KC_NU,   KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R,KC_LCTL,
    KC_LSFT,KC_NA,   KC_ALGR, KC_NA,   KC_NA,   KC_NA,   KC_NU,   KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R,KC_LSFT,
    KC_NA,   KC_NA,   KC_NA,   KC_BTN1, KC_BTN3, KC_BTN2
  ),
  [MEDR] = LAYOUT_split_3x6_3(
    KC_LALT,KC_RST,  KC_NA,   KC_NA,   KC_NA,   KC_NA,   KC_TOG,  KC_MOD,  KC_HUI,  KC_SAI,  KC_VAI,TG(GAME),
    KC_LCTL,KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, KC_NA,   KC_NU,   KC_MPRV, KC__VOLDOWN, KC__VOLUP, KC_MNXT,KC_LCTL,
    KC_LSFT,KC_NA,   KC_ALGR, KC_NA,   KC_NA,   KC_NA,   RESET,   KC_NU,   KC_SCROLLLOCK,   KC_PAUSE,   KC_NU,KC_LSFT,
    KC_NA,   KC_NA,   KC_NA,   KC_MSTP, KC_MPLY, KC_MUTE
  ),
  [FUNL] = LAYOUT_split_3x6_3(
    KC_LALT,KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_PSCR, KC_NA,   KC_NA,   KC_NA,   KC_NA,   KC_RST,TG(GAME),
    KC_LCTL,KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_SLCK, KC_NA,   KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL,KC_LCTL,
    KC_LSFT,KC_F10,  KC_F1,   KC_F2,   KC_F3,   KC_PAUS, KC_NA,   KC_NA,   KC_NA,   KC_ALGR, KC_NA,KC_LSFT,
    KC_APP,  KC_SPC,  KC_TAB,  KC_NA,   KC_NA,   KC_NA
  ),
  [NSL] = LAYOUT_split_3x6_3(
    KC_LALT,KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC, KC_NA,   KC_NA,   KC_NA,   KC_NA,   KC_RST,TG(GAME),
    KC_LCTL,KC_SCLN, KC_4,    KC_5,    KC_6,    KC_EQL,  KC_NA,   KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL,KC_LCTL,
    KC_LSFT,KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS, KC_NA,   KC_NA,   KC_NA,   KC_ALGR, KC_NA,KC_LSFT,
    KC_DOT,  KC_0,    KC_MINS, KC_NA,   KC_NA,   KC_NA
  ),
  [NSSL] = LAYOUT_split_3x6_3(
    KC_LALT,KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, KC_NA,   KC_NA,   KC_NA,   KC_NA,   KC_RST,TG(GAME),
    KC_LCTL,KC_COLN, KC_DLR,  KC_PERC, KC_CIRC, KC_PLUS, KC_NA,   KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL,KC_LCTL,
    KC_LSFT,KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_PIPE, KC_NA,   KC_NA,   KC_NA,   KC_ALGR, KC_NA,KC_LSFT,
    KC_LPRN, KC_RPRN, KC_UNDS, KC_NA,   KC_NA,   KC_NA
  ),
  [GAME] = LAYOUT_split_3x6_3(
    KC_LALT, KC_Q,  KC_W,  KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,    KC_I,    KC_O,   KC_P, TG(GAME),
    KC_LCTL, KC_A,  KC_S,  KC_D,   KC_F,   KC_G,   KC_H,   KC_J,    KC_K,    KC_L,   KC_QUOT,    KC_LCTL,
    KC_LSFT, KC_Z,  KC_X,  KC_C,   KC_V,   KC_B,   KC_N,   KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_LSFT,
                           KC_ESC, KC_SPC, KC_TAB, KC_ENT, KC_BSPC, KC_DEL
  )
};


//const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//  /*
//  // BASE, workman layout
//  [0] = LAYOUT_split_3x6_3(
//  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//      KC_ESC,   KC_Q,    KC_D,   KC_R,    KC_W,     KC_B,                         KC_J,    KC_F,   KC_U,    KC_P,   KC_QUOT, KC_BSPC,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_TAB,   KC_A,    KC_S,   KC_H,    KC_T,     KC_G,                         KC_Y,    KC_N,   KC_E,    KC_O,   KC_I,    KC_SCLN,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_LSPO,  KC_Z,    KC_X,   KC_M,    KC_C,     KC_V,                         KC_K,    KC_L,   KC_COMM, KC_DOT, KC_SLSH, KC_RSPC,
//  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                          	          KC_LCTL, MO(1), KC_SPC,    KC_ENT, MO(2), KC_RALT
//                                      //`--------------------------'  `--------------------------'
//    ),
//  */
//  // BASE
//  [_BASE] = LAYOUT_split_3x6_3(
//  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//      KC_ESC,   KC_Q,    KC_W,   KC_E,    KC_R,     KC_T,                         KC_Y,    KC_U,   KC_I,    KC_O,   KC_P,    KC_BSPC,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_TAB,   KC_A,    KC_S,   KC_D,    KC_F,     KC_G,                         KC_H,    KC_J,   KC_K,    KC_L,   KC_SCLN, KC_QUOT,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_LSFT,  KC_Z,    KC_X,   KC_C,    KC_V,     KC_B,                         KC_N,    KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
//  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                          	          KC_LCTL, MO(_SYM), KC_SPC,       KC_ENT, MO(_NUM), KC_LALT
//                                      //`--------------------------'  `--------------------------'
//    ),
//
//  // SYM
//  [_SYM] = LAYOUT_split_3x6_3(
//  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//      KC_GRV, KC_EXLM,  KC_AT,  KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC,  KC_AMPR, KC_ASTR, KC_MINS, KC_EQL, KC_BSLS,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_TAB,  KC_NO,   KC_NO,  KC_NO,    KC_NO,  KC_NO,                        KC_LEFT,  KC_DOWN, KC_UP,   KC_RGHT, KC_LPRN, KC_RPRN,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_LSFT, KC_NO,   KC_NO,  KC_NO,    KC_NO,  KC_NO,                        KC_HOME,  KC_PGDN, KC_PGUP, KC_END,  KC_LBRC, KC_RBRC,
//  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                                          KC_LGUI, KC_TRNS, KC_SPC,     KC_ENT,  UC_MOD,   KC_TRNS
//                                      //`--------------------------'  `--------------------------'
//  ),
//
//  // NUM
//  [_NUM] = LAYOUT_split_3x6_3(
//  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//      KC_PSLS, KC_PAST, KC_PEQL, KC_7,     KC_8,   KC_9,                         KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_PMNS, KC_PPLS, KC_PDOT, KC_4,     KC_5,   KC_6,                         KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_CIRC, KC_PERC, KC_0,    KC_1,     KC_2,   KC_3,                         UC_MOD,   KC_NO,   KC_NO,  MO(_GREEK),MO(_MATH),TG(_GAME),
//  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                                          KC_TRNS, UC_MOD, KC_SPC,         KC_ENT, KC_TRNS, KC_TRNS
//                                      //`--------------------------'  `--------------------------'
//  ),
//
//  // GREEK
//  [_GREEK] = LAYOUT_split_3x6_3(
//  //,----------------------------------------------------------------------------------------------------------.                    ,---------------------------------------------------------------------------------------------------.
//     KC_NO,    KC_NO,            KC_NO,            X(EPS_LO),           X(RHO_LO),             X(TAU_LO),                             X(UPS_LO), XP(THETA_LO,THETA_UP),  X(IOTA_LO),          KC_O,             XP(PI_LO,PI_UP), KC_NO,
//  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
//     KC_NO,   X(ALPHA_LO), XP(SIG_LO,SIG_UP), XP(DELTA_LO,DELTA_UP), XP(PHI_LO,PHI_UP), XP(GAMMA_LO,GAMMA_UP),                       X(ETA_LO),  XP(XI_UP,XI_LO),        X(KAPPA_LO), XP(LAMBDA_LO,LAMBDA_UP),  KC_NO,          KC_RSFT,
//  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
//     KC_LSFT, X(ZETA_LO),    X(CHI_LO),       XP(PSI_LO,PSI_UP),     XP(OMEG_LO,OMEG_UP),    X(BETA_LO),                               X(NU_LO),   X(MU_LO),               KC_NO,             KC_TRNS,            KC_TRNS,      KC_TRNS,
//  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
//												 KC_TAB, KC_NO, KC_SPC,      KC_ENT, KC_TRNS,  KC_NO
//											      //`--------------------------'  `--------------------------'
//  ),
//
//  // MATH
//  [_MATH] = LAYOUT_split_3x6_3(
//  //,----------------------------------------------------------------------------------------------------------.                    ,---------------------------------------------------------------------------------------------------.
//     KC_NO,    X(RAT),            KC_NO,            XP(IN,NIN),           X(REAL),             KC_NO,                                 KC_NO,      KC_NO,                   KC_NO,          X(NUL),                   KC_NO,      KC_NO,
//  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
//     KC_NO,   KC_NO,        XP(SIG_UP,INTEG),   XP(DEL,NABLA),        XP(PHI_LO,PHI_UP), XP(GAMMA_LO,GAMMA_UP),                      KC_NO,       KC_NO,                 KC_NO,           KC_NO,                KC_NO,          KC_RSFT,
//  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
//     KC_LSFT, X(INTS),        KC_NO,                  X(COMPLEX),       KC_NO,                 KC_NO,                                   X(NATURAL),   KC_NO,               X(LE),             X(GE),            KC_NO,         KC_NO,
//  //|--------+------------+------------------+----------------------+------------------+-----------------------|                    |-----------+-----------------------+-----------+-------------------------+----------------+--------|
//												 KC_TAB, KC_NO, KC_SPC,      KC_ENT, KC_TRNS,  KC_NO
//											      //`--------------------------'  `--------------------------'
//  ),
//  // GAME
//  [_GAME] = LAYOUT_split_3x6_3(
//  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//      KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_BSPC,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_TAB, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_NO,  KC_PGDN, KC_PGUP,  KC_NO,   KC_NO, KC_TRNS,
//  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                                          KC_LSFT, MO(_WPN), KC_SPC,     KC_ENT, KC_TRNS, TG(_DICE)
//                                      //`--------------------------'  `--------------------------'
//  ),
//
//  // WPN
//  [_WPN] = LAYOUT_split_3x6_3(
//  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//      KC_ESC,  KC_1,     KC_2,    KC_3,   KC_4,    KC_5,                         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_TRNS, KC_6,     KC_7,    KC_8,   KC_9,    KC_0,                         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_TRNS, KC_LCTL,  KC_Z,    KC_X,   KC_C,    KC_V,                         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
//  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                                          KC_TRNS, KC_TRNS, KC_SPC,    KC_ENT, KC_NO, KC_NO
//                                      //`--------------------------'  `--------------------------'
//  ),
//
//  [_DICE] = LAYOUT_split_3x6_3(
//  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
//      KC_ESC,  KC_1,     KC_2,    KC_3,   KC_4,    KC_5,                         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_TRNS, KC_6,     KC_7,    KC_8,   KC_9,    KC_0,                         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
//  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
//      KC_TRNS, KC_LCTL,  KC_Z,    KC_X,   KC_C,    KC_V,                         KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
//  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
//                                          KC_TRNS, KC_TRNS, NEXT_DIE,    ROLL, KC_TRNS, KC_TRNS
//                                      //`--------------------------'  `--------------------------'
//  )
//};
//


// Shift + Backspace for Delete
// Initialize variable holding the binary representation of active modifiers.
uint8_t mod_state;
char roll_str[5] = {};
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
		num_keypresses = num_keypresses + 1;
        if (num_keypresses % GROW_RATE == 0) {
            current_frame = (current_frame + 1) % ANIM_FRAMES;
        }
    }
    // Store the current modifier state in the variable for later reference
    mod_state = get_mods();
    switch (keycode) {

    case KC_BSPC:
        {
        // Initialize a boolean variable that keeps track
        // of the delete key status: registered or not?
        static bool delkey_registered;
        if (record->event.pressed) {
            // Detect the activation of either shift keys
            if (mod_state & MOD_MASK_SHIFT) {
                // First temporarily canceling both shifts so that
                // shift isn't applied to the KC_DEL keycode
                del_mods(MOD_MASK_SHIFT);
                register_code(KC_DEL);
                // Update the boolean variable to reflect the status of KC_DEL
                delkey_registered = true;
                // Reapplying modifier state so that the held shift key(s)
                // still work even after having tapped the Backspace/Delete key.
                set_mods(mod_state);
                return false;
            }
        } else { // on release of KC_BSPC
            // In case KC_DEL is still being sent even after the release of KC_BSPC
            if (delkey_registered) {
                unregister_code(KC_DEL);
                delkey_registered = false;
                return false;
            }
        }
        // Let QMK process the KC_BSPC keycode as usual outside of shift
        return true;
	}
	case TG(_DICE):
		{
		if (record->event.pressed) {
		} else {
			oled_clear();
		}
		}
    case NEXT_DIE:
	{
	if (record->event.pressed) {
		if (die_idx < (DICE_FRAMES - 1)) {
			die_idx = die_idx + 1;
		} else {
			die_idx = 0;
		}
	}
	break;
	}
    case ROLL:
		{
		if (record->event.pressed) {
			roll_result = roll(dice[die_idx]);
			snprintf(roll_str, sizeof(roll_str), "%d ", roll_result);
		}
		}
    }
    return true;
};

uint16_t roll(uint16_t n) {
  if ((n - 1) >= RAND_MAX) {
    return rand();
  } else {
    // Chop off all of the values that would cause skew...
    int end = RAND_MAX / n; // truncate skew
    //assert (end > 0);
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end);

    return (r % n) + 1;
  }
}


#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  srand(time(0));  // seed random for ROLL
  return OLED_ROTATION_270;
}


char die_str[4] = {};
void oled_render_layer_state(void) {
	oled_set_cursor(0, 6);
    switch (get_highest_layer(layer_state)) {
        case BASE:
            oled_write_raw_P(apple_logo, LOGO_SIZE);
            break;
        case NAVR:
            oled_write_raw_P(coffee_logo, LOGO_SIZE);
            break;
        case NSSL:
            oled_write_raw_P(sym_logo, LOGO_SIZE);
            break;
        case MEDR:
            oled_write_raw_P(apple_logo, LOGO_SIZE);
            break;
        case FUNL:
            oled_write_raw_P(num_logo, LOGO_SIZE);
            break;
        case MOUR:
            oled_write_raw_P(greek_logo, LOGO_SIZE);
            break;
        case NSL:
            oled_write_raw_P(math_logo, LOGO_SIZE);
            break;
        case GAME:
	        oled_scroll_off();
            oled_write_raw_P(game_logo, LOGO_SIZE);
            break;
//        case FUNL:
//            oled_write_raw_P(game_logo, LOGO_SIZE);
//	        oled_scroll_right();
//			break;
//        case NSL:
//			oled_set_cursor(2, 4);
//			oled_write(roll_str, false);
//			oled_set_cursor(0, 6);
//            oled_write_raw_P(dice_frames[die_idx], LOGO_SIZE);
//			snprintf(die_str, sizeof(die_str), "d%d ", dice[die_idx]);
//			oled_set_cursor(2, 11);
//			oled_write(die_str, false);
//            break;
    }
}
void oled_render_animation_phase(void) {
	oled_write_raw_P(plant_anim[current_frame], ANIM_SIZE);
}


void oled_task_user(void) {
    if (is_master) {
        oled_render_layer_state();
    } else {
        oled_render_animation_phase();
    }
}

#endif // OLED_DRIVER_ENABLE
