#ifndef J73_H
#define J73_H

#include <stdint.h>
#include "j_addr.h"


typedef union {
	uint32_t allmem;
	struct {
		uint8_t SA  :8;
		uint8_t PS  :8;
		uint8_t PF  :8;
		uint8_t DP  :1;
		uint8_t EDP :1;
		uint8_t P   :3;
		uint8_t UNSD:3; 
	};
} J1939_ID_t;


#define PF_DM14 217
#define PF_DM15 216
#define PF_DM17 214
#define PF_ADDR_CLAIM 238


typedef union {//from J73 str 65
	uint64_t allmem; 
	struct {
		uint8_t num_req_bytes_lsb      :8;
		uint8_t sae_reserved           :1;
		uint8_t command                :3;
		uint8_t pointer_type           :1; 
		uint8_t num_req_bytes_msb      :3;
		uint8_t pointer_lsb            :8;
		uint8_t pointer_mid            :8;
		uint8_t pointer_msb            :8;
		uint8_t pointer_ext            :8;
		uint8_t key_lsb                :8;
		uint8_t key_msb                :8;
	};
} J1939_DM14_pkg_t; //73


typedef union {
	uint64_t allmem;
	struct {
		uint8_t num_allowed_bytes_lsb :8;
		uint8_t sae_reserved_2        :1;
		uint8_t status                :3;
		uint8_t sae_reserved_1        :1;
		uint8_t num_allowed_bytes_msb :3;
		uint8_t EDC_param_lsb         :8;
		uint8_t EDC_param_mid         :8;
		uint8_t EDC_param_msb         :8;
		uint8_t EDC_param_ext         :8;
		uint8_t seed_lsb              :8;
		uint8_t seed_msb              :8;
	};
} J1939_DM15_pkg_t;


typedef union {
	uint64_t allmem;
	struct {
		uint8_t iden_num_lsb     :8; //1
		uint8_t iden_num_mid     :8; //2
		uint8_t iden_num_msb     :5;
		uint8_t manuf_code_lsb   :3; //3
		uint8_t manuf_code_msb   :8; //4
		uint8_t ecu_inst         :3;
		uint8_t fun_inst         :5; //5
		uint8_t fun              :8; //6
		uint8_t reserved         :1;
		uint8_t vehicle_sys      :7; //7
		uint8_t vehicle_sys_inst :4;
		uint8_t industry_group   :3;
		uint8_t arb_addr_cab     :1; //8
	};
} J81_addr_claim_pkg_t;


typedef enum {// J73 str 69
	DM14_COMMAND_ERASE        = 0x0U, 
	DM14_COMMAND_READ         = 0x1U,
	DM14_COMMAND_WRITE        = 0x2U,
	DM14_COMMAND_STATUS_REQ   = 0x3U,
	DM14_COMMAND_OP_COMPLETED = 0x4U,
	DM14_COMMAND_OP_FAILED    = 0x5U,
	DM14_COMMAND_BOOT_LOAD    = 0x6U,
	DM14_COMMAND_EDCP_GEN     = 0x7U,
} dm14_command;


typedef enum {
	DM15_STATUS_PROCEED      = 0x0U,
	DM15_STATUS_BUSY         = 0x1U,
	DM15_STATUS_OP_COMPLETED = 0x4U,
	DM15_STATUS_OP_FAILED    = 0x5U,
} dm15_status;


typedef enum {
	PROCESSING_BOOT_LOAD_REQ = 0x000016,
} EDC_param;


typedef enum {
	AGRICULTURE_AND_FORESTRY_EQUIPMENT = 1, // no need others
} INDUSTRY_GROUP;


void formur_dm14_id(J1939_ID_t* dm14_id, uint8_t DST_ADDR);
void formur_dm15_id(J1939_ID_t* dm15_id, uint8_t DST_ADDR); //J73 str 71
void formur_dm17_id(J1939_ID_t* dm17_id, uint8_t DST_ADDR);
void formur_addr_claim_id(J1939_ID_t* addr_claim_id, uint8_t src_addr);

void formur_dm14_pkg(J1939_DM14_pkg_t* dm14_pkg, uint16_t sz, uint16_t key, uint32_t ptr_in_ecu_mem);
void formur_dm15_pkg(J1939_DM15_pkg_t* dm15_pkg, uint16_t allowed_bytes, uint16_t seed, uint8_t status);

int pkg_from_target(uint32_t id, uint8_t target_addr);
int pkg_is_addr_claim(uint32_t id);
int pkg_is_dm15(uint32_t id);


