/* Copyright 2021 Jay Greco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "action_layer.h"

// Define layer names
enum layer_names {
  PROC,
  ILL1,
  ILL2,
};

bool numlock_set = false;


// Define Macro names
enum custom_keycodes {
    PROG = SAFE_RANGE,
};


enum td_keycodes {
    TD_ENTER_LAYER
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for KP_ENTER, twice for _FUNC layer
    [TD_ENTER_LAYER] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_KP_ENTER, 1),
};

// Keymap with layers

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Procreate Layer
  [PROC] = LAYOUT(
              LGUI(KC_RBRC),  LGUI(KC_LBRC), KC_LALT, 
  TO(ILL1),   LGUI(KC_U),     KC_X,          LGUI(KC_X), 
  LGUI(KC_K), KC_V,           KC_C,          LGUI(KC_C), 
  KC_L,       LGUI(KC_D),     KC_E,          LGUI(KC_Z), 
  LGUI(KC_0), KC_S,           KC_B,          KC_SPC 
  ),
  // Illustrator Base Layer
  [ILL1] = LAYOUT(
             LGUI(KC_K),  LGUI(KC_L),              KC_I,
  TO(PROC),  KC_M,        KC_X,                    LGUI(KC_V),
  KC_DEL,    KC_R,        LGUI(LSFT(LALT(KC_R))),  LGUI(KC_C),
  KC_LGUI,   KC_A,        KC_P,                    LGUI(KC_Z),
  KC_SPC,    KC_V,        KC_N,                    MO(ILL2)
  ),
  // Illustrator Mod layer
  [ILL2] = LAYOUT(
             LGUI(LSFT(KC_K)),  LGUI(LSFT(KC_L)),    KC_F7,
  KC_TRNS,   KC_L,              LSFT(KC_X),          LGUI(KC_V),
  KC_TRNS,   KC_S,              KC_F6,               LGUI(KC_D),
  KC_LSFT,   LGUI(KC_A),        KC_NO,               LGUI(LSFT(KC_Z)),
  KC_TRNS,   LGUI(LSFT(KC_A)),  LSFT(KC_W),          KC_NO
  ),
};


// OLED Screen

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_0; }


// Include bitmaps that show the mapped keys on each layer

#include "l1_procreate_text.c"
#include "l2_illustrator_text.c"
#include "l3_illustrator_text.c"

// Render the correct image for each layer

bool oled_task_user(void) {

    switch (get_highest_layer(layer_state)) {
        case PROC:
            render_proc_keymap_1();
            break;
        case ILL1:
            render_ai_keymap_1();
            break;
        case ILL2:
            render_ai_keymap_2();
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    return false;
}



#endif

void matrix_init_user(void) { matrix_init_remote_kb(); }

void matrix_scan_user(void) { matrix_scan_remote_kb(); }


// Define macros

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    process_record_remote_kb(keycode, record);

    // Get the current NLCK status & set if not set.
    // Only do this once, in case user has a NLCK key
    // and wants to disable it later on.
    if (!numlock_set && record->event.pressed) {
        led_t led_state = host_keyboard_led_state();
        if (!led_state.num_lock) {
            register_code(KC_NLCK);
        }
        numlock_set = true;
    }

    switch (keycode) {
        case PROG:
            if (record->event.pressed) {
                set_bitc_LED(LED_DIM);
                rgblight_disable_noeeprom();
                oled_off();
                bootloader_jump();  // jump to bootloader
            }
            break;

        default:
            break;
    }
    return true;
}


// Rotary encoder

bool encoder_update_user(uint8_t index, bool clockwise) {
if (layer_state_is(0)) {

        // On the default (procreate) layer, adjust brush size
        if (clockwise) {
                tap_code(KC_RBRC);
            } else {
                tap_code(KC_LBRC);
            }

	}

        // Otherwise, zoom in and out
	else

        {
            if (clockwise) {
                register_code(KC_LGUI);
                tap_code(KC_MINS);
                unregister_code(KC_LGUI);
            } else {
                register_code(KC_LGUI);
                tap_code(KC_SLSH);
                unregister_code(KC_LGUI);
            }
        }

 return true;
}

void led_set_kb(uint8_t usb_led) {
    if (usb_led & (1 << USB_LED_NUM_LOCK))
        set_bitc_LED(LED_DIM);
    else
        set_bitc_LED(LED_OFF);
}




// RGB LIGHTING LAYERS


void eeconfig_init_user(void) {  // EEPROM is getting reset!
  // use the non noeeprom versions, to write these values to EEPROM too
  rgblight_enable(); // Enable RGB by default      
  rgblight_sethsv(235,255,255);  // Set it to pink by default
  rgblight_mode(RGBLIGHT_MODE_STATIC_GRADIENT + 9);  // Set it to static gradient
}

layer_state_t layer_state_set_user(layer_state_t state) {
  rgblight_config_t rgblight_config;
  switch(biton32(state)) {
  case ILL1:
    // Illustrator Layer 1, orange
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(10,255,255);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    break;
  case ILL2:
    // Illustrator Layer 2, slightly less yellow
    rgblight_enable_noeeprom();	
    rgblight_sethsv_noeeprom(5,255,255);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    break;
  default:
    // Procreate, pink/purple gradient
    //Read RGB Light State
    rgblight_config.raw = eeconfig_read_rgblight();
    //If enabled, set white
    if (rgblight_config.enable) {
		rgblight_sethsv_noeeprom(235,255,255);
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_GRADIENT + 9);
	} else { //Otherwise go back to disabled
		rgblight_disable_noeeprom();
	}
    break;
}
return state;
}