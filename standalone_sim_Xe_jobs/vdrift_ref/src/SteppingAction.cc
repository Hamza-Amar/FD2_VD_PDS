// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#include "G4SystemOfUnits.hh"
#include "G4SteppingManager.hh"

#include "SteppingAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Alpha.hh"
#include "g4root.hh"

SteppingAction::SteppingAction(RunAction* run, DetectorConstruction* det)
:fRun(run),fDetector(det)
{ 
  idx=0;
  eveti = 0;
}

SteppingAction::~SteppingAction()
{ 

  for(p=imap.begin(); p!=imap.end(); p++){
    G4cout << p->first << " " << p->second << G4endl;
  }

}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{ 
  
  // Analysis manager
  
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  
  /*man->FillNtupleIColumn(1,0,fRun->GetNumEvent());
  man->FillNtupleIColumn(1,1,aStep->GetTrack()->GetDynamicParticle()->GetPDGcode());
  man->FillNtupleIColumn(1,2,aStep->GetTrack()->GetTrackID());
  man->FillNtupleIColumn(1,3,aStep->GetTrack()->GetParentID());
  man->FillNtupleDColumn(1,4,aStep->GetTrack()->GetPosition().x()/nm);
  man->FillNtupleDColumn(1,5,aStep->GetTrack()->GetPosition().y()/nm);
  man->FillNtupleDColumn(1,6,aStep->GetTrack()->GetPosition().z()/nm);
  man->FillNtupleDColumn(1,7,aStep->GetTotalEnergyDeposit()/eV);
  man->FillNtupleDColumn(1,8,aStep->GetTrack()->GetKineticEnergy()/eV);*/

  
  /*  G4cout << "evt: "<< fRun->GetNumEvent();
    G4cout << ", pdg: " << aStep->GetTrack()->GetDynamicParticle()->GetPDGcode();
    G4cout << ", name: " << aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
    G4cout << ", pid: " << aStep->GetTrack()->GetTrackID();
    G4cout << ", mid: " << aStep->GetTrack()->GetParentID();
    G4cout << ", x:   " << aStep->GetTrack()->GetPosition().x()/um;
    G4cout << ", y:   " << aStep->GetTrack()->GetPosition().y()/um;
    G4cout << ", z:   " << aStep->GetTrack()->GetPosition().z()/um;
    G4cout << ", de:  " << aStep->GetTotalEnergyDeposit()/eV;
    G4cout << ", e:   " << aStep->GetTrack()->GetKineticEnergy()/eV; */

    if(aStep->GetTrack()->GetNextVolume()!=0){
      // G4cout << ", vol: " << aStep->GetTrack()->GetNextVolume()->GetName();
      std::pair<int,int> aux = VolumeCode( aStep->GetTrack()->GetNextVolume()->GetName());
      //if (aux.first < 5) return; // since we are not writing the ntuple and only filling one histo, this return statement is not necessary
      G4int hv_id = man->GetH1Id("hv"); // get histogram int identifier, searched by histogram name
      man->FillH1(hv_id,aux.first); // fill histogram at thos volume code value, with weight 1
      /*      man->FillNtupleIColumn(1,9,aux.first);
	      man->FillNtupleIColumn(1,10,aStep->GetPostStepPoint()->GetTouchableHandle()->GetReplicaNumber());*/
      //      G4cout << " " << aux.first;
      //G4cout << " " << aStep->GetPostStepPoint()->GetTouchableHandle()->GetReplicaNumber();
      //if(aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName()!="World"){
	/*	man->FillNtupleIColumn(1,11,aux.second);
		man->FillNtupleIColumn(1,12,aStep->GetPostStepPoint()->GetTouchableHandle()->GetReplicaNumber(1));*/
	//	G4cout << " " << aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume(1)->GetName();
	//G4cout << " " << aStep->GetPostStepPoint()->GetTouchableHandle()->GetReplicaNumber(1);
	//G4cout << " " << aux.second;
      //}else{
	/*	man->FillNtupleIColumn(1,11,aux.second);
		man->FillNtupleIColumn(1,12,0);*/
	//G4cout << " " << "OutOfWorld";
	//G4cout << " " << aux.second;
        //G4cout << " " << 0;
       
      //}
      }else{
      return;
      std::pair<int,int> aux = VolumeCode("OutOfWorld");
      //G4cout << ", vol: OutOfWorld";
      //G4cout << " " << aux.first;
      //G4cout << " " << 0;
      //G4cout << " " << "OutOfWorld";
      //G4cout << " " << aux.second;
      //G4cout << " " << 0;
      G4int hv_id = man->GetH1Id("hv");
      man->FillH1(hv_id,aux.first);
      /*      man->FillNtupleIColumn(1,9,aux.first);
      man->FillNtupleIColumn(1,10,0);
      man->FillNtupleIColumn(1,11,aux.second);
      man->FillNtupleIColumn(1,12,0);*/
    }
    
    if(aStep->GetPostStepPoint()->GetProcessDefinedStep()!=NULL){
      std::string name = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
      //G4cout << ", process: " << name << G4endl;
      p = imap.find(name);
      //if(p!=imap.end()){
	//G4cout << "repeated" <<" "<< p->first <<" "<< p->second << G4endl;
	//	man->FillNtupleIColumn(1,13,p->second);
      //}
      if(p==imap.end()){
	idx++;
	std::pair<std::string, int> aux(name,idx);
	imap.insert(aux);
	//man->FillNtupleIColumn(1,13,idx);
      }

      if(eveti!=fRun->GetNumEvent()){
	eveti=fRun->GetNumEvent();
	//for(p=imap.begin(); p!=imap.end(); p++){
	  // G4cout << p->first << " " << p->second << G4endl;
	//}
	//G4cout << "n: " << idx << G4endl;
      }
      
      }//else{
      // G4cout << ", process: User Limit" << G4endl;
      //man->FillNtupleIColumn(1,13,-1);
//  }
    // man->AddNtupleRow(1); // comment out filling on ntuple for now, as volume code histogram is sufficient

}

std::pair<int,int> SteppingAction::VolumeCode(std::string name){

  if(name=="OutOfWorld"){std::pair<int,int> aux(-1,-1); return aux;}
  if(name=="World"){std::pair<int,int> aux(0,-1); return aux;}
  if(name=="Shell"){std::pair<int,int> aux(1,0); return aux;}
  if(name=="Cathode"){std::pair<int,int> aux(2,0); return aux;}
  if(name=="Anode"){std::pair<int,int> aux(3,0); return aux;}
  if(name=="FieldCage"){std::pair<int,int> aux(4,0); return aux;}
if(name=="ArapucaWindowLat11"){std::pair<int,int> aux(5,0); return aux;}
if(name=="ArapucaWindowLat12"){std::pair<int,int> aux(6,0); return aux;}
if(name=="ArapucaWindowLat13"){std::pair<int,int> aux(7,0); return aux;}
if(name=="ArapucaWindowLat14"){std::pair<int,int> aux(8,0); return aux;}
if(name=="ArapucaWindowLat15"){std::pair<int,int> aux(9,0); return aux;}
if(name=="ArapucaWindowLat16"){std::pair<int,int> aux(10,0); return aux;}
if(name=="ArapucaWindowLat17"){std::pair<int,int> aux(11,0); return aux;}
if(name=="ArapucaWindowLat18"){std::pair<int,int> aux(12,0); return aux;}
if(name=="ArapucaWindowLat19"){std::pair<int,int> aux(13,0); return aux;}
if(name=="ArapucaWindowLat110"){std::pair<int,int> aux(14,0); return aux;}
if(name=="ArapucaWindowLat111"){std::pair<int,int> aux(15,0); return aux;}
if(name=="ArapucaWindowLat112"){std::pair<int,int> aux(16,0); return aux;}
if(name=="ArapucaWindowLat113"){std::pair<int,int> aux(17,0); return aux;}
if(name=="ArapucaWindowLat114"){std::pair<int,int> aux(18,0); return aux;}
if(name=="ArapucaWindowLat115"){std::pair<int,int> aux(19,0); return aux;}
if(name=="ArapucaWindowLat116"){std::pair<int,int> aux(20,0); return aux;}
if(name=="ArapucaWindowLat117"){std::pair<int,int> aux(21,0); return aux;}
if(name=="ArapucaWindowLat118"){std::pair<int,int> aux(22,0); return aux;}
if(name=="ArapucaWindowLat119"){std::pair<int,int> aux(23,0); return aux;}
if(name=="ArapucaWindowLat120"){std::pair<int,int> aux(24,0); return aux;}
if(name=="ArapucaWindowLat21"){std::pair<int,int> aux(25,0); return aux;}
if(name=="ArapucaWindowLat22"){std::pair<int,int> aux(26,0); return aux;}
if(name=="ArapucaWindowLat23"){std::pair<int,int> aux(27,0); return aux;}
if(name=="ArapucaWindowLat24"){std::pair<int,int> aux(28,0); return aux;}
if(name=="ArapucaWindowLat25"){std::pair<int,int> aux(29,0); return aux;}
if(name=="ArapucaWindowLat26"){std::pair<int,int> aux(30,0); return aux;}
if(name=="ArapucaWindowLat27"){std::pair<int,int> aux(31,0); return aux;}
if(name=="ArapucaWindowLat28"){std::pair<int,int> aux(32,0); return aux;}
if(name=="ArapucaWindowLat29"){std::pair<int,int> aux(33,0); return aux;}
if(name=="ArapucaWindowLat210"){std::pair<int,int> aux(34,0); return aux;}
if(name=="ArapucaWindowLat211"){std::pair<int,int> aux(35,0); return aux;}
if(name=="ArapucaWindowLat212"){std::pair<int,int> aux(36,0); return aux;}
if(name=="ArapucaWindowLat213"){std::pair<int,int> aux(37,0); return aux;}
if(name=="ArapucaWindowLat214"){std::pair<int,int> aux(38,0); return aux;}
if(name=="ArapucaWindowLat215"){std::pair<int,int> aux(39,0); return aux;}
if(name=="ArapucaWindowLat216"){std::pair<int,int> aux(40,0); return aux;}
if(name=="ArapucaWindowLat217"){std::pair<int,int> aux(41,0); return aux;}
if(name=="ArapucaWindowLat218"){std::pair<int,int> aux(42,0); return aux;}
if(name=="ArapucaWindowLat219"){std::pair<int,int> aux(43,0); return aux;}
if(name=="ArapucaWindowLat220"){std::pair<int,int> aux(44,0); return aux;}
if(name=="ArapucaWindowLat31"){std::pair<int,int> aux(45,0); return aux;}
if(name=="ArapucaWindowLat32"){std::pair<int,int> aux(46,0); return aux;}
if(name=="ArapucaWindowLat33"){std::pair<int,int> aux(47,0); return aux;}
if(name=="ArapucaWindowLat34"){std::pair<int,int> aux(48,0); return aux;}
if(name=="ArapucaWindowLat35"){std::pair<int,int> aux(49,0); return aux;}
if(name=="ArapucaWindowLat36"){std::pair<int,int> aux(50,0); return aux;}
if(name=="ArapucaWindowLat37"){std::pair<int,int> aux(51,0); return aux;}
if(name=="ArapucaWindowLat38"){std::pair<int,int> aux(52,0); return aux;}
if(name=="ArapucaWindowLat39"){std::pair<int,int> aux(53,0); return aux;}
if(name=="ArapucaWindowLat310"){std::pair<int,int> aux(54,0); return aux;}
if(name=="ArapucaWindowLat311"){std::pair<int,int> aux(55,0); return aux;}
if(name=="ArapucaWindowLat312"){std::pair<int,int> aux(56,0); return aux;}
if(name=="ArapucaWindowLat313"){std::pair<int,int> aux(57,0); return aux;}
if(name=="ArapucaWindowLat314"){std::pair<int,int> aux(58,0); return aux;}
if(name=="ArapucaWindowLat315"){std::pair<int,int> aux(59,0); return aux;}
if(name=="ArapucaWindowLat316"){std::pair<int,int> aux(60,0); return aux;}
if(name=="ArapucaWindowLat317"){std::pair<int,int> aux(61,0); return aux;}
if(name=="ArapucaWindowLat318"){std::pair<int,int> aux(62,0); return aux;}
if(name=="ArapucaWindowLat319"){std::pair<int,int> aux(63,0); return aux;}
if(name=="ArapucaWindowLat320"){std::pair<int,int> aux(64,0); return aux;}
if(name=="ArapucaWindowLat41"){std::pair<int,int> aux(65,0); return aux;}
if(name=="ArapucaWindowLat42"){std::pair<int,int> aux(66,0); return aux;}
if(name=="ArapucaWindowLat43"){std::pair<int,int> aux(67,0); return aux;}
if(name=="ArapucaWindowLat44"){std::pair<int,int> aux(68,0); return aux;}
if(name=="ArapucaWindowLat45"){std::pair<int,int> aux(69,0); return aux;}
if(name=="ArapucaWindowLat46"){std::pair<int,int> aux(70,0); return aux;}
if(name=="ArapucaWindowLat47"){std::pair<int,int> aux(71,0); return aux;}
if(name=="ArapucaWindowLat48"){std::pair<int,int> aux(72,0); return aux;}
if(name=="ArapucaWindowLat49"){std::pair<int,int> aux(73,0); return aux;}
if(name=="ArapucaWindowLat410"){std::pair<int,int> aux(74,0); return aux;}
if(name=="ArapucaWindowLat411"){std::pair<int,int> aux(75,0); return aux;}
if(name=="ArapucaWindowLat412"){std::pair<int,int> aux(76,0); return aux;}
if(name=="ArapucaWindowLat413"){std::pair<int,int> aux(77,0); return aux;}
if(name=="ArapucaWindowLat414"){std::pair<int,int> aux(78,0); return aux;}
if(name=="ArapucaWindowLat415"){std::pair<int,int> aux(79,0); return aux;}
if(name=="ArapucaWindowLat416"){std::pair<int,int> aux(80,0); return aux;}
if(name=="ArapucaWindowLat417"){std::pair<int,int> aux(81,0); return aux;}
if(name=="ArapucaWindowLat418"){std::pair<int,int> aux(82,0); return aux;}
if(name=="ArapucaWindowLat419"){std::pair<int,int> aux(83,0); return aux;}
if(name=="ArapucaWindowLat420"){std::pair<int,int> aux(84,0); return aux;}
if(name=="ArapucaWindowLlat11"){std::pair<int,int> aux(85,0); return aux;}
if(name=="ArapucaWindowLlat12"){std::pair<int,int> aux(86,0); return aux;}
if(name=="ArapucaWindowLlat13"){std::pair<int,int> aux(87,0); return aux;}
if(name=="ArapucaWindowLlat14"){std::pair<int,int> aux(88,0); return aux;}
if(name=="ArapucaWindowLlat15"){std::pair<int,int> aux(89,0); return aux;}
if(name=="ArapucaWindowLlat16"){std::pair<int,int> aux(90,0); return aux;}
if(name=="ArapucaWindowLlat17"){std::pair<int,int> aux(91,0); return aux;}
if(name=="ArapucaWindowLlat18"){std::pair<int,int> aux(92,0); return aux;}
if(name=="ArapucaWindowLlat19"){std::pair<int,int> aux(93,0); return aux;}
if(name=="ArapucaWindowLlat110"){std::pair<int,int> aux(94,0); return aux;}
if(name=="ArapucaWindowLlat111"){std::pair<int,int> aux(95,0); return aux;}
if(name=="ArapucaWindowLlat112"){std::pair<int,int> aux(96,0); return aux;}
if(name=="ArapucaWindowLlat113"){std::pair<int,int> aux(97,0); return aux;}
if(name=="ArapucaWindowLlat114"){std::pair<int,int> aux(98,0); return aux;}
if(name=="ArapucaWindowLlat115"){std::pair<int,int> aux(99,0); return aux;}
if(name=="ArapucaWindowLlat116"){std::pair<int,int> aux(100,0); return aux;}
if(name=="ArapucaWindowLlat117"){std::pair<int,int> aux(101,0); return aux;}
if(name=="ArapucaWindowLlat118"){std::pair<int,int> aux(102,0); return aux;}
if(name=="ArapucaWindowLlat119"){std::pair<int,int> aux(103,0); return aux;}
if(name=="ArapucaWindowLlat120"){std::pair<int,int> aux(104,0); return aux;}
if(name=="ArapucaWindowLlat21"){std::pair<int,int> aux(105,0); return aux;}
if(name=="ArapucaWindowLlat22"){std::pair<int,int> aux(106,0); return aux;}
if(name=="ArapucaWindowLlat23"){std::pair<int,int> aux(107,0); return aux;}
if(name=="ArapucaWindowLlat24"){std::pair<int,int> aux(108,0); return aux;}
if(name=="ArapucaWindowLlat25"){std::pair<int,int> aux(109,0); return aux;}
if(name=="ArapucaWindowLlat26"){std::pair<int,int> aux(110,0); return aux;}
if(name=="ArapucaWindowLlat27"){std::pair<int,int> aux(111,0); return aux;}
if(name=="ArapucaWindowLlat28"){std::pair<int,int> aux(112,0); return aux;}
if(name=="ArapucaWindowLlat29"){std::pair<int,int> aux(113,0); return aux;}
if(name=="ArapucaWindowLlat210"){std::pair<int,int> aux(114,0); return aux;}
if(name=="ArapucaWindowLlat211"){std::pair<int,int> aux(115,0); return aux;}
if(name=="ArapucaWindowLlat212"){std::pair<int,int> aux(116,0); return aux;}
if(name=="ArapucaWindowLlat213"){std::pair<int,int> aux(117,0); return aux;}
if(name=="ArapucaWindowLlat214"){std::pair<int,int> aux(118,0); return aux;}
if(name=="ArapucaWindowLlat215"){std::pair<int,int> aux(119,0); return aux;}
if(name=="ArapucaWindowLlat216"){std::pair<int,int> aux(120,0); return aux;}
if(name=="ArapucaWindowLlat217"){std::pair<int,int> aux(121,0); return aux;}
if(name=="ArapucaWindowLlat218"){std::pair<int,int> aux(122,0); return aux;}
if(name=="ArapucaWindowLlat219"){std::pair<int,int> aux(123,0); return aux;}
if(name=="ArapucaWindowLlat220"){std::pair<int,int> aux(124,0); return aux;}
if(name=="ArapucaWindowLlat31"){std::pair<int,int> aux(125,0); return aux;}
if(name=="ArapucaWindowLlat32"){std::pair<int,int> aux(126,0); return aux;}
if(name=="ArapucaWindowLlat33"){std::pair<int,int> aux(127,0); return aux;}
if(name=="ArapucaWindowLlat34"){std::pair<int,int> aux(128,0); return aux;}
if(name=="ArapucaWindowLlat35"){std::pair<int,int> aux(129,0); return aux;}
if(name=="ArapucaWindowLlat36"){std::pair<int,int> aux(130,0); return aux;}
if(name=="ArapucaWindowLlat37"){std::pair<int,int> aux(131,0); return aux;}
if(name=="ArapucaWindowLlat38"){std::pair<int,int> aux(132,0); return aux;}
if(name=="ArapucaWindowLlat39"){std::pair<int,int> aux(133,0); return aux;}
if(name=="ArapucaWindowLlat310"){std::pair<int,int> aux(134,0); return aux;}
if(name=="ArapucaWindowLlat311"){std::pair<int,int> aux(135,0); return aux;}
if(name=="ArapucaWindowLlat312"){std::pair<int,int> aux(136,0); return aux;}
if(name=="ArapucaWindowLlat313"){std::pair<int,int> aux(137,0); return aux;}
if(name=="ArapucaWindowLlat314"){std::pair<int,int> aux(138,0); return aux;}
if(name=="ArapucaWindowLlat315"){std::pair<int,int> aux(139,0); return aux;}
if(name=="ArapucaWindowLlat316"){std::pair<int,int> aux(140,0); return aux;}
if(name=="ArapucaWindowLlat317"){std::pair<int,int> aux(141,0); return aux;}
if(name=="ArapucaWindowLlat318"){std::pair<int,int> aux(142,0); return aux;}
if(name=="ArapucaWindowLlat319"){std::pair<int,int> aux(143,0); return aux;}
if(name=="ArapucaWindowLlat320"){std::pair<int,int> aux(144,0); return aux;}
if(name=="ArapucaWindowLlat41"){std::pair<int,int> aux(145,0); return aux;}
if(name=="ArapucaWindowLlat42"){std::pair<int,int> aux(146,0); return aux;}
if(name=="ArapucaWindowLlat43"){std::pair<int,int> aux(147,0); return aux;}
if(name=="ArapucaWindowLlat44"){std::pair<int,int> aux(148,0); return aux;}
if(name=="ArapucaWindowLlat45"){std::pair<int,int> aux(149,0); return aux;}
if(name=="ArapucaWindowLlat46"){std::pair<int,int> aux(150,0); return aux;}
if(name=="ArapucaWindowLlat47"){std::pair<int,int> aux(151,0); return aux;}
if(name=="ArapucaWindowLlat48"){std::pair<int,int> aux(152,0); return aux;}
if(name=="ArapucaWindowLlat49"){std::pair<int,int> aux(153,0); return aux;}
if(name=="ArapucaWindowLlat410"){std::pair<int,int> aux(154,0); return aux;}
if(name=="ArapucaWindowLlat411"){std::pair<int,int> aux(155,0); return aux;}
if(name=="ArapucaWindowLlat412"){std::pair<int,int> aux(156,0); return aux;}
if(name=="ArapucaWindowLlat413"){std::pair<int,int> aux(157,0); return aux;}
if(name=="ArapucaWindowLlat414"){std::pair<int,int> aux(158,0); return aux;}
if(name=="ArapucaWindowLlat415"){std::pair<int,int> aux(159,0); return aux;}
if(name=="ArapucaWindowLlat416"){std::pair<int,int> aux(160,0); return aux;}
if(name=="ArapucaWindowLlat417"){std::pair<int,int> aux(161,0); return aux;}
if(name=="ArapucaWindowLlat418"){std::pair<int,int> aux(162,0); return aux;}
if(name=="ArapucaWindowLlat419"){std::pair<int,int> aux(163,0); return aux;}
if(name=="ArapucaWindowLlat420"){std::pair<int,int> aux(164,0); return aux;}
if(name=="ArapucaWindowBot11"){std::pair<int,int> aux(165,0); return aux;}
if(name=="ArapucaWindowBot12"){std::pair<int,int> aux(166,0); return aux;}
if(name=="ArapucaWindowBot13"){std::pair<int,int> aux(167,0); return aux;}
if(name=="ArapucaWindowBot14"){std::pair<int,int> aux(168,0); return aux;}
if(name=="ArapucaWindowBot15"){std::pair<int,int> aux(169,0); return aux;}
if(name=="ArapucaWindowBot16"){std::pair<int,int> aux(170,0); return aux;}
if(name=="ArapucaWindowBot17"){std::pair<int,int> aux(171,0); return aux;}
if(name=="ArapucaWindowBot18"){std::pair<int,int> aux(172,0); return aux;}
if(name=="ArapucaWindowBot21"){std::pair<int,int> aux(173,0); return aux;}
if(name=="ArapucaWindowBot22"){std::pair<int,int> aux(174,0); return aux;}
if(name=="ArapucaWindowBot23"){std::pair<int,int> aux(175,0); return aux;}
if(name=="ArapucaWindowBot24"){std::pair<int,int> aux(176,0); return aux;}
if(name=="ArapucaWindowBot25"){std::pair<int,int> aux(177,0); return aux;}
if(name=="ArapucaWindowBot26"){std::pair<int,int> aux(178,0); return aux;}
if(name=="ArapucaWindowBot27"){std::pair<int,int> aux(179,0); return aux;}
if(name=="ArapucaWindowBot28"){std::pair<int,int> aux(180,0); return aux;}
if(name=="ArapucaWindowBot31"){std::pair<int,int> aux(181,0); return aux;}
if(name=="ArapucaWindowBot32"){std::pair<int,int> aux(182,0); return aux;}
if(name=="ArapucaWindowBot33"){std::pair<int,int> aux(183,0); return aux;}
if(name=="ArapucaWindowBot34"){std::pair<int,int> aux(184,0); return aux;}
if(name=="ArapucaWindowBot35"){std::pair<int,int> aux(185,0); return aux;}
if(name=="ArapucaWindowBot36"){std::pair<int,int> aux(186,0); return aux;}
if(name=="ArapucaWindowBot37"){std::pair<int,int> aux(187,0); return aux;}
if(name=="ArapucaWindowBot38"){std::pair<int,int> aux(188,0); return aux;}
if(name=="ArapucaWindowBot41"){std::pair<int,int> aux(189,0); return aux;}
if(name=="ArapucaWindowBot42"){std::pair<int,int> aux(190,0); return aux;}
if(name=="ArapucaWindowBot43"){std::pair<int,int> aux(191,0); return aux;}
if(name=="ArapucaWindowBot44"){std::pair<int,int> aux(192,0); return aux;}
if(name=="ArapucaWindowBot45"){std::pair<int,int> aux(193,0); return aux;}
if(name=="ArapucaWindowBot46"){std::pair<int,int> aux(194,0); return aux;}
if(name=="ArapucaWindowBot47"){std::pair<int,int> aux(195,0); return aux;}
if(name=="ArapucaWindowBot48"){std::pair<int,int> aux(196,0); return aux;}
if(name=="ArapucaWindowBot51"){std::pair<int,int> aux(197,0); return aux;}
if(name=="ArapucaWindowBot52"){std::pair<int,int> aux(198,0); return aux;}
if(name=="ArapucaWindowBot53"){std::pair<int,int> aux(199,0); return aux;}
if(name=="ArapucaWindowBot54"){std::pair<int,int> aux(200,0); return aux;}
if(name=="ArapucaWindowBot55"){std::pair<int,int> aux(201,0); return aux;}
if(name=="ArapucaWindowBot56"){std::pair<int,int> aux(202,0); return aux;}
if(name=="ArapucaWindowBot57"){std::pair<int,int> aux(203,0); return aux;}
if(name=="ArapucaWindowBot58"){std::pair<int,int> aux(204,0); return aux;}
if(name=="ArapucaWindowBot61"){std::pair<int,int> aux(205,0); return aux;}
if(name=="ArapucaWindowBot62"){std::pair<int,int> aux(206,0); return aux;}
if(name=="ArapucaWindowBot63"){std::pair<int,int> aux(207,0); return aux;}
if(name=="ArapucaWindowBot64"){std::pair<int,int> aux(208,0); return aux;}
if(name=="ArapucaWindowBot65"){std::pair<int,int> aux(209,0); return aux;}
if(name=="ArapucaWindowBot66"){std::pair<int,int> aux(210,0); return aux;}
if(name=="ArapucaWindowBot67"){std::pair<int,int> aux(211,0); return aux;}
if(name=="ArapucaWindowBot68"){std::pair<int,int> aux(212,0); return aux;}
if(name=="ArapucaWindowBot71"){std::pair<int,int> aux(213,0); return aux;}
if(name=="ArapucaWindowBot72"){std::pair<int,int> aux(214,0); return aux;}
if(name=="ArapucaWindowBot73"){std::pair<int,int> aux(215,0); return aux;}
if(name=="ArapucaWindowBot74"){std::pair<int,int> aux(216,0); return aux;}
if(name=="ArapucaWindowBot75"){std::pair<int,int> aux(217,0); return aux;}
if(name=="ArapucaWindowBot76"){std::pair<int,int> aux(218,0); return aux;}
if(name=="ArapucaWindowBot77"){std::pair<int,int> aux(219,0); return aux;}
if(name=="ArapucaWindowBot78"){std::pair<int,int> aux(220,0); return aux;}
if(name=="ArapucaWindowBot81"){std::pair<int,int> aux(221,0); return aux;}
if(name=="ArapucaWindowBot82"){std::pair<int,int> aux(222,0); return aux;}
if(name=="ArapucaWindowBot83"){std::pair<int,int> aux(223,0); return aux;}
if(name=="ArapucaWindowBot84"){std::pair<int,int> aux(224,0); return aux;}
if(name=="ArapucaWindowBot85"){std::pair<int,int> aux(225,0); return aux;}
if(name=="ArapucaWindowBot86"){std::pair<int,int> aux(226,0); return aux;}
if(name=="ArapucaWindowBot87"){std::pair<int,int> aux(227,0); return aux;}
if(name=="ArapucaWindowBot88"){std::pair<int,int> aux(228,0); return aux;}
if(name=="ArapucaWindowBot91"){std::pair<int,int> aux(229,0); return aux;}
if(name=="ArapucaWindowBot92"){std::pair<int,int> aux(230,0); return aux;}
if(name=="ArapucaWindowBot93"){std::pair<int,int> aux(231,0); return aux;}
if(name=="ArapucaWindowBot94"){std::pair<int,int> aux(232,0); return aux;}
if(name=="ArapucaWindowBot95"){std::pair<int,int> aux(233,0); return aux;}
if(name=="ArapucaWindowBot96"){std::pair<int,int> aux(234,0); return aux;}
if(name=="ArapucaWindowBot97"){std::pair<int,int> aux(235,0); return aux;}
if(name=="ArapucaWindowBot98"){std::pair<int,int> aux(236,0); return aux;}
if(name=="ArapucaWindowBot101"){std::pair<int,int> aux(237,0); return aux;}
if(name=="ArapucaWindowBot102"){std::pair<int,int> aux(238,0); return aux;}
if(name=="ArapucaWindowBot103"){std::pair<int,int> aux(239,0); return aux;}
if(name=="ArapucaWindowBot104"){std::pair<int,int> aux(240,0); return aux;}
if(name=="ArapucaWindowBot105"){std::pair<int,int> aux(241,0); return aux;}
if(name=="ArapucaWindowBot106"){std::pair<int,int> aux(242,0); return aux;}
if(name=="ArapucaWindowBot107"){std::pair<int,int> aux(243,0); return aux;}
if(name=="ArapucaWindowBot108"){std::pair<int,int> aux(244,0); return aux;}
if(name=="ArapucaWindowBot111"){std::pair<int,int> aux(245,0); return aux;}
if(name=="ArapucaWindowBot112"){std::pair<int,int> aux(246,0); return aux;}
if(name=="ArapucaWindowBot113"){std::pair<int,int> aux(247,0); return aux;}
if(name=="ArapucaWindowBot114"){std::pair<int,int> aux(248,0); return aux;}
if(name=="ArapucaWindowBot115"){std::pair<int,int> aux(249,0); return aux;}
if(name=="ArapucaWindowBot116"){std::pair<int,int> aux(250,0); return aux;}
if(name=="ArapucaWindowBot117"){std::pair<int,int> aux(251,0); return aux;}
if(name=="ArapucaWindowBot118"){std::pair<int,int> aux(252,0); return aux;}
if(name=="ArapucaWindowBot121"){std::pair<int,int> aux(253,0); return aux;}
if(name=="ArapucaWindowBot122"){std::pair<int,int> aux(254,0); return aux;}
if(name=="ArapucaWindowBot123"){std::pair<int,int> aux(255,0); return aux;}
if(name=="ArapucaWindowBot124"){std::pair<int,int> aux(256,0); return aux;}
if(name=="ArapucaWindowBot125"){std::pair<int,int> aux(257,0); return aux;}
if(name=="ArapucaWindowBot126"){std::pair<int,int> aux(258,0); return aux;}
if(name=="ArapucaWindowBot127"){std::pair<int,int> aux(259,0); return aux;}
if(name=="ArapucaWindowBot128"){std::pair<int,int> aux(260,0); return aux;}
if(name=="ArapucaWindowBot131"){std::pair<int,int> aux(261,0); return aux;}
if(name=="ArapucaWindowBot132"){std::pair<int,int> aux(262,0); return aux;}
if(name=="ArapucaWindowBot133"){std::pair<int,int> aux(263,0); return aux;}
if(name=="ArapucaWindowBot134"){std::pair<int,int> aux(264,0); return aux;}
if(name=="ArapucaWindowBot135"){std::pair<int,int> aux(265,0); return aux;}
if(name=="ArapucaWindowBot136"){std::pair<int,int> aux(266,0); return aux;}
if(name=="ArapucaWindowBot137"){std::pair<int,int> aux(267,0); return aux;}
if(name=="ArapucaWindowBot138"){std::pair<int,int> aux(268,0); return aux;}
if(name=="ArapucaWindowBot141"){std::pair<int,int> aux(269,0); return aux;}
if(name=="ArapucaWindowBot142"){std::pair<int,int> aux(270,0); return aux;}
if(name=="ArapucaWindowBot143"){std::pair<int,int> aux(271,0); return aux;}
if(name=="ArapucaWindowBot144"){std::pair<int,int> aux(272,0); return aux;}
if(name=="ArapucaWindowBot145"){std::pair<int,int> aux(273,0); return aux;}
if(name=="ArapucaWindowBot146"){std::pair<int,int> aux(274,0); return aux;}
if(name=="ArapucaWindowBot147"){std::pair<int,int> aux(275,0); return aux;}
if(name=="ArapucaWindowBot148"){std::pair<int,int> aux(276,0); return aux;}
if(name=="ArapucaWindowBot151"){std::pair<int,int> aux(277,0); return aux;}
if(name=="ArapucaWindowBot152"){std::pair<int,int> aux(278,0); return aux;}
if(name=="ArapucaWindowBot153"){std::pair<int,int> aux(279,0); return aux;}
if(name=="ArapucaWindowBot154"){std::pair<int,int> aux(280,0); return aux;}
if(name=="ArapucaWindowBot155"){std::pair<int,int> aux(281,0); return aux;}
if(name=="ArapucaWindowBot156"){std::pair<int,int> aux(282,0); return aux;}
if(name=="ArapucaWindowBot157"){std::pair<int,int> aux(283,0); return aux;}
if(name=="ArapucaWindowBot158"){std::pair<int,int> aux(284,0); return aux;}
if(name=="ArapucaWindowBot161"){std::pair<int,int> aux(285,0); return aux;}
if(name=="ArapucaWindowBot162"){std::pair<int,int> aux(286,0); return aux;}
if(name=="ArapucaWindowBot163"){std::pair<int,int> aux(287,0); return aux;}
if(name=="ArapucaWindowBot164"){std::pair<int,int> aux(288,0); return aux;}
if(name=="ArapucaWindowBot165"){std::pair<int,int> aux(289,0); return aux;}
if(name=="ArapucaWindowBot166"){std::pair<int,int> aux(290,0); return aux;}
if(name=="ArapucaWindowBot167"){std::pair<int,int> aux(291,0); return aux;}
if(name=="ArapucaWindowBot168"){std::pair<int,int> aux(292,0); return aux;}
if(name=="ArapucaWindowBot171"){std::pair<int,int> aux(293,0); return aux;}
if(name=="ArapucaWindowBot172"){std::pair<int,int> aux(294,0); return aux;}
if(name=="ArapucaWindowBot173"){std::pair<int,int> aux(295,0); return aux;}
if(name=="ArapucaWindowBot174"){std::pair<int,int> aux(296,0); return aux;}
if(name=="ArapucaWindowBot175"){std::pair<int,int> aux(297,0); return aux;}
if(name=="ArapucaWindowBot176"){std::pair<int,int> aux(298,0); return aux;}
if(name=="ArapucaWindowBot177"){std::pair<int,int> aux(299,0); return aux;}
if(name=="ArapucaWindowBot178"){std::pair<int,int> aux(300,0); return aux;}
if(name=="ArapucaWindowBot181"){std::pair<int,int> aux(301,0); return aux;}
if(name=="ArapucaWindowBot182"){std::pair<int,int> aux(302,0); return aux;}
if(name=="ArapucaWindowBot183"){std::pair<int,int> aux(303,0); return aux;}
if(name=="ArapucaWindowBot184"){std::pair<int,int> aux(304,0); return aux;}
if(name=="ArapucaWindowBot185"){std::pair<int,int> aux(305,0); return aux;}
if(name=="ArapucaWindowBot186"){std::pair<int,int> aux(306,0); return aux;}
if(name=="ArapucaWindowBot187"){std::pair<int,int> aux(307,0); return aux;}
if(name=="ArapucaWindowBot188"){std::pair<int,int> aux(308,0); return aux;}
if(name=="ArapucaWindowBot191"){std::pair<int,int> aux(309,0); return aux;}
if(name=="ArapucaWindowBot192"){std::pair<int,int> aux(310,0); return aux;}
if(name=="ArapucaWindowBot193"){std::pair<int,int> aux(311,0); return aux;}
if(name=="ArapucaWindowBot194"){std::pair<int,int> aux(312,0); return aux;}
if(name=="ArapucaWindowBot195"){std::pair<int,int> aux(313,0); return aux;}
if(name=="ArapucaWindowBot196"){std::pair<int,int> aux(314,0); return aux;}
if(name=="ArapucaWindowBot197"){std::pair<int,int> aux(315,0); return aux;}
if(name=="ArapucaWindowBot198"){std::pair<int,int> aux(316,0); return aux;}
if(name=="ArapucaWindowBot201"){std::pair<int,int> aux(317,0); return aux;}
if(name=="ArapucaWindowBot202"){std::pair<int,int> aux(318,0); return aux;}
if(name=="ArapucaWindowBot203"){std::pair<int,int> aux(319,0); return aux;}
if(name=="ArapucaWindowBot204"){std::pair<int,int> aux(320,0); return aux;}
if(name=="ArapucaWindowBot205"){std::pair<int,int> aux(321,0); return aux;}
if(name=="ArapucaWindowBot206"){std::pair<int,int> aux(322,0); return aux;}
if(name=="ArapucaWindowBot207"){std::pair<int,int> aux(323,0); return aux;}
if(name=="ArapucaWindowBot208"){std::pair<int,int> aux(324,0); return aux;}
if(name=="ArapucaWindowBot211"){std::pair<int,int> aux(325,0); return aux;}
if(name=="ArapucaWindowBot212"){std::pair<int,int> aux(326,0); return aux;}
if(name=="ArapucaWindowBot213"){std::pair<int,int> aux(327,0); return aux;}
if(name=="ArapucaWindowBot214"){std::pair<int,int> aux(328,0); return aux;}
if(name=="ArapucaWindowBot215"){std::pair<int,int> aux(329,0); return aux;}
if(name=="ArapucaWindowBot216"){std::pair<int,int> aux(330,0); return aux;}
if(name=="ArapucaWindowBot217"){std::pair<int,int> aux(331,0); return aux;}
if(name=="ArapucaWindowBot218"){std::pair<int,int> aux(332,0); return aux;}
if(name=="ArapucaWindowBot221"){std::pair<int,int> aux(333,0); return aux;}
if(name=="ArapucaWindowBot222"){std::pair<int,int> aux(334,0); return aux;}
if(name=="ArapucaWindowBot223"){std::pair<int,int> aux(335,0); return aux;}
if(name=="ArapucaWindowBot224"){std::pair<int,int> aux(336,0); return aux;}
if(name=="ArapucaWindowBot225"){std::pair<int,int> aux(337,0); return aux;}
if(name=="ArapucaWindowBot226"){std::pair<int,int> aux(338,0); return aux;}
if(name=="ArapucaWindowBot227"){std::pair<int,int> aux(339,0); return aux;}
if(name=="ArapucaWindowBot228"){std::pair<int,int> aux(340,0); return aux;}
if(name=="ArapucaWindowBot231"){std::pair<int,int> aux(341,0); return aux;}
if(name=="ArapucaWindowBot232"){std::pair<int,int> aux(342,0); return aux;}
if(name=="ArapucaWindowBot233"){std::pair<int,int> aux(343,0); return aux;}
if(name=="ArapucaWindowBot234"){std::pair<int,int> aux(344,0); return aux;}
if(name=="ArapucaWindowBot235"){std::pair<int,int> aux(345,0); return aux;}
if(name=="ArapucaWindowBot236"){std::pair<int,int> aux(346,0); return aux;}
if(name=="ArapucaWindowBot237"){std::pair<int,int> aux(347,0); return aux;}
if(name=="ArapucaWindowBot238"){std::pair<int,int> aux(348,0); return aux;}
if(name=="ArapucaWindowBot241"){std::pair<int,int> aux(349,0); return aux;}
if(name=="ArapucaWindowBot242"){std::pair<int,int> aux(350,0); return aux;}
if(name=="ArapucaWindowBot243"){std::pair<int,int> aux(351,0); return aux;}
if(name=="ArapucaWindowBot244"){std::pair<int,int> aux(352,0); return aux;}
if(name=="ArapucaWindowBot245"){std::pair<int,int> aux(353,0); return aux;}
if(name=="ArapucaWindowBot246"){std::pair<int,int> aux(354,0); return aux;}
if(name=="ArapucaWindowBot247"){std::pair<int,int> aux(355,0); return aux;}
if(name=="ArapucaWindowBot248"){std::pair<int,int> aux(356,0); return aux;}
if(name=="ArapucaWindowBot251"){std::pair<int,int> aux(357,0); return aux;}
if(name=="ArapucaWindowBot252"){std::pair<int,int> aux(358,0); return aux;}
if(name=="ArapucaWindowBot253"){std::pair<int,int> aux(359,0); return aux;}
if(name=="ArapucaWindowBot254"){std::pair<int,int> aux(360,0); return aux;}
if(name=="ArapucaWindowBot255"){std::pair<int,int> aux(361,0); return aux;}
if(name=="ArapucaWindowBot256"){std::pair<int,int> aux(362,0); return aux;}
if(name=="ArapucaWindowBot257"){std::pair<int,int> aux(363,0); return aux;}
if(name=="ArapucaWindowBot258"){std::pair<int,int> aux(364,0); return aux;}
if(name=="ArapucaWindowBot261"){std::pair<int,int> aux(365,0); return aux;}
if(name=="ArapucaWindowBot262"){std::pair<int,int> aux(366,0); return aux;}
if(name=="ArapucaWindowBot263"){std::pair<int,int> aux(367,0); return aux;}
if(name=="ArapucaWindowBot264"){std::pair<int,int> aux(368,0); return aux;}
if(name=="ArapucaWindowBot265"){std::pair<int,int> aux(369,0); return aux;}
if(name=="ArapucaWindowBot266"){std::pair<int,int> aux(370,0); return aux;}
if(name=="ArapucaWindowBot267"){std::pair<int,int> aux(371,0); return aux;}
if(name=="ArapucaWindowBot268"){std::pair<int,int> aux(372,0); return aux;}
if(name=="ArapucaWindowBot271"){std::pair<int,int> aux(373,0); return aux;}
if(name=="ArapucaWindowBot272"){std::pair<int,int> aux(374,0); return aux;}
if(name=="ArapucaWindowBot273"){std::pair<int,int> aux(375,0); return aux;}
if(name=="ArapucaWindowBot274"){std::pair<int,int> aux(376,0); return aux;}
if(name=="ArapucaWindowBot275"){std::pair<int,int> aux(377,0); return aux;}
if(name=="ArapucaWindowBot276"){std::pair<int,int> aux(378,0); return aux;}
if(name=="ArapucaWindowBot277"){std::pair<int,int> aux(379,0); return aux;}
if(name=="ArapucaWindowBot278"){std::pair<int,int> aux(380,0); return aux;}
if(name=="ArapucaWindowBot281"){std::pair<int,int> aux(381,0); return aux;}
if(name=="ArapucaWindowBot282"){std::pair<int,int> aux(382,0); return aux;}
if(name=="ArapucaWindowBot283"){std::pair<int,int> aux(383,0); return aux;}
if(name=="ArapucaWindowBot284"){std::pair<int,int> aux(384,0); return aux;}
if(name=="ArapucaWindowBot285"){std::pair<int,int> aux(385,0); return aux;}
if(name=="ArapucaWindowBot286"){std::pair<int,int> aux(386,0); return aux;}
if(name=="ArapucaWindowBot287"){std::pair<int,int> aux(387,0); return aux;}
if(name=="ArapucaWindowBot288"){std::pair<int,int> aux(388,0); return aux;}
if(name=="ArapucaWindowBot291"){std::pair<int,int> aux(389,0); return aux;}
if(name=="ArapucaWindowBot292"){std::pair<int,int> aux(390,0); return aux;}
if(name=="ArapucaWindowBot293"){std::pair<int,int> aux(391,0); return aux;}
if(name=="ArapucaWindowBot294"){std::pair<int,int> aux(392,0); return aux;}
if(name=="ArapucaWindowBot295"){std::pair<int,int> aux(393,0); return aux;}
if(name=="ArapucaWindowBot296"){std::pair<int,int> aux(394,0); return aux;}
if(name=="ArapucaWindowBot297"){std::pair<int,int> aux(395,0); return aux;}
if(name=="ArapucaWindowBot298"){std::pair<int,int> aux(396,0); return aux;}
if(name=="ArapucaWindowBot301"){std::pair<int,int> aux(397,0); return aux;}
if(name=="ArapucaWindowBot302"){std::pair<int,int> aux(398,0); return aux;}
if(name=="ArapucaWindowBot303"){std::pair<int,int> aux(399,0); return aux;}
if(name=="ArapucaWindowBot304"){std::pair<int,int> aux(400,0); return aux;}
if(name=="ArapucaWindowBot305"){std::pair<int,int> aux(401,0); return aux;}
if(name=="ArapucaWindowBot306"){std::pair<int,int> aux(402,0); return aux;}
if(name=="ArapucaWindowBot307"){std::pair<int,int> aux(403,0); return aux;}
if(name=="ArapucaWindowBot308"){std::pair<int,int> aux(404,0); return aux;}
if(name=="ArapucaWindowBot311"){std::pair<int,int> aux(405,0); return aux;}
if(name=="ArapucaWindowBot312"){std::pair<int,int> aux(406,0); return aux;}
if(name=="ArapucaWindowBot313"){std::pair<int,int> aux(407,0); return aux;}
if(name=="ArapucaWindowBot314"){std::pair<int,int> aux(408,0); return aux;}
if(name=="ArapucaWindowBot315"){std::pair<int,int> aux(409,0); return aux;}
if(name=="ArapucaWindowBot316"){std::pair<int,int> aux(410,0); return aux;}
if(name=="ArapucaWindowBot317"){std::pair<int,int> aux(411,0); return aux;}
if(name=="ArapucaWindowBot318"){std::pair<int,int> aux(412,0); return aux;}
if(name=="ArapucaWindowBot321"){std::pair<int,int> aux(413,0); return aux;}
if(name=="ArapucaWindowBot322"){std::pair<int,int> aux(414,0); return aux;}
if(name=="ArapucaWindowBot323"){std::pair<int,int> aux(415,0); return aux;}
if(name=="ArapucaWindowBot324"){std::pair<int,int> aux(416,0); return aux;}
if(name=="ArapucaWindowBot325"){std::pair<int,int> aux(417,0); return aux;}
if(name=="ArapucaWindowBot326"){std::pair<int,int> aux(418,0); return aux;}
if(name=="ArapucaWindowBot327"){std::pair<int,int> aux(419,0); return aux;}
if(name=="ArapucaWindowBot328"){std::pair<int,int> aux(420,0); return aux;}
if(name=="ArapucaWindowBot331"){std::pair<int,int> aux(421,0); return aux;}
if(name=="ArapucaWindowBot332"){std::pair<int,int> aux(422,0); return aux;}
if(name=="ArapucaWindowBot333"){std::pair<int,int> aux(423,0); return aux;}
if(name=="ArapucaWindowBot334"){std::pair<int,int> aux(424,0); return aux;}
if(name=="ArapucaWindowBot335"){std::pair<int,int> aux(425,0); return aux;}
if(name=="ArapucaWindowBot336"){std::pair<int,int> aux(426,0); return aux;}
if(name=="ArapucaWindowBot337"){std::pair<int,int> aux(427,0); return aux;}
if(name=="ArapucaWindowBot338"){std::pair<int,int> aux(428,0); return aux;}
if(name=="ArapucaWindowBot341"){std::pair<int,int> aux(429,0); return aux;}
if(name=="ArapucaWindowBot342"){std::pair<int,int> aux(430,0); return aux;}
if(name=="ArapucaWindowBot343"){std::pair<int,int> aux(431,0); return aux;}
if(name=="ArapucaWindowBot344"){std::pair<int,int> aux(432,0); return aux;}
if(name=="ArapucaWindowBot345"){std::pair<int,int> aux(433,0); return aux;}
if(name=="ArapucaWindowBot346"){std::pair<int,int> aux(434,0); return aux;}
if(name=="ArapucaWindowBot347"){std::pair<int,int> aux(435,0); return aux;}
if(name=="ArapucaWindowBot348"){std::pair<int,int> aux(436,0); return aux;}
if(name=="ArapucaWindowBot351"){std::pair<int,int> aux(437,0); return aux;}
if(name=="ArapucaWindowBot352"){std::pair<int,int> aux(438,0); return aux;}
if(name=="ArapucaWindowBot353"){std::pair<int,int> aux(439,0); return aux;}
if(name=="ArapucaWindowBot354"){std::pair<int,int> aux(440,0); return aux;}
if(name=="ArapucaWindowBot355"){std::pair<int,int> aux(441,0); return aux;}
if(name=="ArapucaWindowBot356"){std::pair<int,int> aux(442,0); return aux;}
if(name=="ArapucaWindowBot357"){std::pair<int,int> aux(443,0); return aux;}
if(name=="ArapucaWindowBot358"){std::pair<int,int> aux(444,0); return aux;}
if(name=="ArapucaWindowBot361"){std::pair<int,int> aux(445,0); return aux;}
if(name=="ArapucaWindowBot362"){std::pair<int,int> aux(446,0); return aux;}
if(name=="ArapucaWindowBot363"){std::pair<int,int> aux(447,0); return aux;}
if(name=="ArapucaWindowBot364"){std::pair<int,int> aux(448,0); return aux;}
if(name=="ArapucaWindowBot365"){std::pair<int,int> aux(449,0); return aux;}
if(name=="ArapucaWindowBot366"){std::pair<int,int> aux(450,0); return aux;}
if(name=="ArapucaWindowBot367"){std::pair<int,int> aux(451,0); return aux;}
if(name=="ArapucaWindowBot368"){std::pair<int,int> aux(452,0); return aux;}
if(name=="ArapucaWindowBot371"){std::pair<int,int> aux(453,0); return aux;}
if(name=="ArapucaWindowBot372"){std::pair<int,int> aux(454,0); return aux;}
if(name=="ArapucaWindowBot373"){std::pair<int,int> aux(455,0); return aux;}
if(name=="ArapucaWindowBot374"){std::pair<int,int> aux(456,0); return aux;}
if(name=="ArapucaWindowBot375"){std::pair<int,int> aux(457,0); return aux;}
if(name=="ArapucaWindowBot376"){std::pair<int,int> aux(458,0); return aux;}
if(name=="ArapucaWindowBot377"){std::pair<int,int> aux(459,0); return aux;}
if(name=="ArapucaWindowBot378"){std::pair<int,int> aux(460,0); return aux;}
if(name=="ArapucaWindowBot381"){std::pair<int,int> aux(461,0); return aux;}
if(name=="ArapucaWindowBot382"){std::pair<int,int> aux(462,0); return aux;}
if(name=="ArapucaWindowBot383"){std::pair<int,int> aux(463,0); return aux;}
if(name=="ArapucaWindowBot384"){std::pair<int,int> aux(464,0); return aux;}
if(name=="ArapucaWindowBot385"){std::pair<int,int> aux(465,0); return aux;}
if(name=="ArapucaWindowBot386"){std::pair<int,int> aux(466,0); return aux;}
if(name=="ArapucaWindowBot387"){std::pair<int,int> aux(467,0); return aux;}
if(name=="ArapucaWindowBot388"){std::pair<int,int> aux(468,0); return aux;}
if(name=="ArapucaWindowBot391"){std::pair<int,int> aux(469,0); return aux;}
if(name=="ArapucaWindowBot392"){std::pair<int,int> aux(470,0); return aux;}
if(name=="ArapucaWindowBot393"){std::pair<int,int> aux(471,0); return aux;}
if(name=="ArapucaWindowBot394"){std::pair<int,int> aux(472,0); return aux;}
if(name=="ArapucaWindowBot395"){std::pair<int,int> aux(473,0); return aux;}
if(name=="ArapucaWindowBot396"){std::pair<int,int> aux(474,0); return aux;}
if(name=="ArapucaWindowBot397"){std::pair<int,int> aux(475,0); return aux;}
if(name=="ArapucaWindowBot398"){std::pair<int,int> aux(476,0); return aux;}
if(name=="ArapucaWindowBot401"){std::pair<int,int> aux(477,0); return aux;}
if(name=="ArapucaWindowBot402"){std::pair<int,int> aux(478,0); return aux;}
if(name=="ArapucaWindowBot403"){std::pair<int,int> aux(479,0); return aux;}
if(name=="ArapucaWindowBot404"){std::pair<int,int> aux(480,0); return aux;}
if(name=="ArapucaWindowBot405"){std::pair<int,int> aux(481,0); return aux;}
if(name=="ArapucaWindowBot406"){std::pair<int,int> aux(482,0); return aux;}
if(name=="ArapucaWindowBot407"){std::pair<int,int> aux(483,0); return aux;}
if(name=="ArapucaWindowBot408"){std::pair<int,int> aux(484,0); return aux;}
if(name=="ArapucaWindowLat121"){std::pair<int,int> aux(485,0); return aux;}
if(name=="ArapucaWindowLat122"){std::pair<int,int> aux(486,0); return aux;}
if(name=="ArapucaWindowLat123"){std::pair<int,int> aux(487,0); return aux;}
if(name=="ArapucaWindowLat124"){std::pair<int,int> aux(488,0); return aux;}
if(name=="ArapucaWindowLat125"){std::pair<int,int> aux(489,0); return aux;}
if(name=="ArapucaWindowLat126"){std::pair<int,int> aux(490,0); return aux;}
if(name=="ArapucaWindowLat127"){std::pair<int,int> aux(491,0); return aux;}
if(name=="ArapucaWindowLat128"){std::pair<int,int> aux(492,0); return aux;}
if(name=="ArapucaWindowLat129"){std::pair<int,int> aux(493,0); return aux;}
if(name=="ArapucaWindowLat130"){std::pair<int,int> aux(494,0); return aux;}
if(name=="ArapucaWindowLat131"){std::pair<int,int> aux(495,0); return aux;}
if(name=="ArapucaWindowLat132"){std::pair<int,int> aux(496,0); return aux;}
if(name=="ArapucaWindowLat133"){std::pair<int,int> aux(497,0); return aux;}
if(name=="ArapucaWindowLat134"){std::pair<int,int> aux(498,0); return aux;}
if(name=="ArapucaWindowLat135"){std::pair<int,int> aux(499,0); return aux;}
if(name=="ArapucaWindowLat136"){std::pair<int,int> aux(500,0); return aux;}
if(name=="ArapucaWindowLat137"){std::pair<int,int> aux(501,0); return aux;}
if(name=="ArapucaWindowLat138"){std::pair<int,int> aux(502,0); return aux;}
if(name=="ArapucaWindowLat139"){std::pair<int,int> aux(503,0); return aux;}
if(name=="ArapucaWindowLat221"){std::pair<int,int> aux(504,0); return aux;}
if(name=="ArapucaWindowLat222"){std::pair<int,int> aux(505,0); return aux;}
if(name=="ArapucaWindowLat223"){std::pair<int,int> aux(506,0); return aux;}
if(name=="ArapucaWindowLat224"){std::pair<int,int> aux(507,0); return aux;}
if(name=="ArapucaWindowLat225"){std::pair<int,int> aux(508,0); return aux;}
if(name=="ArapucaWindowLat226"){std::pair<int,int> aux(509,0); return aux;}
if(name=="ArapucaWindowLat227"){std::pair<int,int> aux(510,0); return aux;}
if(name=="ArapucaWindowLat228"){std::pair<int,int> aux(511,0); return aux;}
if(name=="ArapucaWindowLat229"){std::pair<int,int> aux(512,0); return aux;}
if(name=="ArapucaWindowLat230"){std::pair<int,int> aux(513,0); return aux;}
if(name=="ArapucaWindowLat231"){std::pair<int,int> aux(514,0); return aux;}
if(name=="ArapucaWindowLat232"){std::pair<int,int> aux(515,0); return aux;}
if(name=="ArapucaWindowLat233"){std::pair<int,int> aux(516,0); return aux;}
if(name=="ArapucaWindowLat234"){std::pair<int,int> aux(517,0); return aux;}
if(name=="ArapucaWindowLat235"){std::pair<int,int> aux(518,0); return aux;}
if(name=="ArapucaWindowLat236"){std::pair<int,int> aux(519,0); return aux;}
if(name=="ArapucaWindowLat237"){std::pair<int,int> aux(520,0); return aux;}
if(name=="ArapucaWindowLat238"){std::pair<int,int> aux(521,0); return aux;}
if(name=="ArapucaWindowLat239"){std::pair<int,int> aux(522,0); return aux;}
if(name=="ArapucaWindowLat321"){std::pair<int,int> aux(523,0); return aux;}
if(name=="ArapucaWindowLat322"){std::pair<int,int> aux(524,0); return aux;}
if(name=="ArapucaWindowLat323"){std::pair<int,int> aux(525,0); return aux;}
if(name=="ArapucaWindowLat324"){std::pair<int,int> aux(526,0); return aux;}
if(name=="ArapucaWindowLat325"){std::pair<int,int> aux(527,0); return aux;}
if(name=="ArapucaWindowLat326"){std::pair<int,int> aux(528,0); return aux;}
if(name=="ArapucaWindowLat327"){std::pair<int,int> aux(529,0); return aux;}
if(name=="ArapucaWindowLat328"){std::pair<int,int> aux(530,0); return aux;}
if(name=="ArapucaWindowLat329"){std::pair<int,int> aux(531,0); return aux;}
if(name=="ArapucaWindowLat330"){std::pair<int,int> aux(532,0); return aux;}
if(name=="ArapucaWindowLat331"){std::pair<int,int> aux(533,0); return aux;}
if(name=="ArapucaWindowLat332"){std::pair<int,int> aux(534,0); return aux;}
if(name=="ArapucaWindowLat333"){std::pair<int,int> aux(535,0); return aux;}
if(name=="ArapucaWindowLat334"){std::pair<int,int> aux(536,0); return aux;}
if(name=="ArapucaWindowLat335"){std::pair<int,int> aux(537,0); return aux;}
if(name=="ArapucaWindowLat336"){std::pair<int,int> aux(538,0); return aux;}
if(name=="ArapucaWindowLat337"){std::pair<int,int> aux(539,0); return aux;}
if(name=="ArapucaWindowLat338"){std::pair<int,int> aux(540,0); return aux;}
if(name=="ArapucaWindowLat339"){std::pair<int,int> aux(541,0); return aux;}
if(name=="ArapucaWindowLat421"){std::pair<int,int> aux(542,0); return aux;}
if(name=="ArapucaWindowLat422"){std::pair<int,int> aux(543,0); return aux;}
if(name=="ArapucaWindowLat423"){std::pair<int,int> aux(544,0); return aux;}
if(name=="ArapucaWindowLat424"){std::pair<int,int> aux(545,0); return aux;}
if(name=="ArapucaWindowLat425"){std::pair<int,int> aux(546,0); return aux;}
if(name=="ArapucaWindowLat426"){std::pair<int,int> aux(547,0); return aux;}
if(name=="ArapucaWindowLat427"){std::pair<int,int> aux(548,0); return aux;}
if(name=="ArapucaWindowLat428"){std::pair<int,int> aux(549,0); return aux;}
if(name=="ArapucaWindowLat429"){std::pair<int,int> aux(550,0); return aux;}
if(name=="ArapucaWindowLat430"){std::pair<int,int> aux(551,0); return aux;}
if(name=="ArapucaWindowLat431"){std::pair<int,int> aux(552,0); return aux;}
if(name=="ArapucaWindowLat432"){std::pair<int,int> aux(553,0); return aux;}
if(name=="ArapucaWindowLat433"){std::pair<int,int> aux(554,0); return aux;}
if(name=="ArapucaWindowLat434"){std::pair<int,int> aux(555,0); return aux;}
if(name=="ArapucaWindowLat435"){std::pair<int,int> aux(556,0); return aux;}
if(name=="ArapucaWindowLat436"){std::pair<int,int> aux(557,0); return aux;}
if(name=="ArapucaWindowLat437"){std::pair<int,int> aux(558,0); return aux;}
if(name=="ArapucaWindowLat438"){std::pair<int,int> aux(559,0); return aux;}
if(name=="ArapucaWindowLat439"){std::pair<int,int> aux(560,0); return aux;}
if(name=="ArapucaWindowLlat121"){std::pair<int,int> aux(561,0); return aux;}
if(name=="ArapucaWindowLlat122"){std::pair<int,int> aux(562,0); return aux;}
if(name=="ArapucaWindowLlat123"){std::pair<int,int> aux(563,0); return aux;}
if(name=="ArapucaWindowLlat124"){std::pair<int,int> aux(564,0); return aux;}
if(name=="ArapucaWindowLlat125"){std::pair<int,int> aux(565,0); return aux;}
if(name=="ArapucaWindowLlat126"){std::pair<int,int> aux(566,0); return aux;}
if(name=="ArapucaWindowLlat127"){std::pair<int,int> aux(567,0); return aux;}
if(name=="ArapucaWindowLlat128"){std::pair<int,int> aux(568,0); return aux;}
if(name=="ArapucaWindowLlat129"){std::pair<int,int> aux(569,0); return aux;}
if(name=="ArapucaWindowLlat130"){std::pair<int,int> aux(570,0); return aux;}
if(name=="ArapucaWindowLlat131"){std::pair<int,int> aux(571,0); return aux;}
if(name=="ArapucaWindowLlat132"){std::pair<int,int> aux(572,0); return aux;}
if(name=="ArapucaWindowLlat133"){std::pair<int,int> aux(573,0); return aux;}
if(name=="ArapucaWindowLlat134"){std::pair<int,int> aux(574,0); return aux;}
if(name=="ArapucaWindowLlat135"){std::pair<int,int> aux(575,0); return aux;}
if(name=="ArapucaWindowLlat136"){std::pair<int,int> aux(576,0); return aux;}
if(name=="ArapucaWindowLlat137"){std::pair<int,int> aux(577,0); return aux;}
if(name=="ArapucaWindowLlat138"){std::pair<int,int> aux(578,0); return aux;}
if(name=="ArapucaWindowLlat139"){std::pair<int,int> aux(579,0); return aux;}
if(name=="ArapucaWindowLlat221"){std::pair<int,int> aux(580,0); return aux;}
if(name=="ArapucaWindowLlat222"){std::pair<int,int> aux(581,0); return aux;}
if(name=="ArapucaWindowLlat223"){std::pair<int,int> aux(582,0); return aux;}
if(name=="ArapucaWindowLlat224"){std::pair<int,int> aux(583,0); return aux;}
if(name=="ArapucaWindowLlat225"){std::pair<int,int> aux(584,0); return aux;}
if(name=="ArapucaWindowLlat226"){std::pair<int,int> aux(585,0); return aux;}
if(name=="ArapucaWindowLlat227"){std::pair<int,int> aux(586,0); return aux;}
if(name=="ArapucaWindowLlat228"){std::pair<int,int> aux(587,0); return aux;}
if(name=="ArapucaWindowLlat229"){std::pair<int,int> aux(588,0); return aux;}
if(name=="ArapucaWindowLlat230"){std::pair<int,int> aux(589,0); return aux;}
if(name=="ArapucaWindowLlat231"){std::pair<int,int> aux(590,0); return aux;}
if(name=="ArapucaWindowLlat232"){std::pair<int,int> aux(591,0); return aux;}
if(name=="ArapucaWindowLlat233"){std::pair<int,int> aux(592,0); return aux;}
if(name=="ArapucaWindowLlat234"){std::pair<int,int> aux(593,0); return aux;}
if(name=="ArapucaWindowLlat235"){std::pair<int,int> aux(594,0); return aux;}
if(name=="ArapucaWindowLlat236"){std::pair<int,int> aux(595,0); return aux;}
if(name=="ArapucaWindowLlat237"){std::pair<int,int> aux(596,0); return aux;}
if(name=="ArapucaWindowLlat238"){std::pair<int,int> aux(597,0); return aux;}
if(name=="ArapucaWindowLlat239"){std::pair<int,int> aux(598,0); return aux;}
if(name=="ArapucaWindowLlat321"){std::pair<int,int> aux(599,0); return aux;}
if(name=="ArapucaWindowLlat322"){std::pair<int,int> aux(600,0); return aux;}
if(name=="ArapucaWindowLlat323"){std::pair<int,int> aux(601,0); return aux;}
if(name=="ArapucaWindowLlat324"){std::pair<int,int> aux(602,0); return aux;}
if(name=="ArapucaWindowLlat325"){std::pair<int,int> aux(603,0); return aux;}
if(name=="ArapucaWindowLlat326"){std::pair<int,int> aux(604,0); return aux;}
if(name=="ArapucaWindowLlat327"){std::pair<int,int> aux(605,0); return aux;}
if(name=="ArapucaWindowLlat328"){std::pair<int,int> aux(606,0); return aux;}
if(name=="ArapucaWindowLlat329"){std::pair<int,int> aux(607,0); return aux;}
if(name=="ArapucaWindowLlat330"){std::pair<int,int> aux(608,0); return aux;}
if(name=="ArapucaWindowLlat331"){std::pair<int,int> aux(609,0); return aux;}
if(name=="ArapucaWindowLlat332"){std::pair<int,int> aux(610,0); return aux;}
if(name=="ArapucaWindowLlat333"){std::pair<int,int> aux(611,0); return aux;}
if(name=="ArapucaWindowLlat334"){std::pair<int,int> aux(612,0); return aux;}
if(name=="ArapucaWindowLlat335"){std::pair<int,int> aux(613,0); return aux;}
if(name=="ArapucaWindowLlat336"){std::pair<int,int> aux(614,0); return aux;}
if(name=="ArapucaWindowLlat337"){std::pair<int,int> aux(615,0); return aux;}
if(name=="ArapucaWindowLlat338"){std::pair<int,int> aux(616,0); return aux;}
if(name=="ArapucaWindowLlat339"){std::pair<int,int> aux(617,0); return aux;}
if(name=="ArapucaWindowLlat421"){std::pair<int,int> aux(618,0); return aux;}
if(name=="ArapucaWindowLlat422"){std::pair<int,int> aux(619,0); return aux;}
if(name=="ArapucaWindowLlat423"){std::pair<int,int> aux(620,0); return aux;}
if(name=="ArapucaWindowLlat424"){std::pair<int,int> aux(621,0); return aux;}
if(name=="ArapucaWindowLlat425"){std::pair<int,int> aux(622,0); return aux;}
if(name=="ArapucaWindowLlat426"){std::pair<int,int> aux(623,0); return aux;}
if(name=="ArapucaWindowLlat427"){std::pair<int,int> aux(624,0); return aux;}
if(name=="ArapucaWindowLlat428"){std::pair<int,int> aux(625,0); return aux;}
if(name=="ArapucaWindowLlat429"){std::pair<int,int> aux(626,0); return aux;}
if(name=="ArapucaWindowLlat430"){std::pair<int,int> aux(627,0); return aux;}
if(name=="ArapucaWindowLlat431"){std::pair<int,int> aux(628,0); return aux;}
if(name=="ArapucaWindowLlat432"){std::pair<int,int> aux(629,0); return aux;}
if(name=="ArapucaWindowLlat433"){std::pair<int,int> aux(630,0); return aux;}
if(name=="ArapucaWindowLlat434"){std::pair<int,int> aux(631,0); return aux;}
if(name=="ArapucaWindowLlat435"){std::pair<int,int> aux(632,0); return aux;}
if(name=="ArapucaWindowLlat436"){std::pair<int,int> aux(633,0); return aux;}
if(name=="ArapucaWindowLlat437"){std::pair<int,int> aux(634,0); return aux;}
if(name=="ArapucaWindowLlat438"){std::pair<int,int> aux(635,0); return aux;}
if(name=="ArapucaWindowLlat439"){std::pair<int,int> aux(636,0); return aux;}
  else{
  std::pair<int, int> aux(-3,-4);
  return aux;
  }
}

