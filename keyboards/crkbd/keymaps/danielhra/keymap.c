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
// Mods in home row, using Mod Ta
#define CTL_A MT(MOD_LCTL,KC_A)
#define ALT_S MT(MOD_LALT,KC_S)
#define CMD_D MT(MOD_LGUI,KC_D)
#define SHT_F MT(MOD_LSFT,KC_F)
#define SHT_J MT(MOD_LSFT,KC_J)
#define CMD_K MT(MOD_LGUI,KC_K)
#define ALT_L MT(MOD_LALT,KC_L)
#define CTL_CLN MT(MOD_LCTL,KC_SCLN)
// macOS Shortcuts
#define COPY  LWIN(KC_C)
#define PASTE LWIN(KC_V)
#define CUT LWIN(KC_X)
#define UNDO LWIN(KC_Z)
#define REDO LSFT(LWIN(KC_Z))

#include QMK_KEYBOARD_H
#include <stdlib.h>
#include <time.h>
#include "frames.h"
#include "combos.h"

uint16_t roll(uint16_t N);

static int num_keypresses = 0;
static int current_frame = 0;

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

enum layers { BASE, MEDR, NAVR, MOUR, NSSL, NSL, FUNL, GAME };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_LALT,  KC_Q,    KC_W,    KC_E,    KC_R,     KC_T,                        KC_Y,     KC_U,    KC_I,    KC_O,   KC_P,   TG(GAME),
    //|--------+--------------+----------------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_LCTL, LCTL_T(KC_A),   LALT_T(KC_S),   LGUI_T(KC_D),      LSFT_T(KC_F),      KC_G,              KC_H,              LSFT_T(KC_J),LGUI_T(KC_K),      LALT_T(KC_L),      LCTL_T(KC_QUOT),KC_LCTL,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LSFT,KC_Z,              ALGR_T(KC_X),      KC_C,              KC_V,              KC_B,              KC_N,              KC_M,              KC_COMM,           ALGR_T(KC_DOT),    KC_SLSH,KC_LSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
    LT(MEDR, KC_ESC),  LT(NAVR, KC_SPC),  LT(MOUR, KC_TAB),  LT(NSSL, KC_ENT),  LT(NSL, KC_BSPC),  LT(FUNL, KC_DEL)
                               //`--------------------------'  `--------------------------'
  ),
  [NAVR] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_LALT,KC_RST,  KC_NA,   KC_NA,   KC_NA,   KC_NA,   LGUI(KC_X), LGUI(KC_V), LGUI(KC_C), LGUI(KC_Z), LGUI(S(KC_Z)),TG(GAME),
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LCTL,KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, KC_NA,   KC_CAPS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,KC_LCTL,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LSFT,KC_NA,   KC_ALGR, KC_NA,   KC_NA,   KC_NA,   KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,KC_LSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
    KC_NA,   KC_NA,   KC_NA,   KC_ENT,  KC_BSPC, KC_DEL
                               //`--------------------------'  `--------------------------'
  ),
  [MOUR] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_LALT,KC_RST,  KC_NA,   KC_NA,   KC_NA,   KC_NA,   KC_NU,   KC_NU,   KC_NU,   KC_NU,   KC_NU,TG(GAME),
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LCTL,KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, KC_NA,   KC_NU,   KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R,KC_LCTL,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LSFT,KC_NA,   KC_ALGR, KC_NA,   KC_NA,   KC_NA,   KC_NU,   KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R,KC_LSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
    KC_NA,   KC_NA,   KC_NA,   KC_BTN1, KC_BTN3, KC_BTN2
                               //`--------------------------'  `--------------------------'
  ),
  [MEDR] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_LALT,KC_RST,  KC_NA,   KC_NA,   KC_NA,   KC_NA,   KC_TOG,  KC_MOD,  KC_HUI,  KC_SAI,  KC_VAI,TG(GAME),
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LCTL,KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, KC_NA,   KC_NU,   KC_MPRV, KC__VOLDOWN, KC__VOLUP, KC_MNXT,KC_LCTL,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LSFT,KC_NA,   KC_ALGR, KC_NA,   KC_NA,   KC_NA,   RESET,   KC_NU,   KC_SCROLLLOCK,   KC_PAUSE,   KC_NU,KC_LSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
    KC_NA,   KC_NA,   KC_NA,   KC_MSTP, KC_MPLY, KC_MUTE
                               //`--------------------------'  `--------------------------'
  ),
  [FUNL] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_LALT,KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_PSCR, KC_NA,   KC_NA,   KC_NA,   KC_NA,   KC_RST,TG(GAME),
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LCTL,KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_SLCK, KC_NA,   KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL,KC_LCTL,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LSFT,KC_F10,  KC_F1,   KC_F2,   KC_F3,   KC_PAUS, KC_NA,   KC_NA,   KC_NA,   KC_ALGR, KC_NA,KC_LSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
    KC_APP,  KC_SPC,  KC_TAB,  KC_NA,   KC_NA,   KC_NA
                               //`--------------------------'  `--------------------------'
  ),
  [NSL] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_LALT,KC_LBRC, KC_7,    KC_8,    KC_9,    KC_RBRC, KC_NA,   KC_NA,   KC_NA,   KC_NA,   KC_RST,TG(GAME),
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LCTL,KC_SCLN, KC_4,    KC_5,    KC_6,    KC_EQL,  KC_NA,   KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL,KC_LCTL,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LSFT,KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS, KC_NA,   KC_NA,   KC_NA,   KC_ALGR, KC_NA,KC_LSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
    KC_DOT,  KC_0,    KC_MINS, KC_NA,   KC_NA,   KC_NA
                               //`--------------------------'  `--------------------------'
  ),
  [NSSL] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_LALT,KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, KC_NA,   KC_NA,   KC_NA,   KC_NA,   KC_RST,TG(GAME),
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LCTL,KC_COLN, KC_DLR,  KC_PERC, KC_CIRC, KC_PLUS, KC_NA,   KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL,KC_LCTL,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LSFT,KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_PIPE, KC_NA,   KC_NA,   KC_NA,   KC_ALGR, KC_NA,KC_LSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
    KC_LPRN, KC_RPRN, KC_UNDS, KC_NA,   KC_NA,   KC_NA
                           //`--------------------------'  `--------------------------'
  ),
  [GAME] = LAYOUT_split_3x6_3(
      //,-----------------------------------------------------.                    ,-----------------------------------------------------.
    KC_LALT, KC_Q,  KC_W,  KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,    KC_I,    KC_O,   KC_P, TG(GAME),
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LCTL, KC_A,  KC_S,  KC_D,   KC_F,   KC_G,   KC_H,   KC_J,    KC_K,    KC_L,   KC_QUOT,    KC_LCTL,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    KC_LSFT, KC_Z,  KC_X,  KC_C,   KC_V,   KC_B,   KC_N,   KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_LSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                           KC_ESC, KC_SPC, KC_TAB, KC_ENT, KC_BSPC, KC_DEL
                           //`--------------------------'  `--------------------------'
  )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
		num_keypresses = num_keypresses + 1;
        if (num_keypresses % GROW_RATE == 0) {
            current_frame = (current_frame + 1) % ANIM_FRAMES;
        }
    }
    return true;
};


#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  srand(time(0));  // seed random for ROLL
  return OLED_ROTATION_270;
}

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
