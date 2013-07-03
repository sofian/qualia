/*
 * XFile.cpp
 *
 * This file is part of Qualia https://github.com/sofian/qualia
 *
 * The code is an adaptation of code from Torch 3.1.
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
 * (c) 2003--2004 Ronan Collobert (collober@idiap.ch)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "XFile.h"

XFile::XFile()
{
}

int XFile::taggedRead(void *ptr, int block_size, int n_blocks, const char *tag)
{
  int tag_size;
  this->read(&tag_size, sizeof(int), 1);
  if(tag_size != (int)strlen(tag))
  {
    Q_ERROR("XFile: sorry, the tag <%s> cannot be read!", tag);
    return (-1);
  }

  for (unsigned int i=0; i<strlen(tag); i++)
  {
    char c;
    this->read(&c, sizeof(char), 1);
    if (c != tag[i])
    {
      Q_ERROR("XFile: tag <%s> not found!", tag);
      return (-1);
    }
  }
//#if is_computer()
//  char *tag_ = (char *)Alloc::malloc(tag_size+1);
//  tag_[tag_size] = '\0';
//  this->read(tag_, 1, tag_size);
//
//  ASSERT_ERROR_MESSAGE(strcmp(tag, tag_) == 0, "XFile: tag <%s> not found!", tag);
//  Alloc::free(tag_);
//#endif

  int block_size_;
  int n_blocks_;
  this->read(&block_size_, sizeof(int), 1);
  this->read(&n_blocks_, sizeof(int), 1);

  if (block_size_ != block_size || n_blocks_ != n_blocks)
  {
    Q_ERROR("XFile: tag <%s> has a corrupted size!", tag);
    return (-1);
  }

  return this->read(ptr, block_size, n_blocks);
}

int XFile::taggedWrite(const void *ptr, int block_size, int n_blocks, const char *tag){
  int tag_size = strlen(tag);
  this->write(&tag_size, sizeof(int), 1);
  this->write((char *)tag, 1, tag_size);
  this->write(&block_size, sizeof(int), 1);
  this->write(&n_blocks, sizeof(int), 1);
  return this->write(ptr, block_size, n_blocks);
}

XFile::~XFile()
{
}

long XFile::size() {
  long pos = tell();   // remember position
  seek(0L, SEEK_END);  // go to the end
  long sz = tell();    // get end position (ie. size)
  seek(pos, SEEK_SET); // Go back to position.
  return sz;
}
