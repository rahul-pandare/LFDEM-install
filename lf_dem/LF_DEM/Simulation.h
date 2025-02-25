/**
 \class Simulation
 \brief Class launching the simulation by setting up the System class and performing predefined shear evolution
 \author Ryohei Seto
 \author Romain Mari
 */
//
//  Simulation.h
//  LF_DEM
//
//  Created by Ryohei Seto and Romain Mari on 11/15/12.
//  Copyright (c) 2012-2016 Ryohei Seto and Romain Mari. All rights reserved.
//
#ifndef __LF_DEM__Simulation__
#define __LF_DEM__Simulation__
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <ctime>
#include <map>
#include <set>
#include <algorithm>
#include "global.h"
#include "System.h"
#include "ParameterSet.h"
#include "ParameterSetFactory.h"
#include "DimensionalQty.h"
#include "OutputData.h"
#include "Events.h"
#include "Timer.h"
#include "gsd.h"

enum class DispersionType { mono, bi, poly };

class Simulation {
private:
	Parameters::ParameterSet p_initial;
	std::string header_imported_configulation[2];
	Parameters::ControlVariable control_var;
	Dimensional::DimensionalQty<double> control_value;
	/*
	 * Resultant data
	 */
	Dimensional::Unit output_unit;
	Dimensional::UnitSystem system_of_units;
	double target_stress_input;
	double pressure_drop;
	double input_rate;
	double dimensionless_rate;
	double viscosity;
	double normal_stress_diff1;
	double normal_stress_diff2;
	bool restart_from_chkp;
	double jamming_strain; // Strain stroke to get jamming
	bool stress_reversal;
	double time_last_sj_program;
	double sj_duration_min;
	time_t time_strain_0;
	time_t time_strain_1;
	time_t time_strain_end;
	int timestep_1;
	int timestep_end;
	Sym2Tensor stress_basis_0;
	Sym2Tensor stress_basis_3;
	std::deque<double> sj_program_stress;
	std::deque<double> sj_program_duration;
	/*
	 * For output data.
	 */
	std::ofstream fout_time;
	std::ofstream fout_input;
	OutputData outdata;
	OutputData outdata_st;
	OutputData outdata_pst;
	OutputData outdata_par;
	OutputData outdata_int;
	OutputData outdata_dim;
	OutputData outdata_fabric;
	std::ofstream fout_boxing;
	std::ofstream fout_flow;
	std::ofstream fout_fprofile;
	gsd_handle gsdOut;
	int np1;
	Dimensional::Unit determineUnit(Parameters::ParameterSetFactory &PFact);
	void convertForces(Dimensional::Unit &internal_unit,
					   Parameters::ParameterSetFactory &PFact);
	void setOutputUnit(Parameters::ParameterSetFactory &PFact);
	void exportControlVariable();

	/*
	 * For inputs
	 */
	void setupOptionalSimulation();
	std::vector<Sym2Tensor> getParticleStressGroup(std::string group);
	void checkDispersionType();
	/*********** shear jamming  ************/
	void operateJammingStressReversal(std::set<std::string> &output_events);
	DispersionType dispersion_type;
	std::string indent;
public:
	System sys;
	/* For DEMsystem*/
	Simulation(State::BasicCheckpoint chkp = State::zero_time_basicchkp);
	void simulationSteadyShear(std::string in_args,
							   std::map<std::string, std::string>& input_files,
							   bool binary_conf,
							   Parameters::ControlVariable control_variable_,
							   Dimensional::DimensionalQty<double> control_value_,
							   std::string simu_identifier);
	void simulationFlowField(std::string simulation_type,
							 std::string in_args,
							 std::map<std::string, std::string>& input_files,
							 bool binary_conf,
							 Parameters::ControlVariable control_variable_,
							 Dimensional::DimensionalQty<double> control_value_,
							 std::string simu_identifier);
	void setupSimulation(std::string in_args,
						 std::map<std::string, std::string> &input_files,
						 bool binary_conf,
						 std::string simu_identifier);
	void setupFlow();
	void setupControl(Parameters::ControlVariable control_variable_,
					   Dimensional::DimensionalQty<double> control_value_);
	void setupControl(Parameters::ControlVariable control_variable_,
							  std::string control_value_);
	void setConfigToSystem(bool binary_conf, const std::string &filename);
	void setConfigToSystemDimers(bool binary_conf, const std::string &filename, const std::string &dimer_filename);

	TimeKeeper initTimeKeeper();
	bool keepRunning();
	// void timeEvolution(double& next_output_data);
	void generateOutput(const std::set<std::string> &output_events, int& binconf_counter);
	/*********** Events  ************/
	std::list <Event> events;
	void setupEvents();
	void handleEvents();
	System &getSys()
	{
		return sys;
	}

	void assertParameterCompatibility();
	void openOutputFiles();
	std::string prepareSimulationName(bool binary_conf,
									  const std::string& filename_import_positions,
									  const std::string& filename_parameters,
									  const std::string& simu_identifier);
	void echoInputFiles(std::string in_args,
						std::map<std::string, std::string>& input_files);
//	void contactForceParameter(std::string filename); // @@@ Do we use this?
//	void contactForceParameterBrownian(std::string filename); // @@@ Do we use this?
	void setupNonDimensionalization(Parameters::ParameterSetFactory &PFact);
	void stopShearing(TimeKeeper &tk); //simulation mode 22
	void stressReversal();
	void stressProgram();
	/*
	 * For outputs
	 */
	void createDataHeader(std::stringstream& data_header);
	void getSnapshotHeader(std::stringstream& snapshot_header);
	void outputData();
	void outputDataSedimentatioin();
	void outputConfigurationData();
	void outputFinalConfiguration(const std::string&);
	void outputIntFileTxt();
	void outputParFileTxt();
	void outputPstFileTxt();
	void outputDimFileTxt();
	void outputConfigurationBinary(std::string);
	void outputGSD();
	void dataAdjustGSD(std::vector<vec3d> &pos,
					   std::vector<vec3d> &vel,
					   vec3d &shear_strain,
					   double lx, double ly, double lz);
	void checkpoint();
	vec3d shiftUpCoordinate(double x, double y, double z);
	void outputComputationTime();
	bool kill;
	bool force_to_run;
	/*********** Events  ************/
	void handleEventsShearJamming();
	void handleEventsFragility();
	void handleEventsJammingStressReversal();
	std::string gitVersion();
	std::string simu_name;
	void timeEvolutionUntilNextOutput(const TimeKeeper &tk);
	void printProgress();
};
#endif /* defined(__LF_DEM__Simulation__) */
