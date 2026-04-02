#include <VExercise2.h>

int main() {
  VExercise2 model;
}
#include <VExercise2.h>
#include <cstdint>

// compute one lfsr step
static uint16_t lfsr_step(uint16_t val) {
  uint16_t fb = ((val >> 10) ^ (val >> 8) ^ (val >> 3) ^ (val >> 1)) & 1;
  return ((val << 1) | fb) & 0xFFFF;
}

// perform one full clock cycle (negedge triggers update)
static void tick(VExercise2 &m) {
  m.clk = 1;
  m.eval();
  m.clk = 0;
  m.eval();
}

int main() {
  VExercise2 model;
  model.clk = 0;
  model.eval();

  uint16_t seeds[] = {0xACE1, 0x1234, 0x0000, 0xFFFF, 0x00FF, 0xFF00};

  for (auto seed : seeds) {
    // test reset: out should be ~init after negedge
    model.reset = 1;
    model.init = seed;
    tick(model);
    if (model.out != (uint16_t)(~seed)) return 1;

    // test lfsr stepping for 200 cycles
    model.reset = 0;
    uint16_t expected = ~seed & 0xFFFF;
    for (int i = 0; i < 200; i++) {
      expected = lfsr_step(expected);
      tick(model);
      if (model.out != expected) return 1;
    }

    // test that re-asserting reset mid-sequence works
    model.reset = 1;
    model.init = seed;
    tick(model);
    if (model.out != (uint16_t)(~seed)) return 1;
  }

  return 0;
}
