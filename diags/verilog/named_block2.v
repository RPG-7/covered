module main;

initial begin :foo
end

initial begin
        $dumpfile( "named_block2.vcd" );
        $dumpvars( 0, main );
        #10;
        $finish;
end

endmodule