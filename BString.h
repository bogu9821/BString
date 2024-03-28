#pragma once
#include <stddef.h>
#include <stdbool.h>

#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }

#define BSTRING_NPOS (size_t)-1

#ifdef __cplusplus
EXTERN_C_BEGIN
#endif

typedef struct BString
{
	char* m_str;
	size_t m_size;
} BString;

typedef struct BStringView
{
	const char* m_str;
	size_t m_size;
} BStringView;

#define BStringDefault ((BString){ .m_str = "" })  // Default empty BString

typedef enum eStringError
{
	BSTRING_NONE,           // No error
	BSTRING_ALLOCATION,     // Error due to memory allocation
	BSTRING_NOTFOUND        // Error for not finding an element
} eStringError;

/*
	Create a string with a const char* with a known size
	t_desc: pointer to a BString to which string will be copied
	t_string: pointer to a string to copy from
	t_num: size of the string
*/
eStringError BStringCreateN(BString* t_desc, const char* t_string, const size_t t_num);

/*
	Create a string with a null-terminated const char* with an unknown size
	t_desc: pointer to a BString to which string will be copied
	t_string: pointer to a null-terminated string to copy from
*/
eStringError BStringCreate(BString* t_dest, const char* t_string);

/*
	Assign the content of one BString to another
	t_desc: pointer to the destination BString
	t_string: BString to assign
*/
eStringError BStringAssign(BString* t_desc, BString t_string);

/*
	Join the content of one BString to another
	t_desc: pointer to the destination BString
	t_src: BString to join
*/
eStringError BStringJoin(BString* t_desc, const BString t_src);

/*
	Concatenates the content of multiple BStrings into the destination BString.

	dest: pointer to the destination BString.
	...: variable number of BStringViews to concatenate.
*/
#define BStringConcentrate(dest, ...)\
{\
	const BStringView stringArray[] = { __VA_ARGS__ };\
	BStringConcentrateN(dest, stringArray, sizeof(stringArray)/sizeof(*stringArray));\
}\

/*
	Concentrate the content of x BStrings to to the destination
	t_desc: pointer to the destination BString
	t_stringArray: array to concentrate
	t_stringsNum: size of the array
*/
eStringError BStringConcentrateN(BString* t_desc, const BStringView* t_stringArray, const size_t t_stringsNum);


/*
	This function compares two BStrings lexicographically.
	t_left: The first BString to be compared.
	t_right: The second BString to be compared.
*/
int BStringCompare(const BString t_left, const BString t_right);

/*
	This function compares two BStrings lexicographically.
	t_left The first BString to be compared.
	t_right The second BString to be compared.
*/
bool BStringEquals(const BString t_left, const BString t_right);

/*
	Swap the content of two BStrings
	t_desc: pointer to the first BString
	t_src: pointer to the second BString
*/
void BStringSwap(BString* t_desc, BString* t_src);

/*
	Move the content of one BString to another
	t_desc: pointer to the destination BString
	t_src: pointer to the source BString
*/
void BStringMove(BString* t_desc, BString* t_src);

/*
	Free the memory allocated for a BString
	t_string: pointer to the BString to be freed
*/
void BStringFree(BString* t_string);

/*
	Print the content of a BString
	t_string: BString to be printed
*/
void BStringPrint(const BString t_string);

/*
	Find the position of a substring in a BString
	t_string: BString to search within
	t_toFind: pointer to the substring to find
	t_offset: starting offset for the search
*/
size_t BStringFind(const BString t_string, const char* t_toFind, size_t t_offset);

/*
	Replace a substring in a BString with another substring
	t_string: pointer to the BString to perform replacement on
	t_toReplace: substring to be replaced
	t_replacement: substring to replace with
*/
eStringError BStringReplace(BString* t_string, const BStringView t_toReplace, const BStringView t_replacement);

/*
	Create a BStringView from a null-terminated const char*
	t_string: pointer to the null-terminated string
*/
BStringView BStringViewCreate(const char* t_string);

/*
	Create a BStringView from a const char* with a known size
	t_string: pointer to the string
	t_num: size of the string
*/
static inline BStringView BStringViewCreateN(const char* t_string, const size_t t_num)
{
	return ((BStringView) { .m_str = t_string, .m_size = t_num });
}

/*
	Inserts a substring into a BString at the specified index.

	t_string: Pointer to the BString to modify.
	to_insert: Substring to insert.
	t_index: Index at which to insert the substring.
*/
eStringError BStringInsertAt(BString* t_string, const BStringView t_insert, const size_t t_index);

/*
	Erases a char from a BString at the specified index.

	t_string: Pointer to the BString to modify.
	t_index: Index of the substring to remove.

*/
eStringError BStringEraseAt(BString* t_string, const size_t t_index);

/*
	Convert a BString to a BStringView
	t_string: BString to convert
*/
static inline BStringView BStringToViewStr(const BString t_string)
{
	return BStringViewCreateN(t_string.m_str, t_string.m_size);
}

#define BStringToView(string)					\
	_Generic((string),							\
		BString:		BStringToViewStr,		\
		char*:			BStringViewCreate,		\
		const char*:	BStringViewCreate		\
	)(string)

/*
	Check if a BString contains a substring
	t_string: BString to search within
	t_toFind: pointer to the substring to find
*/
static inline bool BStringContains(const BString t_string, const char* t_toFind)
{
	return BStringFind(t_string, t_toFind, 0) != BSTRING_NPOS;
}

/*
	Get a pointer to the beginning of the data in a BString
	t_string: BString to get the pointer from
*/
static inline const char* BStringBegin(const BString t_string)
{
	return t_string.m_str;
}

/*
	Get a pointer to the end of the data in a BString
	t_string: BString to get the pointer from
*/
static inline const char* BStringEnd(const BString t_string)
{
	return t_string.m_str + t_string.m_size;
}

#ifdef __cplusplus
EXTERN_C_END
#endif

#undef EXTERN_C_BEGIN
#undef EXTERN_C_END

