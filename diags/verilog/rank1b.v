/*
 Name:        rank1b.v
 Author:      Trevor Williams  (phase1geo@gmail.com)
 Date:        08/04/2008
 Purpose:     See script for details.
*/

module main;

reg  a, b;
wire c, z;

adder1 a( 
  .a(a),
  .b(b),
  .c(c),
  .z(z)
);

initial begin
`ifdef DUMP
        $dumpfile( "rank1b.vcd" );
        $dumpvars( 0, main );
`endif
	a = 1'b0;
	b = 1'b0;
        #10;
	a = 1'b1;
        $finish;
end

endmodule