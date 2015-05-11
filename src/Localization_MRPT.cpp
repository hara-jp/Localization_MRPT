// -*- C++ -*-
/*!
 * @file  Localization_MRPT.cpp
 * @brief Localization MRPT Component
 * @date $Date$
 *
 * $Id$
 */

#include "Localization_MRPT.h"
#include "MonteCarloLocalization.h"

// Module specification
// <rtc-template block="module_spec">
static const char* localization_mrpt_spec[] =
  {
    "implementation_id", "Localization_MRPT",
    "type_name",         "Localization_MRPT",
    "description",       "Localization MRPT Component",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "Navigatio",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.min_x", "-0.01",
    "conf.default.max_x", "0.01",
    "conf.default.min_y", "-0.01",
    "conf.default.max_y", "0.01",
    "conf.default.min_phi", "-0.01",
    "conf.default.max_phi", "0.01",
    "conf.default.range_min", "0.3",
    "conf.default.range_max", "10",
    "conf.default.gausianModel_minStdXY", "0.01",
    "conf.default.gausianModel_minStdPHI", "0.01",
    "conf.default.KLD_binSize_PHI", "0.01",
    "conf.default.KLD_binSize_XY", "0.01",
    "conf.default.KLD_delta", "0.02",
    "conf.default.KLD_epsilon", "0.02",
    "conf.default.KLD_maxSampleSize", "1000",
    "conf.default.KLD_minSampleSize", "150",
    "conf.default.KLD_minSamplesPerBin", "0",
    "conf.default.adaptiveSampleSize", "1",
    "conf.default.pfAuxFilterOptimal_MaximumSearchSamples", "10",
    "conf.default.BETA", "0.5",
    "conf.default.sampleSize", "1",
    "conf.default.poseTimeOut", "3.0",
    // Widget
    "conf.__widget__.min_x", "text",
    "conf.__widget__.max_x", "text",
    "conf.__widget__.min_y", "text",
    "conf.__widget__.max_y", "text",
    "conf.__widget__.min_phi", "text",
    "conf.__widget__.max_phi", "text",
    "conf.__widget__.range_min", "text",
    "conf.__widget__.range_max", "text",
    "conf.__widget__.gausianModel_minStdXY", "text",
    "conf.__widget__.gausianModel_minStdPHI", "text",
    "conf.__widget__.KLD_binSize_PHI", "text",
    "conf.__widget__.KLD_binSize_XY", "text",
    "conf.__widget__.KLD_delta", "text",
    "conf.__widget__.KLD_epsilon", "text",
    "conf.__widget__.KLD_maxSampleSize", "text",
    "conf.__widget__.KLD_minSampleSize", "text",
    "conf.__widget__.KLD_minSamplesPerBin", "text",
    "conf.__widget__.adaptiveSampleSize", "text",
    "conf.__widget__.pfAuxFilterOptimal_MaximumSearchSamples", "text",
    "conf.__widget__.BETA", "text",
    "conf.__widget__.sampleSize", "text",
    "conf.__widget__.poseTimeOut", "text",
    // Constraints
    ""
  };
// </rtc-template>
/*!
 * @brief constructor
 * @param manager Maneger Object
 */

ssr::Pose2D OldPose;
ssr::MCLocalization_MRPT mcl;

Localization_MRPT::Localization_MRPT(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_rangeIn("range", m_range),
    m_odometryIn("odometry", m_odometry),
    m_estimatedPoseOut("estimatedPose", m_estimatedPose),
    m_mapServerPort("mapServer")
{
}
    // </rtc-template>

/*!
 * @brief destructor
 */
Localization_MRPT::~Localization_MRPT()
{
}

RTC::ReturnCode_t Localization_MRPT::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("range", m_rangeIn);
  addInPort("odometry", m_odometryIn);
  
  // Set OutPort buffer
  addOutPort("estimatedPose", m_estimatedPoseOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  m_mapServerPort.registerConsumer("mapServer", "RTC::OGMapServer", m_mapServer);
  
  // Set CORBA Service Ports
  addPort(m_mapServerPort);
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("min_x", m_min_x, "-0.01");
  bindParameter("max_x", m_max_x, "0.01");
  bindParameter("min_y", m_min_y, "-0.01");
  bindParameter("max_y", m_max_y, "0.01");
  bindParameter("min_phi", m_min_phi, "-0.01");
  bindParameter("max_phi", m_max_phi, "0.01");
  bindParameter("range_min", m_range_min, "0.3");
  bindParameter("range_max", m_range_max, "10");
  bindParameter("gausianModel_minStdXY", m_minStdXY, "0.01");
  bindParameter("gausianModel_minStdPHI", m_minStdPHI, "0.01");
  bindParameter("KLD_binSize_PHI", m_KLD_binSize_PHI, "0.01");
  bindParameter("KLD_binSize_XY", m_KLD_binSize_XY, "0.01");
  bindParameter("KLD_delta", m_KLD_delta, "0.02");
  bindParameter("KLD_epsilon", m_KLD_epsilon, "0.02");
  bindParameter("KLD_maxSampleSize", m_KLD_maxSampleSize, "1000");
  bindParameter("KLD_minSampleSize", m_KLD_minSampleSize, "150");
  bindParameter("KLD_minSamplesPerBin", m_KLD_minSamplesPerBin, "0");
  bindParameter("adaptiveSampleSize", m_adaptiveSampleSize, "1");
  bindParameter("pfAuxFilterOptimal_MaximumSearchSamples", m_pfAuxFilterOptimal_MaximumSearchSamples, "10");
  bindParameter("BETA", m_BETA, "0.5");
  bindParameter("sampleSize", m_sampleSize, "1");
  bindParameter("poseTimeOut", m_poseTimeOut, "3.0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t Localization_MRPT::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Localization_MRPT::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Localization_MRPT::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
      
RTC::ReturnCode_t Localization_MRPT::onActivated(RTC::UniqueId ec_id)
{
  m_MODE = MODE_NORMAL;  
  m_lastReceivedTime = coil::gettimeofday();

  //Load OGMap
  OGMap* ogmap = new OGMap();
  while (m_mapServerPort.get_connector_profiles()->length() == 0) {
	  coil::sleep(1);
	  std::cout << "[RTC::Localization_MRPT] Waiting for Map Server Connection" << std::endl;
  }

  RTC::ConnectorProfileList& pList = *(m_mapServerPort.get_connector_profiles());
  RTC::RTObjectRef rto = (RTObjectRef)pList[0].ports[0]->get_port_profile()->owner;
  if(std::string((const char*)rto->get_component_profile()->instance_name) == this->getInstanceName()) {
    rto = (RTObjectRef)pList[0].ports[1]->get_port_profile()->owner;
  }
  do {
	  coil::sleep(1);
	  std::cout << "[RTC::Localization_MRPT] Waiting for Map Server Activation" << std::endl;
	  if((*(rto->get_owned_contexts()))[0]->get_component_state(rto) == RTC::LifeCycleState::ERROR_STATE) {
		  std::cout << "[RTC::Localization_MRPT] Map Server RTC is now in ERROR_STATE" << std::endl;
		  return RTC::RTC_ERROR;
	  }
  } while ((*(rto->get_owned_contexts()))[0]->get_component_state(rto) != RTC::LifeCycleState::ACTIVE_STATE);

  RTC::RETURN_VALUE ret = m_mapServer->requestCurrentBuiltMap(ogmap); 

  if (ret != RETURN_VALUE::RETVAL_OK) {
	  std::cout << "[RTC::Localization_MRPT] Acquiring Map from Server Failed." << std::endl;
  }

  std::cout << "[RTC::Localization_MRPT] Initializing Monte Carlo Localization." << std::endl;
  mcl.setMap(*ogmap);
  mcl.min_x = m_min_x;
  mcl.max_x = m_max_x;
  mcl.min_y = m_min_y;
  mcl.max_y = m_max_y;
  mcl.min_phi = m_min_phi;
  mcl.max_phi = m_max_phi;
  mcl.range_min = m_range_min;
  mcl.range_max = m_range_max;

  mcl.minStdXY = m_minStdXY;
  mcl.minStdPHI = m_minStdPHI;
  mcl.KLD_binSize_PHI = m_KLD_binSize_PHI;
  mcl.KLD_binSize_XY = m_KLD_binSize_XY;
  mcl.KLD_delta = m_KLD_delta;
  mcl.KLD_epsilon = m_KLD_epsilon;
  mcl.KLD_maxSampleSize = m_KLD_maxSampleSize;
  mcl.KLD_minSampleSize = m_KLD_minSampleSize;
  mcl.KLD_minSamplesPerBin = m_KLD_minSamplesPerBin;
  mcl.adaptiveSampleSize = m_adaptiveSampleSize;
  mcl.pfAuxFilterOptimal_MaximumSearchSamples = m_pfAuxFilterOptimal_MaximumSearchSamples;
  mcl.BETA = m_BETA;
  mcl.sampleSize = m_sampleSize;

  mcl.initialize();

  OldPose.x = 0;
  OldPose.y = 0;
  OldPose.th = 0;

  m_odomUpdated = m_rangeUpdated = false;
  std::cout << "[RTC::Localization_MRPT] Successfully Activated." << std::endl;
  return RTC::RTC_OK;

}

RTC::ReturnCode_t Localization_MRPT::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

RTC::ReturnCode_t Localization_MRPT::onExecute(RTC::UniqueId ec_id)
{
  coil::TimeValue currentTime = coil::gettimeofday();

  if(m_odometryIn.isNew()){
	  m_odometryIn.read();
	  ssr::Pose2D CurrentPose(m_odometry.data.position.x, m_odometry.data.position.y, m_odometry.data.heading);
      ssr::Pose2D deltaPose = CurrentPose - OldPose;
	  if(deltaPose.x < -5 || deltaPose.x >5){// the number should be add configuration
		m_MODE = MODE_POSE_INVALID_VALUE;
		m_estimatedPoseOut.disconnect("estimatedPose");
	  }else if(deltaPose.y < -5 || deltaPose.y >5){
		m_MODE = MODE_POSE_INVALID_VALUE;
		m_estimatedPoseOut.disconnect("estimatedPose");
	  }
	  OldPose = CurrentPose;
	  mcl.addPose(deltaPose);
	  m_odomUpdated = true;
	  	  
      m_lastReceivedTime = currentTime;
  }else {
    double duration = currentTime - m_lastReceivedTime;
    if (duration > m_poseTimeOut && m_poseTimeOut > 0) {
      m_MODE = MODE_POSE_TIME_OUT;
	  m_estimatedPoseOut.disconnect("estimatedPose");
    }
  }

  if(m_rangeIn.isNew()){
	  m_rangeIn.read();
	  ssr::Range range(&(m_range.ranges[0]), m_range.ranges.length(), m_range.config.maxAngle - m_range.config.minAngle);
	  mcl.addRange(range);
	  m_rangeUpdated = true;
  }

  if(m_rangeUpdated && m_odomUpdated) {
    CPose2D estPose;
    estPose = mcl.getEstimatedPose();
   
	m_estimatedPose.data.position.x = estPose.x();
	m_estimatedPose.data.position.y = estPose.y();
	m_estimatedPose.data.heading    = estPose.phi();
	
	m_estimatedPoseOut.write();
  }

  return RTC::RTC_OK;
}




/*
RTC::ReturnCode_t Localization_MRPT::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Localization_MRPT::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Localization_MRPT::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Localization_MRPT::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t Localization_MRPT::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void Localization_MRPTInit(RTC::Manager* manager)
  {
    coil::Properties profile(localization_mrpt_spec);
    manager->registerFactory(profile,
                             RTC::Create<Localization_MRPT>,
                             RTC::Delete<Localization_MRPT>);
  }
  
};


