#include "ctrlFase.h"

eFase cultivation( eFase date )
{
	
	switch ( date )
	{
		/* GERMINATION STAGE */
		case faseGerminacion:
			if( ( Date[1] == setMonth ) )
			{
				if( ( Date[0] - setDate ) == 20 )
				{
					return fasePreFructificacion;
				}
				else
				{
					return faseGerminacion;
				}
			}
			else if( Date[1] == setMonth + 1 )
			{
				if( ( Date[0] == 1 ) || ( Date[0] == 3 ) || ( Date[0] == 7 ) || ( Date[0] == 8 ) || ( Date[0] == 10 ) || ( Date[0] == 12 ) )
				{
					if( ( setDate - Date[0] ) == 12 )
					{
						return fasePreFructificacion;
					}
					else
					{
						return faseGerminacion;
					}
				}
				else if( ( Date[0] == 4 ) || ( Date[0] == 6 ) || ( Date[0] == 9 ) || ( Date[0] == 11 ) )
				{
					if( ( setDate - Date[0] ) == 11 )
					{
						return fasePreFructificacion;
					}
					else
					{
						return faseGerminacion;
					}
				}
				else
				{
					if( ( setDate - Date[0] ) == 9 )
					{
						return fasePreFructificacion;
					}
					else
					{
						return faseGerminacion;
					}
				}
			}
	/* GERMINATION STAGE */
			break;
		
	/* PRE-FRUCTIFICATION STAGE */
		case fasePreFructificacion:
			if( ( Date[1] == setMonth ) )
			{
				if( ( Date[0] - setDate ) == 13 )
				{
					return faseIndFructificacion;
				}
				else
				{
					return fasePreFructificacion;
				}
			}
			else if( Date[1] == setMonth + 1 )
			{
				if( ( Date[0] == 1 ) || ( Date[0] == 3 ) || ( Date[0] == 7 ) || ( Date[0] == 8 ) || ( Date[0] == 10 ) || ( Date[0] == 12 ) )
				{
					if( ( setDate - Date[0] ) == 19 )
					{
						return faseIndFructificacion;
					}
					else
					{
						return fasePreFructificacion;
					}
				}
				else if( ( Date[0] == 4 ) || ( Date[0] == 6 ) || ( Date[0] == 9 ) || ( Date[0] == 11 ) )
				{
					if( ( setDate - Date[0] ) == 18 )
					{
						return faseIndFructificacion;
					}
					else
					{
						return fasePreFructificacion;
					}
				}
				else
				{
					if( ( setDate - Date[0] ) == 16 )
					{
						return faseIndFructificacion;
					}
					else
					{
						return fasePreFructificacion;
					}
				}
			}
	/* PRE-FRUCTIFICATION STAGE */
			break;
	
		case faseIndFructificacion:
	/* FRUCTIFICATION INDUCTION STAGE */
			if( ( Date[1] == setMonth ) )
			{
				if( ( Date[0] - setDate ) == 7 )
				{
					return fasePlFructificacion;
				}
				else
				{
					return faseIndFructificacion;
				}
			}
			else if( Date[1] == setMonth + 1 )
			{
			if( ( Date[0] == 1 ) || ( Date[0] == 3 ) || ( Date[0] == 7 ) || ( Date[0] == 8 ) || ( Date[0] == 10 ) || ( Date[0] == 12 ) )
				{
					if( ( setDate - Date[0] ) == 25 )
					{
						return fasePlFructificacion;
					}
					else
					{
						return faseIndFructificacion;
					}
				}
				else if( ( Date[0] == 4 ) || ( Date[0] == 6 ) || ( Date[0] == 9 ) || ( Date[0] == 11 ) )
				{
					if( ( setDate - Date[0] ) == 24 )
					{
						return fasePlFructificacion;
					}
					else
					{
						return faseIndFructificacion;
					}
				}
				else
				{
					if( ( setDate - Date[0] ) == 22 )
					{
						return fasePlFructificacion;
					}
					else
					{
						return faseIndFructificacion;
					}
				}
			}
	/* FRUCTIFICATION INDUCTION STAGE */
			break;
	
		case fasePlFructificacion:
	/* ALL FRUCTIFICATION STAGE */
			if( ( Date[1] == setMonth ) )
			{
				if( ( Date[0] - setDate ) == 15 )
				{
					return  faseResFructificacion;
				}
				else
				{
					return fasePlFructificacion;
				}
			}
			else if( Date[1] == setMonth + 1 )
			{
				if( ( Date[0] == 1 ) || ( Date[0] == 3 ) || ( Date[0] == 7 ) || ( Date[0] == 8 ) || ( Date[0] == 10 ) || ( Date[0] == 12 ) )
				{
					if( ( setDate - Date[0] ) == 17 )
					{
						return faseResFructificacion;
					}
					else
					{
						return fasePlFructificacion;
					}
				}
				else if( ( Date[0] == 4 ) || ( Date[0] == 6 ) || ( Date[0] == 9 ) || ( Date[0] == 11 ) )
				{
					if( ( setDate - Date[0] ) == 16 )
					{
						return faseResFructificacion;
					}
					else
					{
						return fasePlFructificacion;
					}
				}
				else
				{
					if( ( setDate - Date[0] ) == 14 )
					{
						return faseResFructificacion;
					}
					else
					{
						return fasePlFructificacion;
					}
				}
			}
	/* ALL FRUCTIFICATION STAGE */
			break;
	
		case faseResFructificacion:
	/* THE CYCLE HAS FINISHED */
			if( ( Date[1] == setMonth ) )
			{
				if( ( Date[0] - setDate ) == 30 )
				{
					return  faseMax;
				}
				else
				{
					return faseResFructificacion;
				}
			}
			else if( Date[1] == setMonth + 1 )
			{
				if( ( Date[0] == 1 ) || ( Date[0] == 3 ) || ( Date[0] == 7 ) || ( Date[0] == 8 ) || ( Date[0] == 10 ) || ( Date[0] == 12 ) )
				{
					if( ( setDate - Date[0] ) == 2 )
					{
						return faseMax;
					}
					else
					{
						return faseResFructificacion;
					}
				}
				else if( ( Date[0] == 4 ) || ( Date[0] == 6 ) || ( Date[0] == 9 ) || ( Date[0] == 11 ) )
				{
					if( ( setDate - Date[0] ) == 1 )
					{
						return faseMax;
					}
					else
					{
						return faseResFructificacion;
					}
				}
			}
	/* THE CYCLE HAS FINISHED */
			break;
		default:
			break;

	}
	
	return error;
	
}	
