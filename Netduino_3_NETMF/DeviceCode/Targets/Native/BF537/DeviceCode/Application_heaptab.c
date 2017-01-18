/* MANAGED-BY-SYSTEM-BUILDER                                    */

/*
** User heap source file generated on Mar 26, 2007 at 16:45:09.
**
** Copyright (C) 2000-2006 Analog Devices Inc., All Rights Reserved.
**
** This file is generated automatically based upon the options selected
** in the LDF Wizard. Changes to the LDF configuration should be made by 
** changing the appropriate options rather than editing this file. 
**
** Configuration:-
**     crt_doj:                                .\Debug\Application_basiccrt.doj
**     processor:                              ADSP-BF537
**     si_revision:                            automatic
**     using_cplusplus:                        true
**     mem_init:                               false
**     use_vdk:                                false
**     use_eh:                                 true
**     use_argv:                               false
**     running_from_internal_memory:           true
**     user_heap_src_file:                     D:\sporoot\current\client_v3_0\OEM\ADI\VisualDSP_Files\Application\BlackfinClient\Application_heaptab.c
**     libraries_use_stdlib:                   true
**     libraries_use_fileio_libs:              false
**     libraries_use_ieeefp_emulation_libs:    false
**     libraries_use_eh_enabled_libs:          false
**     system_heap:                            L1
**     system_heap_min_size:                   2K
**     system_stack:                           L1
**     system_stack_min_size:                  2K
**     use_sdram:                              true
**     use_sdram_size:                         64M
**     use_sdram_partitioned:                  default
**
*/


extern "asm" int ldf_heap_space;
extern "asm" int ldf_heap_length;


struct heap_table_t
{
  void          *base;
  unsigned long  length;
  long int       userid;
};

#pragma file_attr("libData=HeapTable")
#pragma section("constdata")
struct heap_table_t heap_table[2] = 
{


  { &ldf_heap_space, (int) &ldf_heap_length, 0 },


  { 0, 0, 0 }
};

