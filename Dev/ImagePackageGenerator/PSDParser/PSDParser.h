
#pragma once

#include "PSDParser.Base.h"

#include "psd/psd.h"

namespace PSDParser
{
	enum class LayerObjectType
	{
		Image,
		Button,
	};

	enum class LayerAdditionalObjectType
	{
		None,
		Normal,
		Pressed,
		Hovered,
	};

	struct Rect
	{
		int32_t Top;
		int32_t Left;
		int32_t Bottom;
		int32_t Right;
	};

	class Layer
		: public std::enable_shared_from_this<Layer>
	{
		friend class Document;

		std::basic_string<uchar>	name;
		std::vector<uint8_t>		data;
		Rect						area;
		bool						isFolderBegin;
		bool						isFolderEnd;
		std::basic_string<uchar>	text;
	public:

#if !SWIG
		Layer(const uint8_t* data, Rect area, bool isFolderBegin, bool isFolderEnd, std::basic_string<uchar> name)
		{
			auto width = area.Right - area.Left;
			auto height = area.Bottom - area.Top;

			this->data.resize(width * height * 4);
			memcpy(this->data.data(), data, this->data.size());

			this->area = area;
			this->name = name;

			this->isFolderBegin = isFolderBegin;
			this->isFolderEnd = isFolderEnd;
		}
#endif
		Layer() {}
		virtual ~Layer() {}

		void Extend(Rect newArea)
		{
			if (area.Bottom == newArea.Bottom &&
				area.Top == newArea.Top &&
				area.Left == newArea.Left &&
				area.Right == newArea.Right) return;

			auto width = area.Right - area.Left;
			auto height = area.Bottom - area.Top;

			auto nwidth = newArea.Right - newArea.Left;
			auto nheight = newArea.Bottom - newArea.Top;

			std::vector<uint8_t> ndata;
			ndata.resize(nwidth * nheight * 4);
			memset(ndata.data(), 0, ndata.size());
			
			for (int32_t y = newArea.Top; y < newArea.Bottom; y++)
			{
				if (y < area.Top) continue;
				if (y >= area.Bottom) continue;

				for (int32_t x = newArea.Left; x < newArea.Right; x++)
				{
					if (x < area.Left) continue;
					if (x >= area.Right) continue;

					auto dx = x - newArea.Left;
					auto dy = y - newArea.Top;
					auto sx = x - area.Left;
					auto sy = y - area.Top;

					ndata[(dx + nwidth * dy) * 4 + 0] = data[(sx + width * sy) * 4 + 0];
					ndata[(dx + nwidth * dy) * 4 + 1] = data[(sx + width * sy) * 4 + 1];
					ndata[(dx + nwidth * dy) * 4 + 2] = data[(sx + width * sy) * 4 + 2];
					ndata[(dx + nwidth * dy) * 4 + 3] = data[(sx + width * sy) * 4 + 3];
				}
			}

			area = newArea;
			data = ndata;
		}

		const void* GetData()
		{
			return data.data();
		}

		Rect GetRect()
		{
			return area;
		}

		bool IsFolderBegin() const { return isFolderBegin; }

		bool IsFolderEnd() const { return isFolderEnd; }

		const uchar* GetName() const { return name.c_str(); }

		LayerObjectType	ObjectType = LayerObjectType::Image;

		LayerAdditionalObjectType AdditionalObjectType = LayerAdditionalObjectType::None;
	};

	class Document
		: public std::enable_shared_from_this<Document>
	{
	private:

		std::vector<std::shared_ptr<Layer>>	layers;
		int32_t						colorDepth = 0;
		int32_t						docWidth = 0;
		int32_t						docHeight = 0;

	public:

		Document();

		virtual ~Document();

#if !SWIG
		bool Load(const void* data, int32_t size);
#endif

		static std::shared_ptr<Document> Create(const void* data, int32_t size);

		int32_t GetLayerCount();

		std::shared_ptr<Layer> GetLayer(int32_t index);

		int32_t GetWidth() const;

		int32_t GetHeight() const;

		size_t GetDepth();

	};
}

