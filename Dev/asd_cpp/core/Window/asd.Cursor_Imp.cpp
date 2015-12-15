
#include "asd.Cursor_Imp.h"

#include "../IO/asd.File.h"
#include "../Graphics/asd.Graphics_Imp.h"

namespace asd
{
	Cursor_Imp::Cursor_Imp(GLFWcursor* native)
		: native(native)
	{
	
	}

	Cursor_Imp::~Cursor_Imp()
	{
		assert(native != nullptr);
		glfwDestroyCursor(native);
		native = nullptr;
	}

	Cursor* Cursor_Imp::Create(File* file, const achar* path, Vector2DI hot)
	{
		auto f = file->CreateStaticFile(path);
		if (f == nullptr) return nullptr;

		int32_t width, height;
		std::vector<uint8_t> dst;

		if (!ImageHelper::LoadPNGImage(f->GetData(), f->GetSize(), false, width, height, dst, nullptr)) return nullptr;

		GLFWimage img;
		img.pixels = dst.data();
		img.width = width;
		img.height = height;

		auto cursor = glfwCreateCursor(&img, hot.X, hot.Y);
		if (cursor == nullptr) return nullptr;

		return new Cursor_Imp(cursor);
	}
}