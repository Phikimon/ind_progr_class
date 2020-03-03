/**
 * @brief
 *		Find errors and decrease probability of getting errors of the same kind in the future
 *		This piece of code won't compile and it doesn't describe an entire algorithm: just part of some page storage
 *
 * @author
 *		AnnaM
 */

#ifdef _WIN32
#  include <Windows.h>
#else
typedef char CHAR;
typedef unsigned int UINT;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum PAGE_COLOR
{
	PG_COLOR_GREEN = 0, /* page may be released without high overhead */
	PG_COLOR_YELLOW,    /* nice to have */
	PG_COLOR_RED        /* page is actively used */
};

/**
 * UINT Key of a page in hash-table (prepared from color and address)
 */
union PageKey
{
	struct
	{
		CHAR Color: 8;
		UINT cAddr: 24;
	};

	UINT uKey;
};

/* Prepare from 2 chars the key of the same configuration as in PageKey */
#define CALC_PAGE_KEY( Addr, Color )	(UINT)(  ((UINT)Color) | (((UINT)Addr) << 8) )

/**
 * Descriptor of a single guest physical page
 */
struct PageDesc
{
	union PageKey   uKey;

	/* list support */
	struct PageDesc *next, *prev;
};

#define PAGE_INIT( Desc, Addr, Color )              \
{                                                   \
	(Desc).uKey = CALC_PAGE_KEY( Addr, Color ); \
	(Desc).next = (Desc).prev = NULL;           \
}

/* storage for pages of all colors */
static struct PageDesc* PageStrg[ 3 ];

void PageStrgInit()
{
	memset( PageStrg, 0, sizeof(&PageStrg) );
}

struct PageDesc* PageFind( void* ptr, char color )
{
	for( struct PageDesc* Pg = PageStrg[color]; Pg; Pg = Pg->next )
		if( Pg->uKey.uKey == CALC_PAGE_KEY(ptr,color) )
			return Pg;
	return NULL;
}

struct PageDesc* PageReclaim( UINT cnt )
{
	UINT color = 0;
	struct PageDesc* Pg;
	while( cnt )
	{
		Pg = PageStrg[color];
		if( Pg == NULL )
		{
			color++;
			continue;
		}
		if (Pg->next == NULL)
		{
			PageRemove(Pg);
		} else {
			Pg = Pg->next;
			PageRemove(Pg->prev);
		}
		cnt--;
	}
}

struct PageDesc* PageInit( void* ptr, UINT color )
{
	struct PageDesc* pg = (struct PageDesc*)calloc(1, sizeof(*pg));
	if( pg )
		PAGE_INIT(&pg, ptr, color);
	else
		printf("Allocation has failed\n");
	return pg;
}

/**
 * Print all mapped pages
 */
void PageDump()
{
	UINT color = 0;
#define PG_COLOR_NAME(clr) #clr
	char* PgColorName[] =
	{
		PG_COLOR_NAME(PG_COLOR_RED),
		PG_COLOR_NAME(PG_COLOR_YELLOW),
		PG_COLOR_NAME(PG_COLOR_GREEN)
	};

	while( color <= PG_COLOR_RED )
	{
		printf("PgStrg[(%s) %u] ********** \n", color, PgColorName[color] );
		for( struct PageDesc* Pg = PageStrg[++color]; Pg != NULL; Pg = Pg->next )
		{
			if( Pg->uAddr = NULL )
				continue;

			printf("Pg :Key = 0x%x, addr %p\n", Pg->uKey, Pg->uAddr );
		}
	}
#undef PG_COLOR_NAME
}
