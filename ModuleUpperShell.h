#pragma once
#include "KitchenMesh.h"
#include "KitchenUI.h"

class ModuleUpperShelf : public KitchenModule, public IFadaceSwitchable
{
	friend class KitchenConfigurator;
public:
	ModuleUpperShelf(KitchenMaterialPack* matPack, BOX_POS pos);
	~ModuleUpperShelf();
	void resizeModule(float dL, MODIFY_TYPE mod) override;
	void switchFacade() override;
	void switchBaseMaterial() override;
	void switchFacadeMaterial();
	void switchFacadeType(bool isVert);

	// PART ///////////////////////////////////////////////////////////////////
	void switchPartHandle(bool isHandleOn) override;
	void switchPartRotation(OPT_ROTATION curHandle) override;
	void switchPartMesh(OPT_MESH curMesh) override;
	void switchPartMaterial(std::string newMat) override;
	void scalePart(float dL, OPT_SCALE curScale, OPT_SCALE_PARAM param) override;
	void openPartFacade(float dL) override;
	void closePartFacade(float dL) override;
	void switchCurFacade();
	////////////////////////////////////////////////////////////////////////////

private:
	KitchenMeshBase* box;
	std::array<KitchenMeshBase*, 2> Shelfs;
	std::array<KitchenMeshBase*, 2> handles;
	std::array<std::array<KitchenMeshBase*, 2>, 3> facade_vertical;
	std::array<std::array<KitchenMeshBase*, 3>, 2> facade_horizontal;

	bool isFacadeVertical = true;
	size_t curHorId = 0;
	Facade<3, FACADE_VIEW::ROT, 1>* facadeVerticalLeft;
	Facade<3, FACADE_VIEW::ROT, 1>* facadeVerticalRight;
	Facade<2, FACADE_VIEW::ROT, 1>* facadeHorizontalUpper;
	Facade<2, FACADE_VIEW::ROT, 1>* facadeHorizontalMiddle;
	FacadePaired* facadeHorizontalBottom;

	std::vector<ModulePart*> initParts;

	KitchenUI* ui = nullptr;

	KitchenModule *moduleLeft, *moduleMiddle, *moduleRight;
};