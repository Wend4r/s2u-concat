#include <array>

#include <concat.hpp>

#define CONCAT_LINE_STRING_CONSTRUCT(starts, before, between, ends, endsAndStartsWith) ConcatLine_t<const char *>{starts, before, between, ends, endsAndStartsWith}

const std::array<const CConcatLineString, 8> g_arrEmbedsConcat 
{
	{
		// 0-3
		CONCAT_LINE_STRING_CONSTRUCT
		(
			"",     // Starts with.
			":",    // Before key.
			": ",   // Between key & value.
			"\n",   // Ends.
			"\n"    // Ends and starts of next line.
		),
		CONCAT_LINE_STRING_CONSTRUCT
		(
			"\t",
			":",
			": ",
			"\n",
			"\n\t"
		),
		CONCAT_LINE_STRING_CONSTRUCT
		(
			"\t\t",
			":",
			": ",
			"\n",
			"\n\t\t"
		),

		// 3-7
		CONCAT_LINE_STRING_CONSTRUCT
		(
			"\t\t\t",
			":",
			": ",
			"\n",
			"\n\t\t\t"
		),
		CONCAT_LINE_STRING_CONSTRUCT
		(
			"\t\t\t\t",
			":",
			": ",
			"\n",
			"\n\t\t\t\t"
		),
		CONCAT_LINE_STRING_CONSTRUCT
		(
			"\t\t\t\t\t",
			":",
			": ",
			"\n",
			"\n\t\t\t\t\t"
		),
		CONCAT_LINE_STRING_CONSTRUCT
		(
			"\t\t\t\t\t\t",
			":",
			": ",
			"\n",
			"\n\t\t\t\t\t\t"
		),
		CONCAT_LINE_STRING_CONSTRUCT
		(
			"\t\t\t\t\t\t\t",
			":",
			": ",
			"\n",
			"\n\t\t\t\t\t\t\t"
		)
	}
};
