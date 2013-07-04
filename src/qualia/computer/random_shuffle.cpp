/*
 * random_shuffle.cpp
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
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

#include "random_shuffle.h"

void randomShuffledIndices(int *indices, int nIndices)
{
  for(int i = 0; i < nIndices; i++)
    indices[i] = i;

  randomShuffle(indices, nIndices);
}

void randomShuffle(int* tabular, int nElems)
{
  if (nElems > (int) (RANDOM_MAX / 10000UL)) {
    randomShuffle(tabular, sizeof(int), nElems);
  }
  // Code that requires no allocation. Code taken from:
  // http://stackoverflow.com/questions/6127503/shuffle-array-in-c
  else if (nElems > 1)
  {
      int i;
      for (i = 0; i < nElems - 1; i++)
      {
        int j = i + ::random() / (RANDOM_MAX / (nElems - i) + 1);
        int t = tabular[j];
        tabular[j] = tabular[i];
        tabular[i] = t;
      }
  }
}

void randomShuffle(void *tabular, int sizeElem, int nElems)
{
  void *save = Alloc::malloc(sizeElem);
  char *tab = (char *)tabular;

  for(int i = 0; i < nElems-1; i++)
  {
    int z = random(nElems-i);
    memcpy(save, tab+i*sizeElem, sizeElem);
    memmove(tab+i*sizeElem, tab+(z+i)*sizeElem, sizeElem);
    memcpy(tab+(z+i)*sizeElem, save, sizeElem);
  }
  Alloc::free(save);
}





