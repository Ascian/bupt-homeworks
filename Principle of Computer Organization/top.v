module cpu (
    input clr,
    input t3,
    input swa,
    input swb,
    input swc,
    input [7:4] ir,
    input w1,
    input w2,
    input w3,
    input c,
    input z,
    output drw,
    output pcinc,
    output lpc,
    output lar,
    output pcadd,
    output arinc,
    output selctl,
    output memw,
    output stop,
    output lir,
    output ldz,
    output ldc,
    output cin,
    output [3:0] s;
    output m,
    output abus,
    output sbus,
    output mbus,
    output short,
    output long,
    output sel0,
    output sel1,
    output sel2,
    output sel3);

    wire st0; 
    wire sst0;
    wire[2:0] sw;

    assign sw = {swc,swb,swa};
    assign st0 = 0;
    assign sst0 = 0;

    always @(clr or t3 or w1 or w2 or w3 or sw or c or z or ir or st0) begin
        if(!clr)
            st0 <= 0;
        else if(negedge t3) begin
            if(st0==1 && w2 == 1 && sw == 3'b100) begin
                st0<=0;
            end
            if(sst0 == 1)
                st0=1;
        end
        s <= 4'b0000;
		m <= 1'b0;
		cin <= 1'b0;
		sel3 <= 1'b0;
		sel2 <= 1'b0;
		sel1 <= 1'b0;
		sel0 <= 1'b0;
		selctl <= 1'b0;
		lir <= 1'b0;
		ldc <= 1'b0;
		ldz <= 1'b0;
		lpc <= 1'b0;
		lar <= 1'b0;
		pcinc <= 1'b0;
		pcadd <= 1'b0;
		arinc <= 1'b0;
		long <= 1'b0;
		short <= 1'b0;
		abus <= 1'b0;
		mbus <= 1'b0;
		sbus <= 1'b0;
		drw <= 1'b0;
		memw <= 1'b0;
		stop <= 1'b0;
		sst0 <= 1'b0;

        case (sw)
            3'b100:
            ;
            3'b011: 
            ;
            3'b010:
            ;
            3'b001:
            ;
            3'b000:
                if(st0==0) begin
                    
                end
                else if(st0==1) begin
                    
                end
            ;
            default:
            ;
        endcase
    end
endmodule