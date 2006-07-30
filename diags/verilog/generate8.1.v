module main;

parameter A = 0;

generate
  if( A < 1 )
    begin : b
      reg [31:0] count = 0;
      foo f();
      initial begin : c
	      integer i;
     	      for( i=0; i<4; i=i+1 )
                f.x = i;
      end
      always @(f.x) count = count + 1;
    end
endgenerate

initial begin
        $dumpfile( "generate8.1.vcd" );
        $dumpvars( 0, main );
        #10;
        $finish;
end

endmodule

//---------------------------------------

module foo;

reg x;

endmodule