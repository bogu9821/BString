# BString
A C library that simplifies string creation and manipulation. Styled on a C++ string library.

# Example
```c
#include "BString.h"

int main()
{
	// Declare and initialize a BString variable with default parameters.
	BString hello = BStringDefault;
	// Declare an eStringError variable to store error status and create a BString containing "Hello.".
	eStringError error = BStringCreate(&hello, "Hello.");

	// Check if there was an error during BStringCreate.
	// Return value of all functions returning eStringError should be checked.
	if (error != BSTRING_NONE)
	{
		return -1;
	}

	BString world = BStringDefault;
	BStringCreate(&world, "World");

	// Insert "!" at the end of the BString world.
	BStringInsertAt(&world, BStringToView("!"), world.m_size);
	// Replace "." with a space in the hello BString.
	BStringReplace(&hello, BStringToView("."), BStringToView(" "));
	// Join the contents of the world BString to the end of the hello BString.
	BStringJoin(&hello, world);

	// Print the contents of the hello BString.
	BStringPrint(hello);

	// Find the offset of the substring "Wo" in the world BString, starting from index 0.
	const size_t wordOffset = BStringFind(world, "Wo", 0);

	// Check if the substring "Wo" was found in the world BString.
	if (wordOffset != BSTRING_NPOS)
	{
		BString substring = BStringDefault;
		// Extract a substring from the world BString starting from wordOffset with length sizeof("Wo") - 1.
		BStringSubstring(&substring, world, wordOffset, sizeof("Wo") - 1);

		// Concatenate "nderful " and the world BString to the substring BString.
		BStringConcentrate(&substring, BStringToView("nderful "), BStringToView(world));

		BStringPrint(substring);

		// Free the memory allocated for the substring BString.
		BStringFree(&substring);
	}

	// Free the memory allocated for the hello and world BStrings.
	BStringFree(&hello);
	BStringFree(&world);

	return 0;
}
```
