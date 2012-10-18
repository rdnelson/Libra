/*-------------------------------------*\
|
|  File Name: TestInstruction.cpp
|
|  Creation Date: 07-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  5:34:36 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "gtest/gtest.h"

#include "Instruction.hpp"


TEST(InstructionFlagsTest, OverflowAdd16) {

	ASSERT_FALSE(Instruction::OverflowAdd(0, 0, 2));
	ASSERT_FALSE(Instruction::OverflowAdd(0x7FFF, 0, 2)); // 0x7FFF (32767)
	ASSERT_TRUE(Instruction::OverflowAdd(0x7FFF, 1, 2)); // 0x8000 (-32768)
	ASSERT_TRUE(Instruction::OverflowAdd(0x4000, 0x4000, 2)); // 0x8000 (-32768)
	ASSERT_FALSE(Instruction::OverflowAdd(0xFFFF, 0xFFFE, 2)); // 0xFFFD (-3)
	ASSERT_TRUE(Instruction::OverflowAdd(0xFFFF, 0x8000, 2)); // 0x7FFF (32767)
	ASSERT_FALSE(Instruction::OverflowAdd(0xFFFF, 0x0001, 2)); // 0x0000 (0)
}

TEST(InstructionFlagsTest, OverflowAdd8) {

	ASSERT_FALSE(Instruction::OverflowAdd(0, 0, 1));
	ASSERT_FALSE(Instruction::OverflowAdd(0x7F, 0, 1)); // 0x7F (127)
	ASSERT_TRUE(Instruction::OverflowAdd(0x7F, 1, 1)); // 0x80 (-128)
	ASSERT_TRUE(Instruction::OverflowAdd(0x40, 0x40, 1)); // 0x80 (-128)
	ASSERT_FALSE(Instruction::OverflowAdd(0xFF, 0xFE, 1)); // 0xFD (-3)
	ASSERT_TRUE(Instruction::OverflowAdd(0xFF, 0x80, 1)); // 0x7F (127)
	ASSERT_FALSE(Instruction::OverflowAdd(0xFF, 0x01, 1)); // 0x00 (0)

}

TEST(InstructionFlagsTest, OverflowSub8) {

	ASSERT_FALSE(Instruction::OverflowSub(0, 0, 1));
	ASSERT_FALSE(Instruction::OverflowSub(0x7F, 0x00, 1)); // 0x7F (127)
	ASSERT_FALSE(Instruction::OverflowSub(0x7F, 0x01, 1)); // 0x7E (126)
	ASSERT_FALSE(Instruction::OverflowSub(0x00, 0x01, 1)); // 0xFF (-3)
	ASSERT_TRUE(Instruction::OverflowSub(0x7F, 0xFF, 1)); // 0x7F (32767)
	ASSERT_TRUE(Instruction::OverflowSub(0x80, 0x01, 1)); // 0x7F (32767)

}

TEST(InstructionFlagsTest, Parity) {

	ASSERT_TRUE(Instruction::Parity(0x00));
	ASSERT_TRUE(Instruction::Parity(0xFC));
	ASSERT_TRUE(Instruction::Parity(0xFF));
	ASSERT_TRUE(Instruction::Parity(0x03));
	ASSERT_FALSE(Instruction::Parity(0x01));
	ASSERT_FALSE(Instruction::Parity(0xFE));
	ASSERT_FALSE(Instruction::Parity(0x80));

}
