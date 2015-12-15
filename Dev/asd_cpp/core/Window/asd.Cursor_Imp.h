
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Cursor.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Cursor_Imp
		: public Cursor
		, public ReferenceObject
	{
	private:
		GLFWcursor*		native = nullptr;

	public:
		Cursor_Imp(GLFWcursor* native);
		virtual ~Cursor_Imp();

		static Cursor* Create(File* file, const achar* path, Vector2DI hot);

		GLFWcursor* GetNative() { return native; }
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}