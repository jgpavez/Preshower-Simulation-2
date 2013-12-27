
#include "globals.hh"

#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"

#include "G4VSDFilter.hh"
#include "G4SDParticleFilter.hh"


#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"



	DetectorConstruction::DetectorConstruction()
: pSD(0),
	pWorldPhys(0)
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

	//Materials____________________________________________________________

	//Get Elements From Material Data Base
	G4NistManager* manager = G4NistManager::Instance();

	G4Material* Air  = manager->FindOrBuildMaterial("G4_AIR");

	G4cout<<Air<<G4endl;

	//LYSO Material____________________________________________________________

	G4Element* O  = manager->FindOrBuildElement("O");
	G4Element* Lu = manager->FindOrBuildElement("Lu");
	G4Element* Y  = manager->FindOrBuildElement("Y");

	//Lutetium Oxide Lu2O3
	G4Material* LutetiumOxide = new G4Material("LutetiumOxide", 9.41*g/cm3, 2);
	LutetiumOxide->AddElement(Lu, 2);	
	LutetiumOxide->AddElement(O, 3);

	//Silicon Dioxide SiO2
	G4Material* SiliconDioxide  = manager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

	//Yttrium Oxide Y2O3
	G4Material* YttriumOxide = new G4Material("YttriumOxide", 5.01*g/cm3, 2);
	YttriumOxide->AddElement(Y, 2);
	YttriumOxide->AddElement(O, 3);

	//Build LYSO Material
	G4Material* LYSO = new G4Material("LYSO", 7.1*g/cm3, 3);
	LYSO->AddMaterial(LutetiumOxide,  81*perCent);
	LYSO->AddMaterial(SiliconDioxide, 14*perCent);
	LYSO->AddMaterial(YttriumOxide,    5*perCent);	

	G4Material* Polyethylene  = manager->FindOrBuildMaterial("G4_POLYETHYLENE");

	G4Material* UVTAcrylic   = manager->FindOrBuildMaterial("G4_PLEXIGLASS");

	G4cout<<LYSO<<G4endl;


	//Material Propertie
	
	const G4int nEntries = 45;


	//LYSO Crystal
	G4double PhotonEnergy[nEntries] =
	{ 1.94*eV,2.00*eV,2.06*eV,2.12*eV,2.18*eV,
		2.24*eV,2.30*eV,2.36*eV,2.42*eV,2.48*eV,
		2.54*eV,2.60*eV,2.66*eV,2.72*eV,2.78*eV,
		2.84*eV,2.90*eV,2.96*eV,3.02*eV,3.08*eV,
		3.14*eV,3.20*eV,3.26*eV,3.32*eV,3.38*eV,
		3.44*eV,3.50*eV,3.56*eV,3.62*eV,3.68*eV,
		3.74*eV,3.8*eV,3.86*eV,3.92*eV,3.98*eV,
		4.04*eV,4.1*eV,4.16*eV,4.22*eV,4.28*eV,
		4.34*eV,4.4*eV,4.46*eV,4.52*eV,4.58*eV};

	//G4double fastLYSO[nEntries] =
	//{ 0.092795968802, 0.374539584218, 0.963888919018, 1.65682419462,2.43831159163 ,
	//	3.71632697909, 5.50077214492, 7.56698642074, 9.99928599144,12.4188742608 ,
	//	15.1862445009, 17.7883990961, 20.3955005348, 22.5827235294, 24.0555735326,
	//	24.792566649, 24.3898099075, 22.6112651148, 19.2450936031, 14.653497191,
	//	9.55177610256, 5.4441767564, 2.66771641639,1.0200045365 , 0.463423437655,
	//	0.13095859308, 0.0504877716094, 0.0511161, 0.0, 0.0,
	//	0.0, 0.0, 0.0, 0.0, 0.0,
	//	0.0, 0.0, 0.0, 0.0, 0.0,
	//	0.0, 0.0, 0.0, 0.0, 0.0
	//};
	// Emission Spectrum
	const G4int nEntriesFastLYSO = 37;
	G4double fastLYSOEnergy[nEntriesFastLYSO] =
	{ 3.341588*eV, 3.298346*eV,3.265478*eV, 3.235545*eV, 3.208396*eV, 3.181690*eV, 3.159783*eV, 3.138168*eV,
		3.108400*eV, 3.093727*eV, 3.060708*eV, 3.032385*eV, 3.004582*eV, 2.954280*eV, 2.894640*eV, 2.847923*eV,
		2.789055*eV, 2.730953*eV, 2.684610*eV, 2.639814*eV, 2.606816*eV, 2.578976*eV, 2.541827*eV, 2.508476*eV,
		2.473317*eV, 2.433954*eV, 2.397076*eV, 2.356447*eV, 2.311333*eV, 2.279186*eV, 2.241330*eV, 2.197319*eV,
		2.153989*eV, 2.116236*eV, 2.068532*eV, 2.021147*eV, 1.979291*eV};
	
	G4double fastLYSO[nEntriesFastLYSO] =
	{0.674847, 1.656440, 2.730060, 4.141100, 5.613500, 7.392640, 9.141100,
		11.196300, 12.975500, 14.539900, 16.687100, 19.049100, 21.135000, 23.067500,
			24.601200, 24.938700, 23.987700, 22.576700, 20.766900, 18.926400, 17.638000,
		16.380400, 14.908000, 13.251500, 11.748500, 10.061300, 8.619630, 7.147240,
		5.644170, 4.509200, 3.680980, 2.668710, 1.993870, 1.411040, 1.042940,
		0.552147, 0.337423};



	G4double rLYSO[nEntries] =
	{
		1.81, 1.81, 1.81, 1.81, 1.81,
		1.81, 1.81, 1.81, 1.81, 1.81,
		1.81, 1.81, 1.81, 1.81, 1.81,
		1.81, 1.81, 1.81, 1.81, 1.81,
		1.81, 1.81, 1.81, 1.81, 1.81,
		1.81, 1.81, 1.81, 1.81, 1.81,
		1.81, 1.81, 1.81, 1.81, 1.81,
		1.81, 1.81, 1.81, 1.81, 1.81,
		1.81, 1.81, 1.81, 1.81, 1.81
	};

	G4double absLYSO[nEntries] =
	{50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
		50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
		50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
		50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
		50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
		50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
		50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
		50*cm, 50*cm, 50*cm, 50*cm, 50*cm,
		50*cm, 50*cm, 50*cm, 50*cm, 50*cm
	};

	G4MaterialPropertiesTable* LYSO_MPT = new G4MaterialPropertiesTable();
	//LYSO_MPT->AddProperty("FASTCOMPONENT", PhotonEnergy, fastLYSO, nEntries);
	LYSO_MPT->AddProperty("FASTCOMPONENT", fastLYSOEnergy, fastLYSO, nEntriesFastLYSO);
	LYSO_MPT->AddProperty("ABSLENGTH", PhotonEnergy, absLYSO, nEntries);
	LYSO_MPT->AddProperty("RINDEX", PhotonEnergy, rLYSO , nEntries);
	LYSO_MPT->AddConstProperty("SCINTILLATIONYIELD",32./keV);
	LYSO_MPT->AddConstProperty("FASTTIMECONSTANT",41*ns);
	LYSO_MPT->AddConstProperty("RESOLUTIONSCALE", 1);


	LYSO->SetMaterialPropertiesTable(LYSO_MPT);


	G4double RefractiveIndexClad1[nEntries] =
	{ 1.48871, 1.48964 ,1.49059 , 1.49156, 1.4925,
		1.49341, 1.49428 , 1.49508, 1.49582, 1.49651,
		1.49718, 1.49786 ,1.49861 , 1.49952, 1.50071,
		1.50233, 1.50456, 1.50763, 1.51182, 1.51746,
		1.52494, 1.5347, 1.54725, 1.56313, 1.58297,
		1.60742, 1.63718, 1.67296, 1.71549, 1.76548,
		1.82363, 1.89059, 1.96697, 2.05331, 2.15012,
		2.25783, 2.37681, 2.50741, 2.64992, 2.80461,
		2.97172, 3.15149, 3.34412, 3.54986, 3.76891};



	//UVT Acrylic

	G4double AbsUVT[nEntries] =
	{ 37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,
		37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,
		37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,
		37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm,
		37.0*mm,37.0*mm,37.0*mm,37.0*mm,37.0*mm };

	G4MaterialPropertiesTable* UVT_MPT = new G4MaterialPropertiesTable();
	UVT_MPT->AddProperty("RINDEX"   , PhotonEnergy, RefractiveIndexClad1 , nEntries);
	UVT_MPT->AddProperty("ABSLENGTH", PhotonEnergy, AbsUVT, nEntries);

	UVTAcrylic->SetMaterialPropertiesTable(UVT_MPT);



	// AIR
	G4double RefractiveIndex[nEntries] =
	{ 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		1.00, 1.00, 1.00, 1.00, 1.00 };

	G4MaterialPropertiesTable* Air_MPT = new G4MaterialPropertiesTable();

	Air_MPT->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex, nEntries);
	Air->SetMaterialPropertiesTable(Air_MPT);




	//Volumes Definition________________________________________

	//Parameters--------

	//LYSO Radiation Length
	G4double Rl = 1.125*cm;
	G4double nslices = 4;
	G4double Gap = 0.2*mm;

	G4int nx = 25.;
	G4int ny = 25.;

	//Slice
	G4double Sx = 2*mm + Gap/2.;
	G4double Sy = 2*mm + Gap/2.;
	G4double Sz = Rl/2;

	//Crystals
	G4double Cx = 2*mm;
	G4double Cy = 2*mm;
	G4double Cz = nslices*Sz;


	//Preshower
	G4double Px = 50*mm + 25.*(Gap/2.);
	G4double Py = 50*mm + 25.*(Gap/2.);
	G4double Pz = nslices*Sz;

	G4double Platex = Px;
	G4double Platey = Py;
	G4double Platez = 1.5*mm;

	G4cout<<"Preshower Depth"<<2.*Pz/mm<<"[mm]"<<G4endl;

	//Detector
	G4double Dx = 20*Rl;
	G4double Dy = 20*Rl;
	G4double Dz = 20*Rl;

	//World
	G4double Wx = 1.2*Dx;
	G4double Wy = 1.2*Dy;
	G4double Wz = 1.2*Dz;



	G4ThreeVector Id_tr(0.,0.,0.);
	G4RotationMatrix* Id_rot = new G4RotationMatrix(0.,0.,0.);

	//Construction-------------

	//World
	G4Box* pWorldSolid = new G4Box("WorldBox", Wx, Wy, Wz);
	G4LogicalVolume* pWorldLog = new G4LogicalVolume(pWorldSolid, Air, "WorldLogical");
	pWorldPhys = new G4PVPlacement(Id_rot, Id_tr, pWorldLog, "World", 0, false, 0);
	pWorldLog->SetVisAttributes(G4VisAttributes(true, G4Colour::White()));

	//Detector
	G4Box* pDetSolid = new G4Box("DetBox", Dx, Dy, Dz);
	G4LogicalVolume* pDetLog = new G4LogicalVolume(pDetSolid, Air, "DetLogical");
	new G4PVPlacement(Id_rot, Id_tr, pDetLog, "Detector", pWorldLog, false, 0);
	pDetLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Red()));

	//Preshower
	G4Box* pPreSolid = new G4Box("PreBox", Px, Py, Pz);
	G4LogicalVolume* pPreLog = new G4LogicalVolume(pPreSolid, Air, "PreLogical");
	G4PVPlacement*  pPrePhys = new G4PVPlacement(Id_rot, G4ThreeVector(0.,0.,Dz-Pz-2*Platez), pPreLog, "Preshower", pDetLog, false, 0);
	pPreLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Blue()));

	G4Box* pXDivSolid = new G4Box("XSegmentBox", Sx, Py, Pz);
	G4LogicalVolume* pXDivLog = new G4LogicalVolume(pXDivSolid, Air, "XSegmentLogical");
	G4VPhysicalVolume* pXDivPhys = new G4PVReplica("XSegment", pXDivLog, pPrePhys, kXAxis, nx, 2.*Sx);
	G4VisAttributes* pXDiv_VA = new G4VisAttributes(true);
	pXDivLog->SetVisAttributes(pXDiv_VA);

	G4Box* pCSurfSolid = new G4Box("CrystalSurfaceBox", Sx, Sy, Pz);
	G4LogicalVolume* pCSurfLog = new G4LogicalVolume(pCSurfSolid, Air, "YSegmentLogical");
	G4VPhysicalVolume* pCSurfPhys = new G4PVReplica("YDiv", pCSurfLog, pXDivPhys, kYAxis, ny, 2.*Sy);
	G4VisAttributes* pCSurf_VA = new G4VisAttributes(true);
	pCSurfLog->SetVisAttributes(pCSurf_VA);

	//Crystal
	G4Box* pCrySolid = new G4Box("CrystalBox", Cx, Cy, Pz);
	G4LogicalVolume* pCryLog = new G4LogicalVolume(pCrySolid, LYSO, "CrystalLogical");
	G4PVPlacement* pCrystalPhys = new G4PVPlacement(Id_rot, G4ThreeVector(0.,0.,0.), pCryLog, "Crystal", pCSurfLog, false, 0);
	pCryLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Green()));

	//Acrylic Plate
	G4Box* pPlateSolid = new G4Box("PlateBox", Platex, Platey, Platez);
	G4LogicalVolume* pPlateLog = new G4LogicalVolume(pPlateSolid, Air, "PlateLogical");
	//G4LogicalVolume* pPlateLog = new G4LogicalVolume(pPlateSolid, UVTAcrylic, "PlateLogical");
	G4PVPlacement* pPlatePhys = new G4PVPlacement(Id_rot, G4ThreeVector(0,0,Dz-Platez), pPlateLog, "Plate", pDetLog, false, 0);
	G4VisAttributes* plateVA = new G4VisAttributes(true, G4Colour::Red());
	plateVA->SetForceWireframe(true);
	pPlateLog->SetVisAttributes(plateVA);

	G4Box* pXPlateDivSolid = new G4Box("XPlateSegmentBox", Sx, Py, Platez);
	G4LogicalVolume* pXPlateDivLog = new G4LogicalVolume(pXPlateDivSolid, Air, "XPlateSegmentLogical");
	G4VPhysicalVolume* pXPlateDivPhys = new G4PVReplica("XPlateSegment", pXPlateDivLog, pPlatePhys, kXAxis, nx, 2.*Sx);
	G4VisAttributes* pXPlateDiv_VA = new G4VisAttributes(true,G4Colour::Red());
	pXPlateDivLog->SetVisAttributes(pXPlateDiv_VA);

	G4Box* pCPlateSurfSolid = new G4Box("PlateSurfaceBox", Sx, Sy, Platez);
	G4LogicalVolume* pCPlateSurfLog = new G4LogicalVolume(pCPlateSurfSolid, Air, "YPlateSegmentLogical");
	G4VPhysicalVolume* pCPlateSurfPhys = new G4PVReplica("YPlateDiv", pCPlateSurfLog, pXPlateDivPhys, kYAxis, ny, 2.*Sy);
	G4VisAttributes* pCPlateSurf_VA = new G4VisAttributes(true,G4Colour::Red());
	pCPlateSurfLog->SetVisAttributes(pCPlateSurf_VA);

	//Slice
	//G4Box* pSliceSolid = new G4Box("SliceBox",Cx,Cy,Sz);
	//G4LogicalVolume* pSliceLog = new G4LogicalVolume(pSliceSolid,LYSO,"SliceLogical");
	//new G4PVReplica("SlicePhys",pSliceLog,pCrystalPhys,kZAxis,nslices,2*Sz);
	//pSliceLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Green()));

	//Optical Surface

	const G4int n = 2;

	G4double pp[n] = {0.1*eV, 10.*eV};
	G4double crystalReflectivity[n] = {0.98, 0.98};
	G4MaterialPropertiesTable* CMPT = new G4MaterialPropertiesTable();
	CMPT->AddProperty("REFLECTIVITY",pp,crystalReflectivity,n);
	G4OpticalSurface* crystalOpSurface = new G4OpticalSurface("CrystalOpticalSurface");
	crystalOpSurface->SetModel(unified);
	crystalOpSurface->SetType(dielectric_dielectric);
	crystalOpSurface->SetFinish(polishedfrontpainted);
	crystalOpSurface->SetMaterialPropertiesTable(CMPT);
	new G4LogicalBorderSurface("CrystalSurface",pCrystalPhys,pCSurfPhys,crystalOpSurface);

	//Sensitive Detector_______________________________________________________

	pSD = new SensitiveDetector("Detector/SensitiveDetector","DetectorHitsCollection");
	//G4SDParticleFilter* particleFilter = new G4SDParticleFilter("PhotonFilter","gamma");
	//pSD->SetFilter(particleFilter);

	G4SDManager* sdm = G4SDManager::GetSDMpointer();
	sdm->AddNewDetector(pSD);

	//pWorldLog->SetSensitiveDetector(pSD);
	//pDetLog->SetSensitiveDetector(pSD);
	//pSliceLog->SetSensitiveDetector(pSD);
	//pCryLog->SetSensitiveDetector(pSD);
	pCPlateSurfLog->SetSensitiveDetector(pSD);

	return pWorldPhys;
}
