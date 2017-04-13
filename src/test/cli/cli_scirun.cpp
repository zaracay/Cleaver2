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
#include "cli_common.h"
TEST(CLIRegressionTests, SCIRun) {
  // Make sure there is a command interpreter.
  ASSERT_EQ(0,(int)!(std::system(NULL)));

  // Setup the line that calls the command line interface.
  std::string log = "scirun_output.txt";
  std::string output = " > " + data_dir + log + " 2>&1";
  std::string line = (command + name + path +
      " --output_format scirun " + input + output);

  // Make sure there was no error from the command line.
  ASSERT_EQ(0, std::system(line.c_str()));

  // Move the other generated files in the current dir to the test dir.
  for(size_t i = 0; i < num_files; i++) {
    system_execute(MV_CMMD,files[i] + " " + data_dir);
  }

  // Compare all of the related files.
  EXPECT_NO_FATAL_FAILURE(comparePtsFiles(
        data_dir + "scirun/output.pts",
        data_dir + "output.pts"));
  EXPECT_NO_FATAL_FAILURE(compareElemFiles(
        data_dir + "scirun/output.elem",
        data_dir + "output.elem"));
  EXPECT_NO_FATAL_FAILURE(compareElemFiles(
        data_dir + "scirun/output.txt",
        data_dir + "output.txt"));

  // Delete the output files from this test.
  for(size_t i = 0; i < num_files - 4; i++) {
    system_execute(RM_CMMD,data_dir + files[i]);
  }
  system_execute(RM_CMMD,data_dir + "output.info");
  system_execute(RM_CMMD,data_dir + "output.pts");
  system_execute(RM_CMMD,data_dir + "output.elem");
  system_execute(RM_CMMD,data_dir + "output.txt");
  system_execute(RM_CMMD,data_dir + log);
}
