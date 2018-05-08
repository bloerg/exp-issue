
// OpenCL
#include <CL/cl.hpp>
#include <iostream>
#include <fstream>



// 256 threads per block ensures the possibility of full occupancy
// for all compute capabilities if thread count small enough
#define WORKERS_PER_BLOCK 256


using namespace std;

int main() 
{

  unsigned NUM_WORKERS = 0;
  int REQUESTED_GPU  = -1;
  uint L = 16;
  uint N = L*L;

  // select device
  vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);
  if (all_platforms.size() == 0) {
      std::cout << "No platforms found. Check OpenCL installation!\n";
      exit(1);
  }
  cl::Platform default_platform=all_platforms[1];
  std::cout << "\nUsing platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

  vector<cl::Device> all_devices;
  default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices); 
  int deviceCount = all_devices.size();
  if (deviceCount == 0) {
      std::cout << "No devices found. Check OpenCL installation!\n";
      exit(1);
  }
exit(0);
  cl::Device device;
  if(REQUESTED_GPU >= 0 and REQUESTED_GPU < deviceCount)
  {
    device = all_devices[REQUESTED_GPU];
  }
  else 
  {
    device = all_devices[0];
  }
  std::cout << "Using device: " << device.getInfo<CL_DEVICE_NAME>() << "\n";


  //~ // prefer cache over shared memory
  // cudaDeviceSetCacheConfig(cudaFuncCachePreferL1);

  // figure out optimal execution configuration
  // based on GPU architecture and generation
  int maxresidentthreads = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
  int totalmultiprocessors = device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
  int optimum_number_of_workers = maxresidentthreads*totalmultiprocessors;
  if (NUM_WORKERS == 0) {
    NUM_WORKERS = optimum_number_of_workers;
  }
  std::cout << "GPU capabilities\nCL_DEVICE_MAX_WORK_GROUP_SIZE: " << maxresidentthreads << "\nCL_DEVICE_MAX_COMPUTE_UNITS: " << totalmultiprocessors << "\n";
 
  cl::Context cl_context({device});
  cl::CommandQueue cl_queue(cl_context, device);

 // read the kernel from source file
  std::ifstream cl_program_file_ising("test_exp.cl");
  std::string cl_program_string_ising(
      std::istreambuf_iterator<char>(cl_program_file_ising),
      (std::istreambuf_iterator<char>())
  );

 
  cl::Program cl_program_ising(cl_context, cl_program_string_ising, true);
  
  if (cl_program_ising.build({ device }, "-I ./include/ -I ./include/Random123/examples/") != CL_SUCCESS){
      std::cout << " Error building: " << cl_program_ising.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
      getchar();
      exit(1);
  }
  
  cl::Kernel cl_kernel_compute_exp(cl_program_ising, "computeExp");
  
  vector<cl_float> exps(100000);
  vector<cl_float> rngs(100000);
  //~ for (unsigned worker=0; worker < NUM_WORKERS; worker++) {
    //~ for (size_t i = 0; i < N+1; i++) {
      //~ h_lattice.at(i*NUM_WORKERS+worker) = -1;
    //~ }
  //~ }
  
  
  int memory_operation_status;
  
  cl::Buffer d_exps_buffer (
    cl_context,
    CL_MEM_READ_WRITE,
    100000 * sizeof(cl_float),
    NULL,
    &memory_operation_status
  );
  std::cout << "clCreateBuffer status: " << memory_operation_status << "\n";
  //~ memory_operation_status = cl_queue.enqueueWriteBuffer(d_lattice_buf, CL_TRUE, 0, NUM_WORKERS * N * sizeof(cl_char), &h_lattice[0], NULL, &writeEvt);

  cl::Buffer d_rngs_buffer (
    cl_context,
    CL_MEM_READ_WRITE,
    100000 * sizeof(cl_float),
    NULL,
    &memory_operation_status
  );
  std::cout << "clCreateBuffer status: " << memory_operation_status << "\n";
//  memory_operation_status = cl_queue.enqueueWriteBuffer(d_rngs_buf, CL_TRUE, 0, 100000 * sizeof(cl_float), NULL);


  cl_kernel_compute_exp.setArg(0, d_exps_buffer);
  cl_kernel_compute_exp.setArg(1, d_rngs_buffer);

  //FIXME: Machen diese Parameter Sinn?
  int res = cl_queue.enqueueNDRangeKernel(cl_kernel_compute_exp, cl::NDRange(0), cl::NDRange(1), cl::NDRange(1));
  cout << "res: " << res << "\n";
  //~ memory_operation_status = cl_queue.enqueueReadBuffer(d_lattice_buf, CL_TRUE, 0, (NUM_WORKERS*N*sizeof(cl_char) ), &h_lattice[0]);
  memory_operation_status = cl_queue.enqueueReadBuffer(d_exps_buffer, CL_TRUE, 0, (100000 * sizeof(cl_float) ), &exps[0]);
  memory_operation_status = cl_queue.enqueueReadBuffer(d_rngs_buffer, CL_TRUE, 0, (100000 * sizeof(cl_float) ), &rngs[0]);
  
  for (size_t counter = 0; counter < 100000 ; counter++) {
    std::cout << rngs.at(counter) << "," << exps.at(counter) << "\n";
  }

//free memory section

 

return 0;
}
