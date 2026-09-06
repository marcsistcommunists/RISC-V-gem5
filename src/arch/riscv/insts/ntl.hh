/*
 * Copyright (c) 2024 RISC-V Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ARCH_RISCV_INST_NTL_HH__
#define __ARCH_RISCV_INST_NTL_HH__

#include "arch/riscv/insts/static_inst.hh"
#include "cpu/exec_context.hh"
#include "mem/request.hh"

namespace gem5
{

namespace RiscvISA
{

/**
 * Base class for Zihintntl (Non-Temporal Load/Store Hint) instructions.
 * These instructions provide hints to the hardware about non-temporal data
 * access patterns, suggesting that the data accessed will not be reused soon
 * and should be treated specially by the cache hierarchy.
 */
class NTLInst : public RiscvStaticInst
{
  protected:
    int64_t offset;
    Request::Flags memAccessFlags;

    NTLInst(const char *mnem, ExtMachInst _machInst, OpClass __opClass)
        : RiscvStaticInst(mnem, _machInst, __opClass), offset(0)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

/**
 * NTL.P1 - Non-Temporal Load Hint with locality level 1
 * Suggests that the data at the address will be used once and then discarded.
 * Data should be cached with lowest priority or bypassed entirely.
 */
class NTL_P1 : public NTLInst
{
  public:
    NTL_P1(ExtMachInst _machInst)
        : NTLInst("ntl.p1", _machInst, No_OpClass)
    {}

    Fault execute(ExecContext *xc, trace::InstRecord *traceData) const override;
};

/**
 * NTL.PALL - Non-Temporal Load Hint for all caches
 * Suggests that the data should bypass all cache levels.
 */
class NTL_PALL : public NTLInst
{
  public:
    NTL_PALL(ExtMachInst _machInst)
        : NTLInst("ntl.pall", _machInst, No_OpClass)
    {}

    Fault execute(ExecContext *xc, trace::InstRecord *traceData) const override;
};

/**
 * NTL.S1 - Non-Temporal Store Hint with locality level 1
 * Suggests that the store data will not be reused and should be written
 * directly to memory with minimal cache pollution.
 */
class NTL_S1 : public NTLInst
{
  public:
    NTL_S1(ExtMachInst _machInst)
        : NTLInst("ntl.s1", _machInst, No_OpClass)
    {}

    Fault execute(ExecContext *xc, trace::InstRecord *traceData) const override;
};

/**
 * NTL.ALL - Non-Temporal Hint for all subsequent accesses
 * Sets a mode where subsequent memory accesses are treated as non-temporal
 * until changed by another hint.
 */
class NTL_ALL : public NTLInst
{
  public:
    NTL_ALL(ExtMachInst _machInst)
        : NTLInst("ntl.all", _machInst, No_OpClass)
    {}

    Fault execute(ExecContext *xc, trace::InstRecord *traceData) const override;
};

} // namespace RiscvISA
} // namespace gem5

#endif // __ARCH_RISCV_INST_NTL_HH__
