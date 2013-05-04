Introduction to Libra
=====================

Libra is an 8086 emulator with a focus on features useful in an academic setting.
The goal is to deliver an emulator that not only allows 8086 code to be executed,
but also provides a deeper understanding of the processes occuring inside the
CPU. This is accomplished with features such as:

* Memory access logging

	* Operand Read/Write activity
	* Instruction Read/Write activity

* Cache simulation (Planned)
* Operand highlighting
* Instruction memory highlighting
* Various stepping methods
* Various breakpoint types

Libra has a companion assembler, lasm, which should be used to compile programs
for execution.

For information about the various simulated I/O devices available from within
Libra, please see the :ref:`peripherals` page.
