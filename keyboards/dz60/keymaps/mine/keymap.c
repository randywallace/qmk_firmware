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

enum layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum tmux_tap_dance {
  TD_TMUX = 0,
  TMUX_L1,
  TMUX_L2,
  TMUX_L3,
  TMUX_L4,
  TMUX_L5
};

LEADER_EXTERNS();

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();

    SEQ_ONE_KEY (KC_1) { SEND_STRING(SS_LCTRL("a"));       }
    SEQ_ONE_KEY (KC_2) { SEND_STRING(SS_LCTRL("a")"a");    }
    SEQ_ONE_KEY (KC_3) { SEND_STRING(SS_LCTRL("a")"aa");   }
    SEQ_ONE_KEY (KC_4) { SEND_STRING(SS_LCTRL("a")"aaa");  }
    SEQ_ONE_KEY (KC_5) { SEND_STRING(SS_LCTRL("a")"aaaa"); }
  }
}

void dance_tmux (qk_tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    case TMUX_L1: SEND_STRING(SS_LCTRL("a"))      ; reset_tap_dance(state); break;
    case TMUX_L2: SEND_STRING(SS_LCTRL("a")"a")   ; reset_tap_dance(state); break;
    case TMUX_L3: SEND_STRING(SS_LCTRL("a")"aa")  ; reset_tap_dance(state); break;
    case TMUX_L4: SEND_STRING(SS_LCTRL("a")"aaa") ; reset_tap_dance(state); break;
    case TMUX_L5: SEND_STRING(SS_LCTRL("a")"aaaa"); reset_tap_dance(state); break;
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
 [TD_TMUX] = ACTION_TAP_DANCE_FN (dance_tmux)
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
        KC_LSPO    , XXXXXXX, KC_Z    , KC_X    , KC_C   , KC_V   , KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSPC    ,          XXXXXXX,
        KC_LCTL    ,          KC_LGUI , KC_LALT , KC_SPC ,          KC_SPC ,          KC_SPC , KC_RALT, KC_LEAD, XXXXXXX, MO(_RAISE) ,          KC_RCTL),

    /* Keymap : Second Layer (WASD and Media)
   * ,-----------------------------------------------------------.
   * |Esc~|F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|  DEL  |
   * |-----------------------------------------------------------|
   * |     |   | T1| T2| T3| T4|   |PUP|   |   |   |   |   |     |
   * |-----------------------------------------------------------|
   * |       |   |   |PDN|   |   | LT| DN| UP| RT|INS|   |       |
   * |-----------------------------------------------------------|
   * |        |   |   |   |   |   |   |   |   |   |   |          |
   * |-----------------------------------------------------------|
   * |    |    |    |                       |    |    |    |     |
   * `-----------------------------------------------------------'
   */

    [_LOWER] = LAYOUT(
        KC_GRV , KC_F1  , KC_F2  , KC_F3  , KC_F4      , KC_F5   , KC_F6   , KC_F7  , KC_F8  , KC_F9   , KC_F10 , KC_F11 , KC_F12 , _______, KC_DEL ,
        _______,          _______, _______, _______    , _______, _______ , _______ , KC_PGUP, _______ , _______, _______, _______, _______, _______,
        _______, _______, _______, KC_PGDN, TD(TD_TMUX), _______ , KC_LEFT , KC_DOWN, KC_UP  , KC_RGHT , KC_INS , _______,          _______         ,
        _______, _______, _______, _______, _______    , _______ , _______ , _______, _______, _______ , _______, _______, _______,          _______,
        _______,          _______, _______, _______    ,           _______ ,          _______, _______ , _______, _______, _______,          _______),

   /* Keymap : Third Layer (RGB / Reset)
   * ,-----------------------------------------------------------.
   * | ` |   |   |   |   |   |   |   |   |   |   |   |   |       |
   * |-----------------------------------------------------------|
   * |     |   |   |   |   |   |   |   |   |   |   |   |   |     |
   * |-----------------------------------------------------------|
   * |      |   |   |   |   |   |   |       |   |   |   |        |
   * |-----------------------------------------------------------|
   * |        |   |   |   |   |   |   |   |   |   |   |          |
   * |-----------------------------------------------------------|
   * |    |    |    |                        |    |    |    |    |
   * `-----------------------------------------------------------'
   */

    [_RAISE] = LAYOUT(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_SLEP ,
      _______,          RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______, _______, RESET   ,
      _______, RGB_STA, RGB_BRE, RGB_RAI, RGB_SWI, RGB_SNA, RGB_KNI, RGB_GRA, _______, KC_MPRV, KC_MPLY, KC_MNXT,          _______          ,
      _______, _______, BL_DEC , BL_TOGG, BL_INC , BL_STEP, _______, _______, _______, _______, KC_VOLD, KC_VOLU, KC_MUTE,          _______ ,
      _______,          _______, _______, _______,          _______,          _______, _______, _______, _______, _______,          _______ )

};

