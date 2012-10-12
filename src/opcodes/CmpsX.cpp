/*-------------------------------------*\
|
|  File Name: CmpsX.cpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Fri, Oct 12, 2012  9:54:24 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "CmpsX.hpp"

#include "../Processor.hpp"

#include <cstdio>

CmpsX::CmpsX(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* CmpsX::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == CMPSB) {
		snprintf(buf, 65, "CMPSB");
		GETINST(preSize + 1);
		return new CmpsX(pre, buf, inst, (int)*opLoc);
	} else if ( *opLoc == CMPSW){
		snprintf(buf, 65, "CMPSW");
		GETINST(preSize + 1);
		return new CmpsX(pre, buf, inst, (int)*opLoc);
	}
	return 0;
}

int CmpsX::Execute(Processor* proc) {

	unsigned int si = proc->GetRegister(REG_SI);
	unsigned int di = proc->GetRegister(REG_DI);
	unsigned int ds = proc->GetRegister(REG_DS);
	unsigned int size = mOpcode == CMPSB ? 1 : 2;

	proc->SetRegister(REG_SI, si + size);
	proc->SetRegister(REG_DI, di + size);

	unsigned int dst = proc->GetMemory(ds * 0x10 + si, size);
	unsigned int src = proc->GetMemory(ds* 0x10 + di, size);
	unsigned int val = dst - src;

	proc->SetFlag(FLAGS_CF, val > 0xFFFF);
	val &= 0xFFFF;

	proc->SetFlag(FLAGS_OF, OverflowAdd(val, dst, src, size));
	proc->SetFlag(FLAGS_ZF, val == 0);
	proc->SetFlag(FLAGS_PF, Parity(val));
	proc->SetFlag(FLAGS_SF, (val & (1 << (8 * size))) != 0);
	proc->SetFlag(FLAGS_AF, (val & 0xF) != 0);

	return 0;
}
