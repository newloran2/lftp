/*
 * lftp and utils
 *
 * Copyright (c) 2004-2008 by Alexander V. Lukyanov (lav@yars.free.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* $Id$ */

#ifndef STRINGSET_H
#define STRINGSET_H

#include "xarray.h"

class StringSet
{
   xarray_p<char> set;

   const StringSet &operator=(const StringSet &); // disable assignment

public:
   StringSet() {}
   StringSet(const char *const *s,int n) { Assign(s,n); }
   StringSet(const StringSet &o) { Assign(o.set.get(),o.set.count()); }
   StringSet(const char *s) { Assign(&s,1); }

   void Empty() { set.truncate(); }
   void Assign(const char *const *s,int n);
   void Assign(const char *s) { Assign(&s,1); }
   bool IsEqual(const char *const *s,int n) const;
   bool IsEqual(const StringSet &o) const { return IsEqual(o.Set(),o.Count()); }
   void Append(const char *);
   void AppendFormat(const char *,...) PRINTF_LIKE(2,3);
   void InsertBefore(int,const char *);
   void Replace(int,const char *);
   char *Pop(int i=0);
   void Remove(int i) { xfree(Pop(i)); }

   const char *const *Set() const { return set.get(); }
   char **SetNonConst() { return set.get_non_const(); }
   int Count() const { return set.count(); }
   const char *String(int i) const { return i>=0 && i<Count() ? set[i] : 0; }
   const char *LastString() const { return String(Count()-1); }
   const char *operator[](int i) const { return String(i); }

   void MoveHere(StringSet &o);

   char **borrow() { return set.borrow(); }
};

#endif // STRINGSET_H
