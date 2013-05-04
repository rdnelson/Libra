
.. index::
	single: Screen
	single: Peripheral; Screen

.. _screen:

Screen
======

The screen peripheral is the most obvious of the three Libra peripherals.
The frontend of the screen can be seen on the :ref:`main window <screen-view>`.
This is the easiest way to interact with, and currently the only way to give
output to the user.

.. index::
	pair: Screen; IO Ports

I/O Ports
---------

There are four ports associated with the screen. The control port (\ ``0x4E8``\ )
serves no purpose at the moment, but is reserved for future development. The
data port (\ ``0x4E9``\ ) is used to either read or write to the character buffer.
The remaining two ports are the :ref:`coordinate ports <screen-coord-port>`
(\ ``0x4EA``\ ,\ ``0x4EB``\ ) and are used to either read from or write to the
internal X and Y coordinate registers.


.. index::
	pair: Screen; Control Port

.. _screen-ctrl-port:

Control Port
^^^^^^^^^^^^

The control port, ``0x4E8`` currently has no function, but is reserved for future
use.

.. _screen-ctrl-table:
.. table:: Control Port (\ ``0x4E8``\ )

	+-------+-------+-------+-------+-------+-------+-------+-------+
	| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
	+=======+=======+=======+=======+=======+=======+=======+=======+
	| 0     | 0     | 0     | 0     | 0     | 0     | 0     | 0     |
	+-------+-------+-------+-------+-------+-------+-------+-------+

.. _screen-data-port:

.. index::
	pair: Screen; Data Port

Data Port
^^^^^^^^^

The data port of the screen is read/write. When reading from the data port, the
contents of the screen at the current X and Y coordinates (see :ref:`Coordinate Ports <screen-coord-port>`)
When writing to the port, the new value will be loaded into the screen
buffer at the current X and Y coordinates. In addition, the X coordinate will
be incremented automatically. If this increment pushes the X coordinate past
the edge of the screen, then the Y coordinate will be incremented and the X
coordinate reset to zero.

.. _screen-data-table:
.. table:: Data Port (\ ``0x4E9``\ )

	+---------+---------+---------+---------+---------+---------+---------+---------+
	| Bit 7   | Bit 6   | Bit 5   | Bit 4   | Bit 3   | Bit 2   | Bit 1   | Bit 0   |
	+=========+=========+=========+=========+=========+=========+=========+=========+
	| *DATA7* | *DATA6* | *DATA5* | *DATA4* | *DATA3* | *DATA2* | *DATA1* | *DATA0* |
	+---------+---------+---------+---------+---------+---------+---------+---------+

**DATA[0:7]** - Screen Buffer Data
	Holds the character located at the screen's current X and Y coordinates, or
	updates the same before updating the X and Y coordinates to the next location.


.. index::
	pair: Screen; Coordinates

.. _screen-coord-port:

Coordinate Ports
^^^^^^^^^^^^^^^^

The coordinate ports of the screen are read/write. When reading, they return the
current X or Y coordinate respectively. When writing, they update the X or Y
coordinate. Invalid coordinates (beyond the bounds of the screen) will be rejected,
and the value will not be changed.

.. _screen-coord-table:
.. table:: X/Y Coordinate Port (\ ``0x4EA``\ /\ ``0x4EB``\ )

	+---------+---------+---------+---------+---------+---------+---------+---------+
	| Bit 7   | Bit 6   | Bit 5   | Bit 4   | Bit 3   | Bit 2   | Bit 1   | Bit 0   |
	+=========+=========+=========+=========+=========+=========+=========+=========+
	| *COOR7* | *COOR6* | *COOR5* | *COOR4* | *COOR3* | *COOR2* | *COOR1* | *COOR0* |
	+---------+---------+---------+---------+---------+---------+---------+---------+

**COOR[0:7]** - Coordinate Data
	Holds the current X/Y coordinate of the screen. This is the location of the
	active character on screen. The active character is the character which will
	be returned on :ref:`data port <screen-data-port>` reads, or updated on
	:ref:`data port <screen-data-port>` writes.

.. index::
	pair: Screen; Examples

Example Code
------------

Printing to the screen
^^^^^^^^^^^^^^^^^^^^^^

This is some simple example code that will output the string "Hi" to the screen.::

	MOV DX, 0x04E9	;Load data port
	MOV AL, 'H'	;Load first character
	OUT DX, AL	;Print first character
	MOV AL, 'i'	;Load second character
	OUT DX, AL	;Print second character

Reading a character at a specific location
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following is to read the character at position (1, 1).::

	MOV DX, 0x04EA	;Load X port
	MOV AL, 0x01	;Load desired position (1)
	OUT DX, AL	;Update X posistion
	MOV DX, 0x04EB	;Load Y port
	OUT DX. AL	;Update Y position
	MOV DX, 0x04E9	;Load data port
	IN  AL, DX	;Read character from screen (stored in AL)
