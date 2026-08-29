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

#include "arch/riscv/insts/ntl.hh"

#include <sstream>
#include <string>

#include "arch/riscv/regs/int.hh"
#include "cpu/exec_context.hh"
#include "mem/request.hh"

namespace gem5
{

namespace RiscvISA
{

std::string
NTLInst::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const
{
    std::stringstream ss;
    ss << mnemonic;
    return ss.str();
}

Fault
NTL_P1::execute(ExecContext *xc, Trace::InstRecord *traceData) const
{
    // NTL.P1 - Non-Temporal Load Hint with locality level 1
    // This is a hint instruction that suggests the next load should be
    // treated as non-temporal with lowest cache priority.
    
    DPRINTF(RiscvNtl, "Executing NTL.P1 at PC=%#x\n", xc->pcState().pc());
    
    // Set non-temporal flag for subsequent memory access
    // This will be picked up by load instructions
    xc->setMiscReg(MISCREG_NTL_HINT, 1); // Locality level 1
    
    return NoFault;
}

Fault
NTL_PALL::execute(ExecContext *xc, Trace::InstRecord *traceData) const
{
    // NTL.PALL - Non-Temporal Load Hint for all caches
    // Suggests that data should bypass all cache levels
    
    DPRINTF(RiscvNtl, "Executing NTL.PALL at PC=%#x\n", xc->pcState().pc());
    
    // Set non-temporal flag for all cache levels
    xc->setMiscReg(MISCREG_NTL_HINT, 2); // All caches
    
    return NoFault;
}

Fault
NTL_S1::execute(ExecContext *xc, Trace::InstRecord *traceData) const
{
    // NTL.S1 - Non-Temporal Store Hint with locality level 1
    // Suggests that store data will not be reused and should bypass cache
    
    DPRINTF(RiscvNtl, "Executing NTL.S1 at PC=%#x\n", xc->pcState().pc());
    
    // Set non-temporal flag for subsequent store
    xc->setMiscReg(MISCREG_NTL_HINT, 3); // Store hint level 1
    
    return NoFault;
}

Fault
NTL_ALL::execute(ExecContext *xc, Trace::InstRecord *traceData) const
{
    // NTL.ALL - Non-Temporal Hint for all subsequent accesses
    // Sets a persistent mode where all subsequent memory accesses 
    // are treated as non-temporal until changed
    
    DPRINTF(RiscvNtl, "Executing NTL.ALL at PC=%#x\n", xc->pcState().pc());
    
    // Set persistent non-temporal mode
    xc->setMiscReg(MISCREG_NTL_HINT, 4); // Persistent mode
    
    return NoFault;
}

} // namespace RiscvISA
} // namespace gem5
