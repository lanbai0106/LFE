

(* CORE_GENERATION_INFO="topk_topk,hls_ip_2020_2,{HLS_INPUT_TYPE=cxx,HLS_INPUT_FLOAT=0,HLS_INPUT_FIXED=0,HLS_INPUT_PART=xcu280-fsvh2892-2L-e,HLS_INPUT_CLOCK=10.000000,HLS_INPUT_ARCH=dataflow,HLS_SYN_CLOCK=7.611057,HLS_SYN_LAT=53,HLS_SYN_TPT=11,HLS_SYN_MEM=106,HLS_SYN_DSP=0,HLS_SYN_FF=12284,HLS_SYN_LUT=13412,HLS_VERSION=2020_2}" *)

module topk (
        key,
        valid,
        ap_clk,
        ap_rst,
        key_ap_vld,
        valid_ap_vld,
        ap_start,
        ap_done,
        ap_ready,
        ap_idle
);


input  [31:0] key;
input   valid;
input   ap_clk;
input   ap_rst;
input   key_ap_vld;
input   valid_ap_vld;
input   ap_start;
output   ap_done;
output   ap_ready;
output   ap_idle;

wire    topk_entry3_U0_ap_start;
wire    topk_entry3_U0_ap_done;
wire    topk_entry3_U0_ap_continue;
wire    topk_entry3_U0_ap_idle;
wire    topk_entry3_U0_ap_ready;
wire    topk_entry3_U0_start_out;
wire    topk_entry3_U0_start_write;
wire   [0:0] topk_entry3_U0_valid;
wire   [31:0] topk_entry3_U0_key_out_din;
wire    topk_entry3_U0_key_out_write;
wire   [0:0] topk_entry3_U0_valid_out_din;
wire    topk_entry3_U0_valid_out_write;
wire    cm_stage_U0_ap_start;
wire    cm_stage_U0_ap_done;
wire    cm_stage_U0_ap_continue;
wire    cm_stage_U0_ap_idle;
wire    cm_stage_U0_ap_ready;
wire   [63:0] cm_stage_U0_candidate_stream_din;
wire    cm_stage_U0_candidate_stream_write;
wire    cm_stage_U0_start_out;
wire    cm_stage_U0_start_write;
wire    cm_stage_U0_key_read;
wire    cm_stage_U0_valid_read;
wire    heap_stage_U0_ap_start;
wire    heap_stage_U0_ap_done;
wire    heap_stage_U0_ap_continue;
wire    heap_stage_U0_ap_idle;
wire    heap_stage_U0_ap_ready;
wire    heap_stage_U0_candidate_stream_read;
wire    ap_sync_continue;
wire    key_c_full_n;
wire   [31:0] key_c_dout;
wire    key_c_empty_n;
wire    valid_c_full_n;
wire   [0:0] valid_c_dout;
wire    valid_c_empty_n;
wire    candidate_stream_full_n;
wire   [63:0] candidate_stream_dout;
wire    candidate_stream_empty_n;
wire    ap_sync_done;
wire    ap_sync_ready;
wire   [0:0] start_for_cm_stage_U0_din;
wire    start_for_cm_stage_U0_full_n;
wire   [0:0] start_for_cm_stage_U0_dout;
wire    start_for_cm_stage_U0_empty_n;
wire   [0:0] start_for_heap_stage_U0_din;
wire    start_for_heap_stage_U0_full_n;
wire   [0:0] start_for_heap_stage_U0_dout;
wire    start_for_heap_stage_U0_empty_n;
wire    heap_stage_U0_start_full_n;
wire    heap_stage_U0_start_write;

topk_topk_entry3 topk_entry3_U0(
    .ap_clk(ap_clk),
    .ap_rst(ap_rst),
    .ap_start(topk_entry3_U0_ap_start),
    .start_full_n(start_for_cm_stage_U0_full_n),
    .ap_done(topk_entry3_U0_ap_done),
    .ap_continue(topk_entry3_U0_ap_continue),
    .ap_idle(topk_entry3_U0_ap_idle),
    .ap_ready(topk_entry3_U0_ap_ready),
    .start_out(topk_entry3_U0_start_out),
    .start_write(topk_entry3_U0_start_write),
    .key(key),
    .key_ap_vld(key_ap_vld),
    .valid(topk_entry3_U0_valid),
    .valid_ap_vld(valid_ap_vld),
    .key_out_din(topk_entry3_U0_key_out_din),
    .key_out_full_n(key_c_full_n),
    .key_out_write(topk_entry3_U0_key_out_write),
    .valid_out_din(topk_entry3_U0_valid_out_din),
    .valid_out_full_n(valid_c_full_n),
    .valid_out_write(topk_entry3_U0_valid_out_write)
);

topk_cm_stage cm_stage_U0(
    .ap_clk(ap_clk),
    .ap_rst(ap_rst),
    .ap_start(cm_stage_U0_ap_start),
    .start_full_n(start_for_heap_stage_U0_full_n),
    .ap_done(cm_stage_U0_ap_done),
    .ap_continue(cm_stage_U0_ap_continue),
    .ap_idle(cm_stage_U0_ap_idle),
    .ap_ready(cm_stage_U0_ap_ready),
    .candidate_stream_din(cm_stage_U0_candidate_stream_din),
    .candidate_stream_full_n(candidate_stream_full_n),
    .candidate_stream_write(cm_stage_U0_candidate_stream_write),
    .start_out(cm_stage_U0_start_out),
    .start_write(cm_stage_U0_start_write),
    .key_dout(key_c_dout),
    .key_empty_n(key_c_empty_n),
    .key_read(cm_stage_U0_key_read),
    .valid_dout(valid_c_dout),
    .valid_empty_n(valid_c_empty_n),
    .valid_read(cm_stage_U0_valid_read)
);

topk_heap_stage heap_stage_U0(
    .ap_clk(ap_clk),
    .ap_rst(ap_rst),
    .ap_start(heap_stage_U0_ap_start),
    .ap_done(heap_stage_U0_ap_done),
    .ap_continue(heap_stage_U0_ap_continue),
    .ap_idle(heap_stage_U0_ap_idle),
    .ap_ready(heap_stage_U0_ap_ready),
    .candidate_stream_dout(candidate_stream_dout),
    .candidate_stream_empty_n(candidate_stream_empty_n),
    .candidate_stream_read(heap_stage_U0_candidate_stream_read)
);

topk_fifo_w32_d2_S key_c_U(
    .clk(ap_clk),
    .reset(ap_rst),
    .if_read_ce(1'b1),
    .if_write_ce(1'b1),
    .if_din(topk_entry3_U0_key_out_din),
    .if_full_n(key_c_full_n),
    .if_write(topk_entry3_U0_key_out_write),
    .if_dout(key_c_dout),
    .if_empty_n(key_c_empty_n),
    .if_read(cm_stage_U0_key_read)
);

topk_fifo_w1_d2_S valid_c_U(
    .clk(ap_clk),
    .reset(ap_rst),
    .if_read_ce(1'b1),
    .if_write_ce(1'b1),
    .if_din(topk_entry3_U0_valid_out_din),
    .if_full_n(valid_c_full_n),
    .if_write(topk_entry3_U0_valid_out_write),
    .if_dout(valid_c_dout),
    .if_empty_n(valid_c_empty_n),
    .if_read(cm_stage_U0_valid_read)
);

topk_fifo_w64_d1024_A candidate_stream_U(
    .clk(ap_clk),
    .reset(ap_rst),
    .if_read_ce(1'b1),
    .if_write_ce(1'b1),
    .if_din(cm_stage_U0_candidate_stream_din),
    .if_full_n(candidate_stream_full_n),
    .if_write(cm_stage_U0_candidate_stream_write),
    .if_dout(candidate_stream_dout),
    .if_empty_n(candidate_stream_empty_n),
    .if_read(heap_stage_U0_candidate_stream_read)
);

topk_start_for_cm_stage_U0 start_for_cm_stage_U0_U(
    .clk(ap_clk),
    .reset(ap_rst),
    .if_read_ce(1'b1),
    .if_write_ce(1'b1),
    .if_din(start_for_cm_stage_U0_din),
    .if_full_n(start_for_cm_stage_U0_full_n),
    .if_write(topk_entry3_U0_start_write),
    .if_dout(start_for_cm_stage_U0_dout),
    .if_empty_n(start_for_cm_stage_U0_empty_n),
    .if_read(cm_stage_U0_ap_ready)
);

topk_start_for_heap_stage_U0 start_for_heap_stage_U0_U(
    .clk(ap_clk),
    .reset(ap_rst),
    .if_read_ce(1'b1),
    .if_write_ce(1'b1),
    .if_din(start_for_heap_stage_U0_din),
    .if_full_n(start_for_heap_stage_U0_full_n),
    .if_write(cm_stage_U0_start_write),
    .if_dout(start_for_heap_stage_U0_dout),
    .if_empty_n(start_for_heap_stage_U0_empty_n),
    .if_read(heap_stage_U0_ap_ready)
);

assign ap_done = heap_stage_U0_ap_done;

assign ap_idle = (topk_entry3_U0_ap_idle & heap_stage_U0_ap_idle & cm_stage_U0_ap_idle);

assign ap_ready = topk_entry3_U0_ap_ready;

assign ap_sync_continue = 1'b1;

assign ap_sync_done = heap_stage_U0_ap_done;

assign ap_sync_ready = topk_entry3_U0_ap_ready;

assign cm_stage_U0_ap_continue = 1'b1;

assign cm_stage_U0_ap_start = start_for_cm_stage_U0_empty_n;

assign heap_stage_U0_ap_continue = 1'b1;

assign heap_stage_U0_ap_start = start_for_heap_stage_U0_empty_n;

assign heap_stage_U0_start_full_n = 1'b1;

assign heap_stage_U0_start_write = 1'b0;

assign start_for_cm_stage_U0_din = 1'b1;

assign start_for_heap_stage_U0_din = 1'b1;

assign topk_entry3_U0_ap_continue = 1'b1;

assign topk_entry3_U0_ap_start = ap_start;

assign topk_entry3_U0_valid = valid;

endmodule //topk
