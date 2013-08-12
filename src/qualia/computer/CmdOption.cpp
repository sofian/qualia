// Copyright (C) 2003--2004 Ronan Collobert (collober@idiap.ch)
//                
// This file is part of Torch 3.1.
//
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "CmdOption.h"
#include <cstring>

CmdOption::CmdOption(const char *name_, const char *type_name_, const char *help_, bool save_)
{
  name = (char *)Alloc::malloc(strlen(name_)+1);
  strcpy(name, name_);
  type_name = (char *)Alloc::malloc(strlen(type_name_)+1);
  strcpy(type_name, type_name_);
  help = (char *)Alloc::malloc(strlen(help_)+1);
  strcpy(help, help_);
  needsSave = save_;
  is_setted = false;

  is_option = true;
  is_argument = false;
  is_text = false;
  is_master_switch = false;
}

bool CmdOption::isOption(bool set_)
{
  if(set_)
  {
    is_option = true;
    is_argument = false;
    is_text = false;
    is_master_switch = false;    
  }
  return is_option;
}

bool CmdOption::isArgument(bool set_)
{
  if(set_)
  {
    is_option = false;
    is_argument = true;
    is_text = false;
    is_master_switch = false;    
  }
  return is_argument;
}

bool CmdOption::isText(bool set_)
{
  if(set_)
  {
    is_option = false;
    is_argument = false;
    is_text = true;
    is_master_switch = false;    
  }
  return is_text;
}

bool CmdOption::isMasterSwitch(bool set_)
{
  if(set_)
  {
    is_option = false;
    is_argument = false;
    is_text = false;
    is_master_switch = true;
  }
  return is_master_switch;
}

void CmdOption::initValue()
{
}

void CmdOption::printValue(DiskXFile *file_)
{
}

void CmdOption::read(int *argc_, char ***argv_)
{
}

bool CmdOption::isCurrent(int *argc_, char ***argv_)
{
  if(!is_option && !is_master_switch)
    return false;

  if(strcmp((*argv_)[0], name))
    return false;
  else
  {
    (*argc_)--;
    (*argv_)++;
    return true;
  }
}

CmdOption::~CmdOption()
{
  Alloc::free(name);
  Alloc::free(type_name);
  Alloc::free(help);
}

//-------------------------- int

IntCmdOption::IntCmdOption(const char *name_, int *ptr_, int init_value_, const char *help_, bool save_)
  : CmdOption(name_, "<int>", help_, save_)
{
  ptr = ptr_;
  init_value = init_value_;
}

void IntCmdOption::initValue()
{
  *ptr = init_value;
}

void IntCmdOption::printValue(DiskXFile *file_)
{
  if(is_setted)
    file_->printf("[%d]", *ptr);
  else
    file_->printf("[%d]", init_value);
}


void IntCmdOption::read(int *argc_, char ***argv_)
{
  char **argv = *argv_;
  char *maryline;

  if(*argc_ == 0)
    Q_ERROR("IntCmdOption: cannot correctly set <%s>", name);

  *ptr = strtol(argv[0], &maryline, 10);
  if( *maryline != '\0' )
    Q_ERROR("IntCmdOption: <%s> requires an integer", name);

  (*argc_)--;
  (*argv_)++;
}

void IntCmdOption::load(DiskXFile *file)
{
  file->taggedRead(ptr, sizeof(int), 1, name);
}

void IntCmdOption::save(DiskXFile *file)
{
  file->taggedWrite(ptr, sizeof(int), 1, name);
}

IntCmdOption::~IntCmdOption()
{
}


//-------------------------- real

RealCmdOption::RealCmdOption(const char *name_, real *ptr_, real init_value_, const char *help_, bool save_)
  : CmdOption(name_, "<real>", help_, save_)
{
  ptr = ptr_;
  init_value = init_value_;
}

void RealCmdOption::initValue()
{
  *ptr = init_value;
}

void RealCmdOption::printValue(DiskXFile *file_)
{
  if(is_setted)
    file_->printf("[%g]", *ptr);
  else
    file_->printf("[%g]", init_value);
}


void RealCmdOption::read(int *argc_, char ***argv_)
{
  char **argv = *argv_;
  char *maryline;

  if(*argc_ == 0)
    Q_ERROR("RealCmdOption: cannot correctly set <%s>", name);

  *ptr = strtod(argv[0], &maryline);
  if( *maryline != '\0' )
    Q_ERROR("RealCmdOption: <%s> requires a real", name);

  (*argc_)--;
  (*argv_)++;
}

void RealCmdOption::load(DiskXFile *file)
{
  file->taggedRead(ptr, sizeof(real), 1, name);
}

void RealCmdOption::save(DiskXFile *file)
{
  file->taggedWrite(ptr, sizeof(real), 1, name);
}

RealCmdOption::~RealCmdOption()
{
}

//-------------------------- switch

BoolCmdOption::BoolCmdOption(const char *name_, bool *ptr_, bool init_value_, const char *help_, bool save_)
  : CmdOption(name_, "", help_, save_)
{
  ptr = ptr_;
  init_value = init_value_;
}

void BoolCmdOption::initValue()
{
  *ptr = init_value;
}

void BoolCmdOption::read(int *argc_, char ***argv_)
{
  *ptr = !(*ptr);
}

void BoolCmdOption::load(DiskXFile *file)
{
  int melanie;
  file->taggedRead(&melanie, sizeof(int), 1, name);
  *ptr = (melanie ? 1 : 0);
}

void BoolCmdOption::save(DiskXFile *file)
{
  int melanie = (*ptr ? 1 : 0);
  file->taggedWrite(&melanie, sizeof(int), 1, name);
}

BoolCmdOption::~BoolCmdOption()
{
}

//-------------------------- string

StringCmdOption::StringCmdOption(const char *name_, char **ptr_, const char *init_value_, const char *help_, bool save_)
  : CmdOption(name_, "<string>", help_, save_)
{
  ptr = ptr_;  
  init_value = (char *)Alloc::malloc(strlen(init_value_)+1);
  strcpy(init_value, init_value_);
}

void StringCmdOption::initValue()
{
  *ptr = (char *)Alloc::malloc(strlen(init_value)+1);
  strcpy(*ptr, init_value);
}

void StringCmdOption::printValue(DiskXFile *file_)
{
  if(is_setted)
    file_->printf("[%s]", *ptr);
  else
    file_->printf("[%s]", init_value);
}

void StringCmdOption::read(int *argc_, char ***argv_)
{
  char **argv = *argv_;

  if(*argc_ == 0)
    Q_ERROR("StringCmdOption: cannot correctly set <%s>", name);

  Alloc::free(*ptr);
  *ptr = (char *)Alloc::malloc(strlen(argv[0])+1);
  strcpy(*ptr, argv[0]);

  (*argc_)--;
  (*argv_)++;
}

void StringCmdOption::load(DiskXFile *file)
{
  int melanie;
  file->taggedRead(&melanie, sizeof(int), 1, "SIZE");
  *ptr = (char *)Alloc::malloc(melanie);
  file->taggedRead(*ptr, 1, melanie, name);
}

void StringCmdOption::save(DiskXFile *file)
{
  int melanie = strlen(*ptr)+1;
  file->taggedWrite(&melanie, sizeof(int), 1, "SIZE");
  file->taggedWrite(*ptr, 1, melanie, name);
}

StringCmdOption::~StringCmdOption()
{
  //Alloc::free(ptr);
  Alloc::free(init_value);
}
