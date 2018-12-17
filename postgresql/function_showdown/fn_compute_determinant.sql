CREATE OR REPLACE FUNCTION public.fn_compute_determinant_c
(
    a DOUBLE PRECISION,
    b DOUBLE PRECISION,
    c DOUBLE PRECISION,
    d DOUBLE PRECISION
)
RETURNS DOUBLE PRECISION AS
 'fn_compute_determinant.so',
 'fn_compute_determinant'
    LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION fn_compute_determinant_sql
(
    a DOUBLE PRECISION,
    b DOUBLE PRECISION,
    c DOUBLE PRECISION,
    d DOUBLE PRECISION
)
RETURNS DOUBLE PRECISION AS
 $_$
    SELECT a * d - c * b;
 $_$
    LANGUAGE SQL IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION fn_compute_determinant_plpgsql
(
    a DOUBLE PRECISION,
    b DOUBLE PRECISION,
    c DOUBLE PRECISION,
    d DOUBLE PRECISION
)
RETURNS DOUBLE PRECISION AS
 $_$
DECLARE
    my_result DOUBLE PRECISION;
BEGIN
    my_result := a * d - b * c;

    RETURN my_result;
END
 $_$
    LANGUAGE 'plpgsql' IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION fn_compute_determinant_plpython
(
    a DOUBLE PRECISION,
    b DOUBLE PRECISION,
    c DOUBLE PRECISION,
    d DOUBLE PRECISION
)
RETURNS DOUBLE PRECISION AS
 $_$
    det = a * d - b * c;

    return det;
 $_$
    LANGUAGE plpythonu IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION fn_compute_determinant_plperl
(
    a DOUBLE PRECISION,
    b DOUBLE PRECISION,
    c DOUBLE PRECISION,
    d DOUBLE PRECISION
)
RETURNS DOUBLE PRECISION AS
 $_$
    my( $a, $b, $c, $d ) = @_;
    my $determinant = $a * $d - $c * $b;
    return $determinant;
 $_$
    LANGUAGE plperlu IMMUTABLE STRICT PARALLEL SAFE;
