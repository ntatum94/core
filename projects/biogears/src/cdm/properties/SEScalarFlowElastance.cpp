/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in Elastance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <biogears/cdm/properties/SEScalarFlowElastance.h>
#include <biogears/cdm/stdafx.h>

const FlowElastanceUnit FlowElastanceUnit::cmH2O_Per_L("cmH2O/L");
const FlowElastanceUnit FlowElastanceUnit::mmHg_Per_mL("mmHg/mL");
const FlowElastanceUnit FlowElastanceUnit::Pa_Per_m3("Pa/m^3");

CDM::ScalarFlowElastanceData* SEScalarFlowElastance::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::ScalarFlowElastanceData* data(new CDM::ScalarFlowElastanceData());
  SEScalarQuantity::Unload(*data);
  return data;
}

bool FlowElastanceUnit::IsValidUnit(const std::string& unit)
{
  if (cmH2O_Per_L.GetString().compare(unit) == 0)
    return true;
  if (mmHg_Per_mL.GetString().compare(unit) == 0)
    return true;
  if (Pa_Per_m3.GetString().compare(unit) == 0)
    return true;
  return false;
}

const FlowElastanceUnit& FlowElastanceUnit::GetCompoundUnit(const std::string& unit)
{
  if (cmH2O_Per_L.GetString().compare(unit) == 0)
    return cmH2O_Per_L;
  if (mmHg_Per_mL.GetString().compare(unit) == 0)
    return mmHg_Per_mL;
  if (Pa_Per_m3.GetString().compare(unit) == 0)
    return Pa_Per_m3;
  std::stringstream err;
  err << unit << " is not a valid FlowElastance unit";
  throw CommonDataModelException(err.str());
}