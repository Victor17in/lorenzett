#include "OptimalFilter.h"

using namespace Gaugi;


OptimalFilter::OptimalFilter( std::string name ) : 
  IMsgService(name),
  CaloTool()
{
  declareProperty( "OutputLevel", m_outputLevel=1 );
}



OptimalFilter::~OptimalFilter()
{}


StatusCode OptimalFilter::initialize()
{
  setMsgLevel(m_outputLevel);
  m_ofweights = {-0.3781, -0.3572, 0.1808, 0.8125, 0.2767, -0.2056, -0.3292};
  return StatusCode::SUCCESS;
}


StatusCode OptimalFilter::finalize()
{
  return StatusCode::SUCCESS;
}



StatusCode OptimalFilter::executeTool( xAOD::RawCell *cell ) const
{
	auto pulse = cell->pulse();
	float energy=0.0;

	if( m_ofweights.size() != pulse.size() ){
		MSG_ERROR( "The ofweights size its different than the pulse size." );
		return StatusCode::FAILURE;
	}else{
		for( unsigned sample=0; sample < pulse.size(); ++sample) 
			energy += pulse[sample]*m_ofweights[sample];
	}

	cell->setEnergy(energy);
	return StatusCode::SUCCESS;
}


// Just for python import in ROOT
StatusCode OptimalFilter::executeTool( xAOD::CaloCell * ) const {return StatusCode::SUCCESS;}
StatusCode OptimalFilter::executeTool( xAOD::CaloCluster * ) const {return StatusCode::SUCCESS;}
StatusCode OptimalFilter::executeTool( xAOD::TruthParticle * ) const {return StatusCode::SUCCESS;}


