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
#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuitNode.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuitPath.h>
#include <biogears/schema/ThermalCircuitData.hxx>
class SECircuitManager;

class BIOGEARS_API SEThermalCircuit : public SECircuit<CDM::ThermalCircuitData, SEThermalCircuitNode, CDM::ThermalCircuitNodeData, SEThermalCircuitPath, CDM::ThermalCircuitPathData> {
  friend class SECircuitManager;

protected:
  SEThermalCircuit(const std::string& name, SECircuitManager& mgr);

public:
  virtual ~SEThermalCircuit();

  SEThermalCircuitNode& CreateNode(const std::string& name);
  SEThermalCircuitPath& CreatePath(SEThermalCircuitNode& src, SEThermalCircuitNode& tgt, const std::string& name);

  void AddCircuit(SEThermalCircuit& circuit);

protected:
  SECircuitManager& m_Mgr;
};