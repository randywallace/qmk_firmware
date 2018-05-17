#include "dz60.h"

#define _______ KC_TRNS
#define XXXXXXX KC_NO

#define RGB_STA RGB_M_P  //rgb static
#define RGB_BRE RGB_M_B  //rgb breathe
#define RGB_RAI RGB_M_R  //rgb rainbow
#define RGB_SWI RGB_M_SW //rgb swirl
#define RGB_SNA RGB_M_SN //rgb snake
#define RGB_KNI RGB_M_K  //rgb knight
#define RGB_GRA RGB_M_G  //rgb gradient

#define TMUX_DANCE_TIMEOUT 800
#define TMUX_PREFIX "a"

enum layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum tapdance_keys {
  TD_TMUX = 0
};

enum custom_keycodes {
    KC_HIST = SAFE_RANGE, // Bash History Search ( Ctrl + r )
    KC_SPVUP,             // VoiceMeeter Spotify Vol Up (LAlt + PgUp)
    KC_SPVDN,             // VoiceMeeter Spotify Vol Down (LAlt + PgDown)
    KC_SLACK,             // AHK - Switch to slack (Ctrl + Win + Alt + s)
    KC_TERM               // AHK - Switch to terminal ( Ctrl + Esc )
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

// For my use, the tmux prefix is Ctrl + a. I use nested tmux sessions, and switching
// around them requires Ctrl + a, a, a, depending on the nested layer I am editing.  This is
// accomplished with the 'bind a send-prefix' option in tmux.  For many years, I have assumed
// a strong muscle memory for this pattern (for better or worse); with QMK, I have added
// the ability to tap-dance or macro this pattern quite effectively with the Preprocessor
// directive below.  Note that, as I work with nested tmux sessions potentially running on
// different hosts or users, that the TMUX_DELAY directive is essential to prevent sending the
// subsequent keypresses too quickly
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
    [_BASE] = LAYOUT(
        KC_GESC    , KC_1   , KC_2    , KC_3    , KC_4   , KC_5   , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS, KC_EQL     , XXXXXXX, KC_BSPC,
        KC_TAB     ,          KC_Q    , KC_W    , KC_E   , KC_R   , KC_T   , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_LBRC    , KC_RBRC, KC_BSLS,
        MO(_LOWER) , KC_A   , KC_S    , KC_D    , KC_F   , KC_G   , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,              KC_ENT          ,
        KC_LSFT    , XXXXXXX, KC_Z    , KC_X    , KC_C   , KC_V   , KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT    ,          XXXXXXX,
        KC_LCTL    ,          KC_LGUI , KC_LALT , KC_SPC ,          KC_SPC ,          KC_SPC , KC_RALT, KC_LEAD, XXXXXXX, MO(_RAISE) ,          KC_RCTL),

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

    [_LOWER] = LAYOUT(
        KC_GRV , KC_F1  , KC_F2   , KC_F3  , KC_F4      , KC_F5   , KC_F6   , KC_F7  , KC_F8  , KC_F9   , KC_F10 , KC_F11  , KC_F12  , _______, KC_DEL ,
        _______,          _______ , _______, _______    , KC_HIST , KC_TERM , _______, KC_PGUP, _______ , _______, _______ , KC_SPVUP, _______, _______,
        _______, _______, KC_SLACK, KC_PGDN, TD(TD_TMUX), _______ , KC_LEFT , KC_DOWN, KC_UP  , KC_RGHT , KC_INS , KC_SPVDN,           _______         ,
        _______, _______, _______ , _______, _______    , _______ , _______ , _______, _______, _______ , _______, _______ , _______ ,          _______,
        _______,          _______ , _______, _______    ,           _______ ,          _______, _______ , _______, _______ , _______ ,          _______),

   /* Keymap : Third Layer (RGB / Reset)
    * ,-----------------------------------------------------------.
    * |   |   |   |   |   |   |   |   |   |   |   |   |   | SLEEP |
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

    [_RAISE] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_SLEP ,
        _______,          RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______, _______, RESET   ,
        _______, RGB_STA, RGB_BRE, RGB_RAI, RGB_SWI, RGB_SNA, RGB_KNI, RGB_GRA, _______, KC_MPRV, KC_MPLY, KC_MNXT,          _______          ,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_VOLD, KC_VOLU, KC_MUTE,          _______ ,
        _______,          _______, _______, _______,          _______,          _______, _______, _______, _______, _______,          _______ )

};
