CREATE OR REPLACE FUNCTION public.fn_rotate_polygon_c( in_poly POLYGON, in_radians FLOAT )
RETURNS POLYGON IMMUTABLE PARALLEL SAFE LANGUAGE C AS 'fn_rotate_polygon.so', 'fn_rotate_polygon';

CREATE OR REPLACE FUNCTION public.fn_get_polygon_points( in_polygon POLYGON )
RETURNS POINT[] IMMUTABLE PARALLEL SAFE LANGUAGE C AS 'fn_rotate_polygon.so', 'fn_get_polygon_points';

DROP FUNCTION IF EXISTS fn_rotate_polygon_plpython( POINT[], FLOAT );
DROP FUNCTION IF EXISTS fn_rotate_polygon_plpython( POLYGON, FLOAT );
CREATE OR REPLACE FUNCTION fn_rotate_polygon_plpython
(
    my_poly    POLYGON,
    rotation  FLOAT
)
RETURNS POLYGON AS
 $$
    import re
    import plpy
    import math

    min_x = 9999
    min_y = 9999
    max_x = -9999
    max_y = -9999
    center_x = 0.0
    center_y = 0.0
    i = 0
    points = re.findall( "\(([-]?\d*\.?\d+?),([-]?\d*\.?\d+?)\)",my_poly )
    new_pts = [0] * len(points)

    for pt in points:
        x = float(pt[0])
        y = float(pt[1])
        min_x = min( min_x, x )
        min_y = min( min_y, y )
        max_x = max( max_x, x )
        max_y = max( max_y, y )

    center_x = ( max_x + min_x ) / 2 
    center_y = ( max_y + min_y ) / 2

    for pt in points:
        x = float(pt[0])
        y = float(pt[1])
        nx = x * math.cos( rotation ) - y * math.sin( rotation ) + ( center_x - ( center_x * math.cos( rotation ) - center_y * math.sin( rotation ) ) )
        ny = x * math.sin( rotation ) + y * math.cos( rotation ) + ( center_y - ( center_x * math.sin( rotation ) + center_y * math.cos( rotation ) ) )
        new_pts[i] = "(" + str(nx) + "," + str(ny) + ")"
        i = i + 1 

    return "(" + ",".join(new_pts) + ")"
 $$
    LANGUAGE plpythonu IMMUTABLE PARALLEL SAFE;

DROP FUNCTION IF EXISTS fn_rotate_polygon_sql( POINT[], FLOAT );
DROP FUNCTION IF EXISTS fn_rotate_polygon_sql( POLYGON, FLOAT );
CREATE OR REPLACE FUNCTION public.fn_rotate_polygon_sql
(
    in_polygon  POLYGON,
    in_radians  FLOAT
)
RETURNS POLYGON AS
 $_$
    WITH tt_decomposed_polygon AS
    (
        SELECT ( '(' || unnest( regexp_split_to_array( regexp_replace( regexp_replace( in_polygon::VARCHAR, '^\(\(', '' ), '\)\)$', '' ), '\),\(' ) )::VARCHAR || ')' )::POINT AS point
    ),
    tt_center AS
    (
        SELECT point(
            ( min( point[0] ) + max( point[0] ) ) / 2,
            ( min( point[1] ) + max( point[1] ) ) / 2
          ) AS center
          FROM tt_decomposed_polygon
    ),
    tt_rotated_points AS
    (
        SELECT point(
            ttdp.point[0]::FLOAT * cos( in_radians )
          - ttdp.point[1]::FLOAT * sin( in_radians )
          + ( ttc.center[0]::FLOAT - ( ttc.center[0]::FLOAT * cos( in_radians ) - ttc.center[1]::FLOAT * sin( in_radians ) ) ),
            ttdp.point[0]::FLOAT * sin( in_radians )
          + ttdp.point[1]::FLOAT * cos( in_radians )
          + ( ttc.center[1]::FLOAT - ( ttc.center[0]::FLOAT * sin( in_radians ) + ttc.center[1]::FLOAT * cos( in_radians ) ) )
          ) as rotated_point
          FROM tt_decomposed_polygon ttdp
    CROSS JOIN tt_center ttc
    )
    SELECT ( '(' ||
            string_agg(ttrp.rotated_point::VARCHAR, ', ' ) ||
            ')' )::polygon
      FROM tt_rotated_points ttrp;
 $_$
    LANGUAGE SQL IMMUTABLE PARALLEL SAFE;

DROP FUNCTION IF EXISTS fn_rotate_polygon_plpgsql( POINT[], FLOAT );
DROP FUNCTION IF EXISTS fn_rotate_polygon_plpgsql( POLYGON, FLOAT );
CREATE OR REPLACE FUNCTION public.fn_rotate_polygon_plpgsql
(
    in_polygon  POLYGON,
    in_radians  FLOAT
)
RETURNS POLYGON AS
 $_$
DECLARE
    my_poly POLYGON;
BEGIN

    WITH tt_decomposed_polygon AS
    ( -- Convert polygon into component points
        SELECT ( '(' || unnest( regexp_split_to_array( regexp_replace( regexp_replace( in_polygon::VARCHAR, '^\(\(', '' ), '\)\)$', '' ), '\),\(' ) )::VARCHAR || ')' )::POINT AS point
    ),
    -- @@ is built-in but this is how MW calculates center
    tt_center AS
    (
        SELECT point(
            ( min( point[0] ) + max( point[0] ) ) / 2,
            ( min( point[1] ) + max( point[1] ) ) / 2
          ) AS center
          FROM tt_decomposed_polygon
    ),
    tt_rotated_points AS
    (
        SELECT point(
            ttdp.point[0]::FLOAT * cos( in_radians )
          - ttdp.point[1]::FLOAT * sin( in_radians )
          + ( ttc.center[0]::FLOAT - ( ttc.center[0]::FLOAT * cos( in_radians ) - ttc.center[1]::FLOAT * sin( in_radians ) ) ),
            ttdp.point[0]::FLOAT * sin( in_radians )
          + ttdp.point[1]::FLOAT * cos( in_radians )
          + ( ttc.center[1]::FLOAT - ( ttc.center[0]::FLOAT * sin( in_radians ) + ttc.center[1]::FLOAT * cos( in_radians ) ) )
          ) as rotated_point
          FROM tt_decomposed_polygon ttdp
    CROSS JOIN tt_center ttc
    )
    SELECT ( '(' ||
            string_agg(ttrp.rotated_point::VARCHAR, ', ' ) ||
            ')' )::polygon
      INTO my_poly
      FROM tt_rotated_points ttrp;

    return my_poly;
END
 $_$
    LANGUAGE plpgsql IMMUTABLE PARALLEL SAFE;

DROP FUNCTION IF EXISTS fn_rotate_polygon_plperl( POINT[], FLOAT );
DROP FUNCTION IF EXISTS fn_rotate_polygon_plperl( POLYGON, FLOAT );
CREATE OR REPLACE FUNCTION fn_rotate_polygon_plperl(
    in_polygon  POLYGON,
    rotation FLOAT
)
RETURNS POLYGON AS
 $$
    my( $polygon, $rotation ) = @_;
    my $min_x, $max_x, $min_y, max_y, $center_x, $center_y;
    my $new_poly = '';
    my $points = $polygon;
    $points =~ s/^\(//;
    $points =~ s/\)$//;
    @$points = split( '\),\(', $points ); 
    
    foreach my $point( @$points ) 
    {
        $point =~ s/^\(//;
        $point =~ s/\)$//;
        @$point = split( ',', $point );
        $min_x = $point->[0] > $min_x ? $min_x : $point->[0];
        $max_x = $point->[0] > $max_x ? $point->[0] : $max_x;
        $min_y = $point->[1] > $min_y ? $min_y : $point->[1];
        $max_y = $point->[1] > $max_y ? $point->[1] : $max_y;
    }

    $center_x = ( $max_x + $min_x ) / 2;
    $center_y = ( $max_y + $min_y ) / 2;
    my @poly_pts;

    foreach my $point( @$points )
    {
        $point =~ s/^\(//;
        $point =~ s/\)$//;
        @$point = split( ',', $point );
        my $new_x = $point->[0] * cos( $rotation ) - $point->[1] * sin( $rotation )
                  + ( $center_x - ( $center_x * cos( $rotation ) - $center_y * sin( $rotation ) ) );
        my $new_y = $point->[0] * sin( $rotation ) + $point->[1] * cos( $rotation )
                  + ( $center_y - ( $center_x * sin( $rotation ) + $center_y * cos( $rotation ) ) );
        push( @poly_pts, "($new_x,$new_y)" );
    }

    return "(" . join( ',', @poly_pts ) . ")";
 $$
    LANGUAGE plperlu IMMUTABLE PARALLEL SAFE;
