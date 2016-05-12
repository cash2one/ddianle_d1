#include "linuxfunc.h"


#ifndef WIN32
#include <assert.h>
#include <string.h>


#ifndef _LINUX_FILL_STRING
#define _LINUX_FILL_STRING(_String, _Size, _Offset)											\
	if ( (_Size) > (size_t)(_Offset) )														\
	{																						\
		memset( (_String) + (_Offset), 0xfd, ((_Size) - (_Offset)) * sizeof(*(_String)) );	\
	}
#endif

int strcpy_s(char *_DEST, size_t _SIZE, const char *_SRC)
{
	char* p = _DEST;
	size_t available = _SIZE;

	while ( (*p++ = *_SRC++) != 0 && --available > 0 )
	{
	}

	if (available == 0)
	{
		*_DEST = 0;
		assert( (L"Buffer is too small" && 0) );

		_LINUX_FILL_STRING( _DEST, _SIZE, 1 );
		return 34;
	}

	_LINUX_FILL_STRING( _DEST, _SIZE, _SIZE - available + 1 );
	return 0;
}


#endif

