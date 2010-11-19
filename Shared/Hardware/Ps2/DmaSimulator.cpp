///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2004 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Hardware/Ps2/DmaSimulator.h>
#include <Shared/Base/Debug/Assert.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Hardware/Ps2/Vif.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

DmaSimulator::DmaSimulator()
{
	timeout = 4096*2;
	vifstate = VS_READTAG;
	gifstate = GS_READTAG;
	gifqcount = 0;
}

DmaSimulator::~DmaSimulator()
{
}

void DmaSimulator::start(u32* ptr)
{
	disdma(ptr);
}

class Sps2
{
public:
	unsigned int phys2virt(int addr) { return addr; }
};


void DmaSimulator::disdma(u32* ptr)
{
	Sps2 sps2;
	printf("adress %08x\n", (int)ptr);
	while(timeout > 0)
	{
		u64 tag = *(u64*)ptr;
		ptr += 2;
		printf("%08x %08x  ", (int)(tag >> 32), (int)tag);
		timeout--;
		int qwc = tag & 0xffff;
		int pce = (tag >> 26) & 3;
		int id = (tag >> 28) & 7;
		int irq = (tag >> 31) & 1;
		int addr = (tag >> 32) & 0x7fffffff;
		int spr = (tag >> 63) & 1;
		switch(id)
		{
		case 1:
			printf("CNT: %d qw (pce = %d, irq = %d, spr = %d)\n\n", qwc, pce, irq, spr);
			disdata(ptr, 2 + qwc * 4);
			ptr += 2 + qwc * 4;
			break;
		case 2:
			printf("NEXT: %08x, %d qw (pce = %d, irq = %d, spr = %d)\n\n",
					addr, qwc, pce, irq, spr);
			disdata(ptr, 2 + qwc * 4);
			ptr = (u32*)(sps2.phys2virt(addr));
			//printf("%x -> %p\n\n", (int)addr, ptr);
			break;
		case 3:
			printf("REF: %08x, %d qw (pce = %d, irq = %d, spr = %d)\n\n",
					addr, qwc, pce, irq, spr);
			disdata(ptr, 2);
			disdata((u32*)(sps2.phys2virt(addr)), qwc * 4);
			ptr += 2;
			break;
		case 5:
			printf("CALL: %08x, %d qw (pce = %d, irq = %d, spr = %d)\n\n",
					addr, qwc, pce, irq, spr);
			disdata(ptr, 2 + qwc * 4);
			ptr += 2 + qwc * 4;
			disdma((u32*)addr);
			break;
		case 6:
			printf("RET: %d qw (pce = %d, irq = %d, spr = %d)\n\n", qwc, pce, irq, spr);
			disdata(ptr, 2 + qwc * 4);
			return;
			break;
		case 7:
			printf("END: %d qw (pce = %d, irq = %d, spr = %d)\n\n", qwc, pce, irq, spr);
			disdata(ptr, 2 + qwc * 4);
			return;
			break;
		default:
			printf("implementieren sie bitte dma id %d\n\n", id);
			*(volatile char *)0 = 0x666;
			break;
		}
	}
}

void DmaSimulator::disdata(u32 *ptr, int num)
{
	for(; num > 0; num--)
		disvif(*ptr++,ptr-1);
}

void DmaSimulator::disvif(u32 data, u32* ptr)
{
	int i, cmd, num, imm, cmd_c;
	switch(vifstate)
	{
	case VS_READTAG:
		imm = data & 0xffff;
		num = (data >> 16) & 255;
		cmd = (data >> 24) & 0x7f;
		i = data >> 31;
		printf("%08x %08x ", (int)ptr, (int)data);
		if(i)
			printf("[I]       ");
		else
			printf("          ");
		timeout--;
		cmd_c = cmd;
		if((cmd & 0x60) == 0x60)
			cmd = 0x60;
		switch(cmd)
		{
		case 0:
			printf("NOP\n\n");
			break;
		case 1:
			printf("STCYCL: cl = %d, wl = %d\n\n", imm & 255, (imm >> 8) & 255);
			break;
		case 5:
			printf("STMOD: mode = %d\n", imm & 255);
			break;
		case 0x50:
			printf("DIRECT: %d qw\n\n", imm);
			vifstate = VS_DIRECT;
			vifcount = imm * 4;
			break;
		case 0x4a:
			if(num == 0)
				num = 256;
			printf("MPG: to %03x, %d dw\n\n", imm, num);
			vifstate = VS_MPG;
			vifcount = num * 2;
			break;
		case 0x4:
			printf("ITOP: %d\n\n", imm);
			break;
		case 0x20:
			printf("STMASK\n\n");
			vifstate = VS_MPG;
			vifcount = 1;
			break;
		case 0x30:
			printf("STROW\n\n");
			vifstate = VS_UNPACK;
			vifcount = 4;
			break;
		case 0x31:
			printf("STCOL\n\n");
			vifstate = VS_UNPACK;
			vifcount = 4;
			break;
		case 0x10:
			printf("FLUSHE\n\n");
			break;
		case 0x11:
			printf("FLUSH\n\n");
			break;
		case 0x14:
			printf("MSCAL: %03x\n\n", imm);
			break;
		case 0x15:
			printf("MSCALF: %03x\n\n", imm);
			break;
		case 0x17:
			printf("MSCNT\n\n");
			break;
		case 0x2:
			printf("OFFSET: %03x\n\n", imm);
			break;
		case 0x3:
			printf("BASE: %03x\n\n", imm);
			break;
		case 0x60:
			printf("UNPACK: ");
			switch((cmd_c >> 2) & 3)
			{
			case 0: printf("S-"); break;
			case 1: printf("V2-"); break;
			case 2: printf("V3-"); break;
			case 3: printf("V4-"); break;
			}
			switch(cmd_c & 3)
			{
			case 0: printf("32"); break;
			case 1: printf("16"); break;
			case 2: printf("8"); break;
			case 3: printf("5"); break;
			}
			printf(" to %03x, %d qw", imm & 1023, num);
			if(imm & 0x8000)
				printf(", use tops");
			if(imm & 0x4000)
				printf(", unsigned");
			if(cmd_c & 0x10)
				printf(", masking write");
			vifstate = VS_UNPACK;
			{
				int z, n;
				z = num * (((cmd_c >> 2) & 3) + 1);
				n = 1 << (cmd_c & 3);
				vifcount = (z + n - 1) / n;
			}
			printf(", %d words", vifcount);
			printf("\n\n");
			break;
		default:
			printf("unknown vifcode 0x%x\n\n", cmd);
			*(volatile char *)0 = 0x666;
			break;
		}
		break;
	case VS_DIRECT:
		//disgif(data);
		if(--vifcount == 0)
			vifstate = VS_READTAG;
		break;
	case VS_UNPACK:
	case VS_MPG:
		if(--vifcount == 0)
			vifstate = VS_READTAG;
		break;
	default:
		printf("unknown vifstate!\n\n");
		*(volatile char *)0 = 0x666;
		break;
	}
}

void DmaSimulator::disgif(u32 data)
{
/*
	int nloop, eop, pre, prim, flg, reg;
	u64 regs;
	switch(gifstate)
	{
	case GS_READTAG:
		if(gifqcount == 0)
			giftag = 0;
		giftag |= ((u128)data) << (gifqcount * 32);
		if(++gifqcount < 4)
			return;
		gifqcount = 0;
		nloop = giftag & 0x7fff;
		eop = (giftag >> 15) & 1;
		pre = (giftag >> 46) & 1;
		prim = (giftag >> 47) & 0x7ff;
		flg = (giftag >> 58) & 3;
		nreg = (giftag >> 60) & 15;
		regs = giftag >> 64;
		printf("%08x %08x %08x %08x  ", (int)(giftag >> 96), (int)(giftag >> 64),
				(int)(giftag >> 32), (int)giftag);
		switch(flg)
		{
		case 0:
			printf("PACKED: nloop %d, eop %d, pre %d, prim %x, flg %d, nreg %d, regs %08x%08x\n\n",
					nloop, eop, pre, prim, flg, nreg, (int)(regs >> 32), (int)regs);
			if(nloop)
			{
				gifstate = GS_PACKED;
				gifcount = nloop * nreg;
			}
			break;
		case 2:
			printf("IMAGE: nloop %d\n\n", nloop);
			if(nloop)
			{
				gifstate = GS_IMAGE;
				gifcount = nloop;
			}
			break;
		default:
			printf("baaaaeeh!\n\n");
			asm("break");
			break;
		}
		break;
	case GS_PACKED:
		if(gifqcount == 0)
			gifqword = 0;
		gifqword |= ((u128)data) << (gifqcount * 32);
		if(++gifqcount < 4)
			return;
		gifqcount = 0;
		reg = nreg - 1 - (gifcount - 1) % nreg;
		reg = (giftag >> (64 + reg * 4)) & 15;
		printf("%08x %08x %08x %08x  ", (int)(gifqword >> 96), (int)(gifqword >> 64),
				(int)(gifqword >> 32), (int)gifqword);
		switch(reg)
		{
		case 0x1:
			printf("RGBAQ: %02x%02x%02x%02x\n\n", (int)(gifqword >> 96), (int)(gifqword >> 64),
				(int)(gifqword >> 32), (int)gifqword);
			break;
		case 0x2:
			printf("STQ: %f, %f, %f\n\n", *(float*)&gifqword, ((float*)&gifqword)[1],
					((float*)&gifqword)[2]);
			break;
		case 0x3:
			printf("UV: %.4f, %.4f\n\n", ((float)(int)gifqword) / 16.0f,
					((float)(int)(gifqword >> 32)) / 16.0f);
			break;
		case 0x5:
			printf("XYZ2: %.4f, %.4f, 0x%x  ", ((float)(int)gifqword) / 16.0f,
					((float)(int)(gifqword >> 32)) / 16.0f, (int)(gifqword >> 64));
			if((gifqword >> 111) & 1)
				printf("no drawing kick\n\n");
			else
				printf("drawing kick\n\n");
			break;
		case 0xe:
			printf("A+D: ");
			disad(gifqword >> 64, gifqword);
			break;
		default:
			printf("Unknown Direct code %x\n\n", reg);
			asm("break");
			break;
		}
		if(--gifcount == 0)
			gifstate = GS_READTAG;
		break;
	case GS_IMAGE:
		if(++gifqcount < 4)
			return;
		gifqcount = 0;
		if(--gifcount == 0)
			gifstate = GS_READTAG;
		break;
	default:
		printf("unknown gifstate\n\n");
		asm("break");
		break;
	}
*/
}


void DmaSimulator::disad(int reg, u64 data)
{
/*
	switch(reg)
	{
	case 0x6:
	case 0x7:
		printf("TEX0_%d: tpb = %d, tbw = %d, psm = ", reg - 5, (int)data & 16383,
				(int)(data >> 14) & 63);
		switch((data >> 20) & 63)
		{
			case 0x00: printf("PSMCT32"); break;
			case 0x01: printf("PSMCT24"); break;
			case 0x02: printf("PSMCT16"); break;
			case 0x0a: printf("PSMCT16S"); break;
			case 0x13: printf("PSMT8"); break;
			case 0x14: printf("PSMT4"); break;
			case 0x1b: printf("PSMT8H"); break;
			case 0x24: printf("PSMT4HL"); break;
			case 0x2c: printf("PSMT4HH"); break;
			case 0x30: printf("PSMZ32"); break;
			case 0x31: printf("PSMZ24"); break;
			case 0x32: printf("PSMZ16"); break;
			case 0x3a: printf("PSMZ16S"); break;
			default: ZENIC_ASSERT(!"UNKNOWN pixel format");
		}
		printf(", tw = %d, th = %d, tcc = %d, tfx = ", (int)(data >> 26) & 15,
				(int)(data >> 30) & 15, (int)(data >> 34) & 1);
		switch((data >> 35) & 3)
		{
			case 0: printf("MODULATE"); break;
			case 1: printf("DECAL"); break;
			case 2: printf("HIGHLIGHT"); break;
			case 3: printf("HIGHLIGHT2"); break;
		}
		printf(", cbp = %d, cpsm = ", (int)(data >> 37) & 16383);
		switch((data >> 51) & 15)
		{
			case 0x00: printf("PSMCT32"); break;
			case 0x02: printf("PSMCT16"); break;
			case 0x0a: printf("PSMCT16S"); break;
			default: ZENIC_ASSERT(!"UNKNOWN pixel format");
		}
		printf(", csm = %d, csa = %d, cld = %d\n\n", (int)(data >> 55) & 1,
				(int)(data >> 56) & 31, (int)(data >> 61) & 7);
		break;
	case 0x18:
	case 0x19:
		printf("XYOFFSET_%d: %.4f, %.4f\n\n", reg - 0x17, ((float)(int)data) / 16.0f,
				((float)(int)(data >> 32)) / 16.0f);
		break;
	case 0x1a:
		printf("PRMODECONT: %d\n\n", (int)data);
		break;
	case 0x3f:
		printf("TEXFLUSH\n\n");
		break;
	case 0x40:
		printf("SCISSOR_%d: %d, %d - %d, %d\n\n", reg - 0x3f, (int)data & 2047,
				(int)(data >> 32) & 2047, (int)(data >> 16) & 2047, (int)(data >> 48) & 2047);
		break;
	case 0x42:
	case 0x43:
		printf("ALPHA_%d: (", reg - 0x41);
		switch(data & 3)
		{
			case 0: printf("Cs - "); break;
			case 1: printf("Cd - "); break;
			case 2: printf("0 - "); break;
			case 3: printf("Err - "); break;
		}
		switch((data >> 2) & 3)
		{
			case 0: printf("Cs) * "); break;
			case 1: printf("Cd) * "); break;
			case 2: printf("0) * "); break;
			case 3: printf("Err) * "); break;
		}
		switch((data >> 4) & 3)
		{
			case 0: printf("As + "); break;
			case 1: printf("Ad + "); break;
			case 2: printf("%d + ", (int)(data >> 32) & 255); break;
			case 3: printf("Err + "); break;
		}
		switch((data >> 6) & 3)
		{
			case 0: printf("Cs\n\n"); break;
			case 1: printf("Cd\n\n"); break;
			case 2: printf("0\n\n"); break;
			case 3: printf("Err\n\n"); break;
		}
		break;
	case 0x45:
		printf("DTHE: %d\n\n", (int)data);
		break;
	case 0x46:
		printf("COLCLAMP: %d\n\n", (int)data);
		break;
	case 0x47:
	case 0x48:
		printf("TEST_%d: ATE: %d, ATST: ", reg - 0x46, (int)(data & 1));
		switch((data >> 1) & 7)
		{
		case 0: printf("NEVER"); break;
		case 1: printf("ALWAYS"); break;
		case 2: printf("LESS"); break;
		case 3: printf("LEQUAL"); break;
		case 4: printf("EQUAL"); break;
		case 5: printf("GEQUAL"); break;
		case 6: printf("GREATER"); break;
		case 7: printf("NOTEQUAL"); break;
		}
		printf(", AREF: %d, AFAIL: ", (int)(data >> 4) & 255);
		switch((data >> 12) & 3)
		{
		case 0: printf("KEEP"); break;
		case 1: printf("FB_ONLY"); break;
		case 2: printf("ZB_ONLY"); break;
		case 3: printf("RGB_ONLY"); break;
		}
		printf(", DATE: %d, DATM: %d, ZTE: %d, ZTST: ", (int)(data >> 14) & 1,
				(int)(data >> 15) & 1, (int)(data >> 16) & 1);
		switch((data >> 17) & 3)
		{
		case 0: printf("NEVER\n\n"); break;
		case 1: printf("ALWAYS\n\n"); break;
		case 2: printf("GEQUAL\n\n"); break;
		case 3: printf("GREATER\n\n"); break;
		}
		break;
	case 0x4c:
	case 0x4d:
		printf("FRAME_%d: FBP: %d, FBW: %d, PSM: %x, FBMSK: %08x\n\n", reg - 0x4b,
				(int)data & 511, (int)(data >> 16) & 63, (int)(data >> 24) & 63,
				(int)(data >> 32));
		break;
	case 0x4e:
	case 0x4f:
		printf("ZBUF_%d: ZBP: %d, PSM: %x, ZMSK: %d\n\n", reg - 0x4d,
				(int)data & 511, (int)(data >> 24) & 15, (int)(data >> 32) & 1);
		break;
	default:
		printf("reg %x, value %08x%08x\n\n", reg, (int)(data >> 32), (int)data);
		break;
	}
*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

