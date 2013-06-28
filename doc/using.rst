.. _using:

Using Libra
===========

Libra has many features that can help improve the users's understanding of what
happens during the execution on 8086 code. Some of these features need a bit of
explanation to use to their full potential. This page will describe most of the
featues of the interface, so that it can be used to the greatest extent possible.

Main Window
===========

The main window of Libra has several sections, each of which will be described in
their own section below.

1. :ref:`Instruction Listing <instruction-list>`
2. :ref:`Screen <screen-view>`
3. :ref:`Memory View <memory-view>`
4. :ref:`Register View <register-view>`
5. :ref:`Toolbar <toolbar>`

.. _instruction-list:

Instruction Listing
-------------------

The instruction listing is the section of the screen where the instructions of the
currently loaded program are displayed. When program execution is paused (or
hasn't begun) any instructions or labels that are at the address currently stored
in IP are highlighted in yellow. Right clicking on an instruction will provide
the ability to set a breakpoint. Breakpoints are highlighted in red. If execution
hits a breakpoint while running, it will pause before the instruction is executed.

.. _screen-view:

Screen
------

The screen is the front end of the :ref:`screen <screen>` peripheral. It is updated every
30\ :superscript:`th` of a second while the program is running, and every instruction
when execution is being stepped.

.. _memory-view:

Memory View
-----------

The memory view shows a full listing of the internal memory of the Libra emulator.
There are several colors of highlighting that can appear in the memory view that
each have different definitions.

Highlighting Colours
^^^^^^^^^^^^^^^^^^^^

Memory is only highlighted when execution is paused.

* **Yellow**:	The encoded instruction, not including encoded operands.
* **Red**:	The encoded representation of the 2\ :superscript:`nd` operand.
* **Green**:	The encoded representation of the 1\ :superscript:`st` operand.
* **Magenta**:	The encoded representation of the 3\ :superscript:`rd` operand.

In some cases, the instruction encoding makes it impossible to highlight some
or all of the operands. In these cases, any operands that are impossible to
highlight will not have their colour appear.

.. _register-view:

Register View
-------------

The register view shows the current status of all of the registers available on
an 8086 processor. Similar to the :ref:`memory view <memory-view>`, if one of
the registers is an operand in the current instruction, then it will be
highlighted using the same colour scheme described above. In addition to the
16 bit register values, there are checkboxes which are used to indicate the
status of all of the 8086 flags.

.. _toolbar:

Toolbar
-------

The toolbar icons will be familiar to most people that have used an IDE (especially
netbeans), but each icon corresponds to a menu item which shares the same symbol and
name. The definition of each action will be detailed below:

File Menu
---------

The file menu has actions that deal with the opening of files, or general
program actions like quitting.

.. _load-obj:

Load Object File
^^^^^^^^^^^^^^^^

This action loads an assembled program into Libra so that it can be simulated.
All memory is reset to ``0xFF`` before the program is loaded into memory.

Reload File
^^^^^^^^^^^

Only enabled once a file has been loaded using :ref:`load-obj`, this action
refreshes any area of memory that is set by the object file. All other memory
is left in its current state. This option can be used to change the value of
data, but fix any instruction corruption if instructions were overwritten.

Exit
^^^^

Pretty self explanatory, if you need a help file to figure this out, Libra
is probably a bit too complicated for you.

Debug Menu
----------

The debug menu holds all of the actions that are used to delve into the
operation of the loaded program. These include actions that can be used
to pause execution at necessary times, or to execute very small sections
of the program at a time.

Run
^^^

This action begins (or resumes) execution of the currently loaded program.
This action is only available once a program has been loaded.

Pause
^^^^^

This action pauses the currently running execution of a program. This
simply stops performing the fetch-execute cycle after the current one
completes. The interface is then updated, and all appropriate highlighting
is done.

Stop
^^^^

Very similar to pausing the execution, clicking stop halts the fetch-execute cycle,
but also resets IP to the beginning of the program, and reinitializes all
other registers to ``0x0000``.

Step Out
^^^^^^^^

*Step Out* is one of the stepping functions intended to step through sections of code.
Specifically, step out will do one of two things:

A. It will return from the current function, not showing any deeper calls.
   This leaves IP pointing to the instruction following the ``CALL``
   instruction that just returned.

B. If there are no ``CALL``\ s currently in process, *step out* will simply
   behave the same as *step in*. A byproduct of this behaviour is that
   if there are no current ``CALL``\ s in process, but the next line is a
   ``CALL``\ , *step out* will execute the call and pause on the first line
   within the new function. This is expected behaviour. A subsequent use of
   *step out* would cause the execution to run until the ``CALL`` completes.

Step In
^^^^^^^

*Step In* is the simplest of the stepping functions. It simply executes the
current line, and pauses before executing the next one.

Step Over
^^^^^^^^^

*Step Over* is the final of the stepping functions. It can behave in two ways:

A. If the next line to be executed is a ``CALL`` then *step over* will execute
   the call, and pause execution on the line following the ``CALL``.

B. If the next line to be executed is not a ``CALL`` then *step over* will
   behave the same as *step in*.


