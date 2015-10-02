
#pragma once

#include "../../../asd.Core.Base.h"
#include "../../../asd.ReferenceObject.h"

namespace asd
{
	/**
	@brief	地形のクラス
	*/
	class Terrain3D
		: public IReference
	{
		friend class Accessor;
	protected:
		Terrain3D(){}
		virtual ~Terrain3D(){}

	public:

		/**
			@brief	マップに対する変更を適用してメッシュを生成する。
		*/
		virtual bool Commit() = 0;

		/**
			@brief	新規に地形を作成する。
			@param	gridSize		グリッド1つ当たりの大きさ
			@param	gridWidthCount	横方向のグリッド数
			@param	gridHeightCount	縦方向のグリッド数
		*/
		virtual void New(float gridSize, int32_t gridWidthCount, int32_t gridHeightCount) = 0;

		/**
			@brief	メモリから地形のデータを読み込む。
			@param	buffer	バッファ
			@note
			テクスチャのパスは保存されないので、読み込んだ後にAddSurfaceする必要がある。
		*/
		virtual void LoadFromMemory(const std::vector<uint8_t>& buffer) = 0;

		/**
			@brief	メモリに地形のデータを保存する。
			@return	地形のデータ
		*/
		virtual std::vector<uint8_t> SaveToMemory() = 0;

		/**
			@brief	素材を追加する。
			@param	name	素材名
			@param	size	素材のテクスチャ1枚あたりの大きさ
			@param	color	色テクスチャのパス
			@param	normal	法線テクスチャのパス
			@param	metalness	スペキュラテクスチャのパス
		*/
		virtual void AddSurface(const achar* name, float size, const achar* color, const achar* normal, const achar* metalness) = 0;

		/**
			@brief	素材インデックスを取得する。
			@param	name	素材名
			@return	素材インデックス
		*/
		virtual int32_t GetSurfaceIndex(const achar* name) = 0;

		virtual void SetCliffTexture(Texture2D* diffuseTexture, Texture2D* normalTexture, Texture2D* metalnessTexture) = 0;

		/**
			@brief	素材を円形に割り当てる。
			@param	surfaceIndex	素材インデックス
			@param	x	円形の中心座標
			@param	y	円形の中心座標
			@param	radius	円形の半径
			@param	value	割り当てる値(-255～255)
			@param	fallout	周囲のぼかし(0～1)
		*/
		virtual void AssignSurfaceWithCircle(int32_t surfaceIndex, float x, float y, float radius, float value, float fallout) = 0;

		/**
		@brief	材質を設定する。
		@param	material		材質
		*/
		virtual void SetMaterial(Material3D* material) = 0;

		/**
		@brief	円形に地形を上下させる。
		@param	x	円形の中心座標
		@param	y	円形の中心座標
		@param	radius	円形の半径
		@param	value	値
		@param	fallout	周囲のぼかし(0～1)
		*/
		virtual void RaiseHeightWithCircle(float x, float y, float radius, float value, float fallout) = 0;

		/**
		@brief	円形に地形を指定した高度に設定する。
		@param	x	円形の中心座標
		@param	y	円形の中心座標
		@param	radius	円形の半径
		@param	value	高度
		@param	fallout	周囲のぼかし(0～1)
		*/
		virtual void ChangeHeightWithCircle(float x, float y, float radius, float value, float fallout) = 0;

		/**
		@brief	円形に地形を平滑化させる。
		@param	x	円形の中心座標
		@param	y	円形の中心座標
		@param	radius	円形の半径
		@param	value	平滑度(0～1)
		@param	fallout	周囲のぼかし(0～1)
		*/
		virtual void SmoothHeightWithCircle(float x, float y, float radius, float value, float fallout) = 0;

		/**
		@brief	円形に崖の生成を伴う地形の上下をさせる。
		@param	x	円形の中心座標
		@param	y	円形の中心座標
		@param	radius	円形の半径
		@param	value	値
		*/
		virtual void ChangeCliffesWithCircle(float x, float y, float radius, int32_t value) = 0;

		/**
		@brief	光線を飛ばし、衝突した位置を取得する。
		@param	from	光線を飛ばす元の位置
		@param	to		光線を飛ばす先の位置
		@return	光線が地形に衝突した位置。衝突しない場合、NaNを返す。
		*/
		virtual Vector3DF CastRay(const Vector3DF& from, const Vector3DF& to) = 0;
#if !SWIG
		/**
		@brief	材質を設定する。
		@param	material		材質
		*/
		void SetMaterial(std::shared_ptr<Material3D> material)
		{
			SetMaterial(material.get());
		}
#endif
	};
}