#include "postgres.h"
#include "utils/array.h"
#include "utils/geo_decls.h"
#include "catalog/pg_type.h"
#include "utils/lsyscache.h"

#include "fmgr.h"

#ifndef DBL_EPSILON
#define DBL_EPSILON (2.2204460492503131e-16)
#endif // DBL_EPSILON

#ifndef PI
#define PI (3.14159265358979)
#endif // PI

#ifndef DBL_MAX
#define DBL_MAX (1.79769e+308)
#endif // DBL_MAX

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif // PG_MODULE_MAGIC

PG_FUNCTION_INFO_V1( fn_rotate_polygon );
PG_FUNCTION_INFO_V1( fn_get_polygon_points );

Datum fn_rotate_polygon( PG_FUNCTION_ARGS )
{
    POLYGON * poly    = NULL;
    float8    radians = 0.0;
    int32     i       = 0;
    Point     center  = {0.0};
    float8    min_x   = DBL_MAX;
    float8    max_x   = -DBL_MAX;
    float8    min_y   = DBL_MAX;
    float8    max_y   = -DBL_MAX;

    if( PG_ARGISNULL(0) )
    {
        PG_RETURN_NULL();
    }

    poly    = PG_GETARG_POLYGON_P_COPY(0);
    radians =  PG_GETARG_FLOAT8(1);

    if(
            radians > ( 2 * PI + DBL_EPSILON )
         || radians < - ( 2 * PI - DBL_EPSILON )
      )
    {
        ereport(
            ERROR,
            (
                errcode( ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE ),
                errmsg( "radians argument must be between -2pi and 2pi" )
            )
        );
    }

    if( poly == NULL )
    {
        ereport(
            ERROR,
            (
                errcode( ERRCODE_NULL_VALUE_NOT_ALLOWED ),
                errmsg( "Polygon may not be NULL" )
            )
        );
    }

    if( fabs( radians ) <= DBL_EPSILON )
    {
        PG_RETURN_POLYGON_P( poly );
    }

    /*
    for( i = 0; i < poly->npts; i++ )
    {
        center.x += poly->p[i].x;
        center.y += poly->p[i].y;
    }

    center.x = center.x / poly->npts;
    center.y = center.y / poly->npts;
    */
    for( i = 0; i < poly->npts; i++ )
    {
        max_x = ( poly->p[i].x > max_x ) ? poly->p[i].x : max_x;
        min_x = ( poly->p[i].x < min_x ) ? poly->p[i].x : min_x;
        max_y = ( poly->p[i].y > max_y ) ? poly->p[i].y : max_y;
        min_y = ( poly->p[i].y < min_y ) ? poly->p[i].y : min_y;
    }

    center.x = ( max_x + min_x ) / 2;
    center.y = ( max_y + min_y ) / 2;

    for( i = 0; i < poly->npts; i++ )
    {
        poly->p[i].x =
            poly->p[i].x * cos( radians )
          - poly->p[i].y * sin( radians )
          + ( center.x
               - (
                    center.x * cos( radians )
                  - center.y * sin( radians )
                 )
            );

        poly->p[i].y =
            poly->p[i].x * sin( radians )
          + poly->p[i].y * cos( radians )
          + ( center.y
               - (
                    center.x * sin( radians )
                  + center.y * cos( radians )
                 )
            );
    }

    PG_RETURN_POLYGON_P( poly );
}

Datum fn_get_polygon_points( PG_FUNCTION_ARGS )
{
    Datum *     point_array = NULL;
    POLYGON *   my_polygon  = NULL;
    ArrayType * result      = NULL;
    int32       i           = 0;
    int16       typlen      = 0;
    bool        typbyval    = false;
    char        typalign    = '\0';


    if( PG_ARGISNULL(0) )
    {
        PG_RETURN_NULL();
    }

    my_polygon = PG_GETARG_POLYGON_P(0);

    if( my_polygon == NULL )
    {
        PG_RETURN_NULL();
    }

    point_array = ( Datum * ) palloc0(
        sizeof( Datum )
      * my_polygon->npts
    );
    
    for( i = 0; i < my_polygon->npts; i++ )
    {
        point_array[i] = PointerGetDatum( &(my_polygon->p[i]) );
    }

    get_typlenbyvalalign(
        POINTOID,
        &typlen,
        &typbyval,
        &typalign
    );

    result = construct_array(
        point_array,
        my_polygon->npts,
        POINTOID,
        typlen,
        typbyval,
        typalign
    );

    PG_RETURN_ARRAYTYPE_P( result );
}
