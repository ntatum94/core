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

#pragma once
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/schema/ScalarHeatCapacitancePerAmountData.hxx>

class BIOGEARS_API HeatCapacitancePerAmountUnit : public CCompoundUnit {
public:
  HeatCapacitancePerAmountUnit(const std::string& u)
    : CCompoundUnit(u)
  {
  }
  virtual ~HeatCapacitancePerAmountUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const HeatCapacitancePerAmountUnit& GetCompoundUnit(const std::string& unit);

  static const HeatCapacitancePerAmountUnit J_Per_K_mol;
};

class BIOGEARS_API SEScalarHeatCapacitancePerAmount : public SEScalarQuantity<HeatCapacitancePerAmountUnit> {
public:
  SEScalarHeatCapacitancePerAmount() {}
  virtual ~SEScalarHeatCapacitancePerAmount() {}

  CDM::ScalarHeatCapacitancePerAmountData* Unload() const;
};