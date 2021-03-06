#!/usr/bin/perl

use strict;
use warnings;
use DBI;
use Readonly;
use English qw( -no_match_vars );
use Time::HiRes qw( gettimeofday tv_interval );

$OUTPUT_AUTOFLUSH = 1;

Readonly my $NUMBER_TRIALS => 10;
Readonly my $NUMBER_TESTS => 50;
Readonly my $FUNCTIONS => {
    'c'        => 'fn_compute_determinant_c( a,b,c,d )',
    'sql'      => 'fn_compute_determinant_sql( a,b,c,d )', 
    'plpgsql'  => 'fn_compute_determinant_plpgsql( a,b,c,d )',
    'plpython' => 'fn_compute_determinant_plpython( a,b,c,d )',
    'plperl'   => 'fn_compute_determinant_plperl( a,b,c,d )', 
};

Readonly my $OUT_CSV => '/tmp/test_result.csv';
Readonly my $CSV_HEADER => "test_name,trial_number,test_number,row_count,function_language,time_delta\n";
Readonly my $CSV_LINE   => "__TEST__,__TRIAL_NUMBER__,__TEST_NUMBER__,__ROW_COUNT__,__FUNCTION__,__DELTA__\n";
Readonly my $COMMAND => <<"END_SQL";
    SELECT __FUNCTION__
      FROM tb_test_set
     LIMIT __LIMIT__
END_SQL

Readonly my $TESTS => {
    'cold' => {
        'restart_between_test' => 1,
        'restart_between_rowcount_change' => 1,
        'row_counts' => [ 1,10,100,1000,10000,100000 ],
    },
    'hot' => {
        'restart_between_test' => 0,
        'restart_between_rowcount_change' => 0,
        'row_counts' => [ 1,10,100,1000,10000,100000 ],
    },
    'warmup' => {
        'restart_between_test' => 0,
        'restart_between_rowcount_change' => 1,
        'row_counts' => [ 1,10,100,1000,10000,100000 ],
    }
};

my $handle;

sub _connect()
{
    $handle = DBI->connect( 'dbi:Pg:dbname=test;host=localhost;port=5432', 'postgres', undef );
    unless( $handle )
    {
        die "Failed to connect to DB\n";
    }
    return $handle;
}

sub _restart_server()
{
    unless( system( 'systemctl restart postgresql-9.6' ) )
    {
        sleep(10);
        unless( system( 'systemctl restart postgresql-9.6' ) )
        {
            die "Failed to restart server\n";
        }
    }
    $handle = _connect();
}

if( -e $OUT_CSV )
{
    unlink( $OUT_CSV );
}

open( FH, ">$OUT_CSV" ) or die( "Failed to open CSV: $@" );
print FH $CSV_HEADER;
$handle = _connect();
my $trial_number;

for( $trial_number = 0; $trial_number < $NUMBER_TRIALS; $trial_number++ )
{
    foreach my $test ( keys %$TESTS )
    {
        my $restart_between_test = $TESTS->{$test}->{restart_between_test};
        my $restart_between_rowcount_change = $TESTS->{$test}->{restart_between_rowcount_change};
        my $row_counts = $TESTS->{$test}->{row_counts};

        foreach my $function( keys %$FUNCTIONS )
        {
            foreach my $row_count ( @$row_counts )
            {
                print "Running $NUMBER_TESTS tests for $function with row count $row_count\n";
                my $test_number;
                for( $test_number = 0; $test_number < $NUMBER_TESTS; $test_number++ )
                {
                    my $delta = 999;
                    my $function_call = $FUNCTIONS->{$function};
                    my $command = $COMMAND;
                    $command =~ s/__FUNCTION__/$function_call/;
                    $command =~ s/__LIMIT__/$row_count/;
                    my $start_time = [gettimeofday()];
                    my $sth = $handle->prepare( $command );
                    if( $sth->execute() )
                    {
                        my $end_time = [gettimeofday()];
                        $delta = tv_interval( $start_time, $end_time );
                    }
                    else
                    {
                        warn "TEST FAILED\n";
                    }
                    my $csv_line = $CSV_LINE;
                    $csv_line =~ s/__TEST__/$test/g;
                    $csv_line =~ s/__TEST_NUMBER__/$test_number/g;
                    $csv_line =~ s/__ROW_COUNT__/$row_count/g;
                    $csv_line =~ s/__FUNCTION__/$function/g;
                    $csv_line =~ s/__DELTA__/$delta/g;
                    $csv_line =~ s/__TRIAL_NUMBER__/$trial_number/g;
                    print FH $csv_line;
                    
                    if( $restart_between_test )
                    {
                        _restart_server();
                    }
                }

                if( $restart_between_rowcount_change )
                {
                    _restart_server();
                }
            }
        }
    }
}

close( FH );
