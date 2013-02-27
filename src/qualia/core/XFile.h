/*
 * XFile.h
 *
 * Abstract interface for files.
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
#ifndef X_FILE_INC
#define X_FILE_INC

#include <qualia/core/common.h>
#include <string.h>

#ifndef SEEK_SET // from stdio.h
#define SEEK_SET 0x0
#define SEEK_CUR 0x1
#define SEEK_END 0x2
#endif

/** XFile. A File which could be anything.
    The syntax (and results) for method is very similar
    to C FILE. (Except for the FILE pointer which is not
    given in the parameters!).

    @author Ronan Collobert (collober@idiap.ch)
*/
class XFile
{
  public:

    ///
    XFile();

    /// Read something.
    /// Returns the number of blocks read or a value < 0 if there was an error.
    virtual int read(void *ptr, int block_size, int n_blocks) = 0;

    /// Write.
    /// Returns the number of blocks written or a value < 0 if there was an error.
    virtual int write(const void *ptr, int block_size, int n_blocks) = 0;

    /** Read and check the tag/the size. To be used with #taggedWrite()#.
        If the tag and the size readed doesn't correspond to the given
        tag and size, an error will occur and the returned value will be (-1).
    */
    int taggedRead(void *ptr, int block_size, int n_blocks, const char *tag);

    /// Write and write the tag/the size.
    int taggedWrite(const void *ptr, int block_size, int n_blocks, const char *tag);

    /// Are we at the end ?
    virtual int eof() = 0;

    /// Flush the file.
    virtual int flush() = 0;

    /// Seek.
    virtual int seek(long offset, int whence) = 0;

    /// Tell me where am I...
    virtual long tell() = 0;
    
    /// Rewind.
    virtual void rewind() = 0;

    /// Print some text.
    //virtual int printf(const char *format, ...) = 0;

    /// Scan some text.
    //virtual int scanf(const char *format, void *ptr) = 0;

    /// Get one line (read at most #size_# characters).
    virtual char *gets(char *dest, int size_) = 0;

    virtual long size();

    //-----

    virtual ~XFile();
};

#endif
