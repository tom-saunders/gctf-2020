module check(
    input clk,

    input [6:0] data,
    output wire open_safe,
    output wire [63:0] meut,
    output wire [63:0] maut,
    output wire [63:0] kout,
    output wire [63:0] exp
);

reg [6:0] memory [7:0];
reg [2:0] idx = 0;

wire [55:0] magic = {
    {memory[0], memory[5]},
    {memory[6], memory[2]},
    {memory[4], memory[3]},
    {memory[7], memory[1]}
};


wire [55:0] kittens = { magic[9:0],  magic[41:22], magic[21:10], magic[55:42] };
assign open_safe = kittens == 56'd3008192072309708;

wire [63:0] meut = {
    1'0, memory[7],
    1'0, memory[6],
    1'0, memory[5],
    1'0, memory[4],
    1'0, memory[3],
    1'0, memory[2],
    1'0, memory[1],
    1'0, memory[0]
};
wire [63:0] maut = {
    1'0, magic[55:49],
    1'0, magic[48:42],
    1'0, magic[41:35],
    1'0, magic[34:28],
    1'0, magic[27:21],
    1'0, magic[20:14],
    1'0, magic[13:7],
    1'0, magic[6:0]
};
wire [63:0] kout = {
    1'0, kittens[55:49],
    1'0, kittens[48:42],
    1'0, kittens[41:35],
    1'0, kittens[34:28],
    1'0, kittens[27:21],
    1'0, kittens[20:14],
    1'0, kittens[13:7],
    1'0, kittens[6:0]
};

reg [55:0] act_exp = 56'd3008192072309708;
wire [63:0] exp = {
    1'0, act_exp[55:49],
    1'0, act_exp[48:42],
    1'0, act_exp[41:35],
    1'0, act_exp[34:28],
    1'0, act_exp[27:21],
    1'0, act_exp[20:14],
    1'0, act_exp[13:7],
    1'0, act_exp[6:0]
};

always_ff @(posedge clk) begin
    memory[idx] <= data;
    idx <= idx + 5;
end

endmodule


