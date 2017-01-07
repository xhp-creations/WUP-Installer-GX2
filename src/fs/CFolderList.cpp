/****************************************************************************
 * Copyright (C) 2011
 * by Dj_Skual
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 * CFolderList.cpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#include "CFolderList.hpp"
#include "DirList.h"
#include "common/retain_vars.h"
#include "dynamic_libs/os_functions.h"
#include <stdio.h>

void CFolderList::AddFolder()
{
	FolderStruct * newFolder = new FolderStruct;
	newFolder->name = "";
	newFolder->path = "";
	newFolder->selected = false;
	
	Folders.push_back(newFolder);
}

std::string CFolderList::GetName(int ind)
{
	if(ind < 0 || ind >= (int) Folders.size())
		return "";

	return Folders.at(ind)->name;
}

std::string CFolderList::GetPath(int ind)
{
	if(ind < 0 || ind >= (int) Folders.size())
		return "";

	return Folders.at(ind)->path;
}

bool CFolderList::IsSelected(int ind)
{
	if(ind < 0 || ind >= (int) Folders.size())
		return false;

	return Folders.at(ind)->selected;
}

void CFolderList::Select(int ind)
{
	if(ind < 0 || ind >= (int) Folders.size())
		return;

	Folders.at(ind)->selected = true;
}

void CFolderList::UnSelect(int ind)
{
	if(ind < 0 || ind >= (int) Folders.size())
		return;

	Folders.at(ind)->selected = false;
}

void CFolderList::SelectAll()
{
	if(!Folders.size())
		return;
	
	for(u32 i = 0; i < Folders.size(); i++)
		Folders.at(i)->selected = true;
}

void CFolderList::UnSelectAll()
{
	if(!Folders.size())
		return;
	
	for(u32 i = 0; i < Folders.size(); i++)
		Folders.at(i)->selected = false;
}

int CFolderList::GetFirstSelected()
{
	if(!Folders.size())
		return -1;
	
	int found = -1;
	for(u32 i = 0; i < Folders.size() && found < 0; i++)
	{
		if(Folders.at(i)->selected)
			found = i;
	}
	
	return found;
}

void CFolderList::Click(int ind)
{
	if(ind < 0 || ind >= (int) Folders.size())
		return;

	Folders.at(ind)->selected = !Folders.at(ind)->selected;
}

void CFolderList::Reset()
{
	Folders.clear();
}

int CFolderList::GetSelectedCount()
{
	int selectedCount = 0;
	
	for(u32 i = 0; i < Folders.size(); i++)
	{
		if(Folders.at(i)->selected)
			selectedCount++;
	}
	
	return selectedCount;
}

int CFolderList::Get()
{
	Reset();
	
	DirList dir("fs:/vol/external01/install", NULL, DirList::Dirs);
	
	int cnt = dir.GetFilecount();
	if(cnt > 0)
	{
		for(int i = 0; i < cnt; i++)
		{
			AddFolder();
			Folders.at(i)->name = dir.GetFilename(i);
			Folders.at(i)->path = dir.GetFilepath(i);
			Folders.at(i)->selected = false;
		}
	}
	else
	{
		dir.LoadPath("fs:/vol/external01/install", ".tik", DirList::Files);
		
		cnt = dir.GetFilecount();
		if(cnt > 0)
		{
			AddFolder();
			Folders.at(0)->name = "install";
			Folders.at(0)->path = "fs:/vol/external01/install";
			Folders.at(0)->selected = false;
		}
	}
	
	return Folders.size();
}

int CFolderList::GetFromArray()
{
	Reset();
	
	u32 dir  = 0;
	
	for(dir = 0; dir < 1024; dir++)
	{
		std::string path = gFolderPath[dir];
		
		if(!path.size())
			break;
		else
		{
			std::string name = path;
			name.erase(0, name.find_last_of("/")+1);
			
			AddFolder();
			Folders.at(dir)->name = name;
			Folders.at(dir)->path = path;
			Folders.at(dir)->selected = true;
		}
	}
	
	return Folders.size();
}

void CFolderList::SetArray()
{
	u32 dir = 0;
	u32 i = 0;
	
	for(dir = 0; dir < 1024; dir++)
	{
		snprintf(gFolderPath[dir], 1, "\0");
		
		bool found = false;
		while(i < Folders.size() && !found)
		{
			if(Folders.at(i)->selected == true)
			{
				snprintf(gFolderPath[dir], Folders.at(i)->path.size()+1, Folders.at(i)->path.c_str());
				found = true;
			}
			
			i++;
		}
	}
}
