/**
  @brief A module of mysterious purpose

  @input clk    clock
  @input reset  active-high reset
  @input a      an input
  @input b      an input
  @input c      an input

  @output out   output
*/
module Exercise3 (
    input clk,
    input reset,
    input [3:0] a,
    input [15:0] b,
    input [15:0] c,
    output [15:0] out
);
  logic [7:0] a_in;
  logic [7:0] b_in;

  Mystery1 alpha (
      .a(a[1:0]),
      .b(b[7:0]),
      .c(c[7:0]),
      .d(a_in)
  );
  Mystery1 beta (
      .a(a[3:2]),
      .b(b[15:8]),
      .c(c[15:8]),
      .d(b_in)
  );

  Mystery2 gamma (.*);
endmodule
