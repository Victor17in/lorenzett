
#include "EventInfo/EventInfo.h"
#include "G4Kernel/ParticleGun.h"
#include "G4Kernel/EventLoop.h"
#include "G4RunManager.hh"
#include "G4LorentzVector.hh"
#include "G4Event.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4TransportationManager.hh"
//#include "TVector3.h"


ParticleGun::ParticleGun():
    IMsgService("ParticleGun"),
    PropertyService(),
    m_evt(0)
{
  declareProperty( "EventKey"           , m_eventKey="EventInfo"    );
  declareProperty( "NumberOfParticles"  , m_nofParticles=1          ); 
  declareProperty( "Particle"           , m_particle="-e"           );
  declareProperty( "ParticleEnergy"     , m_particleEnergy=50.*MeV  );
  declareProperty( "Direction"          , m_direction={0,1,0}       );


  m_gun = new G4ParticleGun(m_nofParticles);
  G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(m_particle);
  m_gun->SetParticleDefinition(particleDefinition);
  
  m_gun->SetParticleMomentumDirection(G4ThreeVector(m_direction.at(0),
                                                    m_direction.at(1),
                                                    m_direction.at(2)));
  m_gun->SetParticleEnergy(m_particleEnergy);

}


ParticleGun::~ParticleGun()
{
  delete m_gun;
}


void ParticleGun::Initialize()
{

}



// Call by geant
void ParticleGun::GeneratePrimaryVertex( G4Event* anEvent )
{
  m_evt++;
  EventLoop *loop = static_cast<EventLoop*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());


 
  
  SG::WriteHandle<xAOD::EventInfoContainer>  event(m_eventKey, loop->getContext());
  event.record( std::unique_ptr<xAOD::EventInfoContainer>( new xAOD::EventInfoContainer() ) );
  xAOD::EventInfo *evt = new xAOD::EventInfo();
 

  //G4ThreeVector pos(m_direction.at(0)*mm, m_direction.at(1)*mm, m_direction.at(2)*mm);
  //TVector3 vpos( pos.x(), pos.y(), pos.z());
  //float eta = vpos.PseudoRapidity();
  //float phi = vpos.Phi();



  xAOD::seed_t seed{m_particleEnergy, 0, 0, 0};

  evt->setEventNumber( m_evt );
  evt->setAvgmu( 0.0 );
  evt->push_back(seed);

  m_gun->GeneratePrimaryVertex(anEvent);

  MSG_INFO( "Event id         : " << evt->eventNumber() );
  MSG_INFO( "Number of seeds  : " << evt->size() );
      
  event->push_back(evt);
}


bool ParticleGun::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* world= navigator->GetWorldVolume();
  G4VSolid* solid= world-> GetLogicalVolume()->GetSolid();
  EInside qinside= solid->Inside(pos);
  if( qinside != kInside) return false;
  else return true;
}

