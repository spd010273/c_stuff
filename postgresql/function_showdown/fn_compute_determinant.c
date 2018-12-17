#include "postgres.h"
#include "fmgr.h"

#ifndef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif // PG_MODULE_MAGIC

PG_FUNCTION_INFO_V1( fn_compute_determinant );


Datum fn_compute_determinant( PG_FUNCTION_ARGS )
{
    float8 determinant = 0.0;
    float8 a = 0.0;
    float8 b = 0.0;
    float8 c = 0.0;
    float8 d = 0.0;
    
    if( PG_ARGISNULL(0) || PG_ARGISNULL(1) || PG_ARGISNULL(2) || PG_ARGISNULL(3) )
    {
        PG_RETURN_NULL();

    }

    a = PG_GETARG_FLOAT8(0);
    b = PG_GETARG_FLOAT8(1);
    c = PG_GETARG_FLOAT8(2);
    d = PG_GETARG_FLOAT8(3);

    determinant = a * d - c * b;

    PG_RETURN_FLOAT8( determinant );
}
