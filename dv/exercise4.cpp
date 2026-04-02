#include <VExercise3.h>
#include <cstdint>

// reference model for mystery1
static uint8_t mystery1(uint8_t a, uint8_t b, uint8_t c) {
  switch (a) {
    case 0: return ((b & 0x7) << 3) | (c & 0x7);
    case 1: return 0x40 | ((c & 0x7) << 3) | (b & 0x7);
    case 2: return b;
    case 3: return c;
    default: return 0;
  }
}

// xor-reduce a 16-bit value to 1 bit
static uint16_t parity(uint16_t v) {
  uint16_t p = 0;
  for (int i = 0; i < 16; i++) { p ^= (v >> i) & 1; }
  return p;
}

// compute mystery2 next output given current state and inputs
static uint16_t mystery2_step(int state, uint16_t out, uint8_t a_in, uint8_t b_in) {
  switch (state) {
    case 0: return ((uint16_t)a_in << 8) | (out & 0xFF);
    case 1: return (out & 0xFF00) | b_in;
    case 2: return ((out & 0xFF) << 8) | ((out >> 8) & 0xFF);
    case 3: return ((out & 0xF) << 12) | (((out >> 4) & 0xF) << 8) |
                   (((out >> 8) & 0xF) << 4) | ((out >> 12) & 0xF);
    case 4: return parity(out);
    default: return 0;
  }
}

static void tick(VExercise3 &m) {
  m.clk = 1;
  m.eval();
  m.clk = 0;
  m.eval();
}

// test one combination of inputs through reset and state cycling
static int test_case(VExercise3 &model, uint8_t a, uint16_t b, uint16_t c) {
  model.a = a;
  model.b = b;
  model.c = c;
  model.eval();

  uint8_t a_in = mystery1(a & 0x3, b & 0xFF, c & 0xFF);
  uint8_t b_in = mystery1((a >> 2) & 0x3, (b >> 8) & 0xFF, (c >> 8) & 0xFF);

  // reset
  model.reset = 1;
  tick(model);
  uint16_t expected = ((uint16_t)b_in << 8) | a_in;
  if (model.out != expected) return 1;

  // cycle through states twice (10 cycles)
  model.reset = 0;
  for (int cycle = 0; cycle < 10; cycle++) {
    expected = mystery2_step(cycle % 5, expected, a_in, b_in);
    tick(model);
    if (model.out != expected) return 1;
  }

  return 0;
}

int main() {
  VExercise3 model;
  model.clk = 0;
  model.eval();

  // test all 16 combinations of mystery1 modes (a[1:0] x a[3:2])
  // with representative b and c values
  uint16_t b_vals[] = {0xABCD, 0x1234};
  uint16_t c_vals[] = {0x5678, 0x9ABC};

  for (auto bv : b_vals) {
    for (auto cv : c_vals) {
      for (int a = 0; a < 16; a++) {
        if (test_case(model, a, bv, cv)) return 1;
      }
    }
  }

  // edge cases: all zeros, all ones
  if (test_case(model, 0, 0x0000, 0x0000)) return 1;
  if (test_case(model, 0xF, 0xFFFF, 0xFFFF)) return 1;

  return 0;
}
