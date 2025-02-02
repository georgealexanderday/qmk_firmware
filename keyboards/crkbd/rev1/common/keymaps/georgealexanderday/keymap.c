/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
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
#include <stdio.h>

char wpm_str[10];

enum layers {
    _BASE,
    _LOWER,
    _RAISE,
    _MOUSE,
};

enum custom_keycodes {
  PIPE = SAFE_RANGE,
  LSA,
  RSA,
  RDA,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_NO,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                         KC_J,    KC_L,   KC_U, KC_Y,   KC_SCLN,  KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_ESC, LSFT_T(KC_A), KC_R,    KC_S,    KC_T,    KC_G,                      KC_M,    KC_N,    KC_E,    KC_I, LSFT_T(KC_O) , KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LGUI, LCTL_T(KC_Z),  KC_X,    KC_C,    KC_D,    KC_V,                     KC_K,    KC_H, KC_COMM,  KC_DOT, LCTL_T(KC_SLSH),  KC_LALT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                            KC_HYPR, LT(3, KC_TAB),  LT(1, KC_SPC),   LT(2, KC_BSPC), KC_ENTER, KC_MEH
                                      //`--------------------------'  `--------------------------'

  ),

  [_LOWER] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_NO,  KC_NO,   KC_PERC, KC_LCBR, KC_RCBR,  KC_NO,                         KC_NO, KC_AMPR, KC_PIPE, KC_HASH,  KC_GRV, KC_DELETE,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO, KC_TILD, KC_BSLS, KC_LPRN, KC_RPRN, KC_CIRC,                      KC_DLR, KC_MINS,   KC_COLN, KC_UNDS, KC_EQL, KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO, KC_LCTRL, LSA, KC_LBRC, KC_RBRC, KC_NO,                              PIPE, KC_EXLM, KC_AT, RSA, RDA, KC_NO,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
            KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
                                      //`--------------------------'  `--------------------------'
  ),

  [_RAISE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_NO, KC_SLSH,   KC_7, KC_8,  KC_9, KC_ASTR,                              KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO, LSFT_T(KC_0), KC_4, KC_5, KC_6, KC_PLUS,                                 KC_NO,  KC_LEFT, KC_DOWN, KC_UP, KC_RGHT,  KC_NO ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO, KC_EQL, KC_1, KC_2, KC_3, KC_MINS,                                     RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, RGB_M_P,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
                                      //`--------------------------'  `--------------------------'
  ),
  [_MOUSE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_NO, KC_NO,   KC_NO, KC_MS_U,  KC_NO, KC_NO,                              KC_NO, KC_PASTE, KC_COPY, KC_CUT, KC_NO, KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO, KC_NO,   KC_MS_L, KC_MS_D,  KC_MS_R, KC_NO,                              KC_NO, KC_BTN1, KC_BTN2, KC_BTN3 , KC_NO, KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO, KC_NO,   KC_NO, KC_NO,  KC_NO, KC_NO,                              KC_NO, KC_WH_L, KC_WH_D, KC_WH_U , KC_WH_R, KC_NO,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
        KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,                 KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
                                      //`--------------------------'  `--------------------------'
  ),

};


#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

#define L_BASE 0
#define L_LOWER 2
#define L_RAISE 4
#define L_MOUSE 8

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR("Default"), false);
            break;
        case L_LOWER:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case L_RAISE:
            oled_write_ln_P(PSTR("Raise"), false);
            break;
        case L_MOUSE:
            oled_write_ln_P(PSTR("Mouse"), false);

    }
}


char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

static void render_status(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_ln_P(PSTR("UwU"), false);
            oled_write_ln_P(PSTR(""), false);
            oled_write_P(PSTR("Colemak Mod DH"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower         "), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise         "), false);
            break;
        case _MOUSE:
            oled_write_P(PSTR("Mouse         "), false);
            break;
        default:
            oled_write_ln_P(PSTR("FUCKED        "), false);
    }
}


void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status();
    } else {
        oled_render_logo();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_keylog(keycode, record);
  }
  switch (keycode) {
    case PIPE:
    if (record->event.pressed) {
            SEND_STRING("|> ");
        }
        break;
    case RSA:
    if (record->event.pressed) {
            SEND_STRING("-> ");
        }
        break;
    case LSA:
    if (record->event.pressed) {
            SEND_STRING("<- ");
        }
        break;
    case RDA:
    if (record->event.pressed) {
            SEND_STRING("=> ");
        }
        break;
  }
  return true;
};
#endif  // OLED_DRIVER_ENABLE
