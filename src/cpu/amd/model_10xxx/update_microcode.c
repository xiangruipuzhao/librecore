/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2015 Timothy Pearson <tpearson@raptorengineeringinc.com>, Raptor Engineering
 * Copyright (C) 2007 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdint.h>
#include <cpu/amd/microcode.h>

/* From the Revision Guide :
 * Equivalent Processor Table for AMD Family 10h Processors
 *
 * Installed Processor   Equivalent Processor   Patch Level
 * Revision ID           Revision ID
 * 00100F00h             1000h                  01000020h
 * 00100F01h             1000h                  01000020h
 * 00100F02h             1000h                  01000020h
 * 00100F20h             1020h                  01000096h
 * 00100F21h (DR-B1)     1020h                  01000096h
 * 00100F2Ah (DR-BA)     1020h                  01000096h
 * 00100F22h (DR-B2)     1022h                  01000095h
 * 00100F23h (DR-B3)     1022h                  01000095h
 * 00100F42h (RB-C2)     1041h                  01000086h
 * 00100F43h (RB-C3)     1043h                  010000b6h
 * 00100F62h (DA-C2)     1062h                  0100009Fh
 * 00100F63h (DA-C3)     1043h                  010000b6h
 * 00100F81h (HY-D1)     1081h                  010000c4h
 * 00100FA0h (PH-E0)     10A0h                  010000bfh
 */

struct id_mapping {
        uint32_t orig_id;
        uint16_t new_id;
};

static u16 get_equivalent_processor_rev_id(u32 orig_id) {
	static const struct id_mapping id_mapping_table[] = {
		{ 0x100f00, 0x1000 },
		{ 0x100f01, 0x1000 },
		{ 0x100f02, 0x1000 },
		{ 0x100f20, 0x1020 },
		{ 0x100f21, 0x1020 },
		{ 0x100f2A, 0x1020 },
		{ 0x100f22, 0x1022 },
		{ 0x100f23, 0x1022 },
		{ 0x100f42, 0x1041 },
		{ 0x100f43, 0x1043 },
		{ 0x100f62, 0x1062 },
		{ 0x100f63, 0x1043 },
		{ 0x100f81, 0x1081 },
		{ 0x100fa0, 0x10A0 },

		/* Array terminator */
		{ 0xffffff, 0x0000 },
	};

	u32 new_id;
	int i;

	new_id = 0;

	for (i = 0; id_mapping_table[i].orig_id != 0xffffff; i++ ) {
		if (id_mapping_table[i].orig_id == orig_id) {
			new_id = id_mapping_table[i].new_id;
			break;
		}
	}

	return new_id;

}

void update_microcode(u32 cpu_deviceid)
{
	u32 equivalent_processor_rev_id = get_equivalent_processor_rev_id(cpu_deviceid);
	amd_update_microcode_from_cbfs(equivalent_processor_rev_id);
}
