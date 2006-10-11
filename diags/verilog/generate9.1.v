module main;

localparam A = 1;

generate
  if( A < 1 )
    task foo_task;
      reg x;
      begin
        x = 1'b0;
        #10;
        x = 1'b1;
      end
    endtask
  else
    task foo_task;
      reg x;
      begin
        x = 1'b1;
        #10;
        x = 1'b0;
      end
    endtask
endgenerate

initial begin
	foo_task;
end

initial begin
`ifndef VPI
        $dumpfile( "generate9.1.vcd" );
        $dumpvars( 0, main );
`endif
        #20;
        $finish;
end

endmodule
