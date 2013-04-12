/*-------------------------------------*\
|
|  File Name: CmpsX.cpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  5:05:26 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "CmpsX.hpp"

#include "../Processor8086.hpp"

#include <cstdio>

CmpsX::CmpsX(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* CmpsX::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;

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

int CmpsX::Execute() {

	unsigned int si = mProc->GetRegister(Processor8086::REG_SI);
	unsigned int di = mProc->GetRegister(Processor8086::REG_DI);
	unsigned int ds = mProc->GetRegister(Processor8086::REG_DS);
	unsigned int size = mOpcode == CMPSB ? 1 : 2;

	mProc->SetRegister(Processor8086::REG_SI, si + size);
	mProc->SetRegister(Processor8086::REG_DI, di + size);

	unsigned int dst = mProc->GetMemory(ds * 0x10 + si, size);
	unsigned int src = mProc->GetMemory(ds * 0x10 + di, size);
	unsigned int val = dst - src;

	mProc->SetFlag(Processor8086::FLAGS_CF, val > 0xFFFF);
	val &= 0xFFFF;

	mProc->SetFlag(Processor8086::FLAGS_OF, OverflowAdd(dst, src, size));
	mProc->SetFlag(Processor8086::FLAGS_ZF, val == 0);
	mProc->SetFlag(Processor8086::FLAGS_PF, Parity(val));
	mProc->SetFlag(Processor8086::FLAGS_SF, (val & (1 << (8 * size))) != 0);
	mProc->SetFlag(Processor8086::FLAGS_AF, (val & 0xF) != 0);

	return 0;
}
