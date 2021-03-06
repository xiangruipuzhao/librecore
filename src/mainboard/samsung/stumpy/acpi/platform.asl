/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2007-2009 coresystems GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/* The _PTS method (Prepare To Sleep) is called before the OS is
 * entering a sleep state. The sleep state number is passed in Arg0
 */

Method(_PTS,1)
{
	Store (Zero, GP08)  // Disable Bluetooth

	If (LEqual (Arg0, 3)) {
		// NVS has a flag to determine USB policy in S3
		If (S3U0) {
			Store (One, GP47)   // Enable USB0
		} Else {
			Store (Zero, GP47)  // Disable USB0
		}

		// NVS has a flag to determine USB policy in S3
		If (S3U1) {
			Store (One, GP56)   // Enable USB1
		} Else {
			Store (Zero, GP56)  // Disable USB1
		}
	}
	If (LEqual (Arg0, 5)) {
		// NVS has a flag to determine USB policy in S5
		If (S5U0) {
			Store (One, GP47)   // Enable USB0
		} Else {
			Store (Zero, GP47)  // Disable USB0
		}

		// NVS has a flag to determine USB policy in S5
		If (S5U1) {
			Store (One, GP56)   // Enable USB1
		} Else {
			Store (Zero, GP56)  // Disable USB1
		}
	}
}

/* The _WAK method is called on system wakeup */

Method(_WAK,1)
{
	Return(Package(){0,0})
}

/* CMOS Access */
OperationRegion (CMOS, SystemIO, 0x70, 0x71)
Field (CMOS, ByteAcc, NoLock, Preserve)
{
	NVRI, 8,
	NVRD, 8,
}

IndexField (NVRI, NVRD, ByteAcc, NoLock, Preserve)
{
	Offset (0x32),
	US3B, 8,	// USB Controller Reset S3 behavior
}

#define USB_RESET_DISABLE_MAGIC 0xdd

/* Disable USB Controller Reset in S3 (defaults to enabled) */
Method (USBR, 0, Serialized)
{
	Store (USB_RESET_DISABLE_MAGIC, US3B)
}
