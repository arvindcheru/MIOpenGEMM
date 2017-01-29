#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include <tinygemm/tinygemmsolution.hpp>
#include <tinygemm/sizingup.hpp>
#include <tinygemm/betackernelutil.hpp>
#include <tinygemm/tinygemmerror.hpp>

namespace tinygemm{


TinyGemmSolutionStatistics::TinyGemmSolutionStatistics(float median_benchmark_time_, float median_benchmark_gflops_, float solution_discovery_time_): 
  median_benchmark_time(median_benchmark_time_), median_benchmark_gflops(median_benchmark_gflops_), solution_discovery_time(solution_discovery_time_) {}
    
TinyGemmSolution::TinyGemmSolution(const std::string & betac_kernel_, const std::string & betac_kernel_function_name_,  const std::string & main_kernel_, const std::string &  main_kernel_function_name_, const hyperparams::HyperParams & hp_, const tinygemm::TinyGemmGeometry & geometry_, char floattype_, TinyGemmSolutionStatistics tgss_): 

betac_kernel(betac_kernel_), betac_kernel_function_name(betac_kernel_function_name_), main_kernel(main_kernel_), main_kernel_function_name(main_kernel_function_name_),  hp(hp_), geometry(geometry_), floattype(floattype_), statistics(tgss_){
  
  
  
  
  }
  
  
  
std::map<std::string, size_t> TinyGemmSolution::get_main_kernel_worksize_params(unsigned m, unsigned n){

  /* to populate and return */
  std::map<std::string, size_t> worksize_params;
  
  /* performing a check for map keys */
  std::vector<std::string> integral_params_needed {"n_work_items_per_c_elm", "macro_tile_height", "macro_tile_width", "micro_tile_height", "micro_tile_width"}; 
  
  //for (auto & x : integral_params_needed){
    //if (hp.params.count(x) == 0){
      //std::string errm("\nThis error is being thrown while trying to set the kernel work size. It appears that the parameter `");
      //errm += x;
      //errm += "' is not available in hp.params. This is unexpected, and requires further investigation.\n"; 
      //throw tinygemm_error(errm);
    //}
  //}
  
  unsigned n_workitems_per_workgroup = (hp.macro_tile_height * hp.macro_tile_width ) / ( hp.micro_tile_height * hp.micro_tile_width );
    
  /* outsource the calculation */
  size_t n_work_groups, local_work_size, global_work_size;
  sizingup::set_workforce(n_work_groups, local_work_size, global_work_size, m, n, hp.n_work_items_per_c_elm, hp.macro_tile_height, hp.macro_tile_width, n_workitems_per_workgroup);
  
  /* populate and return */
  worksize_params["n_work_groups"] = n_work_groups;
  worksize_params["local_work_size"] = local_work_size;
  worksize_params["global_work_size"] = global_work_size;
  
  return worksize_params;

}


std::map<std::string, size_t> TinyGemmSolution::get_betac_kernel_worksize_params(unsigned m, unsigned n){

  std::map<std::string, size_t> betac_worksize_params;

  unsigned dim_coal, dim_uncoal;
  size_t betac_global_work_size, betac_local_work_size;

  if (floattype != 'd' && floattype != 'f'){
    std::string errm("Just checking that floattype is either 'd' or 'f' in get_betac_kernel_worksize_params in tinygemmsolution.cpp, and it is not, it is `");
    errm += floattype;
    errm += "'. This is strange (14/11/2016, we only support 32 and 64 bit floats)";
    throw tinygemm_error(errm);
  }
  
  betac::set_betackernel_sizes(floattype, geometry.isColMajor, geometry.tC, m, n, dim_coal, dim_uncoal, betac_global_work_size, betac_local_work_size);  

  betac_worksize_params["dim_coal"] = dim_coal;
  betac_worksize_params["dim_uncoal"] = dim_uncoal;
  betac_worksize_params["global_work_size"] = betac_global_work_size;
  betac_worksize_params["local_work_size"] = betac_local_work_size;    

  return betac_worksize_params;

}


std::string TinyGemmSolution::get_networkconfig_string() const{
  return geometry.get_networkconfig_string();
}

std::string TinyGemmSolution::get_hyper_param_string() const{
  return hp.get_string();
}


} //namespace
