/*
 * lftp and utils
 *
 * Copyright (c) 1996-1997 by Alexander V. Lukyanov (lav@yars.free.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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

#ifndef LSCACHE_H
#define LSCACHE_H

#include <time.h>
#include "Cache.h"

class Buffer;
class FileAccess;

class LsCacheEntryLoc
{
   friend class LsCache;
   xstring_c arg;
   FileAccess *loc;
   int	 mode;

public:
   bool Matches(const FileAccess *p_loc,const char *a,int m);
   LsCacheEntryLoc(const FileAccess *p_loc,const char *a,int m);
   ~LsCacheEntryLoc();
   int EstimateSize() const { return xstrlen(arg)+(arg!=0); }
   const char *GetClosure() const;
};
class LsCacheEntryData
{
   int	 err_code;
   xstring data;
   FileSet *afset;    // associated file set
public:
   LsCacheEntryData(int e,const char *d,int l,const FileSet *fs);
   ~LsCacheEntryData();
   void SetData(int e,const char *d,int l,const FileSet *fs);
   void GetData(int *e,const char **d,int *l,const FileSet **fs);
   const FileSet *GetFileSet(const FileAccess *parser);
   int EstimateSize() const { return data.length()+(afset?afset->EstimateMemory():0); }
};

class LsCacheEntry : public CacheEntry, public LsCacheEntryLoc, public LsCacheEntryData
{
public:
   int EstimateSize() const;
   ~LsCacheEntry();
   LsCacheEntry(const FileAccess *p_loc,const char *a,int m,int e,const char *d,int l,const FileSet *fs);
};

class LsCache : public Cache
{
   LsCacheEntry *Find(const FileAccess *p_loc,const char *a,int m);
   LsCacheEntry *IterateFirst() { return (LsCacheEntry*)Cache::IterateFirst(); }
   LsCacheEntry *IterateNext()  { return (LsCacheEntry*)Cache::IterateNext(); }
   LsCacheEntry *IterateDelete(){ return (LsCacheEntry*)Cache::IterateDelete(); }
public:
   LsCache();
   void Add(const FileAccess *p_loc,const char *a,int m,int err,const char *d,int l,const FileSet *f=0);
   void Add(const FileAccess *p_loc,const char *a,int m,int err,const Buffer *ubuf,const FileSet *f=0);
   bool Find(const FileAccess *p_loc,const char *a,int m,int *err,const char **d, int *l,const FileSet **f=0);
   const FileSet *FindFileSet(const FileAccess *p_loc,const char *a,int m);

   int IsDirectory(const FileAccess *p_loc,const char *dir);
   void SetDirectory(const FileAccess *p_loc, const char *path, bool dir);

   enum change_mode { FILE_CHANGED, DIR_CHANGED, TREE_CHANGED };
   void Changed(change_mode m,const FileAccess *f,const char *what);
   void FileChanged(const FileAccess *f,const char *file)
      {
	 Changed(FILE_CHANGED,f,file);
      }
   void DirectoryChanged(const FileAccess *f,const char *dir)
      {
	 Changed(DIR_CHANGED,f,dir);
      }
   void TreeChanged(const FileAccess *f,const char *dir)
      {
	 Changed(TREE_CHANGED,f,dir);
      }

   void List();
};

#endif//LSCACHE_H
