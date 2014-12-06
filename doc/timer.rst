.. _timer:

.. index::
	single: Timer
	single: Peripheral; Timer

Timer
=====

The timer peripheral adds a timing ability to Libra. The timer functions as a
simple counter, starting from zero, and counting internally every millisecond.
When the required number of milliseconds have elapsed, an interrupt (IRQ 8) is
triggered, and the timer is reset.

**The timer only counts time when the program is running. If execution is paused
or stepped, the timer will not function**

.. index::
	pair: Timer; IO Ports

I/O Ports
---------

The timer has two associated ports. The :ref:`control port <timer-ctrl-port>`
(\ ``0x43``\ ) is used to enable/disable the timer, and the
:ref:`data port <timer-data-port>` (\ ``0x40``\ )  sets the required time
before the interrupt is triggered.

.. _timer-ctrl-port:

.. index::
	pair: Timer; Control Port

Control Port
^^^^^^^^^^^^

The control port, ``0x43`` is used to enable/disable the timer.

.. _timer-ctrl-table:
.. table:: Control Port (\ ``0x43``\ )

	+------------+------------+------------+------------+------------+--------+--------+------------+
	| Bit 7      | Bit 6      | Bit 5      | Bit 4      | Bit 3      | Bit 2  | Bit 1  | Bit 0      |
	+============+============+============+============+============+========+========+============+
	| *reserved* | *reserved* | *reserved* | *reserved* | *reserved* | *TIE*  | *TEN*  | *reserved* |
	+------------+------------+------------+------------+------------+--------+--------+------------+

**TIE** - Timer Interrupt Enable
	When this bit is set, the timer will trigger IRQ 8 when the timer is elapsed.
	If this bit is clear, the timer will be disabled, because there is currently
	no way to poll the timer to determine if the elapsed time has passed.

    **Note: While interrupts are enabled, the HLT instruction will not terminate
    execution. This allows programs that use interrupts to put the processor to
    sleep pending an interrupt. The only way to terminate execution of programs
    while interrupts are enabled is to manually stop it.**

**TEN** - Timer Enabled
	When this bit is set, the timer is enabled. When clear, the timer is disabled.

.. _timer-data-port:

.. index::
	pair: Timer; Data Port

Data Port
^^^^^^^^^

The data port, ``0x40`` is used to set the number of milliseconds before the timer
triggers an interrupt.

.. _timer-data-table:
.. table:: Data Port (\ ``0x40``\ )

	+----------+----------+----------+----------+----------+----------+---------+---------+
	| Bit 15   | Bit 14   | Bit 13   | Bit 12   | Bit 11   | Bit 10   | Bit 9   | Bit 8   |
	+==========+==========+==========+==========+==========+==========+=========+=========+
	| *TIME15* | *TIME14* | *TIME13* | *TIME12* | *TIME11* | *TIME10* | *TIME9* | *TIME8* |
	+----------+----------+----------+----------+----------+----------+---------+---------+

+----------+----------+----------+----------+----------+----------+---------+---------+
| Bit 7    | Bit 6    | Bit 5    | Bit  4   | Bit 3    | Bit 2    | Bit 1   | Bit 0   |
+==========+==========+==========+==========+==========+==========+=========+=========+
| *TIME7*  | *TIME6*  | *TIME5*  | *TIME4*  | *TIME3*  | *TIME2*  | *TIME1* | *TIME0* |
+----------+----------+----------+----------+----------+----------+---------+---------+

**TIME[0:15]** - Timer Count
	This 16 bit register holds the desired number of milliseconds before the next
	interrupt.

Examples
--------

A simple example of enabling and setting up the timer is shown below.::

	.ORG 0x10	;location of ISR8
	dw timer	;install ISR8
	.ORG 0x200	;past all ISR addresses (0x00 - 0x1FF)
	MOV DX, 0x40	;load data port
	MOV AX, 0x0A	;10 millisecond count
	OUT DX, AX	;Store the new count in the timer
	MOV DX, 0x43	;load control port
	MOV AL, 0x06	;enable timer and timer interrupts (0x02 | 0x04 == 0x06)
	OUT DX, AL	;start timer
	Done:
	HLT		;Main program is done
	JMP Done	;Interrupts trigger a resume from HLT, so jump back to the HLT

	timer:		;Timer ISR
	INC	CX	;Count the number of times the timer ticks
	IRET		;Return from interrupt
