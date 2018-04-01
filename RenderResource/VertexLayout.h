#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"

namespace vertexlayout
{
	typedef struct VERTEX_LAYOUT_TYPE_0
	{
		D3DXVECTOR3 Pos;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} VL_Pos;

	typedef struct VERTEX_LAYOUT_TYPE_1
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 UV;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} VL_PosTex;

	typedef struct VERTEX_LAYOUT_TYPE_2
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 UV;
		D3DXVECTOR3 Nor;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} VL_PosTexNor;

	typedef struct VERTEX_LAYOUT_TYPE_3
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 UV;
		D3DXVECTOR3 Nor;
		D3DXVECTOR3 Tan;
		D3DXVECTOR3 BiN;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} VL_PosTexNorTanBiN;

	typedef struct VERTEX_LAYOUT_TYPE_4
	{
		D3DXMATRIX World;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} VL_Instance;

	typedef struct VERTEX_LAYOUT_TYPE_EXTRA_0
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 UVQ;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} VL_Extra_0;	 // Trapezoid ¿ë
}

using namespace vertexlayout;

class VertexLayout
{
public:
	ID3D11Buffer*			GetVertexBuffer() { return mVertexBuffer; }
	ID3D11Buffer*			GetIndexBuffer() { return mIndexBuffer; }

protected:
	vector<VL_PosTexNor>	mTempData;
	ID3D11Buffer*			mVertexBuffer;
	ID3D11Buffer*			mIndexBuffer;


	VertexLayout();
	virtual ~VertexLayout();
	virtual void			InitializeData() = 0;
	virtual void			InitializeBuffer() = 0;
	void					CreateMeshDataFromFile(wstring meshName);
	void					CreateBuffer(D3D11_BIND_FLAG bindFlag, UINT size, D3D11_USAGE usage, void* data, ID3D11Buffer** buffer);

};

