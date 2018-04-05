#pragma once
#include "..\Common\Pch.h"
#include "..\Graphic\Direct3D.h"

#pragma region D3D11_INPUT_ELEMENT_DESC 커스터마이징 구조체 정의
namespace inputlayout
{
	typedef struct INPUT_LAYOUT_TYPE_0
	{
		D3DXVECTOR3 Pos;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} IL_Pos;

	typedef struct INPUT_LAYOUT_TYPE_1
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 Tex;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} IL_PosTex;

	typedef struct INPUT_LAYOUT_TYPE_2
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 Tex;
		D3DXVECTOR3 Nor;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} IL_PosTexNor;

	typedef struct INPUT_LAYOUT_TYPE_3
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 Tex;
		D3DXVECTOR3 Nor;
		D3DXVECTOR3 Tan;
		D3DXVECTOR3 BiN;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} IL_PosTexNorTanBiN;

	typedef struct INPUT_LAYOUT_TYPE_4
	{
		D3DXMATRIX World;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} IL_PosInst;

	typedef struct INPUT_LAYOUT_TYPE_5
	{
		D3DXMATRIX World;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} IL_PosTexInst;

	typedef struct INPUT_LAYOUT_TYPE_EXTRA_0
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Tex;

		static const D3D11_INPUT_ELEMENT_DESC sElementDesc[];
		static const UINT sElementCount;

	} IL_Extra_0;	 // Trapezoid
}

using namespace inputlayout;
#pragma endregion

class InputLayout
{
public:
	bool					CheckInstanced();
	bool					CheckTessellated();
	ID3D11Buffer*			GetVertexBuffer();
	ID3D11Buffer*			GetIndexBuffer();
	ID3D11Buffer*			GetInstanceBuffer();
	UINT					GetVertexCount();
	UINT					GetIndexCount();
	UINT					GetInstanceCount();
	UINT					GetDrawCount();
	UINT					GetInputLayoutSize();
	UINT					GetInputLayoutInstanceSize();
	void					SetPosition(FLOAT x, FLOAT y, FLOAT z);

protected:
	struct WorldBufferData
	{
		D3DXMATRIX World;
	};

	D3DXMATRIX				mWorld;						// 인스턴싱은 사용x
	ID3D11Buffer*			mWorldBuffer;
	bool					mInstanced;
	bool					mTessellated;
	ID3D11Buffer*			mVertexBuffer;
	ID3D11Buffer*			mIndexBuffer;
	ID3D11Buffer*			mInstanceBuffer;
	UINT					mVertexCount;
	UINT					mIndexCount;
	UINT					mInstanceCount;
	UINT					mDrawCount;
	UINT					mInputLayoutSize;			// Strides
	UINT					mInputLayoutInstanceSize;
	vector<IL_PosTexNor>	mTempData;

	InputLayout();
	virtual ~InputLayout();
	//virtual void			Update(FLOAT delta) = 0;
	//virtual void			InitializeData() = 0;
	virtual void			InitializeBuffer() = 0;
	void					GetInputLayoutFromFile(wstring meshName, FLOAT scale = 1.0F);
	void					SetWorldBuffer();
	virtual void			SetBuffer() = 0;
};