#pragma once
#include "KitchenMesh.h"
#include "KitchenUI.h"

class ModuleUpperCargo : public KitchenModule
{
	friend class KitchenConfigurator;
public:
	ModuleUpperCargo(KitchenMaterialPack* matPack);
	~ModuleUpperCargo();
	void resizeModule(float dL, MODIFY_TYPE mod) override;
	void switchBaseMaterial() override;
	void switchFacadeMaterial();

	// PART ///////////////////////////////////////////////////////////////////
	void switchPartHandle(bool isHandleOn) override;
	void switchPartRotation(OPT_ROTATION curHandle) override;
	void switchPartMesh(OPT_MESH curMesh) override;
	void switchPartMaterial(std::string newMat) override;
	void openPartFacade(float dL) override;
	void closePartFacade(float dL) override;
	////////////////////////////////////////////////////////////////////////////

private:
	KitchenMeshBase* box;
	KitchenMeshBase* handle;
	std::array<KitchenMeshBase*, 2> shelfs;
	std::array<KitchenMeshBase*, 3> facades;

	Facade<3, FACADE_VIEW::ROT, 2>* facade;

	KitchenUI* ui = nullptr;
};