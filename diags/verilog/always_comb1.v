module main;

reg a, b, c, d, e, f;

always_comb
  begin
    a = b | c;
    d = e & f;
  end

initial begin
`ifndef VPI
        $dumpfile( "always_comb1.vcd" );
        $dumpvars( 0, main );
`endif
	#100;
	$finish;
end

initial begin
	#10;
	b = 1'b0;
	c = 1'b1;
        e = 1'b1;
        f = 1'b0;
	#10;
	c = 1'b0;
	#10;
	f = 1'b1;
end

endmodule
