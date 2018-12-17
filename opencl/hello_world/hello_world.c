#include <stdio.h>
#include <stdlib.h>

#include <CL/cl.h>

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int main( void )
{
    cl_device_id     device_id         = NULL;
    cl_context       context           = NULL;
    cl_command_queue command_queue     = NULL;
    cl_mem           memobj            = NULL;
    cl_program       program           = NULL;
    cl_kernel        kernel            = NULL;
    cl_platform_id   platform_id       = NULL;
    cl_uint          ret_num_devices   = 0;
    cl_uint          ret_num_platforms = 0;
    cl_int           ret               = 0;

    char             string[MEM_SIZE];
    FILE *           fp;
    char             filename[]        = "./hello_world.cl";
    char *           source_string     = NULL;
    size_t           source_size       = 0;

    fp = fopen( filename, "R" );

    if( !fp )
    {
        fprintf( stderr, "Failed to load kernel.\n" );
        exit( 1 );
    }

    source_string = ( char * ) malloc( MAX_SOURCE_SIZE );

    source_size = fread( source_string, 1, MAX_SOURCE_SIZE, fp );

    fclose( fp );

    ret = clGetPlatformIDs( 1, &platform_id, &ret_num_platforms );

    if( ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to enumerate OpenCL Platforms" );
        exit( 1 );
    }

    if( ret_num_platforms == 0 )
    {
        fprintf( stdout, "No OpenCL Platforms present in system" );
        exit( 0 );
    }

    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices );
    
    if( ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to enumerate OpenCL Devices" );
        exit( 1 );
    }

    if( ret_num_devices == 0 )
    {
        fprintf( stdout, "No OpenCL Devices present in system" );
        exit( 0 );
    }

    context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret );

    if( context == NULL || ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to create OpenCL context on device ID %p", device_id );
        exit( 1 );
    }

    command_queue = clCreateCommandQueue( context, device_id, 0, &ret );

    if( command_queue == NULL || ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to setup OpenCL Command queue on device ID %p", device_id );
        exit( 1 );
    }

    memobj = clCreateBuffer( context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof( char ), NULL, &ret );

    if( memobj == NULL || ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to create OpenCL memory allocation" );
        exit( 1 );
    }

    program = clCreateProgramWithSource( context, 1, ( const char ** ) &source_string, ( const size_t * ) source_size, &ret );

    if( program == NULL || ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to create OpenCL program" );
        exit( 1 );
    }

    ret = clBuildProgram( program, 1, &device_id, NULL, NULL, NULL );

    if( ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to build OpenCL program" );
        exit( 1 );
    }

    kernel = clCreateKernel( program, "hello_world", &ret );

    if( kernel == NULL || ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to create OpenCL kernel" );
        exit( 1 );
    }

    ret = clSetKernelArg( kernel, 0, sizeof( cl_mem ), ( void * ) &memobj );

    if( ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to setup kernel memory" );
        exit( 1 );
    }

    ret = clEnqueueTask( command_queue, kernel, 0, NULL, NULL );

    if( ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to enqueue kernel execution" );
        exit( 1 );
    }

    ret = clEnqueueReadBuffer( command_queue, memobj, CL_TRUE, 0, MEM_SIZE * sizeof( char ), string, 0, NULL, NULL );

    if( ret != CL_SUCCESS )
    {
        fprintf( stderr, "Failed to read results" );
        exit( 1 );
    }

    printf( string );

    ret = clFlush( command_queue );
    ret = clFinish( command_queue );
    ret = clReleaseKernel( kernel );
    ret = clReleaseProgram( program );
    ret = clReleaseMemObject( memobj );
    ret = clReleaseCommandQueue( command_queue );
    ret = clReleaseContext( context );

    free( source_string );

    return 0;
}

