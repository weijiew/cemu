#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <iostream>
#include "../src/cup.h"
#include "test_util.h"

namespace crvemu {

// Test addi instruction
TEST(RVTests, TestAddi) {
  {
    std::string code = start +
        "addi x31, x0, 42 \n";
    Cpu cpu = rv_helper(code, "test_addi", 1);
    EXPECT_EQ(cpu.regs[31], 42) << "Error: x31 should be 42 after ADDI instruction";
  }
  {
    std::string code = start +
        "addi x2, x0, -10 \n";  // ADDI instruction with a negative immediate value
    Cpu cpu = rv_helper(code, "test_addi", 1);

    EXPECT_EQ(cpu.regs[2], static_cast<uint64_t>(-10)) << "Error: x2 should be -10 after ADDI instruction";
  }
  {
    std::string code = start +
        "addi x2, x0, 10 \n"    // 将 10 加载到 x2 中
        "addi x3, x0, -10 \n"   // 将 -10 加载到 x3 中
        "addi x4, x0, 0 \n";    // 将 0 加载到 x4 中
    Cpu cpu = rv_helper(code, "test_addi", 3);

    // 验证 x2, x3, x4 的值是否正确
    EXPECT_EQ(cpu.regs[2], 10) << "Error: x2 should be 10 after ADDI instruction";
    EXPECT_EQ(cpu.regs[3], static_cast<uint64_t>(-10)) << "Error: x3 should be -10 after ADDI instruction";
    EXPECT_EQ(cpu.regs[4], 0) << "Error: x4 should be 0 after ADDI instruction";    
  }
}


// Test slli instruction
TEST(RVTests, TestSlli) {
  std::string code = start +
      "addi x2, x0, 5 \n"    // Load 5 into x2
      "slli x1, x2, 3 \n";   // x1 = x2 << 3
  Cpu cpu = rv_helper(code, "test_slli", 2);

  // Verify if x1 has the correct value
  EXPECT_EQ(cpu.regs[1], 5 << 3) << "Error: x1 should be the result of SLLI instruction";
}

// Test slti instruction
TEST(RVTests, TestSlti) {
  std::string code = start +
      "addi x2, x0, 8 \n"    // 将 8 加载到 x2 中
      "slti x1, x2, 10 \n";  // x1 = (x2 < 10) ? 1 : 0
  Cpu cpu = rv_helper(code, "test_slti", 2);

  // 验证 x1 的值是否正确
  EXPECT_EQ(cpu.regs[1], 1) << "Error: x1 should be the result of SLTI instruction";
}

// Test sltiu instruction
TEST(RVTests, TestSltiu) {
  std::string code = start +
      "addi x2, x0, 8 \n"      // 将 8 加载到 x2 中
      "sltiu x1, x2, 10 \n";   // x1 = (x2 < 10) ? 1 : 0 (unsigned comparison)
  Cpu cpu = rv_helper(code, "test_sltiu", 2);

  // 验证 x1 的值是否正确
  EXPECT_EQ(cpu.regs[1], 1) << "Error: x1 should be the result of SLTIU instruction";
}

// Test sltiu instruction
TEST(RVTests, TestSltiu2) {
  std::string code = start +
      "addi x2, x0, 10 \n"      // 将 10 加载到 x2 中
      "sltiu x1, x2, 8 \n";     // x1 = (x2 < 8) ? 1 : 0 (unsigned comparison)
  Cpu cpu = rv_helper(code, "test_sltiu", 2);

  // 验证 x1 的值是否正确
  EXPECT_EQ(cpu.regs[1], 0) << "Error: x1 should be the result of SLTIU instruction";
}


// Test xori instruction
TEST(RVTests, TestXori) {
  std::string code = start +
      "addi x2, x0, 15 \n"      // Load 15 into x2
      "xori x1, x2, 7 \n";      // x1 = x2 ^ 7
  Cpu cpu = rv_helper(code, "test_xori", 2);

  // Verify if x1 has the correct value
  EXPECT_EQ(cpu.regs[1], 8) << "Error: x1 should be the result of XORI instruction";
}

// Test srli instruction
TEST(RVTests, TestSrli) {
  std::string code = start +
      "addi x2, x0, 32 \n"     // Load 32 into x2
      "srli x1, x2, 3 \n";     // x1 = x2 >> 3 (logical right shift)
  Cpu cpu = rv_helper(code, "test_srli", 2);

  // Verify if x1 has the correct value
  EXPECT_EQ(cpu.regs[1], 4) << "Error: x1 should be the result of SRLI instruction";
}

// Test srai instruction
TEST(RVTests, TestSrai) {
  std::string code = start +
      "addi x2, x0, -16 \n"    // Load -16 into x2
      "srai x1, x2, 3 \n";     // x1 = x2 >> 3 (arithmetic right shift)
  Cpu cpu = rv_helper(code, "test_srai", 2);

  int64_t result = static_cast<int64_t>(cpu.regs[1]);
  EXPECT_EQ(cpu.regs[2], -16) << "Error: x1 should be the result of XORI instruction";
  EXPECT_EQ(result, -2) << "Error: x1 should be the result of SRAI instruction";
}

// Test add instruction
TEST(RVTests, TestAdd) {
  std::string code = start +
      "addi x2, x0, 10 \n"   // 将 10 加载到 x2 中
      "addi x3, x0, 20 \n"   // 将 20 加载到 x3 中
      "add x1, x2, x3 \n";  // x1 = x2 + x3
  Cpu cpu = rv_helper(code, "test_add", 3);

  // 验证 x1 的值是否正确
  EXPECT_EQ(cpu.regs[1], 30) << "Error: x1 should be the result of ADD instruction";
}

// Test ori instruction
TEST(RVTests, TestOri) {
  std::string code = start +
      "addi x2, x0, 15 \n"      // Load 15 into x2
      "ori x1, x2, 7 \n";       // x1 = x2 | 7
  Cpu cpu = rv_helper(code, "test_ori", 2);

  // Verify if x1 has the correct value
  EXPECT_EQ(cpu.regs[1], 15 | 7) << "Error: x1 should be the result of ORI instruction";
}

// Test andi instruction
TEST(RVTests, TestAndi) {
  std::string code = start +
      "addi x2, x0, 15 \n"      // Load 15 into x2
      "andi x1, x2, 7 \n";      // x1 = x2 & 7
  Cpu cpu = rv_helper(code, "test_andi", 2);

  // Verify if x1 has the correct value
  EXPECT_EQ(cpu.regs[1], 15 & 7) << "Error: x1 should be the result of ANDI instruction";
}

TEST(RVTests, TestLui) {
  std::string code = start +
      "lui a0, 42 \n";      // Load 42 into a0
  Cpu cpu = rv_helper(code, "test_lui", 1);

  // Verify if x1 has the correct value
  EXPECT_EQ(cpu.getRegValueByName("a0"), 42 << 12) << "Error: a0 should be the result of LUI instruction";
}

TEST(RVTests, TestAUIPC) {
  std::string code = start +
      "auipc a0, 42 \n";      // Load 15 into x2
  Cpu cpu = rv_helper(code, "test_auipc", 1);

  EXPECT_EQ(cpu.getRegValueByName("a0"), DRAM_BASE + (42 << 12)) << "Error: a0 should be the result of AUIPC instruction";
}

TEST(RVTests, TestJAL) {
  std::string code = start +
    "jal a0, 42\n";
  Cpu cpu = rv_helper(code, "test_jal", 1);
  EXPECT_EQ(cpu.getRegValueByName("a0"), DRAM_BASE + 4)
      << "Error: a0 should be the result of JAL instruction";
  EXPECT_EQ(cpu.pc, DRAM_BASE + 42)
      << "Error: pc should be the target address after JAL instruction";
}

TEST(RVTests, TestJALR) {
  std::string code = start +
    "addi a1, zero, 42\n"
    "jalr a0, -8(a1)\n";
  Cpu cpu = rv_helper(code, "test_jalr", 2);

  EXPECT_EQ(cpu.getRegValueByName("a0"), DRAM_BASE + 8)
      << "Error: a0 should be the result of JALR instruction";
  EXPECT_EQ(cpu.pc, 34)
      << "Error: pc should be 34 after JALR instruction";
}

TEST(RVTests, TestFENCE) {
  std::string code = start +
      "fence\n";  // FENCE instruction

  Cpu cpu = rv_helper(code, "test_fence", 1);

  // 在这个模拟器中，FENCE指令不会修改寄存器或程序计数器(PC)，
  // 因此我们只需检查指令的执行是否完成且没有错误。
  EXPECT_EQ(cpu.pc, DRAM_BASE + 4)
  << "Error: FENCE instruction execution failed";
}

// TEST(RVTests, TestLb) {
//   std::string code = start +
//       "addi sp, sp, -16\n"  // 将栈指针减小128字节
//       "addi a1, zero, 42\n"  // 将 42 加载到 a1
//       "sb a1, 0(sp)\n"  // 在地址 (sp + 0) 处存储 a1 的值
//       "lb a0, 0(sp)\n";  // 从地址 (sp + 0) 加载一个字节到 a0
//   Cpu cpu = rv_helper(code, "test_lb", 4);
//
//   EXPECT_EQ(cpu.regs[10], 42)
//       << "Error: a0 should be 42 after LB instruction";
// }

// TEST(RVTests, TestBEQ2) {
//   std::string code = start +
//       "addi x1, x0, 10\n"
//       "addi x2, x0, 20\n"
//       "beq x1, x2, 42\n";
//   Cpu cpu = rv_helper(code, "test_beq", 3);
//   EXPECT_EQ(cpu.pc, DRAM_BASE + 42 + 4)
//       << "Error: pc should be the target address after BEQ instruction";
// }

// TEST(RVTests, TestBEQ) {
//   std::string code = start +
//       "beq x0, x0, 42\n";
//   Cpu cpu = rv_helper(code, "test_beq", 1);
//   EXPECT_EQ(cpu.pc, DRAM_BASE + 42)
//       << "Error: pc should be the target address after BEQ instruction";
// }

// TEST(RVTests, TestBNE) {
//       std::string code = start +
//         "addi x1, x0, 10\n"
//         "bne x0, x1, 42\n";
//
//       Cpu cpu = rv_helper(code, "test_bne", 2);
//
//       EXPECT_EQ(cpu.pc, DRAM_BASE + 42 + 4)
//           << "Error: pc should be the target address after BNE instruction";
// }


// TEST(RVTests, TestBLT) {
//       std::string code = start +
//       "addi x1, x0, 10\n"
//       "addi x2, x0, 20\n"
//       "blt x1, x2, 42\n";
//
//       Cpu cpu = rv_helper(code, "test_blt", 3);
//
//     EXPECT_EQ(cpu.pc, DRAM_BASE + 42 + 8)
//         << "Error: pc should be the target address after BLT instruction";
// }
//
// TEST(RVTests, TestBGE) {
//       std::string code = start +
//       "addi x1, x0, 10\n"
//       "addi x2, x0, 20\n"
//       "bge x2, x1, 42\n";
//
//       Cpu cpu = rv_helper(code, "test_bge", 3);
//
//     // Assuming DRAM_BASE is a defined constant
//     EXPECT_EQ(cpu.pc, DRAM_BASE + 42 + 8)
//         << "Error: pc should be the target address after BGE instruction";
// }
//
// TEST(RVTests, TestBLTU) {
//       std::string code = start +
//       "addi x1, x0, 10\n"
//       "addi x2, x0, 20\n"
//       "bltu x1, x2, 42\n";
//
//       Cpu cpu = rv_helper(code, "test_bltu", 3);
//       EXPECT_EQ(cpu.pc, DRAM_BASE + 42 + 8)
//           << "Error: pc should be the target address after BLTU instruction";
// }
//
// TEST(RVTests, TestBGEU) {
//       std::string code = start +
//       "addi x1, x0, 10\n"
//       "addi x2, x0, 20\n"
//       "bgeu x2, x1, 42\n";
//
//       Cpu cpu = rv_helper(code, "test_bgeu", 3);
//       EXPECT_EQ(cpu.pc, DRAM_BASE + 42 + 8)
//           << "Error: pc should be the target address after BGEU instruction";
// }

// TEST(RVTests, TestStoreLoad1) {
//       std::string code = start +
//       "addi s0, zero, 256\n"
//       "addi sp, sp, -16\n"
//       "sd s0, 8(sp)\n"
//       "lb t1, 8(sp)\n"
//       "lh t2, 8(sp)\n";
//
//       Cpu cpu = rv_helper(code, "test_load1", 5);
//       EXPECT_EQ(cpu.regs[9], 0)
//         << "Error: t1 should be 0 after LB instruction";
//       EXPECT_EQ(cpu.regs[10], 256)
//         << "Error: t2 should be 256 after LH instruction";
// }

// Test slt, slti, and sltiu instructions
TEST(RVTests, TestSlt) {
  std::string code = start +
      "addi a0, zero, 14 \n"    // Load 14 into a0
      "addi a1, zero, 24 \n"    // Load 24 into a1
      "slt  a2, a0, a1 \n"      // a2 = (a0 < a1) ? 1 : 0
      "slti a3, a0, 42 \n"      // a3 = (a0 < 42) ? 1 : 0
      "sltiu a4, a0, 84 \n";    // a4 = (a0 <u 84) ? 1 : 0

  Cpu cpu = rv_helper(code, "test_slt", 5);

  // Verify if a2, a3, and a4 have the correct values
  EXPECT_EQ(cpu.getRegValueByName("a2"), 1) << "Error: a2 should be 1";
  EXPECT_EQ(cpu.getRegValueByName("a3"), 1) << "Error: a3 should be 1";
  EXPECT_EQ(cpu.getRegValueByName("a4"), 1) << "Error: a4 should be 1";
}

// Test xor, xori, and xor instructions
TEST(RVTests, TestXor) {
  std::string code = start +
      "addi a0, zero, 0b10 \n"  // Load 0b10 into a0
      "xori a1, a0, 0b01 \n"    // a1 = a0 XOR 0b01
      "xor  a2, a1, a1 \n";      // a2 = a1 XOR a1

  Cpu cpu = rv_helper(code, "test_xor", 3);

  // Verify if a1 and a2 have the correct values
  EXPECT_EQ(cpu.getRegValueByName("a1"), 3) << "Error: a1 should be 3";
  EXPECT_EQ(cpu.getRegValueByName("a2"), 0) << "Error: a2 should be 0";
}

// Test or, ori, and or instructions
TEST(RVTests, TestOr) {
  std::string code = start +
      "addi a0, zero, 0b10 \n"  // Load 0b10 into a0
      "ori  a1, a0, 0b01 \n"    // a1 = a0 OR 0b01
      "or   a2, a0, a0 \n";     // a2 = a0 OR a0

  Cpu cpu = rv_helper(code, "test_or", 3);

  // Verify if a1 and a2 have the correct values
  EXPECT_EQ(cpu.getRegValueByName("a1"), 0b11) << "Error: a1 should be 0b11";
  EXPECT_EQ(cpu.getRegValueByName("a2"), 0b10) << "Error: a2 should be 0b10";
}

// Test and, andi, and and instructions
TEST(RVTests, TestAnd) {
  std::string code = start +
      "addi a0, zero, 0b10 \n"  // Load 0b10 into a0
      "andi a1, a0, 0b11 \n"    // a1 = a0 AND 0b11
      "and  a2, a0, a1 \n";      // a2 = a0 AND a1

  Cpu cpu = rv_helper(code, "test_and", 3);

  // Verify if a1 and a2 have the correct values
  EXPECT_EQ(cpu.getRegValueByName("a1"), 0b10) << "Error: a1 should be 0b10";
  EXPECT_EQ(cpu.getRegValueByName("a2"), 0b10) << "Error: a2 should be 0b10";
}

// Test sll, slli instructions
TEST(RVTests, TestSll) {
  std::string code = start +
      "addi a0, zero, 1 \n"       // Load 1 into a0
      "addi a1, zero, 5 \n"       // Load 5 into a1
      "sll  a2, a0, a1 \n"        // a2 = a0 << a1
      "slli a3, a0, 5 \n"         // a3 = a0 << 5
      "addi s0, zero, 64 \n"      // Load 64 into s0
      "sll  a4, a0, s0 \n";       // a4 = a0 << s0

  Cpu cpu = rv_helper(code, "test_sll", 6);

  // Verify if a2, a3, and a4 have the correct values
  EXPECT_EQ(cpu.getRegValueByName("a2"), 1 << 5) << "Error: a2 should be 1 << 5";
  EXPECT_EQ(cpu.getRegValueByName("a3"), 1 << 5) << "Error: a3 should be 1 << 5";
  EXPECT_EQ(cpu.getRegValueByName("a4"), 1) << "Error: a4 should be 1";
}

// Test sra, srai, srli, srl instructions
TEST(RVTests, TestSraSrl) {
  std::string code = start +
      "addi a0, zero, -8; \n"      // Load -8 into a0
      "addi a1, zero, 1; \n"       // Load 1 into a1
      "sra  a2, a0, a1; \n"        // a2 = a0 >> a1
      "srai a3, a0, 2; \n"         // a3 = a0 >> 2
      "srli a4, a0, 2; \n"         // a4 = a0 >>> 2
      "srl  a5, a0, a1; \n";       // a5 = a0 >>> a1

  Cpu cpu = rv_helper(code, "test_sra_srl", 6);

  // Verify if a2, a3, a4, and a5 have the correct values
  EXPECT_EQ(cpu.getRegValueByName("a2"), static_cast<uint64_t>(-4)) << "Error: a2 should be -4 as i64 as u64";
  EXPECT_EQ(cpu.getRegValueByName("a3"), static_cast<uint64_t>(-2)) << "Error: a3 should be -2 as i64 as u64";
  EXPECT_EQ(cpu.getRegValueByName("a4"), static_cast<uint64_t>(-8) >> 2) << "Error: a4 should be -8 as i64 as u64 >> 2";
  EXPECT_EQ(cpu.getRegValueByName("a5"), static_cast<uint64_t>(-8) >> 1) << "Error: a5 should be -8 as i64 as u64 >> 1";
}

// Test addw instruction
TEST(RVTests, TestWordOp) {
  std::string code = start +
      "addi a0, zero, 42 \n"      // Load 42 into a0
      "lui  a1, 0x7f000 \n"      // Load 0x7f000 into a1
      "addw a2, a0, a1 \n";      // a2 = a0 + a1 (word operation)

  Cpu cpu = rv_helper(code, "test_word_op", 3);

  // Verify if a2 has the correct value
  EXPECT_EQ(cpu.getRegValueByName("a2"), 0x7f00002a) << "Error: a2 should be 0x7f00002a";
}

// TEST(RVTests, TestSimple) {
//   std::string code = start +
//       "addi sp, sp, -16; \n"   // 将栈指针减小16字节
//       "sd   s0, 8(sp); \n"     // 在地址(sp + 8)处存储寄存器s0的值
//       "addi s0, sp, 16; \n"    // 将(sp + 16)的地址加载到寄存器s0
//       "li   a5, 42; \n"        // 立即数42加载到寄存器a5
//       "mv   a0, a5; \n"        // 将寄存器a5的值移动到寄存器a0
//       "ld   s0, 8(sp); \n"     // 从地址(sp + 8)处加载值到寄存器s0
//       "addi sp, sp, 16; \n"    // 将栈指针增加16字节
//       "jr   ra; \n";           // 跳转到寄存器ra中的返回地址
//
//   Cpu cpu = rv_helper(code, "test_simple", 8);
//
//   // 验证a0是否具有正确的值
//   EXPECT_EQ(cpu.regs[10], 42) << "错误：a0应为42";
// }

}