/*-------------------------------------*\
|
|  File Name: Breakpoint.hpp
|
|  Creation Date: 28-10-2012
|
|  Last Modified:
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include <vector>
#include "Processor.hpp"

class Breakpoint {
	public:
		Breakpoint();
		Breakpoint(unsigned int IP);

		void AddRegisterWatch(eRegisters reg, unsigned int val);
		void AddRegisterChanged(eRegisters reg);

		void AddFlagsWatch(eFlags flag, bool value);
		void AddFlagsChanged(eFlags flag);

		void AddMemWatch(unsigned int addr, unsigned int val, unsigned int size);
		void AddMemChanged(unsigned int addr, unsigned int size);

		bool Evaluate(Processor* proc);

	private:

		enum eConditionType {
			REGISTER_WATCH,
			REGISTER_CHANGE,
			FLAGS_WATCH,
			FLAGS_CHANGE,
			MEM_WATCH,
			MEM_CHANGE			
		};

		struct Condition {
			union {
				eRegisters reg;
				eFlags flag;
				unsigned int addr;
			} source;

			eConditionType type;

			union {
				unsigned int iVal;
				bool bVal;
			} value;

			bool ready;
			unsigned int size;

		};

		std::vector<Condition> mConditions;
		unsigned int mIP;
		bool mUseIP;
};


