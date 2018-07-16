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

#include "HowTo-PainStimulus.h"
#include "HowToTracker.h"

#include <iostream>
// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>

//--------------------------------------------------------------------------------------------------
/// \brief
///
///
/// \details
///
//--------------------------------------------------------------------------------------------------
void HowToPainStimulus()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToPain.log");
  bg->GetLogger()->Info("HowToPain");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
  HowToTracker tracker(*bg);

  //Create variables for scenario
  SEPainStimulus PainStimulus; //pain object
  std::string location; //location of pain stimulus, examples "Arm", "Leg"
  double severity; //severity (scale 0-1)

  //set up the configuration of the pain stimulus
  location = "Arm";
  severity = 0.5;
  PainStimulus.SetLocation(location);
  PainStimulus.GetSeverity().SetValue(severity);

  //Set up substances.  Initialized morphine plasma concentration to 0.  Note that saline is technically a compound--this is
  //so the engine knows to look for multiple components within the same substance file (i.e. Na, Cl, etc)
  SESubstance* epi = bg->GetSubstanceManager().GetSubstance("Epinephrine");
  // Get the Morphine substance from the substance manager
  const SESubstance* morphine = bg->GetSubstanceManager().GetSubstance("Morphine");

  // Create a substance bolus action to administer the substance
  SESubstanceBolus bolus(*morphine);
  bolus.GetConcentration().SetValue(1000, MassPerVolumeUnit::ug_Per_mL);
  bolus.GetDose().SetValue(10.5, VolumeUnit::mL);
  bolus.SetAdminRoute(CDM::enumBolusAdministration::Intravenous);

  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*morphine, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*epi, "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("PainVisualAnalogueScale");

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToPain.txt");

  //lets start the pain
  bg->ProcessAction(PainStimulus);

  // Advance some time to let the body bleed out a bit
  tracker.AdvanceModelTime(300);

  bg->GetLogger()->Info("The patient has been in moderate pain for 300s");
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  ;

  //administer morphine
  bg->ProcessAction(bolus);
  bg->GetLogger()->Info("Giving the patient Morphine.");

  //grab VAS score
  double pain = bg->GetNervousSystem()->GetPainVisualAnalogueScale();
  double dt = bg->GetTimeStep(TimeUnit::s);

  // Advance some time until patient is comfortable
  while (pain > 2.0) {
    //update value
    pain = bg->GetNervousSystem()->GetPainVisualAnalogueScale();
    tracker.AdvanceModelTime(dt);
  }

  //after we get out of the while loop patient should be happy now
  bg->GetLogger()->Info("The patient is comfortable");

  bg->GetLogger()->Info(std::stringstream() << "Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
  bg->GetLogger()->Info(std::stringstream() << "Hemoglobin Content : " << bg->GetBloodChemistrySystem()->GetHemoglobinContent(MassUnit::g) << MassUnit::g);
  bg->GetLogger()->Info(std::stringstream() << "Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
  bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
  bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
  ;

  tracker.AdvanceModelTime(400);
  bg->GetLogger()->Info("Finished");
}
