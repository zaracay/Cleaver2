//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
// Cleaver - A MultiMaterial Conforming Tetrahedral Meshing Library
//
// -- Cleaver-CLI Tests
//
// Primary Author: Brig Bagley (brig@sci.utah.edu)
//
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//
//  Copyright (C) 2014, Brig Bagley
//  Scientific Computing & Imaging Institute
//
//  University of Utah
//  //
//  //  Permission is  hereby  granted, free  of charge, to any person
//  //  obtaining a copy of this software and associated documentation
//  //  files  ( the "Software" ),  to  deal in  the  Software without
//  //  restriction, including  without limitation the rights to  use,
//  //  copy, modify,  merge, publish, distribute, sublicense,  and/or
//  //  sell copies of the Software, and to permit persons to whom the
//  //  Software is  furnished  to do  so,  subject  to  the following
//  //  conditions:
//  //
//  //  The above  copyright notice  and  this permission notice shall
//  //  be included  in  all copies  or  substantial  portions  of the
//  //  Software.
//  //
//  //  THE SOFTWARE IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY  OF ANY
//  //  KIND,  EXPRESS OR IMPLIED, INCLUDING  BUT NOT  LIMITED  TO THE
//  //  WARRANTIES   OF  MERCHANTABILITY,  FITNESS  FOR  A  PARTICULAR
//  //  PURPOSE AND NONINFRINGEMENT. IN NO EVENT  SHALL THE AUTHORS OR
//  //  COPYRIGHT HOLDERS  BE  LIABLE FOR  ANY CLAIM, DAMAGES OR OTHER
//  //  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  //  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
//  //  USE OR OTHER DEALINGS IN THE SOFTWARE.
//  //-------------------------------------------------------------------
//  //-------------------------------------------------------------------
#include "gtest/gtest.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <cmath>
#define _FIELDS " --material_fields "
#define _NAME   " --output_name "
#define _PATH   " --output_path "
#define _CLI    "cleaver-cli "
static std::string diff = std::string("diff -wB ");
static std::string data_dir = std::string(TEST_DATA_DIR);
static std::string command = std::string(BINARY_DIR) + _CLI + " -v ";
static std::string name = _NAME + std::string("output");
static std::string path = _PATH + data_dir;
static std::string input = _FIELDS +
data_dir + "input/spheres1.nrrd " +
data_dir + "input/spheres2.nrrd " +
data_dir + "input/spheres3.nrrd " +
data_dir + "input/spheres4.nrrd " ;
static const size_t num_files = 7;
static std::string files[num_files] = {
  "background.node",
  "background.ele",
  "sizing_field.nrrd",
  "boundary_field.nrrd",
  "boundary.nrrd",
  "feature_field.nrrd",
  "medial.nrrd"};

void compareEleFiles(const std::string a, const std::string b) {
  ASSERT_FALSE(a == b);
  std::ifstream test(b.c_str(),std::ifstream::in),
    ans(a.c_str(),std::ifstream::in);
  size_t count0, dim0, att0;
  size_t count1, dim1, att1;
  char line[512];
  test.getline(line,512);
  ans.getline(line,512);
  test >> count0 >> dim0 >> att0;
  ans  >> count1 >> dim1 >> att1;
  ASSERT_EQ(count0,count1);
  ASSERT_EQ(dim0,dim1);
  ASSERT_EQ(att0,att1);
  test.getline(line,512);
  ans.getline(line,512);
  for (size_t i = 0; i < count0; i++) {
    size_t tmp = 0, tmp2 = 0;
    size_t num0 = 0, num1 = 0;
    test >> num0; ans >> num1;
    ASSERT_EQ(num0,num1);
    for(size_t j = 0; j < dim0; j++) {
      test >> tmp; ans >> tmp2;
      ASSERT_EQ(tmp,tmp2);
    }
    for(size_t j = 0; j < att0; j++) {
      test >> tmp; ans >> tmp2;
      ASSERT_EQ(tmp,tmp2);
    }
  }
}

void compareNodeFiles(const std::string a, const std::string b) {
  ASSERT_FALSE(a == b);
  std::ifstream test(b.c_str(),std::ifstream::in),
    ans(a.c_str(),std::ifstream::in);
  size_t count0, dim0, att0, bnd0;
  size_t count1, dim1, att1, bnd1;
  char line[512];
  test.getline(line,512);
  ans.getline(line,512);
  test >> count0 >> dim0 >> att0 >> bnd0;
  ans  >> count1 >> dim1 >> att1 >> bnd1;
  ASSERT_EQ(count0,count1);
  ASSERT_EQ(dim0,dim1);
  ASSERT_EQ(att0,att1);
  ASSERT_EQ(bnd0,bnd1);
  test.getline(line,512);
  ans.getline(line,512);
  for (size_t i = 0; i < count0; i++) {
    float tmp = 0.f, tmp2 = 0.f;
    size_t num0 = 0, num1 = 0;
    test >> num0; ans >> num1;
    ASSERT_EQ(num0,num1);
    for(size_t j = 0; j < dim0; j++) {
      test >> tmp; ans >> tmp2;
      ASSERT_FLOAT_EQ(tmp,tmp2);
    }
    for(size_t j = 0; j < att0; j++) {
      test >> tmp; ans >> tmp2;
      ASSERT_FLOAT_EQ(tmp,tmp2);
    }
    for(size_t j = 0; j < bnd0; j++) {
      test >> tmp; ans >> tmp2;
      ASSERT_FLOAT_EQ(tmp,tmp2);
    }
  }
}

// Tests basic IO for CLI
TEST(CLIRegressionTests, Basic) {
  //make sure there is a command interpreter
  ASSERT_EQ(0,(int)!(std::system(NULL)));
  //setup the line that calls the command line interface
  std::string log = "basic_output.txt";
  std::string output = " > " + data_dir + log + " 2>&1";
  std::string line = (command + name + path + input + output);
  //make sure there was no error from the command line
  ASSERT_EQ(0, std::system(line.c_str()));
  //move the other generated files in the current dir to the test dir
  for(size_t i = 0; i < num_files; i++) {
    std::system(("mv " + files[i] + " " + data_dir).c_str());
  }
  //compare all of the related files
  EXPECT_NO_FATAL_FAILURE(compareNodeFiles(
        data_dir + "basic/output.node",
        data_dir + "output.node"));
  EXPECT_NO_FATAL_FAILURE(compareEleFiles(
        data_dir + "basic/output.ele",
        data_dir + "output.ele"));
  //delete the output files from this test
  for(size_t i = 0; i < num_files; i++) {
    std::system(("rm " + data_dir + files[i]).c_str());
  }
  std::system(("rm " + data_dir + "output.info").c_str());
  std::system(("rm " + data_dir + "output.node").c_str());
  std::system(("rm " + data_dir + "output.ele").c_str());
  std::system(("rm " + data_dir + log).c_str());
}
// Tests scaling IO for CLI
TEST(CLIRegressionTests, Scaling) {
  //setup the line that calls the command line interface
  std::string log = "scaling_output.txt";
  std::string output = " > " + data_dir + log + " 2>&1";
  std::string scale = " --scale 0.5 ";
  std::string line = (command + name + path + scale + input + output);
  //make sure there was no error from the command line
  ASSERT_EQ(0, std::system(line.c_str()));
  //move the other generated files in the current dir to the test dir
  for(size_t i = 0; i < num_files; i++) {
    std::system(("mv " + files[i] + " " + data_dir).c_str());
  }
  //compare all of the related files
  EXPECT_NO_FATAL_FAILURE(compareNodeFiles(
        data_dir + "scaling/output.node",
        data_dir + "output.node"));
  EXPECT_NO_FATAL_FAILURE(compareEleFiles(
        data_dir + "scaling/output.ele",
        data_dir + "output.ele"));
  //delete the output files from this test
  for(size_t i = 0; i < num_files; i++) {
    std::system(("rm " + data_dir + files[i]).c_str());
  }
  std::system(("rm " + data_dir + "output.info").c_str());
  std::system(("rm " + data_dir + "output.node").c_str());
  std::system(("rm " + data_dir + "output.ele").c_str());
  std::system(("rm " + data_dir + log).c_str());
}
