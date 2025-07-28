#ifndef INPUT_CODES_H_
#define INPUT_CODES_H_

// Canonical values for mouse clicks, keyboard keys, and controller input.
typedef enum {
  kSpacebar = 0x20,
  kLeftArrow = 0x25,
  kUpArrow = 0x26,
  kRightArrow = 0x27,
  kDownArrow = 0x28,
  kAKey = 0x41,
  kDKey = 0x44,
  kSKey = 0x53,
  kWKey = 0x57,
} InputCode;

// Utility used to easily initialize data structures using all the values of
// InputCode. Contents must always match those values.
const InputCode input_codes[] = {
  kSpacebar,
  kLeftArrow,
  kUpArrow,
  kRightArrow,
  kDownArrow,
  kAKey,
  kDKey,
  kSKey,
  kWKey,
};

#endif  // INPUT_CODES_H_
