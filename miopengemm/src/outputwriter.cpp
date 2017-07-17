/*******************************************************************************
 * Copyright (C) 2017 Advanced Micro Devices, Inc. All rights reserved.
 *******************************************************************************/
#include <iostream>
#include <sstream>
#include <string>
#include <miopengemm/enums.hpp>
#include <miopengemm/error.hpp>
#include <miopengemm/outputwriter.hpp>

namespace MIOpenGEMM
{
namespace owrite
{

//void Writer::set_v_bits()
//{
  

  //main_to_file        = false;
  //main_to_terminal    = false;
  //tracker_to_file     = false;
  //tracker_to_terminal = false;
  
  //// currently deps just copies main (JN July 15 2017). 
  //deps_to_file     = false;
  //deps_to_terminal = false;  

  //accu_to_file     = false;
  //accu_to_terminal = false;  

  //if (v == Ver::E::SILENT)
  //{
  //}

  //else if (v == Ver::E::TERMINAL)
  //{
    //main_to_terminal = true;
    //deps_to_terminal = true;
    //accu_to_terminal = true;  

  //}

  //else if (v == Ver::E::SPLIT)
  //{
    //main_to_file     = true;
    //main_to_terminal = true;
    //deps_to_file     = true;
    //deps_to_terminal = true;
    //accu_to_file     = true;
    //accu_to_terminal = true;  

  //}

  //else if (v == Ver::E::TOFILE)
  //{
    //main_to_file = true;
    //deps_to_file = true;
  //}

  //else if (v == Ver::E::TRACK)
  //{
    //tracker_to_terminal = true;
  //}

  //else if (v == Ver::E::STRACK)
  //{
    //main_to_file        = true;
    //deps_to_file        = true;
    //tracker_to_terminal = true;
  //}
  
  //else if (v == Ver::E::ACCURACY){
    //accu_to_terminal = true;
    //tracker_to_terminal = true;
  //}

  //else
  //{
    //throw miog_error("unrecognised Ver enum in set_from_v");
  //}
//}

void Writer::initialise_file()
{
  //if (main_to_file || tracker_to_file || deps_to_file)
  //{
    if (filename.compare("") == 0)
    {
      std::stringstream errm;
      errm << "empty filename passed to Writer, with to_file as true. This is not allowed";
      throw miog_error(errm.str());
    }

    file.open(filename, std::ios::out);

    if (file.good() == false)
    {
      std::stringstream errm;
      errm << "bad filename in constructor of Writer object. "
           << "The filename provided is `" << filename << "'."
           << "The directory of the file must exist, Writers do not create directories. "
           << "Either create all directories in the path, or change the provided path.  ";
      throw miog_error(errm.str());
    }
  //}
}

Writer::Writer(Ver::E v_, std::string filename_) : v(v_), filename(filename_)
{
  if (Ver::fileRequired[v]){
    initialise_file();
  }
  else if (filename != ""){
    throw miog_error("Non-empty filename, but no file writing in Writer. Performing pedantic bail");
  }
  else{
    // no filename and no filename required, good to continue
  }
  
  std::ofstream* ptr_file;
  for (size_t op = 0; op < OutPart::E::N; ++op){
    ptr_file = Ver::toFile[v][op] ? &file : nullptr;
    bw[op]   = BasicWriter(Ver::toTerm[v][op], ptr_file);
  }
}

Writer::~Writer() { file.close(); }

template <>
BasicWriter& BasicWriter::operator<<(Flusher f)
{
  (void)f;

  if (to_terminal)
  {
    std::cout << std::flush;
  }

  if (ptr_file != nullptr)
  {
    ptr_file->flush();
  }
  return *this;
}

template <>
BasicWriter& BasicWriter::operator<<(Endline e)
{
  (void)e;  //.increment();
  if (to_terminal)
  {
    std::cout << std::endl;
  }

  if (ptr_file != nullptr)
  {
    (*ptr_file) << '\n';
    ptr_file->flush();
  }
  return *this;
}
}

owrite::Endline Endl;
owrite::Flusher Flush;
}
