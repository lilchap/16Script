// memloc.h
// symbols defined at link time

#pragma once
#include "type.h"

#define READSYM(x) (uint16_t)&(x)

/*
dos_sector = (sector - 1) + (head * sectors_per_track) +
	       (track * sectors_per_track * num_heads)

physical_sector = 1 + (dos_sector % sectors_per_track)
physical_head = (dos_sector / sectors_per_track) % num_heads
physical_track = dos_sector / (sectors_per_track * num_heads)
*/

/* Disk Geometry: 5.25" DSDD 1.2 MB */

#define C_ENTRY __attribute__((section(".text.main")))

#define SECTORS_PER_TRACK 15
#define DISK_HEADS 2

#define LBA_GET_C(x) x / (SECTORS_PER_TRACK * DISK_HEADS)
#define LBA_GET_H(x) (x / SECTORS_PER_TRACK) % DISK_HEADS
#define LBA_GET_S(x) 1 + (x % SECTORS_PER_TRACK)

extern uint16_t _script_sectors;
extern uint16_t _script_start;
extern uint16_t _script_end;
extern uint16_t _c_end;