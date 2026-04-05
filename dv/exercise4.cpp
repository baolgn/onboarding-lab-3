#include <VExercise4.h>
#include <cstdint>

int main() {
  VExercise4 model;

  // test cs=0: out must be 0 for all input combinations
  // vary every input to ensure none affect output
  model.cs = 0;
  for (int sel = 0; sel < 4; sel++) {
    for (int a = 0; a < 256; a += 17) {
      for (int b = 0; b < 256; b += 37) {
        for (int g = 0; g < 256; g += 51) {
          model.sel = sel;
          model.alpha = a;
          model.beta = b;
          model.gamma = g;
          model.eval();
          if (model.out != 0) return 1;
        }
      }
    }
  }

  // test cs=1, sel=0: out = alpha
  model.cs = 1;
  model.sel = 0;
  for (int a = 0; a < 256; a++) {
    model.alpha = a;
    model.beta = ~a & 0xFF;
    model.gamma = a ^ 0x55;
    model.eval();
    if (model.out != (uint8_t)a) return 1;
  }

  // test cs=1, sel=1: out = beta
  model.sel = 1;
  for (int b = 0; b < 256; b++) {
    model.alpha = ~b & 0xFF;
    model.beta = b;
    model.gamma = b ^ 0xAA;
    model.eval();
    if (model.out != (uint8_t)b) return 1;
  }

  // test cs=1, sel=2: out = gamma
  model.sel = 2;
  for (int g = 0; g < 256; g++) {
    model.alpha = g ^ 0x55;
    model.beta = ~g & 0xFF;
    model.gamma = g;
    model.eval();
    if (model.out != (uint8_t)g) return 1;
  }

  // test cs=1, sel=3: out = alpha & (beta | gamma)
  model.sel = 3;
  for (int a = 0; a < 256; a += 7) {
    for (int b = 0; b < 256; b += 11) {
      for (int g = 0; g < 256; g += 13) {
        model.alpha = a;
        model.beta = b;
        model.gamma = g;
        model.eval();
        uint8_t expected = a & (b | g);
        if (model.out != expected) return 1;
      }
    }
  }

  return 0;
}
