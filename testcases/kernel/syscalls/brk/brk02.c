// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2021 Liam R. Howlett <liam.howlett@oracle.com>
 *
 *  
 *  Expand the brk by 2 pages to ensure there is a newly created VMA and not
 *  expanding the original due to multiple anon pages.  mprotect that new VMA
 *  then brk back to the original address therefore causing a munmap of at
 *  least one full VMA.
 */

#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>

#include "tst_test.h"

void brk_down_vmas(void)
{
	void *brk_addr = sbrk(0);
	unsigned long page_size = getpagesize();
	void *addr = brk_addr + page_size;

	if (brk(addr))
		return;

	addr += page_size;
	if (brk(addr))
		return;

	if (mprotect(addr - page_size, page_size,
			PROT_READ|PROT_WRITE|PROT_EXEC))
		return;

	addr += page_size;
	if (brk(addr))
		return;

	if (brk(brk_addr))
		return;



	tst_res(TPASS, "munmap two VMAs of brk() passed.");
}

static struct tst_test test = {
	.test_all = brk_down_vmas,
};
