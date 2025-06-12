#pragma once
#include "KitchenMesh.h"
#include "KitchenUI.h"

class ModuleTable : public KitchenModule
{
	friend class KitchenConfigurator;
public:
	ModuleTable(KitchenMaterialPack* matPack);
	~ModuleTable();
	void resizeModule(float dL, MODIFY_TYPE mod) override;
	void moveGasPanel(float dL, MODIFY_TYPE mod);
	void moveSinkPanel(float dL, MODIFY_TYPE mod);
	void switchBaseMaterial() override;
	void switchPanelMaterial();
	void switchFacetMaterial();

	// PART ///////////////////////////////////////////////////////////////////
	void switchPartMaterial(std::string newMat) override;
	void movePart(float dL, OPT_MOVE_PARAM param) override;
	void scalePart(float dL, OPT_SCALE curScale, OPT_SCALE_PARAM param) override;
	////////////////////////////////////////////////////////////////////////////

private:
	KitchenMeshBase* desk;
	KitchenMeshBase* panel;
	KitchenMeshBase* coffee;
	struct Electro {
		KitchenMeshBase* base;
		KitchenMeshBase* elems_iron;
		KitchenMeshBase* elems_plastic;
		KitchenMeshBase* elems_duralumin;
		KitchenMeshBase* elems_light;
	} electro;
	struct Facet {
		KitchenMeshBase* up;
		KitchenMeshBase* bottom;
	} facet;
	struct Towel {
		KitchenMeshBase* box;
		KitchenMeshBase* paper;
	} towel;
	struct Sink {
		KitchenMeshBase* box;
		KitchenMeshBase* wood_grid;
	} sink;

	std::vector<KitchenModule*> modulesUpper;
	std::vector<KitchenModule*> modulesBottom;

	KitchenUI* ui;
};