// Remember to define COMBO_COUNT in config.h

enum combos {
  COMBO_CAPSLOCK,
  COMBO_PARENS_LEFT,
  COMBO_PARENS_RIGHT,
  COMBO_CURLY_LEFT,
  COMBO_CURLY_RIGHT,
  COMBO_BRACKET_LEFT,
  COMBO_BRACKET_RIGHT,
  COMBO_COPY,
  COMBO_PASTE,
  COMBO_UNDO,
  COMBO_REDO,
};
const uint16_t PROGMEM combo_capslock[] = {CTL_A, ALT_S, COMBO_END};
const uint16_t PROGMEM combo_parens_left[] = {KC_R, KC_E, KC_W, COMBO_END};
const uint16_t PROGMEM combo_parens_right[] = {KC_U, KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM combo_curly_left[] = {KC_R, KC_W, COMBO_END};
const uint16_t PROGMEM combo_curly_right[] = {KC_U, KC_O, COMBO_END};
const uint16_t PROGMEM combo_bracket_left[] = {KC_Q, KC_R, COMBO_END};
const uint16_t PROGMEM combo_bracket_right[] = {KC_U,KC_P, COMBO_END};
const uint16_t PROGMEM combo_copy[]  = {KC_V, MEH_T(KC_C), COMBO_END};
const uint16_t PROGMEM combo_paste[] = {KC_M, MEH_T(KC_COMM), COMBO_END};
const uint16_t PROGMEM combo_undo[] = {MEH_T(KC_C), ALGR_T(KC_X), COMBO_END};
const uint16_t PROGMEM combo_redo[] = {MEH_T(KC_COMM), ALGR_T(KC_DOT), COMBO_END};


combo_t key_combos[COMBO_COUNT] = {
  [COMBO_CAPSLOCK] = COMBO(combo_capslock, KC_CAPS),
  [COMBO_PARENS_LEFT] =   COMBO(combo_parens_left, KC_LPRN),
  [COMBO_PARENS_RIGHT] =  COMBO(combo_parens_right, KC_RPRN),
  [COMBO_CURLY_LEFT] = COMBO(combo_curly_left, KC_LCBR),
  [COMBO_CURLY_RIGHT] = COMBO(combo_curly_right, KC_RCBR),
  [COMBO_BRACKET_LEFT] =  COMBO(combo_bracket_left, KC_LBRC),
  [COMBO_BRACKET_RIGHT] = COMBO(combo_bracket_right, KC_RBRC),
  [COMBO_COPY] = COMBO(combo_copy, COPY),
  [COMBO_PASTE] = COMBO(combo_paste, PASTE),
  [COMBO_UNDO] = COMBO(combo_undo, UNDO),
  [COMBO_REDO] = COMBO(combo_redo, REDO),
};
