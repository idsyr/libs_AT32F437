#include "J73.h"

void formur_dm14_id(J1939_ID_t* dm14_id, uint8_t DST_ADDR){
	dm14_id->SA  = SRC_ADDR; // define this
	dm14_id->PS  = DST_ADDR;   
	dm14_id->PF  = 217; //from J73 str 65
	dm14_id->DP  = 0; //from J73 str 65
	dm14_id->EDP = 0; //All SAE J1939 messages shall set EDP to zero on transmit //j21 str 12
	dm14_id->P   = 6; //from J73 str 65
}


void formur_dm15_id(J1939_ID_t* dm15_id, uint8_t DST_ADDR){ //J73 str 71
	dm15_id->SA  = SRC_ADDR;
	dm15_id->PS  = DST_ADDR;
	dm15_id->PF  = 216;
	dm15_id->DP  = 0;
	dm15_id->EDP = 0;
	dm15_id->P   = 6;
}


void formur_dm17_id(J1939_ID_t* dm17_id, uint8_t DST_ADDR){
	dm17_id->SA  = SRC_ADDR;
	dm17_id->PS  = DST_ADDR;
	dm17_id->PF  = 214;
	dm17_id->DP  = 0;
	dm17_id->EDP = 0;
	dm17_id->P   = 6;
}


void formur_addr_claim_id(J1939_ID_t* addr_claim_id, uint8_t src_addr){
	addr_claim_id->SA  = src_addr;
	addr_claim_id->PS  = 0xFF; //broadcast
	addr_claim_id->PF  = 238;
	addr_claim_id->DP  = 0; //J81 str 23
	addr_claim_id->EDP = 0;
	addr_claim_id->P   = 6;
}


void formur_dm14_pkg(J1939_DM14_pkg_t* dm14_pkg, uint16_t sz, uint16_t key, uint32_t ptr_in_ecu_mem){
	dm14_pkg->num_req_bytes_lsb = sz; // amount of mem over which the tool desires an operation
	dm14_pkg->num_req_bytes_msb = sz >> 8;
	dm14_pkg->pointer_type      = 0;    // 0 -> ptr_ext is part of ptr, 1 -> way to describe signature
	dm14_pkg->command           = DM14_COMMAND_BOOT_LOAD;
	dm14_pkg->sae_reserved      = 0x1;
	dm14_pkg->pointer_lsb       = (ptr_in_ecu_mem); //0x08010000
	dm14_pkg->pointer_mid       = (ptr_in_ecu_mem >> 8);
	dm14_pkg->pointer_msb       = (ptr_in_ecu_mem >> 16); //addr must now dont use format 0x080000XX
	dm14_pkg->pointer_ext       = (ptr_in_ecu_mem >> 24);
	dm14_pkg->key_lsb           = key;
	dm14_pkg->key_msb           = key >> 8;
}


void formur_dm15_pkg(J1939_DM15_pkg_t* dm15_pkg, uint16_t allowed_bytes, uint16_t seed, uint8_t status){
	dm15_pkg->num_allowed_bytes_lsb = allowed_bytes;
	dm15_pkg->num_allowed_bytes_msb = allowed_bytes >> 8;
	dm15_pkg->sae_reserved_1        = 1;
	dm15_pkg->status                = status;
	dm15_pkg->sae_reserved_2        = 1;
	dm15_pkg->EDC_param_lsb         = (uint32_t)PROCESSING_BOOT_LOAD_REQ;
	dm15_pkg->EDC_param_mid         = (uint32_t)PROCESSING_BOOT_LOAD_REQ >> 8;
	dm15_pkg->EDC_param_msb         = (uint32_t)PROCESSING_BOOT_LOAD_REQ >> 16;
	dm15_pkg->EDC_param_ext         = 0xFF; //no error indicator, we never know anything about errors!
	dm15_pkg->seed_lsb              = seed;
	dm15_pkg->seed_msb              = seed >> 8;
}


int pkg_from_target(uint32_t id, uint8_t target_addr){
	J1939_ID_t* jid = (J1939_ID_t*)&id;
	return jid->SA == target_addr;
}


int pkg_is_addr_claim(uint32_t id){
	J1939_ID_t* jid = (J1939_ID_t*)&id;
	return jid->PF == PF_ADDR_CLAIM;
}


int pkg_is_dm15(uint32_t id){
	J1939_ID_t* jid = (J1939_ID_t*)&id;
	return jid->PF == PF_DM15;
}
