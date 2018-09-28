#pragma once
#ifndef IMAGE_H
#define IMAGE_H
#include "RenderMath.h"

enum class IMAGE_BIND_FLAG
{
	UNBIND,
	RENDER_TARGET,
	SHADER_RESOURCE
};

enum IMAGE_FORMAT : size_t
{
	R32_FLOAT = 0,
	R32G32_FLOAT = 1,
	R32G32B32_FLOAT = 2,
	R32G32B32A32_FLOAT = 3,
	R8G8B8A8_UINT = 4
};

template<typename T>
inline bool TypeCheck(const IMAGE_FORMAT &pFormat)
{
	return false;
}

template<>
inline bool TypeCheck<float>(const IMAGE_FORMAT &pFormat)
{
	if (pFormat == IMAGE_FORMAT::R32_FLOAT)
	{
		return true;
	}

	return false;
}

template<>
inline bool TypeCheck<Vec2f>(const IMAGE_FORMAT &pFormat)
{
	if (pFormat == IMAGE_FORMAT::R32G32_FLOAT)
	{
		return true;
	}

	return false;
}

template<>
inline bool TypeCheck<Vec3f>(const IMAGE_FORMAT &pFormat)
{
	if (pFormat == IMAGE_FORMAT::R32G32B32_FLOAT)
	{
		return true;
	}

	return false;
}

template<>
inline bool TypeCheck<Vec4f>(const IMAGE_FORMAT &pFormat)
{
	if (pFormat == IMAGE_FORMAT::R32G32B32A32_FLOAT)
	{
		return true;
	}

	return false;
}

template<>
inline bool TypeCheck<Vec4<uint8_t>>(const IMAGE_FORMAT &pFormat)
{
	if (pFormat == IMAGE_FORMAT::R8G8B8A8_UINT)
	{
		return true;
	}

	return false;
}

inline size_t GetTextureFormatSize(const IMAGE_FORMAT &pFormat)
{
	static size_t format_size[5] = {sizeof(float), sizeof(Vec2f), sizeof(Vec3f), sizeof(Vec4f), sizeof(Vec4<uint8_t>)};
	return format_size[pFormat];
}

struct ImageDesc
{
	ImageDesc(const IMAGE_FORMAT &pFormat, const size_t &pWidth, const size_t &pHeight)
		: m_format(pFormat), m_width(pWidth), m_height(pHeight)
	{

	}

	ImageDesc(const ImageDesc &pDesc) : m_format(pDesc.m_format), m_width(pDesc.m_width),
		m_height(pDesc.m_height)
	{

	}

	ImageDesc() : m_format(IMAGE_FORMAT::R8G8B8A8_UINT), m_width(0), m_height(0)
	{

	}

	~ImageDesc()
	{

	}

	ImageDesc &operator=(const ImageDesc &pDesc)
	{
		m_format = pDesc.m_format;
		m_width = pDesc.m_width;
		m_height = pDesc.m_height;
	}

	IMAGE_FORMAT m_format;
	size_t m_width;
	size_t m_height;
};

class Image
{
public:
	friend class Context3D;
	Image(const ImageDesc &pDesc) : m_desc(pDesc), m_flag(IMAGE_BIND_FLAG::UNBIND), m_data(nullptr)
	{
		size_t format_size = GetTextureFormatSize(pDesc.m_format);
		m_data = new unsigned char[format_size * pDesc.m_width * pDesc.m_height];
		m_map_flag = true;
	}

	Image(const ImageDesc &pDesc, void *pData) : m_desc(pDesc), m_flag(IMAGE_BIND_FLAG::UNBIND), m_data(nullptr)
	{
		m_data = pData;
		m_map_flag = false;
	}

	virtual ~Image()
	{
		if (m_map_flag == true && m_data != nullptr)
		{
			delete[] m_data;
		}
	}

	IMAGE_FORMAT GetFormat()
	{
		return m_desc.m_format;
	}

	void ResizeImage(const size_t &pWidth, const size_t &pHeight)
	{
		m_desc.m_width = pWidth;
		m_desc.m_height = pHeight;

		if (m_data != nullptr && m_map_flag == true)
		{
			delete[] m_data;
		}

		if (m_map_flag == true)
		{
			size_t format_size = GetTextureFormatSize(m_desc.m_format);
			m_data = new unsigned char[format_size * m_desc.m_width * m_desc.m_height];
		}
	}

	size_t GetWidth() const
	{
		return m_desc.m_width;
	}

	size_t GetHeight() const
	{
		return m_desc.m_height;
	}

	ImageDesc GetDesc()
	{
		return m_desc;
	}

	const void *GetRawData() const
	{
		return m_data;
	}

	template<typename T>
	void Clear(const T &pPixel = T(0))
	{
		if (!TypeCheck<T>(m_desc.m_format))
		{
			throw std::exception("Error: Image type error");
		}
#ifdef PARALL
		concurrency::parallel_for(size_t(0), m_desc.m_width * m_desc.m_height, [&](const size_t &i) {
			*(static_cast<T*>(m_data) + i) = pPixel;
		});
#else
		for (size_t i = 0; i < m_desc.m_width * m_desc.m_height; i++)
		{
			*(static_cast<T*>(m_data) + i) = pPixel;
		}
#endif // PARALL
	}

protected:
	void Unbind()
	{
		m_flag = IMAGE_BIND_FLAG::UNBIND;
	}

	void BindShaderResource()
	{
		if (m_flag == IMAGE_BIND_FLAG::RENDER_TARGET)
		{
			throw std::exception("Error: Image already bind as render target");
		}

		m_flag = IMAGE_BIND_FLAG::SHADER_RESOURCE;
	}

	void BindRenderTarget()
	{
		if (m_flag == IMAGE_BIND_FLAG::SHADER_RESOURCE)
		{
			throw std::exception("Error: Image already bind as shader resource");
		}

		m_flag = IMAGE_BIND_FLAG::RENDER_TARGET;
	}

	ImageDesc m_desc;
	IMAGE_BIND_FLAG m_flag;
	void *m_data;
	bool m_map_flag; // if true image has data, if false image mapped data
};

template<typename T>
class ExtensionImage : public Image
{
public:
	ExtensionImage(const ImageDesc &pDesc, const std::string &pName = " ") : Image(pDesc), m_name(pName)
	{
		if(!TypeCheck<T>(pDesc.m_format))
		{
			throw std::exception("Error: Image type error");
		}
	}

	ExtensionImage(const ImageDesc &pDesc, void *pData, const std::string &pName = " ") : Image(pDesc, pData), m_name(pName)
	{
		if (!TypeCheck<T>(pDesc.m_format))
		{
			throw std::exception("Error: Image type error");
		}
	}

	~ExtensionImage()
	{
	
	}

	void SetPixel(const T &pPixel, const size_t &pX, const size_t &pY)
	{
#if DEBUG
		if (pX >= m_desc.m_width || pY >= m_desc.m_height)
		{
			throw std::exception("Error: Out of range");
		}
#endif // DEBUG
		size_t index = pX + pY * m_desc.m_width;
		*(static_cast<T*>(m_data) + index) = pPixel;
	}

	void SetPixel(const T &pPixel, const Vec2I &pIndex)
	{
#if DEBUG
		if (pIndex.x >= m_desc.m_width || pIndex.y >= m_desc.m_height)
		{
			throw std::exception("Error: Out of range");
		}
#endif // DEBUG
		size_t index = pIndex.x + pIndex.y * m_desc.m_width;
		*(static_cast<T*>(m_data) + index) = pPixel;
	}

	T &GetPixel(const size_t &pX, const size_t &pY) const
	{
		size_t index = pX + pY * m_desc.m_width;
#if DEBUG
		if (pX >= m_desc.m_width || pY >= m_desc.m_height)
		{
			throw std::exception("Error: Out of range");
		}
#endif // DEBUG
		return *(static_cast<T*>(m_data) + index);
	}

	T &GetPixel(const Vec2I &pIndex) const
	{
		size_t index = pIndex.x + pIndex.y * m_desc.m_width;
#if DEBUG
		if (pIndex.x >= m_desc.m_width || pIndex.y >= m_desc.m_height)
		{
			throw std::exception("Error: Out of range");
		}
#endif // DEBUG
		return *(static_cast<T*>(m_data) + index);
	}

private:
	std::string m_name;
};
#endif // !IMAGE_H
