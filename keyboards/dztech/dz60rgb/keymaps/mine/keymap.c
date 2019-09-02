#include QMK_KEYBOARD_H

#define _______ KC_TRNS
#define XXXXXXX KC_NO

#define TMUX_DANCE_TIMEOUT 800
#define TMUX_PREFIX "a"

#define LOWER_ESC LT(_LOWER, KC_ESC)

enum layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST,
  _LAYER0,
  _LAYER1,
  _LAYER2,
  _LAYER3,
  _LAYER4
};

enum tapdance_keys {
  TD_TMUX = 0
};

enum custom_keycodes {
    KC_HIST = SAFE_RANGE, // Bash History Search ( Ctrl + r )
    KC_SPVUP,             // VoiceMeeter Spotify Vol Up (LAlt + PgUp)
    KC_SPVDN,             // VoiceMeeter Spotify Vol Down (LAlt + PgDown)
    KC_SLACK,             // AHK - Switch to slack (Ctrl + Win + Alt + s)
    KC_TERM,              // AHK - Switch to terminal ( Ctrl + Esc )
    KC_LOCK,              // Lock Screen (LGui + L)
    KC_IBANG              // WinCompose Interrobang
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(keycode) {
            case KC_HIST:
                SEND_STRING(SS_LCTRL("r"));
                return false;
            case KC_SPVUP:
                SEND_STRING( SS_DOWN(X_LALT) SS_TAP(X_PGUP) SS_UP(X_LALT) );
                return false;
            case KC_SPVDN:
                SEND_STRING( SS_DOWN(X_LALT) SS_TAP(X_PGDOWN) SS_UP(X_LALT) );
                return false;
            case KC_SLACK:
                SEND_STRING( SS_DOWN(X_LCTRL) SS_DOWN(X_LGUI) SS_DOWN(X_LALT) "s" SS_UP(X_LALT) SS_UP(X_LGUI) SS_UP(X_LCTRL) );
                return false;
            case KC_TERM:
                SEND_STRING( SS_DOWN(X_LCTRL) SS_TAP(X_ESCAPE) SS_UP(X_LCTRL) );
                return false;
            case KC_LOCK:
                SEND_STRING( SS_DOWN(X_LGUI) "l" SS_UP(X_LGUI) );
                return false;
           case KC_IBANG:
                SEND_STRING( SS_TAP(X_RALT) SS_LSFT( "1/" ) );
                return false;
        }
    }
    return true;
};

LEADER_EXTERNS();

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();
  }
}

void dance_tmux_each (qk_tap_dance_state_t *state, void *user_data) {
  if ( state->count == 1 ) {
    SEND_STRING(SS_LCTRL(TMUX_PREFIX));
  } else {
    SEND_STRING(TMUX_PREFIX);
  }
};

qk_tap_dance_action_t tap_dance_actions[] = {
 [TD_TMUX] = ACTION_TAP_DANCE_FN_ADVANCED_TIME ( dance_tmux_each, NULL, NULL, TMUX_DANCE_TIMEOUT)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
   /* Keymap: (Base Layer) Default Layer
    * ,-----------------------------------------------------------.
    * |Esc~| 1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp |
    * |-----------------------------------------------------------|
    * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  |
    * |-----------------------------------------------------------|
    * |_LOWER |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return |
    * |-----------------------------------------------------------|
    * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift     |
    * |-----------------------------------------------------------|
    * |Ctrl|Gui |Alt |      Space            |Alt |LDR|_RAISE|Ctrl|
    * `-----------------------------------------------------------'
    */
    [_BASE] = LAYOUT_ANSI(
        KC_GESC   , KC_1   , KC_2    , KC_3    , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS    , KC_EQL , KC_BSPC,
        KC_TAB    , KC_Q   , KC_W    , KC_E    , KC_R   , KC_T   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_LBRC    , KC_RBRC, KC_BSLS,
        LOWER_ESC , KC_A   , KC_S    , KC_D    , KC_F   , KC_G   , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT    ,          KC_ENT ,
        KC_LSFT   , KC_Z   , KC_X    , KC_C    , KC_V   , KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT    ,
        KC_LCTL   , KC_LGUI , KC_LALT,                    KC_SPC ,                            KC_RALT, KC_LEAD, MO(_RAISE) ,          KC_RCTL),
    /* Keymap : Second Layer (WASD and common)
     * ,-----------------------------------------------------------.
     * |  ` |F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|  DEL  |
     * |-----------------------------------------------------------|
     * |     |   |   |   | ^r|TRM|   |PUP|   |   |   |SUP|   |     |
     * |-----------------------------------------------------------|
     * |       |   |SLK|PDN|TMX|   | LT| DN| UP| RT|INS|SDN|       |
     * |-----------------------------------------------------------|
     * |        |   |   |   |   |   |   |   |   |   |   |          |
     * |-----------------------------------------------------------|
     * |    |    |    |                       |    |    |    |     |
     * `-----------------------------------------------------------'
     */
    [_LOWER] = LAYOUT_ANSI(
        KC_GRV , KC_F1   , KC_F2   , KC_F3   , KC_F4      , KC_F5   , KC_F6   , KC_F7  , KC_F8  , KC_F9   , KC_F10 , KC_F11   , KC_F12  , KC_DEL ,
        _______, _______ , _______ , _______ , KC_HIST    , KC_TERM , _______ , KC_PGUP, _______ , _______, _______, KC_SPVUP , _______ , _______,
        _______, _______ , KC_SLACK, KC_PGDN , TD(TD_TMUX), _______ , KC_LEFT , KC_DOWN, KC_UP  , KC_RGHT , KC_INS , KC_SPVDN ,           _______,
        _______, _______ , _______ , _______ ,    _______ , _______ , _______, _______, _______ , _______, KC_IBANG, _______  ,
        _______, _______ , _______ ,                        _______ ,                             _______, _______ , _______  ,           _______),
   /* Keymap : Third Layer (RGB / Reset)
    * ,-----------------------------------------------------------.
    * |   |   |   |   |   |   |   |   |   |   |   |   |   | LOCK  |
    * |-----------------------------------------------------------|
    * |     |TOG|MOD|HUI|HUD|SAI|SAD|VAI|VAD|   |   |   |   |RESET|
    * |-----------------------------------------------------------|
    * |      |STA|BRE|RAI|SWI|SNA|KNI|GRA|   |PRV|PLY|NXT|        |
    * |-----------------------------------------------------------|
    * |        |   |   |   |   |   |   |   |VDN|VUP|MUT|          |
    * |-----------------------------------------------------------|
    * |    |    |    |                        |    |    |    |    |
    * `-----------------------------------------------------------'
    */
    [_RAISE] = LAYOUT_ANSI(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_LOCK ,
        _______, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______, _______, RESET   ,
        _______, RGB_SPI, RGB_SPD, _______, _______, _______, _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT,          _______ ,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_VOLD, KC_VOLU, KC_MUTE,
        _______, _______, _______,                   _______,                            _______, _______, _______,           _______ ),

    [_LAYER0] = LAYOUT_ANSI( /* Base */
      KC_GESC,         KC_1,     KC_2,     KC_3,  KC_4,  KC_5,  KC_6,    KC_7,  KC_8,    KC_9,     KC_0,      KC_MINS,  KC_EQL,  KC_BSPC,\
      KC_TAB,          KC_Q,     KC_W,     KC_E,  KC_R,  KC_T,  KC_Y,    KC_U,  KC_I,    KC_O,     KC_P,      KC_LBRC,  KC_RBRC, KC_BSLASH,\
      CTL_T(KC_CAPS),  KC_A,     KC_S,     KC_D,  KC_F,  KC_G,  KC_H,    KC_J,  KC_K,    KC_L,     KC_SCLN,   KC_QUOT,           KC_ENT, \
      KC_LSFT,         KC_Z,     KC_X,     KC_C,  KC_V,  KC_B,  KC_N,    KC_M,  KC_COMM, KC_DOT,   KC_SLSH,   KC_RSFT, \
      KC_LCTL,         KC_LGUI,  KC_LALT,                KC_SPC,                         KC_RALT,  MO(1),     MO(2),             KC_RCTL),
    [_LAYER1] = LAYOUT_ANSI( /* FN */
        KC_GESC,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL ,\
          KC_TRNS,  KC_TRNS,  KC_UP,    KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_CALC,  KC_TRNS,  KC_INS,   KC_TRNS,  KC_PSCR,  KC_SLCK,  KC_PAUS,  RESET  ,\
          KC_TRNS,  KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_HOME,  KC_PGUP,            KC_TRNS,\
        KC_MPRV,  KC_VOLD,  KC_VOLU,  KC_MUTE,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_END,   KC_PGDOWN,KC_MNXT, \
        KC_TRNS,  KC_TRNS,  KC_TRNS,                      TO(3),                                  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS),
    [_LAYER2] = LAYOUT_ANSI( /* FN2 */
        KC_TRNS,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL ,\
        KC_TRNS,  RGB_TOG,  KC_TRNS,  RGB_HUI,  RGB_HUD,  RGB_SAI,  RGB_SAD,  RGB_VAI,  RGB_VAD,  RGB_MOD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  RESET  ,\
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  RGB_SPI,  RGB_SPD,  KC_TRNS,  KC_TRNS,            KC_TRNS,\
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS, \
        KC_TRNS,  KC_TRNS,  KC_TRNS,                      KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS),
    [_LAYER3] = LAYOUT_ANSI( /* FN3 */
        KC_GESC,       KC_1,     KC_2,     KC_3,  KC_4,  KC_5,  KC_6,    KC_7,    KC_8,    KC_9,   KC_0,    KC_MINS,  KC_EQL,  KC_BSPC,\
          KC_TAB,        KC_Q,     KC_W,     KC_E,  KC_R,  KC_T,  KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,    KC_LBRC,  KC_RBRC, KC_BSLASH,\
        CTL_T(KC_CAPS),KC_A,     KC_S,     KC_D,  KC_F,  KC_G,  KC_H,    KC_J,    KC_K,    KC_L,   KC_SCLN, KC_QUOT,           KC_ENT, \
          KC_LSFT,       KC_Z,     KC_X,     KC_C,  KC_V,  KC_B,  KC_N,    KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,  \
        KC_LCTL,       KC_LALT,  KC_LGUI,                KC_SPC,                           KC_TRNS,MO(4),   KC_RALT,           KC_RCTL),
    [_LAYER4] = LAYOUT_ANSI( /* FN4 */
        KC_GESC,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL ,\
          KC_TRNS,  RGB_TOG,  KC_TRNS,  RGB_HUI,  RGB_HUD,  RGB_SAI,  RGB_SAD,  RGB_VAI,  RGB_VAD,  RGB_MOD,  KC_PSCR,  KC_SLCK,  KC_PAUS,  RESET  ,\
          KC_TRNS,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RIGHT, KC_TRNS,  KC_TRNS,  KC_TRNS,  RGB_SPI,  RGB_SPD,  KC_HOME,  KC_PGUP,            KC_TRNS,\
        KC_MPRV,  KC_VOLD,  KC_VOLU,  KC_MUTE,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_END,   KC_PGDOWN,KC_MNXT, \
        KC_TRNS,  KC_TRNS,  KC_TRNS,                      TO(0),                                  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS),
    };

void rgb_matrix_layer_helper (uint8_t red, uint8_t green, uint8_t blue, bool default_layer) {
  for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
    if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
        rgb_matrix_set_color( i, red, green, blue );
    }
  }
}

void rgb_matrix_indicators_user(void) {
    uint8_t this_led = host_keyboard_leds();
    if (!g_suspend_state) {
      switch (biton32(layer_state)) {
        case _RAISE:
        rgb_matrix_layer_helper(0xFF, 0x00, 0x00, false); break;
        case _LOWER:
          rgb_matrix_layer_helper(0x00, 0xFF, 0x00, false); break;
        /*case _LAYER3:
          rgb_matrix_layer_helper(0x00, 0x00, 0xFF, false); break;
        case _LAYER4:
          rgb_matrix_layer_helper(0xFF, 0xFF, 0x00, false); break;*/
      }
    }
    if ( this_led & (1<<USB_LED_CAPS_LOCK)) {
          rgb_matrix_set_color(40, 0xFF, 0xFF, 0xFF);
    }

}

