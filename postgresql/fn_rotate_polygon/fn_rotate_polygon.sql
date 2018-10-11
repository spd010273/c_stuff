CREATE OR REPLACE FUNCTION public.fn_rotate_polygon_c( in_poly POLYGON, in_radians FLOAT )
RETURNS POLYGON IMMUTABLE PARALLEL SAFE LANGUAGE C AS 'fn_rotate_polygon.so', 'fn_rotate_polygon';
