#pragma once
#include<vector>
#include<cstdint>
#include<climits>
#include<fstream>
#include<string>
#include"../ace.common.Base.h"
#include "../Math/ace.Vector3DF.h"
#include "../Math/ace.Matrix44.h"

namespace ace
{

/**
@brief	数値、文字列をバイト列にシリアライズし、ファイルに書き出すクラス
@detail	対応する型はint32_t, int16_t, int8_t, std::string, astring, achar*
@warning	エンディアンは現状非互換、正当性の検査等はないので、
データを読み出す際はBinaryReaderで*書き出した順に*読み出すこと
*/
class BinaryWriter
{
protected:
	std::vector<int8_t> m_data; ///< シリアライズされたバイト列
public:
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	扱うデータが大きいことが予想される場合にあらかじめ容量を確保する
	@param	size	確保するサイズ
	*/

	void Reserve(size_t size)
	{
		m_data.reserve(size);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	シリアライズされたバイト列をconst参照で返す。
	@warning	本クラスのインスタンスより寿命を長く取る際はコピーなどして懸垂参照を避けること
	*/

	std::vector<int8_t> const& Get() const
	{
		return m_data;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int32_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	void Push(int32_t content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int16_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	
	void Push(int16_t content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 2; i++)
		{
			m_data.push_back(pc[i]);
		}
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int8_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	void Push(int8_t content){

		m_data.push_back(content);

	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	std::stringをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列
	*/
	void Push(std::string const& content)
	{
		// TODO: string::lengthの返値が64bitでもたれていた場合に危険
		int32_t l = (int32_t)content.length();

		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&l));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (int32_t i = 0; i < l; i++)
		{
			m_data.push_back(static_cast<int8_t>(content.at(i)));
		}

	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	astringをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列
	*/
	void Push(ace::astring const& content)
	{
		auto l = content.length();

		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&l));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (uint32_t i = 0; i < l; i++)
		{
			//copy
			achar a = content.at(i);
			int8_t* pcs = static_cast<int8_t*>(static_cast<void*>(&a));
			m_data.push_back(static_cast<int8_t>(pcs[0]));
			m_data.push_back(static_cast<int8_t>(pcs[1]));
		}

	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	achar*をシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列の先頭ポインタ
	@param	size	シリアライズする文字列のサイズ
	*/
	void Push(ace::achar const* content, int32_t size)
	{
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&size));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (int32_t i = 0; i < size; i++)
		{
			//copy
			achar a = content[i];
			int8_t* pcs = static_cast<int8_t*>(static_cast<void*>(&a));
			m_data.push_back(static_cast<int8_t>(pcs[0]));
			m_data.push_back(static_cast<int8_t>(pcs[1]));
		}

	}


	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	floatをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする実数
	*/
	void Push(float content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	ace::Vector3DFをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズするVector
	*/
	void Push(ace::Vector3DF const& content){
		Push(content.X);
		Push(content.Y);
		Push(content.Z);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	ace::Matrix44をシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズするVector
	*/
	void Push(ace::Matrix44 const& content){
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				Push(content.Values[j][i]);
			}
		}

	}



	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	保持しているバイト列をストリームに書き出す。
	@param	os	書き出すストリーム
	@return 成功すればtrue, 失敗すればfalse
	*/

	bool WriteOut(std::ostream& os) const
	{
		if (!os){ return false; }
		for (auto itr = m_data.cbegin(); itr != m_data.cend(); itr++)
		{
			os << *itr;
		}
		return true;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	保持しているバイト列をファイルに書き出す。
	@param	filename	書き出すファイル名
	@param	isAppend	追記するか否か
	@return 成功すればtrue, 失敗すればfalse
	*/
	bool WriteOut(std::string const& filename, bool isAppend = false) const
	{
		std::ofstream os(
			filename,
			std::ios_base::binary | std::ios_base::out | (isAppend ? std::ios_base::app : std::ios_base::out)
		);
		if (!os) {
			return false;
		}
		if (!WriteOut(os)) { return false; }
		os.close();
		return true;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	デストラクタ
	*/
	virtual ~BinaryWriter(){}
};
}