
#include "globals.hh"

#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4AssemblyVolume.hh"

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

    	G4Element* H  = manager->FindOrBuildElement("H");
    	G4Element* C  = manager->FindOrBuildElement("C");
	G4Element* O  = manager->FindOrBuildElement("O");
	G4Element* Lu = manager->FindOrBuildElement("Lu");
	G4Element* Y  = manager->FindOrBuildElement("Y");
	G4Element* Si = manager->FindOrBuildElement("Si");
	G4Element* Cr = manager->FindOrBuildElement("Cr");
	G4Element* Mn = manager->FindOrBuildElement("Mn");
	G4Element* Fe = manager->FindOrBuildElement("Fe");
	G4Element* Ni = manager->FindOrBuildElement("Ni");


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
	G4Material* Polystyrene  = manager->FindOrBuildMaterial("G4_POLYSTYRENE");
	G4Material* PMMA	     = manager->FindOrBuildMaterial("G4_PLEXIGLASS");

	// PWO
	G4Material* Pwo = manager->FindOrBuildMaterial("G4_PbWO4");

	//Fluorinated Polymer
	G4Material* FPolymer = new G4Material("Fluorinated Polymer",1.43*g/cm3 , 3);
	FPolymer->AddElement(H, 08.0538*perCent);
	FPolymer->AddElement(C, 59.9848*perCent);
	FPolymer->AddElement(O, 31.9614*perCent);

	//Stainless Steel
  	G4Material* StainlessSteel = new G4Material("StainlessSteel", 8.06*g/cm3, 6);
  	StainlessSteel->AddElement(C, 0.001);
  	StainlessSteel->AddElement(Si, 0.007);
  	StainlessSteel->AddElement(Cr, 0.18);
  	StainlessSteel->AddElement(Mn, 0.01);
  	StainlessSteel->AddElement(Fe, 0.712);
  	StainlessSteel->AddElement(Ni, 0.09);

	//Aluminium
	G4Material* Al = new G4Material("Aluminum", 13., 26.98*g/mole, 2.7*g/cm3);

	//Copper
	G4Material* Cu = new G4Material("Copper", 29., 63.55*g/mole, 8.960*g/cm3);

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

	//WLS Fiber Polystirene

	G4double RefractiveIndexWLSfiber[nEntries] =
	{ 1.58666, 1.58838, 1.59017, 1.592,1.59386 ,
		1.59575, 1.59764, 1.59953, 1.60142, 1.60332,
		1.60525, 1.60722, 1.6093, 1.61153, 1.61401,
		1.61683, 1.62013,1.62408 , 1.62886, 1.63473,
		1.64195, 1.65084, 1.66177,1.67515 ,1.69143 ,
		1.71112, 1.73476, 1.76292,1.79619 , 1.8352,
		1.88057, 1.9329, 1.99281, 2.06086, 2.13759,
		2.2235, 2.31905, 2.42464, 2.54065, 2.66741,
		2.80522, 2.95435 , 3.11506, 3.28757, 3.47212
	};

	G4double AbsWLSfiber[nEntries] =
	{ 0.15886875498*m,0.15886875498*m,0.15886875498*m,0.15886875498*m,0.15886875498*m,
		0.15886875498*m,0.15886875498*m,0.15886875498*m,0.15886875498*m,0.15886875498*mm,
		0.15886875498*mm,0.21854127879*mm,0.3614942435*mm,0.458550982808*mm,0.434161551652*mm,
		0.466832621324*mm,0.468697682766*mm,0.408069251303*mm,0.390288203361*mm,0.358362694603*mm,
		0.31342035112*mm,0.284871136412*mm,0.262551953159*mm,0.236219871618*mm,0.218087774682*mm,
		0.204859281866*mm,0.196130649511*mm,0.193091306219*mm,0.193091306219*m,0.193091306219*m,
		0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m,
		0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m,
		0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m,0.193091306219*m
	};


	const int nEntriesEmissionFib = 44;
	G4double EmissionFibEnergy[nEntriesEmissionFib] = 
	{	2.622945*eV, 2.599314*eV, 2.584103*eV, 2.572016*eV, 2.562021*eV, 2.558004*eV, 2.549085*eV, 2.538300*eV,
		2.533394*eV, 2.524666*eV, 2.517930*eV, 2.509319*eV, 2.497035*eV, 2.489550*eV, 2.476592*eV, 2.464699*eV,
		2.454746*eV, 2.448459*eV, 2.444887*eV, 2.433310*eV, 2.428011*eV, 2.413954*eV, 2.398313*eV, 2.387145*eV,
		2.367614*eV, 2.355874*eV, 2.338480*eV, 2.321354*eV, 2.307677*eV, 2.294176*eV, 2.278482*eV, 2.267636*eV,
		2.257649*eV, 2.242453*eV, 2.228196*eV, 2.212638*eV, 2.199479*eV, 2.178592*eV, 2.150418*eV, 2.131804*eV,
		2.116879*eV, 2.098192*eV, 2.076575*eV, 2.053497*eV
	};

	G4double EmissionFib[nEntriesEmissionFib] = 
	{	0.00555352, 0.05720910, 0.12890000, 0.23896200, 0.34068900, 0.43409000,
		0.54416000, 0.64922100, 0.73594700, 0.81766000, 0.87602500, 0.95273400,
		0.99607300, 1.00273000, 0.97433700, 0.92092900, 0.84917800, 0.79411600,
		0.75240600, 0.68065000, 0.64060300, 0.57718000, 0.53710500, 0.50204600,
		0.47697200, 0.47193500, 0.46354600, 0.43514000, 0.40841200, 0.35999900,
		0.31491500, 0.26984600, 0.23311800, 0.18303100, 0.15463000, 0.13456400,
		0.11117000, 0.08774940, 0.07764780, 0.07424870, 0.06919330, 0.03576720,
		0.02401390, 0.01225370
	};


	G4MaterialPropertiesTable* Polystyrene_MPT = new G4MaterialPropertiesTable();

	Polystyrene_MPT->AddProperty("WLSABSLENGTH", PhotonEnergy, AbsWLSfiber, nEntries);
	Polystyrene_MPT->AddProperty("WLSCOMPONENT", EmissionFibEnergy, EmissionFib, nEntriesEmissionFib);
	Polystyrene_MPT->AddProperty("RINDEX"      , PhotonEnergy, RefractiveIndexWLSfiber , nEntries);
	Polystyrene_MPT->AddConstProperty("WLSTIMECONSTANT",0.5*ns);

    	Polystyrene->SetMaterialPropertiesTable(Polystyrene_MPT);
	// PMMA	

    	const int nEntriesAbsClad = 33;

    	G4double AbsCladEnergy[nEntriesAbsClad] = 
	{   4.788033*eV, 4.455543*eV, 4.390095*eV, 4.338364*eV, 4.283987*eV, 4.230912*eV, 4.175520*eV, 4.153756*eV,
	4.086112*eV, 4.027465*eV, 3.950768*eV, 3.886368*eV, 3.736741*eV, 3.598177*eV, 3.477149*eV, 3.378351*eV,
	3.278220*eV, 3.166812*eV, 3.104735*eV, 3.014120*eV, 2.921441*eV, 2.829242*eV, 2.737905*eV, 2.694422*eV,
	2.607162*eV, 2.576051*eV, 2.480526*eV, 2.417361*eV, 2.373842*eV, 2.300241*eV, 2.259718*eV, 2.215417*eV,
	2.137621*eV};
	G4double AbsClad[nEntriesAbsClad] = 
	{   0.4983575*mm, 0.4988259*mm, 0.9348997*mm, 1.6802852*mm, 3.0199934*mm, 
	5.2053268*mm, 9.5936970*mm, 12.3341720*mm, 18.7512403*mm, 28.9881166*mm, 
	54.7910975*mm, 87.5911040*mm, 137.7494901*mm, 202.5955958*mm, 300.4692272*mm, 
	434.5302346*mm, 655.2997466*mm, 850.0608207*mm, 964.1841768*mm, 1103.0367954*mm, 
	1294.0766471*mm, 1569.9651064*mm, 1751.7414240 *mm, 1858.1467068*mm, 2126.0819479*mm, 
 	2199.1254269*mm, 2474.8563733*mm, 2296.6774168 *mm, 2203.5352382*mm, 2223.8218856*mm, 
 	2046.1619295*mm, 1882.8251899*mm, 1900.5530651*mm};


	G4MaterialPropertiesTable* PMMA_MPT = new G4MaterialPropertiesTable();

	PMMA_MPT->AddProperty("RINDEX"   , PhotonEnergy, RefractiveIndexClad1 , nEntries);
	PMMA_MPT->AddProperty("ABSLENGTH", AbsCladEnergy, AbsClad, nEntries);
	
    	PMMA->SetMaterialPropertiesTable(PMMA_MPT);

	// Polymer
    	G4double RefractiveIndexClad2[nEntries] =
    	{ 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
      	1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
      	1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
      	1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
      	1.42, 1.42, 1.42, 1.42, 1.42};

    	G4MaterialPropertiesTable* FPolymer_MPT = new G4MaterialPropertiesTable();
    	FPolymer_MPT->AddProperty("RINDEX"   , PhotonEnergy, RefractiveIndexClad2 , nEntries);
    	FPolymer_MPT->AddProperty("ABSLENGTH", AbsCladEnergy, AbsClad, nEntriesAbsClad);

    	FPolymer->SetMaterialPropertiesTable(FPolymer_MPT);
	
    	// PWO
    	G4double fasten[] = {3.550*eV, 3.358*eV, 3.186*eV, 3.106*eV, 2.958*eV, 2.889*eV, 2.824*eV, 2.701*eV, 2.588*eV, 2.485*eV, 2.259*eV, 2.071*eV};
    	G4double fastsci[] = {0.078, 0.212, 0.452, 0.6, 0.876, 0.965, 0.997, 0.884, 0.61, 0.328, 0.023, 0};
    	G4double rind[] = {2.482, 2.427, 2.382, 2.364, 2.332, 2.318, 2.306, 2.286, 2.27, 2.257, 2.234, 2.221};
    	G4double alenen[] = {3.550*eV, 3.313*eV, 3.106*eV, 2.923*eV, 2.761*eV, 2.616*eV, 2.485*eV, 2.259*eV, 2.071*eV};
    	G4double alen[] = {0.085*m, 1.0*m, 1.5*m, 2.0*m, 3.0*m, 10.55*m, 35.55*m, 52.50*m, 71.43*m};


    	G4MaterialPropertiesTable* pwo_MPT = new G4MaterialPropertiesTable();
    	pwo_MPT->AddProperty("RINDEX", fasten, rind, 12);
    	pwo_MPT->AddProperty("ABSLENGTH", alenen, alen, 9);
    	pwo_MPT->AddProperty("FASTCOMPONENT", fasten, fastsci, 12);
    	pwo_MPT->AddConstProperty("SCINTILLATIONYIELD", 200/MeV);
    	pwo_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
   	pwo_MPT->AddConstProperty("FASTTIMECONSTANT", 10*ns);

    	Pwo->SetMaterialPropertiesTable(pwo_MPT);

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
	G4double CG = 0.015*mm;


	//Preshower
	G4double Px = 50*mm + 25.*(Gap/2.);
	G4double Py = 50*mm + 25.*(Gap/2.);
	G4double Pz = Cz + CG;

	G4double Platex = 59.;
	G4double Platey = 59.;
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
 	
	//Fiber
	G4double CoreR = .44*mm;
 	G4double Clad1R = .47*mm;
	G4double Clad2R = .50*mm;
	G4double Phi = 360.*deg; 

	//Fiber Slot
	G4double Tol= 0.05*mm;
	G4double Fx = Clad2R + Tol;
	G4double Fy = Platey;
	G4double Fz = Clad2R + Tol;
 	
	//Readout Geometry
    	G4double ROh = 4.0*mm;
    	G4double ROw = Px;
    	G4double ROd = 1.5*mm;

 	//Readout Division
 	G4double RODiv = 1.5*mm;

	//MPPC Geometry
    	G4double MPPCh = 1.0*mm;
    	G4double MPPCw = Px;
    	G4double MPPCd = 1.5*mm;

 	//MPPC Division
 	G4double MPPCDiv = 1.5*mm;



	//Cover Geometry
	G4double Cox = 5*mm;
	G4double Cox2 = Px;
	G4double Coy = Py;
	G4double Coz = 21.5*mm;
	G4double CozGap = 1.015;
	G4double FCz = 3.75*mm;
	G4double FC1x = 0.65*mm;
	G4double FC1z = 3.25;

	// Unions
	G4double Ux = 4.5*mm;
	G4double Uy = 0.5*mm;
	G4double Uz = 21*mm; 

	// Pre Readout
	G4double pRx = Platex;
	G4double pRy = 5.*mm;
	G4double pRz = 3.375*mm;

	G4double CRx = 58.5*mm;
	G4double CRy = 10.5*mm;
	G4double CRz = 20.15*mm;

	// ReadoutCover
	G4double zCGap = 5.0*mm;


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
	G4PVPlacement* pDetPhys = new G4PVPlacement(Id_rot, Id_tr, pDetLog, "Detector", pWorldLog, false, 0);
	pDetLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Red()));

	//Preshower
	G4Box* pPreSolid = new G4Box("PreBox", Px, Py, Pz);
	G4LogicalVolume* pPreLog = new G4LogicalVolume(pPreSolid, Air, "PreLogical");
	G4PVPlacement*  pPrePhys = new G4PVPlacement(Id_rot, G4ThreeVector(0.,0.,Dz-2*FCz-Pz-2*Platez - (pRz - Platez) - zCGap), pPreLog, "Preshower", pDetLog, false, 0);
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
	G4Box* pCrySolid = new G4Box("CrystalBox", Cx, Cy, Cz);
	G4LogicalVolume* pCryLog = new G4LogicalVolume(pCrySolid, LYSO, "CrystalLogical");
	G4PVPlacement* pCrystalPhys = new G4PVPlacement(Id_rot, G4ThreeVector(0.,0.,CG), pCryLog, "Crystal", pCSurfLog, false, 0);
	pCryLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Green()));

	//Cover Geometry
	G4Box* pBackCover = new G4Box("backCover", Px+2*Cox, Py+2*Cox, 2.5*mm);
	G4LogicalVolume* pBackCoverLog = new G4LogicalVolume(pBackCover,Al, "backCoverLogical");
	G4PVPlacement* pBackCoverPhys = new G4PVPlacement(Id_rot, G4ThreeVector(0., 0.,Dz-2*FCz-2*Pz-2*Platez-2.5*mm - (pRz - Platez) - zCGap), pBackCoverLog, "CoverBack", pDetLog,false,0);
	pBackCoverLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Grey()));
	
	G4Box* pRightCover = new G4Box("rightCover", Cox, Coy + Cox - Uy, Coz);
	G4LogicalVolume* pRightCoverLog = new G4LogicalVolume(pRightCover,StainlessSteel,"rightCoverLogical");
	G4PVPlacement* pRightCoverPhys = new G4PVPlacement(Id_rot, G4ThreeVector(Px + Cox, -Cox-Uy,Dz-2*FCz-Pz-2*Platez-CozGap- (pRz - Platez) - zCGap), pRightCoverLog, "CoverRight", pDetLog, false, 0);
	pRightCoverLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Grey()));

	G4Box* pLeftCover = new G4Box("leftCover", Cox, Coy + Cox - Uy, Coz);
	G4LogicalVolume* pLeftCoverLog = new G4LogicalVolume(pLeftCover,StainlessSteel,"leftCoverLogical");
	G4PVPlacement* pLeftCoverPhys = new G4PVPlacement(Id_rot, G4ThreeVector(-Px - Cox, -Cox-Uy,Dz-2*FCz-Pz-2*Platez-CozGap- (pRz - Platez) - zCGap), pLeftCoverLog, "CoverLeft", pDetLog, false, 0);
	pLeftCoverLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Grey()));

	G4Box* pTopCover = new G4Box("topCover", Cox2 + 2*Cox, Cox, Coz);
	G4LogicalVolume* pTopCoverLog = new G4LogicalVolume(pTopCover,StainlessSteel,"topCoverLogical");
	G4PVPlacement* pTopCoverPhys = new G4PVPlacement(Id_rot, G4ThreeVector(0., Px + Cox,Dz-2*FCz-Pz-2*Platez-CozGap- (pRz - Platez) - zCGap), pTopCoverLog, "CoverTop", pDetLog, false, 0);
	pTopCoverLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Grey()));

	G4Box* pBottomCover = new G4Box("bottomCover", Cox2 - 2*Uy, Cox, Coz);
	G4LogicalVolume* pBottomCoverLog = new G4LogicalVolume(pBottomCover,StainlessSteel,"bottomCoverLogical");
	G4PVPlacement* pBottomCoverPhys = new G4PVPlacement(Id_rot, G4ThreeVector(0.,-Px-Cox,Dz-2*FCz-Pz-2*Platez-CozGap- (pRz - Platez) - zCGap), pBottomCoverLog, "CoverBottom", pDetLog, false, 0);
	pBottomCoverLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Grey()));

	G4Box* pFrontCover = new G4Box("frontCover",Px + 2*Cox , Py + 2*Cox, FCz);
	G4LogicalVolume* pFrontCoverLog = new G4LogicalVolume(pFrontCover, StainlessSteel, "frontCoverLogical");
	G4PVPlacement* pFrontCoverPhys = new G4PVPlacement(Id_rot, G4ThreeVector(0.,0.,Dz - FCz -zCGap), pFrontCoverLog, "CoverFront", pDetLog, false, 0);
	pFrontCoverLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Grey()));

	G4Box* pFrontCover1 = new G4Box("frontCover1",FC1x ,Py + 2*Cox, Platez+CozGap + (pRz - Platez)/2.);
	G4LogicalVolume* pFrontCover1Log = new G4LogicalVolume(pFrontCover1, StainlessSteel, "frontCover1Logical");
	G4PVPlacement* pFrontCover1Phys = new G4PVPlacement(Id_rot, G4ThreeVector(-Px - 2*Cox + FC1x,0.,Dz - 2.*FCz - Platez-CozGap - (pRz - Platez)/2.- zCGap), pFrontCover1Log, "CoverFront1", pDetLog, false, 0);
	pFrontCover1Log->SetVisAttributes(G4VisAttributes(true, G4Colour::Grey()));


	G4Box* pFrontCover2 = new G4Box("frontCover2" ,Py + 2*Cox-FC1x,FC1x, Platez+CozGap + (pRz - Platez)/2.);
	G4LogicalVolume* pFrontCover2Log = new G4LogicalVolume(pFrontCover2, StainlessSteel, "frontCover2Logical");
	G4PVPlacement* pFrontCover2Phys = new G4PVPlacement(Id_rot, G4ThreeVector(FC1x,-Px - 2*Cox + FC1x,Dz - 2.*FCz - Platez-CozGap - (pRz - Platez)/2. - zCGap), pFrontCover2Log, "CoverFront1", pDetLog, false, 0);
	pFrontCover2Log->SetVisAttributes(G4VisAttributes(true, G4Colour::Grey()));

	G4Box* pUnionLeft = new G4Box("unionLeft", Ux, Uy, Uz);
	G4LogicalVolume* pUnionLeftLog = new G4LogicalVolume(pUnionLeft,Cu,"UnionLeftLogical");
	G4PVPlacement* pUnionLeftPhys = new G4PVPlacement(Id_rot, G4ThreeVector(-Px -Cox, Coy - Uy, Dz-2*FCz-Pz-2*Platez-CozGap - (pRz - Platez) - zCGap), pUnionLeftLog, "UnionLeft", pDetLog, false, 0);
	pUnionLeftLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Yellow())); 


	G4Box* pUnionRight = new G4Box("unionRight", Ux, Uy, Uz);
	G4LogicalVolume* pUnionRightLog = new G4LogicalVolume(pUnionRight,Cu,"UnionRightLogical");
	G4PVPlacement* pUnionRightPhys = new G4PVPlacement(Id_rot, G4ThreeVector(Px + Cox, Coy - Uy, Dz-2*FCz-Pz-2*Platez-CozGap - (pRz - Platez) - zCGap), pUnionRightLog, "UnionRight", pDetLog, false, 0);
	pUnionRightLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Yellow())); 


	G4Box* pUnionBottom = new G4Box("unionBottom", Uy, Ux, Uz);
	G4LogicalVolume* pUnionBottomLog = new G4LogicalVolume(pUnionBottom,Cu,"UnionBottomLogical");
	G4PVPlacement* pUnionBottomPhys = new G4PVPlacement(Id_rot, G4ThreeVector(Cox2 - Uy, -Px-Cox, Dz-2*FCz-Pz-2*Platez - (pRz - Platez) - zCGap), pUnionBottomLog, "UnionBottom", pDetLog, false, 0);
	G4PVPlacement* pUnionBottom2Phys = new G4PVPlacement(Id_rot, G4ThreeVector(-Cox2 + Uy, -Px-Cox, Dz-2*FCz-Pz-2*Platez - (pRz - Platez) - zCGap), pUnionBottomLog, "UnionBottom2", pDetLog, false, 0);
	pUnionBottomLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Yellow())); 

	//Acrylic Plate
	G4Box* pPlateSolid = new G4Box("PlateBox", Platex, Platey, Platez);
	G4LogicalVolume* pPlateLog = new G4LogicalVolume(pPlateSolid, UVTAcrylic, "PlateLogical");
	G4PVPlacement* pPlatePhys = new G4PVPlacement(Id_rot, G4ThreeVector(/*-Px-2*Cox+Platex+2*FC1x*/0.,/*-Px-2*Cox+Platey+2*FC1x*/0.,Dz-2*FCz-Platez- (pRz - Platez) - zCGap), pPlateLog, "Plate", pDetLog, false, 0);
	G4VisAttributes* plateVA = new G4VisAttributes(true, G4Colour::Red());
	plateVA->SetForceWireframe(true);
	pPlateLog->SetVisAttributes(plateVA);

	G4Tubs* pCoreSolid  = new G4Tubs("CoreTube" , 0, CoreR , Px + 2.*Cox - (Py + 2*Cox - Platey)/2., 0, Phi);
	G4Tubs* pClad1Solid = new G4Tubs("Clad1Tube", 0, Clad1R, Px + 2.*Cox - (Py + 2*Cox - Platey)/2., 0, Phi);
	G4Tubs* pClad2Solid = new G4Tubs("Clad2Tube", 0, Clad2R, Px + 2.*Cox - (Py + 2*Cox - Platey)/2., 0, Phi);

	G4LogicalVolume* pCoreLog  = new G4LogicalVolume(pCoreSolid,  Polystyrene, "CoreLogical" );
	G4VisAttributes* coreVA = new G4VisAttributes(true);
	coreVA->SetForceWireframe(true);
	pCoreLog->SetVisAttributes(coreVA);

	G4LogicalVolume* pClad1Log = new G4LogicalVolume(pClad1Solid, PMMA,        "Clad1Logical");
	G4VisAttributes* clad1VA = new G4VisAttributes(true);
	clad1VA->SetForceWireframe(true);
	pClad1Log->SetVisAttributes(clad1VA);

	G4LogicalVolume* pClad2Log = new G4LogicalVolume(pClad2Solid, FPolymer ,   "Clad2Logical");
	G4VisAttributes* clad2VA = new G4VisAttributes(true);
	clad2VA->SetForceWireframe(true);
	pClad2Log->SetVisAttributes(clad2VA);

	G4LogicalVolume* pFiberLog = new G4LogicalVolume(pClad2Solid, FPolymer ,   "FiberLogical");	
	pFiberLog->SetVisAttributes(G4VisAttributes(true, G4Colour::Green()));

	G4PVPlacement* pCorePhys  = new G4PVPlacement(Id_rot, Id_tr, pCoreLog , "Core"     , pClad1Log, false, 0);
	G4PVPlacement* pClad1Phys = new G4PVPlacement(Id_rot, Id_tr, pClad1Log, "Cladding1", pClad2Log, false, 0);
	G4PVPlacement* pClad2Phys = new G4PVPlacement(Id_rot, Id_tr, pClad2Log, "Cladding2", pFiberLog, false, 0);

	//Fiber Assembly
	G4int x;
	G4ThreeVector tr;
	G4RotationMatrix *rot = new G4RotationMatrix();
	rot->rotateX(90*deg);
	G4AssemblyVolume* slot_assembly = new G4AssemblyVolume(pFiberLog, Id_tr, rot);
	for(x = -12; x <= 12; x++){
		tr = G4ThreeVector( x*2*Sx, 0., 0.);
		slot_assembly->AddPlacedVolume(pFiberLog, tr, rot);
	}
	tr =  G4ThreeVector(0.,(Py + 2*Cox - Platey)/2., -Platez+Fz+0.1*mm);
	slot_assembly->MakeImprint(pPlateLog, tr, Id_rot);
	rot = new G4RotationMatrix();
	rot->rotateY(180.*deg);
	rot->rotateZ(90.*deg);
	tr =  G4ThreeVector((Px + 2*Cox - Platex)/2., 0., Platez-Fz-0.1*mm);
	slot_assembly->MakeImprint(pPlateLog, tr, rot);

	// Pre-readout cover
	G4Box *preReadoutTop = new G4Box("preReadoutTop", pRx + (Py + 2.*Cox - Platey)/2., pRy, pRz);
	G4LogicalVolume *preReadoutTopLog = new G4LogicalVolume(preReadoutTop, Polyethylene, "preReadoutTopLog");
	G4PVPlacement *preReadoutTopPhys = new G4PVPlacement(Id_rot, G4ThreeVector((Py + 2.*Cox - Platey)/2., Py + 2*Cox - 5.*mm,Dz-2*FCz-Platez - (pRz - Platez) - zCGap), preReadoutTopLog, "preReadoutTopPhys", pDetLog, false, 0);
	G4VisAttributes* preReadoutTop_VA = new G4VisAttributes(true, G4Colour::Blue());
	preReadoutTopLog->SetVisAttributes(preReadoutTop_VA);
	
	// Pre-readout cover
	G4Box *preReadoutRight = new G4Box("preReadoutRight", pRy, pRx - (2.*pRy - (Py + 2.*Cox - Platey))/2., pRz);
	G4LogicalVolume *preReadoutRightLog = new G4LogicalVolume(preReadoutRight, Polyethylene, "preReadoutRightLog");
	G4PVPlacement *preReadoutRightPhys = new G4PVPlacement(Id_rot, G4ThreeVector(Px + 2*Cox - 5.*mm,-(2.*pRy - (Py + 2.*Cox - Platey))/2.,Dz-2*FCz-Platez - (pRz - Platez) - zCGap), preReadoutRightLog, "preReadoutRightPhys", pDetLog, false, 0);
	G4VisAttributes* preReadoutRight_VA = new G4VisAttributes(true, G4Colour::Blue());
	preReadoutRightLog->SetVisAttributes(preReadoutRight_VA);

	// Readout Cover
	G4Box *coverReadoutTop = new G4Box("coverReadoutTop", CRx,CRy,CRz);
	G4LogicalVolume *coverReadoutTopLog = new G4LogicalVolume(coverReadoutTop, Al, "coverReadoutTopLog");
	G4PVPlacement *coverReadoutTopPhys = new G4PVPlacement(Id_rot, G4ThreeVector(0.,Py + 2*Cox + 10.5*mm,Dz - 20.15*mm), coverReadoutTopLog, "coverReadoutTopPhys", pDetLog, false, 0);
	G4VisAttributes* coverReadoutTop_VA = new G4VisAttributes(true, G4Colour::Grey());
	coverReadoutTopLog->SetVisAttributes(coverReadoutTop_VA);

	// Readout Cover
	G4Box *coverReadoutRight = new G4Box("coverReadoutRight", CRy,CRx,CRz);
	G4LogicalVolume *coverReadoutRightLog = new G4LogicalVolume(coverReadoutRight, Al, "coverReadoutRightLog");
	G4PVPlacement *coverReadoutRightPhys = new G4PVPlacement(Id_rot, G4ThreeVector(Px + 2*Cox + 10.5*mm, 0., Dz - 20.15*mm), coverReadoutRightLog, "coverReadoutRightPhys", pDetLog, false, 0);
	G4VisAttributes* coverReadoutRight_VA = new G4VisAttributes(true, G4Colour::Grey());
	coverReadoutRightLog->SetVisAttributes(coverReadoutRight_VA);


    	//Readout geometry
    	G4Box* pReadoutSolid_X = new G4Box("ReadoutBox_X", ROw, ROh, ROd + Gap/2.);
    	G4Box* pReadoutSolid_Y = new G4Box("ReadoutBox_Y", ROh, ROw, ROd + Gap/2.);
    	G4LogicalVolume* pReadoutLog_X = new G4LogicalVolume(pReadoutSolid_X, Al, "ReadoutLogical_X");
    	G4LogicalVolume* pReadoutLog_Y = new G4LogicalVolume(pReadoutSolid_Y, Al, "ReadoutLogical_Y");

    	G4VisAttributes* readout_VA = new G4VisAttributes(false, G4Colour::Grey());
    	readout_VA->SetForceWireframe(true);
    	pReadoutLog_X->SetVisAttributes(readout_VA);
    	pReadoutLog_Y->SetVisAttributes(readout_VA);

    	G4PVPlacement* ROPhys_X = new G4PVPlacement(Id_rot, G4ThreeVector(0., Px+ROh+2.*Cox - (Py + 2*Cox + 10.5*mm), Dz-2*FCz-Platez - (pRz - Platez) - zCGap - Dz + 20.15*mm), pReadoutLog_X, "Readout_X", coverReadoutTopLog, false, 0);
    	G4PVPlacement* ROPhys_Y = new G4PVPlacement(Id_rot, G4ThreeVector(Py+ROh+2.*Cox - (Px + 2*Cox + 10.5*mm), 0., Dz-2*FCz-Platez - (pRz - Platez) - zCGap - Dz + 20.15*mm), pReadoutLog_Y, "Readout_Y", coverReadoutRightLog, false, 0);

    	//Readout Division: 25 slices
    	G4Box* pRODivSolid_X = new G4Box("RODivBox_X", RODiv, ROh, ROd);
    	G4Box* pRODivSolid_Y = new G4Box("RODivBox_Y", ROh, RODiv, ROd);
   	G4LogicalVolume* pRODivLog_X = new G4LogicalVolume(pRODivSolid_X, UVTAcrylic, "RODivLogical_X");
    	G4LogicalVolume* pRODivLog_Y = new G4LogicalVolume(pRODivSolid_Y, UVTAcrylic, "RODivLogical_Y");
	pRODivLog_X->SetVisAttributes(G4VisAttributes(true , G4Colour::Grey()));
	pRODivLog_Y->SetVisAttributes(G4VisAttributes(true , G4Colour::Grey()));
	G4VPhysicalVolume* pRODivPhys_X = new G4PVReplica("RO_X", pRODivLog_X, ROPhys_X, kXAxis, nx, 2.*ROw/nx);
	G4VPhysicalVolume* pRODivPhys_Y = new G4PVReplica("RO_Y", pRODivLog_Y, ROPhys_Y, kYAxis, ny, 2.*ROw/nx);	

	//MPPC
    	G4Box* pMPPCSolid_X = new G4Box("MPPCBox_X", MPPCw, MPPCh, MPPCd + Gap/2.);
    	G4Box* pMPPCSolid_Y = new G4Box("MPPCBox_Y", MPPCh, MPPCw, MPPCd + Gap/2.);
    	G4LogicalVolume* pMPPCLog_X = new G4LogicalVolume(pMPPCSolid_X, Al, "MPPCLogical_X");
    	G4LogicalVolume* pMPPCLog_Y = new G4LogicalVolume(pMPPCSolid_Y, Al, "MPPCLogical_Y");

    	G4VisAttributes* mppc_VA = new G4VisAttributes(false, G4Colour::Grey());
    	mppc_VA->SetForceWireframe(true);
    	pMPPCLog_X->SetVisAttributes(mppc_VA);
    	pMPPCLog_Y->SetVisAttributes(mppc_VA);

    	G4PVPlacement* MPPCPhys_X = new G4PVPlacement(Id_rot, G4ThreeVector(0., Px+ MPPCh +2.*Cox + 2*ROh - (Py + 2*Cox + 10.5*mm), Dz-2*FCz-Platez - (pRz - Platez) - zCGap - Dz + 20.15*mm ), pMPPCLog_X, "pMPPC_X", coverReadoutTopLog, false, 0);
    	G4PVPlacement* MPPCPhys_Y = new G4PVPlacement(Id_rot, G4ThreeVector(Py + MPPCh + 2.*Cox + 2*ROh - (Py + 2*Cox + 10.5*mm), 0., Dz-2*FCz-Platez - (pRz - Platez) - zCGap - Dz + 20.15*mm), pMPPCLog_Y, "pMPPC_Y", coverReadoutRightLog, false, 0);

    	//MPPC Division: 25 slices
    	G4Box* pMPPCDivSolid_X = new G4Box("MPPCDivBox_X", MPPCDiv, MPPCh, MPPCd);
    	G4Box* pMPPCDivSolid_Y = new G4Box("MPPCDivBox_Y", MPPCh, MPPCDiv, MPPCd);
   	G4LogicalVolume* pMPPCDivLog_X = new G4LogicalVolume(pMPPCDivSolid_X, UVTAcrylic, "MPPCDivLogical_X");
    	G4LogicalVolume* pMPPCDivLog_Y = new G4LogicalVolume(pMPPCDivSolid_Y, UVTAcrylic, "MPPCDivLogical_Y");
	pMPPCDivLog_X->SetVisAttributes(G4VisAttributes(true , G4Colour::Grey()));
	pMPPCDivLog_Y->SetVisAttributes(G4VisAttributes(true , G4Colour::Grey()));
	G4VPhysicalVolume* pMPPCDivPhys_X = new G4PVReplica("MPPC_X", pMPPCDivLog_X, MPPCPhys_X, kXAxis, nx, 2.*MPPCw/nx);
	G4VPhysicalVolume* pMPPCDivPhys_Y = new G4PVReplica("MPPC_Y", pMPPCDivLog_Y, MPPCPhys_Y, kYAxis, ny, 2.*MPPCw/nx);	



	// Readout Reflexivity
	

	//Reflective geometry
	//G4Box* pReflectiveSolid_X = new G4Box("ReflectiveBox_X", ROw, ROh, ROd);
	//G4Box* pReflectiveSolid_Y = new G4Box("ReflectiveBox_Y", ROh, ROw, ROd);
	//G4LogicalVolume* pReflectiveLog_X = new G4LogicalVolume(pReflectiveSolid_X, Air, "ReflectiveLogical_X");
	//G4LogicalVolume* pReflectiveLog_Y = new G4LogicalVolume(pReflectiveSolid_Y, Air, "ReflectiveLogical_Y");

	//G4VisAttributes* reflective_VA = new G4VisAttributes(true, G4Colour::Yellow());
	//reflective_VA->SetForceWireframe(true);
	//pReflectiveLog_X->SetVisAttributes(reflective_VA);
	//pReflectiveLog_Y->SetVisAttributes(reflective_VA);

	//G4PVPlacement* ReflectivePhys_X = new G4PVPlacement(Id_rot, G4ThreeVector(0.,-Platex-ROh, Dz-Platez), pReflectiveLog_X, "Reflective_X", pWorldLog, false, 0);
	//G4PVPlacement* ReflectivePhys_Y = new G4PVPlacement(Id_rot, G4ThreeVector(-Platey-ROh, 0., Dz-Platez), pReflectiveLog_Y, "Reflective_Y", pWorldLog, false, 0);


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
	
	// Readout Surface
	new G4LogicalBorderSurface("ReadoutSurface_X", ROPhys_X, pRODivPhys_X, crystalOpSurface);
	new G4LogicalBorderSurface("ReadoutSurface_Y", ROPhys_Y, pRODivPhys_Y, crystalOpSurface);

	G4double   plateReflectivity[n] = {0.9, 0.9};
	G4MaterialPropertiesTable* PMPT = new G4MaterialPropertiesTable();

	PMPT->AddProperty("REFLECTIVITY",pp,plateReflectivity,n);
	//Plate
	G4OpticalSurface* plateOpSurface = new G4OpticalSurface("PlateOpticalSurface");
	plateOpSurface->SetModel(unified);
	plateOpSurface->SetType(dielectric_dielectric);
	plateOpSurface->SetFinish(groundfrontpainted);
	plateOpSurface->SetMaterialPropertiesTable(PMPT);

	//Fiber end
	new G4LogicalBorderSurface("PlateSurface",pPlatePhys,pDetPhys,plateOpSurface);
	// Sure??
	new G4LogicalBorderSurface("PlateSurface",pPlatePhys,pWorldPhys,plateOpSurface);

    	new G4LogicalBorderSurface("PaintedCore", pCorePhys, pDetPhys,plateOpSurface);
    	new G4LogicalBorderSurface("PaintedClad1",pClad1Phys,pDetPhys,plateOpSurface);
    	new G4LogicalBorderSurface("PaintedClad2",pClad2Phys,pDetPhys,plateOpSurface);


/*	new G4LogicalBorderSurface("PaintedCoreX", pCorePhys,ReflectivePhys_X,plateOpSurface);
	new G4LogicalBorderSurface("PaintedClad1X",pClad1Phys,ReflectivePhys_X,plateOpSurface);
    	new G4LogicalBorderSurface("PaintedClad2X",pClad2Phys,ReflectivePhys_X,plateOpSurface);
 
    	new G4LogicalBorderSurface("PaintedCoreY", pCorePhys,ReflectivePhys_Y,plateOpSurface);
    	new G4LogicalBorderSurface("PaintedClad1Y",pClad1Phys,ReflectivePhys_Y,plateOpSurface);
    	new G4LogicalBorderSurface("PaintedClad2Y",pClad2Phys,ReflectivePhys_Y,plateOpSurface);
*/
    	//new G4LogicalBorderSurface("PaintedPlateX", pPlatePhys,ReflectivePhys_X,plateOpSurface);
    	//new G4LogicalBorderSurface("PaintedPlateY", pPlatePhys,ReflectivePhys_Y,plateOpSurface);
	//Sensitive Detector_______________________________________________________

	pSD = new SensitiveDetector("Detector/SensitiveDetector","DetectorHitsCollection");
	//G4SDParticleFilter* particleFilter = new G4SDParticleFilter("PhotonFilter","gamma");
	//pSD->SetFilter(particleFilter);

	G4SDManager* sdm = G4SDManager::GetSDMpointer();
	sdm->AddNewDetector(pSD);

	//pWorldLog->SetSensitiveDetector(pSD);
	//pDetLog->SetSensitiveDetector(pSD);
	//pSliceLog->SetSensitiveDetector(pSD);
	pMPPCDivLog_X->SetSensitiveDetector(pSD);
	pMPPCDivLog_Y->SetSensitiveDetector(pSD);
	//pCPlateSurfLog->SetSensitiveDetector(pSD);

	return pWorldPhys;
}
