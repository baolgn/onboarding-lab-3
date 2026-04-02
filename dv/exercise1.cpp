#include <VExercise1.h>
#include <cstdint>

int main() {
  VExercise1 model;

  for (int op = 0; op < 4; op++) {
    for (int a = 0; a < 256; a++) {
      for (int b = 0; b < 256; b++) {
        // skip undefined modulo by zero
        if (op == 2 && b == 0) continue;

        model.op = op;
        model.a = a;
        model.b = b;
        model.eval();

        uint8_t expected = 0;
        switch (op) {
          case 0: expected = a ^ b; break;
          case 1: expected = (b >= 8) ? 0 : ((unsigned)a << b) & 0xFF; break;
          case 2: expected = a % b; break;
          case 3: expected = ~(a & b) & 0xFF; break;
        }

        if (model.out != expected) return 1;
      }
    }
  }

  return 0;
}
