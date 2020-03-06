
#include "core/EventLoop.h"
#include "core/geant/RunAction.h"
#include "core/geant/Analysis.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction( std::vector<IAlgTool*> sequence, std::string output )
 : G4UserRunAction(),
   m_sequence(sequence),
   m_output(output)
{;}


RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}


G4Run* RunAction::GenerateRun()
{
  return new EventLoop(m_sequence, m_output);
}


void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{;}


void RunAction::EndOfRunAction(const G4Run* /*run*/)
{;}
