/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/


#include "TestDriver.h"
#include "Verification.h"

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/test/CommonDataModelTest.h>

int main(int argc, char* argv[])
{

  if (argc < 3) {
    std::cout << "Usuage\n\t " << argv[0] << ": <test> <output_dir>" << std::endl;
  }
  else {
    std::string test= argv[1];
    std::string dir = argv[2];
    CommonDataModelTest executor;
    executor.GetLogger()->LogToConsole(true);
    executor.RunTest(test, dir);

  }

  return 0;
}
