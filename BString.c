#include "BString.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//TODO simplify string reallocation

inline static char* AllocateString(const size_t t_num)
{
	char* myString = malloc(t_num + 1);

	if (!myString)
	{
		return NULL;
	}

	myString[t_num] = '\0';

	return myString;
}

inline static char* ReallocateString(char* t_buffer, const size_t t_num)
{
	char* newBuffer = realloc(t_buffer,t_num + 1);

	if (!newBuffer)
	{
		return NULL;
	}

	newBuffer[t_num] = '\0';

	return newBuffer;
}

eStringError BStringCreateN(BString* t_desc, const char* t_string, const size_t t_num)
{
	const size_t size = t_num == BSTRING_NPOS ? strlen(t_string) : t_num;

	if (!size)
	{
		return BSTRING_ALLOCATION;
	}

	char* myString = t_desc->m_size ? ReallocateString(t_desc->m_str, size) : AllocateString(size);

	if (!myString)
	{
		return BSTRING_ALLOCATION;
	}
	
	memcpy(myString, t_string, size);

	t_desc->m_size = size;
	t_desc->m_str = myString;

	return BSTRING_NONE;
}

eStringError BStringCreate(BString* t_desc, const char* t_string)
{
	return BStringCreateN(t_desc,t_string, BSTRING_NPOS);
}

eStringError BStringAssign(BString* t_desc, BString t_string)
{
	char* myString = ReallocateString(t_desc->m_str, t_string.m_size);

	if (!myString)
	{
		return BSTRING_ALLOCATION;
	}

	memcpy(myString, t_string.m_str, t_string.m_size);

	t_desc->m_size = t_string.m_size;
	t_desc->m_str = myString;

	return BSTRING_NONE;
}

eStringError BStringJoin(BString* t_desc, const BString t_src)
{
	const size_t newSize = t_desc->m_size + t_src.m_size;

	char* newBuffer = ReallocateString(t_desc->m_str, newSize);

	if (!newBuffer)
	{
		return BSTRING_ALLOCATION;
	}

	memcpy(newBuffer + t_desc->m_size, t_src.m_str, t_src.m_size);

	t_desc->m_str = newBuffer;
	t_desc->m_size = newSize;

	return BSTRING_NONE;
}

void BStringSwap(BString* t_desc, BString* t_src)
{
	const BString temp = *t_desc;

	*t_desc = *t_src;
	*t_src = temp;
}

void BStringMove(BString* t_desc, BString* t_src)
{
	if (t_desc == t_src)
	{
		return;
	}

	if (t_desc->m_size)
	{
		free(t_desc->m_str);
	}

	*t_desc = *t_src;

	t_src->m_str = NULL;
	t_src->m_size = 0;
}

void BStringFree(BString* t_string)
{
	if (!t_string->m_size)
	{
		return;
	}

	free(t_string->m_str);

	t_string->m_str = NULL;
	t_string->m_size = 0;
}

inline void BStringPrint(const BString t_string)
{
	if (!t_string.m_size)
	{
		return;
	}

	puts(t_string.m_str);
}

size_t BStringFind(const BString t_string, const char* t_toFind, size_t t_offset)
{
	const char* str = strstr(t_string.m_str + t_offset, t_toFind);

	return str ? (str - t_string.m_str) : BSTRING_NPOS;
}


eStringError BStringSubstring(BString* t_source, const BString t_string, const char* t_toFind, size_t t_offset)
{
	const size_t pos = BStringFind(t_string, t_toFind, t_offset);

	if (pos == BSTRING_NPOS)
	{
		return BSTRING_NOTFOUND;
	}

	return BStringCreateN(t_source, t_string.m_str + t_offset + pos, t_string.m_size - t_offset - pos);
}

eStringError BStringInsertAt(BString* t_string, const BStringView t_insert, const size_t t_index)
{
	const size_t newSize = t_string->m_size + t_insert.m_size;

	char* newBuffer = AllocateString(newSize);

	if (!newBuffer)
	{
		return BSTRING_ALLOCATION;
	}

	memcpy(newBuffer, t_string->m_str, t_index);
	memcpy(newBuffer + t_index, t_insert.m_str, t_insert.m_size);
	memcpy(newBuffer + t_index + t_insert.m_size, t_string->m_str + t_index, t_string->m_size - t_index);

	if (t_string->m_size)
	{
		free(t_string->m_str);
	}

	t_string->m_str = newBuffer;
	t_string->m_size = newSize;

	return BSTRING_NONE;
}

eStringError BStringEraseAt(BString* t_string, const size_t t_index)
{
	const size_t newSize = t_string->m_size - 1;

	char* newBuffer = ReallocateString(t_string->m_str,newSize);

	if (!newBuffer)
	{
		return BSTRING_ALLOCATION;
	}

	memcpy(newBuffer, t_string->m_str, t_index);
	memcpy(newBuffer + t_index, t_string->m_str + t_index + 1, t_string->m_size - 1);

	free(t_string->m_str);

	t_string->m_str = newBuffer;
	t_string->m_size = newSize;

	return BSTRING_NONE;
}

eStringError BStringReplace(BString* t_string, const BStringView t_toReplace, const BStringView t_replacement)
{
	const size_t pos = BStringFind(*t_string, t_toReplace.m_str,0);

	if (pos == BSTRING_NPOS)
	{
		return BSTRING_NOTFOUND;
	}
	
	const size_t newSize = t_string->m_size + t_replacement.m_size - t_toReplace.m_size;

	char* newBuffer = AllocateString(newSize);

	if (!newBuffer)
	{
		return BSTRING_ALLOCATION;
	}

	memcpy(newBuffer, t_string->m_str, pos);
	memcpy(newBuffer + pos, t_replacement.m_str, t_replacement.m_size);
	memcpy(newBuffer + pos + t_replacement.m_size, t_string->m_str + pos + t_toReplace.m_size, t_string->m_size - pos - t_toReplace.m_size);

	free(t_string->m_str);

	t_string->m_str = newBuffer;
	t_string->m_size = newSize;

	return BSTRING_NONE;
}

BStringView BStringViewCreate(const char* t_string)
{
	return ((BStringView) { .m_str = t_string, .m_size = strlen(t_string) });
}


eStringError BStringConcentrateN(BString* t_desc, const BStringView* t_stringArray, const size_t t_stringsNum)
{
	size_t newSize = t_desc->m_size;

	for (size_t i = 0; i < t_stringsNum; i++)
	{
		newSize += t_stringArray[i].m_size;
	}

	char* newBuffer = AllocateString(newSize);

	if (!newBuffer)
	{
		return BSTRING_ALLOCATION;
	}

	memcpy(newBuffer, t_desc->m_str, t_desc->m_size);
	
	char* iterator = newBuffer + t_desc->m_size;
	for (size_t i = 0; i < t_stringsNum; i++)
	{
		memcpy(iterator, t_stringArray[i].m_str, t_stringArray[i].m_size);
		iterator += t_stringArray[i].m_size;
	}

	if (t_desc->m_size)
	{
		free(t_desc->m_str);
	}

	t_desc->m_str = newBuffer;
	t_desc->m_size = newSize;

	return BSTRING_NONE;
}
