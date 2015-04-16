/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2015 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
//
// @ORIGINAL_AUTHOR: Artur Klauser
// @EXTENDED: Rodric Rabbah (rodric@gmail.com) 
//

/*! @file
 *  This file contains an ISA-portable cache simulator
 *  data cache hierarchies
 */


#include "pin.H"

#include <iostream>
#include <fstream>

#include "dcache.H"
#include "pin_profile.H"

std::ofstream outFile;
std::ofstream traceFile;

/* ===================================================================== */
/* Commandline Switches */
/* ===================================================================== */

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE,    "pintool",
    "o", "mem_trace.out", "specify mem trace file name");
KNOB<string> KnobTraceFile(KNOB_MODE_WRITEONCE,    "pintool",
    "of", "dcache_trace.out", "specify dcache trace file name");
KNOB<BOOL>   KnobTrackLoads(KNOB_MODE_WRITEONCE,    "pintool",
    "tl", "0", "track individual loads -- increases profiling time");
KNOB<BOOL>   KnobTrackStores(KNOB_MODE_WRITEONCE,   "pintool",
   "ts", "0", "track individual stores -- increases profiling time");
KNOB<UINT32> KnobThresholdHit(KNOB_MODE_WRITEONCE , "pintool",
   "rh", "100", "only report memops with hit count above threshold");
KNOB<UINT32> KnobThresholdMiss(KNOB_MODE_WRITEONCE, "pintool",
   "rm","100", "only report memops with miss count above threshold");
KNOB<UINT32> KnobCacheSize(KNOB_MODE_WRITEONCE, "pintool",
    "l1s","32", "L1 cache size in kilobytes");
KNOB<UINT32> KnobLineSize(KNOB_MODE_WRITEONCE, "pintool",
    "l1b","32", "L1 cache block size in bytes");
KNOB<UINT32> KnobAssociativity(KNOB_MODE_WRITEONCE, "pintool",
    "l1a","4", "L1 cache associativity (1 for direct mapped)");
KNOB<UINT32> KnobL2CacheSize(KNOB_MODE_WRITEONCE, "pintool",
    "l2s","2048", "L2 cache size in kilobytes");
KNOB<UINT32> KnobL2LineSize(KNOB_MODE_WRITEONCE, "pintool",
    "l2b","64", "L2 cache block size in bytes");
KNOB<UINT32> KnobL2Associativity(KNOB_MODE_WRITEONCE, "pintool",
    "l2a","16", "L2 cache associativity (1 for direct mapped)");

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr <<
        "This tool represents a cache simulator.\n"
        "\n";

    cerr << KNOB_BASE::StringKnobSummary() << endl; 
    return -1;
}

/* ===================================================================== */
/* Global Variables */
/* ===================================================================== */

// wrap configuation constants into their own name space to avoid name clashes
namespace DL1
{
    const UINT32 max_sets = KILO; // cacheSize / (lineSize * associativity);
    const UINT32 max_associativity = 256; // associativity;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}

DL1::CACHE* dl1 = NULL;

typedef enum
{
    COUNTER_MISS = 0,
    COUNTER_HIT = 1,
    COUNTER_NUM
} COUNTER;

namespace L2
{
    const UINT32 max_sets = KILO*KILO; // cacheSize / (lineSize * associativity);
    const UINT32 max_associativity = 256; // associativity;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}

L2::CACHE* l2 = NULL;


typedef  COUNTER_ARRAY<UINT64, COUNTER_NUM> COUNTER_HIT_MISS;


// holds the counters with misses and hits
// conceptually this is an array indexed by instruction address
COMPRESSOR_COUNTER<ADDRINT, UINT32, COUNTER_HIT_MISS> profile;
COMPRESSOR_COUNTER<ADDRINT, UINT32, COUNTER_HIT_MISS> profile_l2;

/* ===================================================================== */

VOID LoadMulti(ADDRINT addr, UINT32 size, UINT32 instId)
{
    // first level D-cache
    const BOOL dl1Hit = dl1->Access(addr, size, CACHE_BASE::ACCESS_TYPE_LOAD);

    const COUNTER counter = dl1Hit ? COUNTER_HIT : COUNTER_MISS;
    profile[instId][counter]++;

    outFile << addr << ",";
    if(!dl1Hit)
    {
        const BOOL l2Hit = l2->Access(addr, size, CACHE_BASE::ACCESS_TYPE_LOAD);
        const COUNTER counter = l2Hit ? COUNTER_HIT : COUNTER_MISS;
       
        profile_l2[instId][counter]++;
        if(!l2Hit)
        {
            traceFile << addr << endl;
            outFile << endl;
        }
    }
}

/* ===================================================================== */

VOID StoreMulti(ADDRINT addr, UINT32 size, UINT32 instId)
{
    // first level D-cache
    const BOOL dl1Hit = dl1->Access(addr, size, CACHE_BASE::ACCESS_TYPE_STORE);

    const COUNTER counter = dl1Hit ? COUNTER_HIT : COUNTER_MISS;
    profile[instId][counter]++;

    outFile << addr << ",";
    if(!dl1Hit)
    {
        const BOOL l2Hit = l2->Access(addr, size, CACHE_BASE::ACCESS_TYPE_STORE);
        const COUNTER counter = l2Hit ? COUNTER_HIT : COUNTER_MISS;
        profile_l2[instId][counter]++;
        if(!l2Hit)
        {
            traceFile << addr << endl;
            outFile << endl;
        }
    }
}

/* ===================================================================== */

VOID LoadSingle(ADDRINT addr, UINT32 instId)
{
    // @todo we may access several cache lines for 
    // first level D-cache
    const BOOL dl1Hit = dl1->AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_LOAD);
    const COUNTER counter = dl1Hit ? COUNTER_HIT : COUNTER_MISS;
    profile[instId][counter]++;

    outFile << addr << ",";
    if(!dl1Hit)
    {
        const BOOL l2Hit = l2->AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_LOAD);
        const COUNTER counter = l2Hit ? COUNTER_HIT : COUNTER_MISS;
        profile_l2[instId][counter]++;
        if(!l2Hit)
        {
            traceFile << addr << endl;
            outFile << endl;
        }
    }
}
/* ===================================================================== */

VOID StoreSingle(ADDRINT addr, UINT32 instId)
{
    // @todo we may access several cache lines for 
    // first level D-cache
    const BOOL dl1Hit = dl1->AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_STORE);
    const COUNTER counter = dl1Hit ? COUNTER_HIT : COUNTER_MISS;
    profile[instId][counter]++;

    outFile << addr << ",";
    if(!dl1Hit)
    {
        const BOOL l2Hit = l2->AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_STORE);
        const COUNTER counter = l2Hit ? COUNTER_HIT : COUNTER_MISS;
        profile_l2[instId][counter]++;
        if(!l2Hit)
        {
            traceFile << addr << endl;
            outFile << endl;
        }
    }
}

/* ===================================================================== */

VOID LoadMultiFast(ADDRINT addr, UINT32 size)
{
    const BOOL dl1Hit = dl1->Access(addr, size, CACHE_BASE::ACCESS_TYPE_LOAD);
    outFile << addr << ",";
    if(!dl1Hit)
    {
        if(!l2->Access(addr, size, CACHE_BASE::ACCESS_TYPE_LOAD))
        {
            traceFile << addr << endl;
            outFile << endl;
        }
    }
}

/* ===================================================================== */

VOID StoreMultiFast(ADDRINT addr, UINT32 size)
{
    const BOOL dl1Hit = dl1->Access(addr, size, CACHE_BASE::ACCESS_TYPE_STORE);
    outFile << addr << ",";
    if(!dl1Hit)
    {
        if(!l2->Access(addr, size, CACHE_BASE::ACCESS_TYPE_STORE))
        {
            traceFile << addr << endl;
            outFile << endl;
        }
    }
}

/* ===================================================================== */

VOID LoadSingleFast(ADDRINT addr)
{
    const BOOL dl1Hit = dl1->AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_LOAD);    
    outFile << addr << ",";
    if(!dl1Hit)
    {
        if(!l2->AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_LOAD))
        {
            traceFile << addr << endl;
            outFile << endl;
        }
    }
}

/* ===================================================================== */

VOID StoreSingleFast(ADDRINT addr)
{
    const BOOL dl1Hit = dl1->AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_STORE);    
    outFile << addr << ",";
    if(!dl1Hit)
    {
        if(!l2->AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_STORE))
        {
            traceFile << addr << endl;
            outFile << endl;
        }
    }

}

/* ===================================================================== */

VOID Instruction(INS ins, void * v)
{
    if (INS_IsMemoryRead(ins) && INS_IsStandardMemop(ins))
    {
        // map sparse INS addresses to dense IDs
        const ADDRINT iaddr = INS_Address(ins);
        const UINT32 instId = profile.Map(iaddr);

        const UINT32 size = INS_MemoryReadSize(ins);
        const BOOL   single = (size <= 4);
                
        if( KnobTrackLoads )
        {
            if( single )
            {
                INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE, (AFUNPTR) LoadSingle,
                    IARG_MEMORYREAD_EA,
                    IARG_UINT32, instId,
                    IARG_END);
            }
            else
            {
                INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE,  (AFUNPTR) LoadMulti,
                    IARG_MEMORYREAD_EA,
                    IARG_MEMORYREAD_SIZE,
                    IARG_UINT32, instId,
                    IARG_END);
            }
                
        }
        else
        {
            if( single )
            {
                INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE,  (AFUNPTR) LoadSingleFast,
                    IARG_MEMORYREAD_EA,
                    IARG_END);
                        
            }
            else
            {
                INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE,  (AFUNPTR) LoadMultiFast,
                    IARG_MEMORYREAD_EA,
                    IARG_MEMORYREAD_SIZE,
                    IARG_END);
            }
        }
    }
        
    if ( INS_IsMemoryWrite(ins) && INS_IsStandardMemop(ins))
    {
        // map sparse INS addresses to dense IDs
        const ADDRINT iaddr = INS_Address(ins);
        const UINT32 instId = profile.Map(iaddr);
            
        const UINT32 size = INS_MemoryWriteSize(ins);

        const BOOL   single = (size <= 4);
                
        if( KnobTrackStores )
        {
            if( single )
            {
                INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE,  (AFUNPTR) StoreSingle,
                    IARG_MEMORYWRITE_EA,
                    IARG_UINT32, instId,
                    IARG_END);
            }
            else
            {
                INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE,  (AFUNPTR) StoreMulti,
                    IARG_MEMORYWRITE_EA,
                    IARG_MEMORYWRITE_SIZE,
                    IARG_UINT32, instId,
                    IARG_END);
            }
                
        }
        else
        {
            if( single )
            {
                INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE,  (AFUNPTR) StoreSingleFast,
                    IARG_MEMORYWRITE_EA,
                    IARG_END);
                        
            }
            else
            {
                INS_InsertPredicatedCall(
                    ins, IPOINT_BEFORE,  (AFUNPTR) StoreMultiFast,
                    IARG_MEMORYWRITE_EA,
                    IARG_MEMORYWRITE_SIZE,
                    IARG_END);
            }
        }
            
    }
}

/* ===================================================================== */

VOID Fini(int code, VOID * v)
{
    // print D-cache profile
    // @todo what does this print
    
    /*outFile << "PIN:MEMLATENCIES 1.0. 0x0\n";
            
    outFile <<
        "#\n"
        "# DCACHE stats\n"
        "#\n";
    
    outFile << dl1->StatsLong("# ", CACHE_BASE::CACHE_TYPE_DCACHE);
    outFile << l2->StatsLong("# ", CACHE_BASE::CACHE_TYPE_NUM);

    if( KnobTrackLoads || KnobTrackStores ) {
        outFile <<
            "#\n"
            "# LOAD stats\n"
            "#\n";
        
        outFile << profile.StringLong();
    }*/
    outFile.close();
    traceFile.close();
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char *argv[])
{
    PIN_InitSymbols();

    if( PIN_Init(argc,argv) )
    {
        return Usage();
    }

    outFile.open(KnobOutputFile.Value().c_str());
    traceFile.open(KnobTraceFile.Value().c_str());

    dl1 = new DL1::CACHE("L1 Data Cache", 
                         KnobCacheSize.Value() * KILO,
                         KnobLineSize.Value(),
                         KnobAssociativity.Value());
    
    l2 = new L2::CACHE("L2 Data Cache", 
                         KnobL2CacheSize.Value() * KILO,
                         KnobL2LineSize.Value(),
                         KnobL2Associativity.Value());

    profile.SetKeyName("iaddr          ");
    profile_l2.SetKeyName("iaddr          ");

    profile.SetCounterName("dcache:miss        dcache:hit");
    profile_l2.SetCounterName("l2cache:miss        l2cache:hit");

    COUNTER_HIT_MISS threshold;
    COUNTER_HIT_MISS threshold_l2;

    threshold[COUNTER_HIT] = KnobThresholdHit.Value();
    threshold[COUNTER_MISS] = KnobThresholdMiss.Value();
    
    profile.SetThreshold( threshold );

    threshold_l2[COUNTER_HIT] = KnobThresholdHit.Value();
    threshold_l2[COUNTER_MISS] = KnobThresholdMiss.Value();
    
    profile_l2.SetThreshold( threshold_l2 );
    
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);

    // Never returns

    PIN_StartProgram();
    
    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */

