#include "fase.h"

eFase fase( eFase date )
{
	if( date == faseGerminacion )
	{
		return faseGerminacion;
	}
	else if( date == fasePreFructificacion )
	{
		return fasePreFructificacion;
	}
	else if( date == faseIndFructificacion )
	{
		return faseIndFructificacion;
	}
	else if( date == fasePlFructificacion )
	{
		return fasePlFructificacion;
	}
	
	return faseResFructificacion;
}
