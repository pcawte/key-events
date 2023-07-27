/*
 * Title:			vdp protocl experiments
 * Author:			Paul Cawte
 * Created:			27/07/2023
 *
 * Modinfo:
 */
 
#include <stdio.h>
#include <stdint.h>
#include <mos_api.h>


typedef struct {
    uint32_t time;
    uint8_t vpd_pflags;
    uint8_t keyascii;
    uint8_t keymods;
    uint8_t cursorX;
    uint8_t cursorY;
    uint8_t scrchar;
    union {
        uint24_t scrpixel;
        struct {
            uint8_t scrpixelR;
            uint8_t scrpixelB;
            uint8_t scrpixelG;
        };
    };
    uint8_t audioChannel;
    uint8_t audioSuccess;
    uint16_t scrWidth;
    uint16_t scrHeight;
    uint8_t scrCols;
    uint8_t scrRows;
    uint8_t scrColours;
    uint8_t scrpixelIndex;
    uint8_t vkeycode;
    uint8_t vkeydown;
    uint8_t vkeycount;
    RTC_DATA rtc;
    uint16_t keydelay;
    uint16_t keyrate;
    uint8_t keyled;
    uint8_t scrMode;
    uint24_t errno;
    uint8_t coldBoot;
    uint8_t gp;
    uint8_t serialFlags;
    uint8_t callSM[5];
    uint8_t scratchpad[8];
    union {
    	uint24_t vdp_prot_ctrl;
    	struct {
    		uint8_t vdp_protocol_state;
    		uint8_t vdp_protocol_cmd;
    		uint8_t vdp_protocol_len;
    	};
    };
    uint8_t *vdp_protocol_ptr;
    union {
	    uint8_t vdp_protocol_data[16];
	    uint24_t vdp_prot_data;
    };
} SYSVAR_EXT;

typedef union {
	uint24_t key;
	struct {
		uint8_t mods;
		uint8_t code;
		uint8_t down;
	};
} KEY_EVENT;

static char *up_str = "up";
static char *down_str = "down";

int main( void )
{
	printf("Experiments with VDP protocol bytes:\n\n");
	printf("Press ESC key to quit\n");

	volatile SYSVAR_EXT *sv = (SYSVAR_EXT *)mos_sysvars();

	uint24_t ctrl;
	KEY_EVENT key_event, prev_key_event;
	prev_key_event.key = 0;
	char *up_down;

	do {
		ctrl = sv->vdp_prot_ctrl;
		key_event = (KEY_EVENT)sv->vdp_prot_data;
		if ( ctrl != 0x1d0001 ) continue;
		if ( key_event.key != prev_key_event.key ) {
			if ( key_event.down ) up_down = down_str;
			else up_down = up_str;
			printf( "Key: %2x, modifier: %2x, %s\n",
					key_event.code, key_event.mods, up_down );
			prev_key_event = key_event;
		}
	} while( sv->vdp_protocol_data[1] != '\x7d');

	return 0;
} 