.. _keyboard:

.. index::
	single: Keyboard
	single: Peripheral; Keyboard

Keyboard
========

The keyboard peripheral directly interfaces to the host computer's keyboard,
and has two :ref:`I/O ports <keyboard-io>` associated with it.

.. _keyboard-io:

.. index::
	pair: Keyboard; IO Ports

I/O Ports
---------

There are two I/O ports associated with the keyboard, the status port ``0x64``
and the control/data port ``0x60``

.. index::
	pair: Keyboard; Status Port

Status Port
^^^^^^^^^^^

The status port, ``0x64`` is used to read information about the current state
of the keyboard.

.. _keyboard-status-port:
.. table:: Status Port (\ ``0x64``\ )

	+------------+------------+------------+------------+------------+------------+------------+----------+
	| Bit 7      | Bit 6      | Bit 5      | Bit 4      | Bit 3      | Bit 2      | Bit 1      | Bit 0    |
	+============+============+============+============+============+============+============+==========+
	| *reserved* | *reserved* | *reserved* | *reserved* | *reserved* | *reserved* | *reserved* | *DRDY*   |
	+------------+------------+------------+------------+------------+------------+------------+----------+

.. _keyboard-drdy:

**DRDY** - Data ready
	When set, indicates that the data register holds a character to be read from
	the keyboard. If clear, the data register contents are undefined.

	This flag will be cleared when the data register is read.

.. index::
	pair: Keyboard; Control Port
	pair: Keyboard; Data Port

Control/Data Port
^^^^^^^^^^^^^^^^^

The control port and data port share the same address, ``0x60``. When an I/O
write to port ``0x60`` occurs, the contents will be written to the control port.
When an I/O read from port ``0x60`` occurs, the contents of the data port will
be read.

.. _keyboard-control-port:
.. table:: Control Port (\ ``0x60``\ , Write Only)

	+------------+------------+------------+------------+------------+------------+------------+----------+
	| Bit 7      | Bit 6      | Bit 5      | Bit 4      | Bit 3      | Bit 2      | Bit 1      | Bit 0    |
	+============+============+============+============+============+============+============+==========+
	| *reserved* | *reserved* | *reserved* | *KBIE*     | *reserved* | *reserved* | *reserved* | *KBEN*   |
	+------------+------------+------------+------------+------------+------------+------------+----------+

**KBIE** - Keyboard Interrupt Enable
	When set, IRQ 9 will be triggered each time a keypress is detected,
	regardless of whether the previous key has been read from the data
	buffer.

.. _keyboard-kben:

**KBEN** - Keyboard Enable
	When set, the keyboard will respond to keypresses. When clear, this device
	is disabled, and no state changes or interrupts will occur.

.. _keyboard-data-port:
.. table:: Data Port (\ ``0x60``\ , Read Only)

	+------------+------------+------------+------------+------------+------------+------------+----------+
	| Bit 7      | Bit 6      | Bit 5      | Bit 4      | Bit 3      | Bit 2      | Bit 1      | Bit 0    |
	+============+============+============+============+============+============+============+==========+
	| *DATA7*    | *DATA6*    | *DATA5*    | *DATA4*    | *DATA3*    | *DATA2*    | *DATA1*    | *DATA0*  |
	+------------+------------+------------+------------+------------+------------+------------+----------+

**DATA[0:7]** - Keyboard Character Data
	When :ref:`DRDY <keyboard-drdy>` is set, DATA contains a valid ASCII
	character that is ready to read. This register will not be modified
	until a read has been performed, at which point :ref:`DRDY <keyboard-drdy>`
	will be	cleared, and the contents of this register become undefined until
	:ref:`DRDY <keyboard-drdy>` is set again.

Usage
-----

There are two ways of using the keyboard, polling and interrupt driven. Following
are simple descriptions of the usage patterns for both methodologies.

.. _keyboard-polling:

Polling Driven
^^^^^^^^^^^^^^

To use the keyboard in a polling based system, only a few steps are required:

1. Write ``0x01`` to the :ref:`control port <keyboard-control-port>` to enable the keyboard
2. Read from the :ref:`status port <keyboard-status-port>` and check if :ref:`DRDY <keyboard-drdy>` \
   is set.
3. If :ref:`DRDY <keyboard-drdy>` is set, then read from the :ref:`data port <keyboard-data-port>` to \
   get the ASCII representation of the key that was pressed.
4. If :ref:`DRDY <keyboard-drdy>` is not set, then go to step 2.

Interrupt Driven
^^^^^^^^^^^^^^^^

To use the keyboard in an interrupt based system, a different steps are required
than using the polling system.

1. Ensure the program loads the address of the keyboard ISR into memory address
   ``0x18``. This is the address of IRQ 9, which is executed on every keypress.
2. Write ``0x11`` to the :ref:`control port <keyboard-control-port>` to enable both the
   keyboard and keyboard interrupts.
3. The main program should now perform any background tasks that are required.
4. When a key is pressed, IRQ 9 will trigger the keyboard ISR that was installed in
   step 1.
5. The keyboard ISR should read the ASCII character from the :ref:`data register <keyboard-data-port>`.
6. The ISR must return by calling IRET to ensure the processor returns from the
   interrupt correctly, and resumes normal program execution.

.. index::
	pair: Keyboard; Issues

Known Issues
------------

* No events are triggered on key release.
* Non printable characters (arrow keys, backspace, shift, etc.) do not trigger interrupts
  or update the data register.
* No ability to update the *Lock* keys (scroll lock, num lock, caps lock) on the keyboard.
