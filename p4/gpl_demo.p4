#include<core.p4>
#if __TARGET_TOFINO__ == 2
#include<t2na.p4>
#else
#include<tna.p4>
#endif




header ethernet_t {
    bit<48> dstAddr;
    bit<48> srcAddr;
    bit<16> etherType;
}

header ipv4_t {
    bit<4> version;
    bit<4> ihl;
    bit<8> diffserv;
    bit<16> totalLen;
    bit<16> identification;
    bit<3> flags;
    bit<13> fragOffset;
    bit<8> ttl;
    bit<8> protocol;
    bit<16> hdrChecksum;
    bit<32> srcAddr;
    bit<32> dstAddr;
}


header rclt_count_t{

}

struct headers{
    ethernet_t ethernet_hdr;
	ipv4_t ipv4_hdr;
    rclt_count_t rclt_count_hdr;
}


struct ingress_metadata_t {
    bit<16> hash1;
    bit<16> hash2;
    bit<16> est1;
    bit<16> est2;
    bit<16> ml1;
    bit<16> ml2;
    bit<16> max;
    bit<16> min;
    bit<16> cha;
    bit<16> final_est;
    bit<8> flag1;
    bit<8> flag2;
    bit<8> flag3;
    bit<8> flag4;
    bit<8> a;
    bit<8> b;

 }

struct egress_metadata_t {
}





parser dpmcacheParser(
        packet_in packet,
        out headers hdr,
        out ingress_metadata_t meta,
        out ingress_intrinsic_metadata_t ig_intr_md)
{
        state start{
            packet.extract(ig_intr_md);
            packet.advance(PORT_METADATA_SIZE);
		    transition parse_ethernet;
        }

        state parse_ethernet {
		    packet.extract(hdr.ethernet_hdr);
		    transition select(hdr.ethernet_hdr.etherType) {
			    0x0800: parse_ipv4;
			    default: reject;
            }
		}

        state parse_ipv4 {
		    packet.extract(hdr.ipv4_hdr);
		    transition accept;
	    }

}




typedef bit<9>  egressSpec_t;
control dpmcacheIngress (
    inout headers                       hdr,
    inout ingress_metadata_t                      meta,
    /* Intrinsic */
    in    ingress_intrinsic_metadata_t               ig_intr_md,
    in    ingress_intrinsic_metadata_from_parser_t   ig_prsr_md,
    inout ingress_intrinsic_metadata_for_deparser_t  ig_dprsr_md,
    inout ingress_intrinsic_metadata_for_tm_t        ig_tm_md){
	
    Hash<bit<16>>(HashAlgorithm_t.CRC32) hash1_calc;
	action hash_1(){
		meta.hash1 = hash1_calc.get({
             hdr.ipv4_hdr.dstAddr
		});
	}
    @pragma stage 0
    table hash_1_table{
        key = {
            hdr.ipv4_hdr.dstAddr:exact;
        }
        actions = {
            hash_1;
        }
		size = 16;
    }

	Hash<bit<16>>(HashAlgorithm_t.CRC16) hash2_calc;
	action hash_2() {
		meta.hash2 =  hash2_calc.get({
            hdr.ipv4_hdr.dstAddr
		});
	}
    @pragma stage 0
    table hash_2_table{
        key = {
            hdr.ipv4_hdr.dstAddr:exact;
        }
        actions = {
            hash_2;
        }
		size = 16;
    }







Register<bit<16>,bit<16>>(65536) counter_count1;
RegisterAction<bit<16>,bit<16>,bit<16>>(counter_count1) counter_count_alu_add1 ={
	void apply(inout bit<16> register_data,out bit<16> alu_data){
		register_data  = register_data + 1;
		alu_data = register_data;
	}
};

Register<bit<16>,bit<16>>(65536) counter_count2;

RegisterAction<bit<16>,bit<16>,bit<16>>(counter_count2) counter_count_alu_add2 ={
	void apply(inout bit<16> register_data,out bit<16> alu_data){
		register_data  = register_data + 1;
		alu_data = register_data;
	}
};


action condition_a_0(){
  meta.ml1 = meta.est1;
}
action condition_a_1(){
  meta.ml1 = meta.est1>>1;

}
action condition_a_2(){
  meta.ml1 = meta.est1>>2;

}
action condition_a_3(){
  meta.ml1 = meta.est1>>3;
}



@pragma stage 2
table get_ml1_table{
	key = {
		meta.a:exact;
		}
	actions = {
		condition_a_0;
		condition_a_1;
        condition_a_2;
        condition_a_3;
	}
	size = 8;
}


action condition_b_0(){
  meta.ml2 = meta.est2;
}
action condition_b_1(){
  meta.ml2 = meta.est2>>1;

}
action condition_b_2(){
  meta.ml2 = meta.est2>>2;

}
action condition_b_3(){
  meta.ml2 = meta.est2>>3;
}
@pragma stage 2
table get_ml2_table{
	key = {
		meta.b:exact;
		}
	actions = {
		condition_b_0;
		condition_b_1;
        condition_b_2;
        condition_b_3;
	}
	size = 8;
}

action get_final1(){
    meta.final_est  = meta.min;
 }
action get_final2(){
    meta.final_est  = meta.ml1;
 }
@pragma stage 3
table con_threshold_table{
	key = {
		meta.cha:range;
		}
	actions = {
		get_final1;
		get_final2;
	}
	size = 8;
}

action set_flag_c1(){
    meta.flag1 = 1;
}

action set_flag_c2(){
    meta.flag1 = 2;
    meta.flag2 = 1;
}

action set_flag_c3(){
    meta.flag2 = 2;
    meta.flag3 = 1;
}


action set_flag_c4(){
    meta.flag3 = 2;
    meta.flag4 = 1;
}

@pragma stage 4
table get_flag_table{
	key = {
		meta.final_est:exact;
		}
	actions = {
		set_flag_c1;
		set_flag_c2;
		set_flag_c3;
		set_flag_c4;
	}
	size = 8;
}

// 1-2

Register<bit<16>,bit<16>>(2) h1;
RegisterAction<bit<16>,bit<16>,bit<16>>(h1) h1_alu_add ={
	void apply(inout bit<16> register_data,out bit<16> alu_data){
		register_data  = register_data + 1;
		alu_data = register_data;
	}
};
RegisterAction<bit<16>,bit<16>,bit<16>>(h1) h1_alu_dec ={
	void apply(inout bit<16> register_data,out bit<16> alu_data){
		register_data  = register_data - 1;
		alu_data = register_data;
	}
};
action h1_add(){
    h1_alu_add.execute(0);
}

action h1_dec(){
    h1_alu_dec.execute(0);
}
@pragma stage 5
table h1_table{
	key = {
		meta.flag1:exact;
		}
	actions = {
		h1_add;
		h1_dec;
	}
	size = 8;
}

Register<bit<16>,bit<16>>(2) h2;
RegisterAction<bit<16>,bit<16>,bit<16>>(h2) h2_alu_add ={
	void apply(inout bit<16> register_data,out bit<16> alu_data){
		register_data  = register_data + 1;
		alu_data = register_data;
	}
};
RegisterAction<bit<16>,bit<16>,bit<16>>(h2) h2_alu_dec ={
	void apply(inout bit<16> register_data,out bit<16> alu_data){
		register_data  = register_data - 1;
		alu_data = register_data;
	}
};
action h2_add(){
    h2_alu_add.execute(0);
}

action h2_dec(){
    h2_alu_dec.execute(0);
}
@pragma stage 6
table h2_table{
	key = {
		meta.flag2:exact;
		}
	actions = {
		h2_add;
		h2_dec;
	}
	size = 8;
}
//  3-4
Register<bit<16>,bit<16>>(2) h3;
RegisterAction<bit<16>,bit<16>,bit<16>>(h3) h3_alu_add ={
	void apply(inout bit<16> register_data,out bit<16> alu_data){
		register_data  = register_data + 1;
		alu_data = register_data;
	}
};
RegisterAction<bit<16>,bit<16>,bit<16>>(h3) h3_alu_dec ={
	void apply(inout bit<16> register_data,out bit<16> alu_data){
		register_data  = register_data - 1;
		alu_data = register_data;
	}
};
action h3_add(){
    h3_alu_add.execute(0);
}

action h3_dec(){
    h3_alu_dec.execute(0);
}
@pragma stage 7
table h3_table{
	key = {
		meta.flag3:exact;
		}
	actions = {
		h3_add;
		h3_dec;
	}
	size = 8;
}

Register<bit<16>,bit<16>>(2) h4;
RegisterAction<bit<16>,bit<16>,bit<16>>(h4) h4_alu_add ={
	void apply(inout bit<16> register_data,out bit<16> alu_data){
		register_data  = register_data + 1;
		alu_data = register_data;
	}
};
RegisterAction<bit<16>,bit<16>,bit<16>>(h4) h4_alu_dec ={
	void apply(inout bit<16> register_data,out bit<16> alu_data){
		register_data  = register_data - 1;
		alu_data = register_data;
	}
};
action h4_add(){
    h4_alu_add.execute(0);
}

action h4_dec(){
    h4_alu_dec.execute(0);
}
@pragma stage 8
table h4_table{
	key = {
		meta.flag4:exact;
		}
	actions = {
		h4_add;
		h4_dec;
	}
	size = 8;
}


    
    apply{
        hash_1_table.apply();
        hash_2_table.apply();
        meta.est1 = counter_count_alu_add1.execute(meta.hash1);
        meta.est2 = counter_count_alu_add2.execute(meta.hash2);
        meta.max = max(meta.est1,meta.est2);
        meta.min = min(meta.est1,meta.est2);
        get_ml1_table.apply();
        get_ml2_table.apply();
        meta.ml1 = min(meta.ml1,meta.ml2);
        meta.cha = meta.max - meta.min;
        con_threshold_table.apply();
        get_flag_table.apply();
        h1_table.apply();
        h2_table.apply();
        h3_table.apply();
        h4_table.apply();

        
 
    }

    }

//Ingress deparser
control IngressDeparser(packet_out packet,
    /* User */
    inout headers                       hdr,
    in    ingress_metadata_t                      meta,
    /* Intrinsic */
    in    ingress_intrinsic_metadata_for_deparser_t  ig_dprsr_md)
{
	apply{
		packet.emit(hdr);
	}
}

//egress parser
parser EgressParser(packet_in      packet,
    /* User */
    out headers          hdr,
    out egress_metadata_t         meta,
    /* Intrinsic */
    out egress_intrinsic_metadata_t  eg_intr_md)
{
    state start{
		packet.extract(eg_intr_md);
		transition accept;
	}

}

control dpmcacheEgress(    /* User */
    inout headers                          hdr,
    inout egress_metadata_t                         meta,
    /* Intrinsic */    
    in    egress_intrinsic_metadata_t                  eg_intr_md,
    in    egress_intrinsic_metadata_from_parser_t      eg_prsr_md,
    inout egress_intrinsic_metadata_for_deparser_t     eg_dprsr_md,
    inout egress_intrinsic_metadata_for_output_port_t  eg_oport_md){ 

        apply{
            
        }
    }


control dpmcacheDeparser(packet_out packet,
	inout headers hdr,
	in egress_metadata_t meta,
	in egress_intrinsic_metadata_for_deparser_t eg_dprsr_md)
{
	apply{
		packet.emit(hdr);
	}
}


Pipeline(
    dpmcacheParser(),
    dpmcacheIngress(),
    IngressDeparser(),
    EgressParser(),
    dpmcacheEgress(),
    dpmcacheDeparser()
) pipe;

Switch(pipe) main;
