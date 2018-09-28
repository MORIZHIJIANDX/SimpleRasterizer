#pragma once
#ifndef BUFFER_H
#define BUFFER_H

#include "PCH.h"

struct BufferDesc
{
	BufferDesc(const BufferDesc &pDesc) : m_num_of_element(pDesc.m_num_of_element), m_stride(pDesc.m_stride)
	{
		m_buffer_size = m_num_of_element * m_stride;
		m_data = pDesc.m_data;
	}

	BufferDesc &operator=(const BufferDesc &pDesc)
	{
		m_num_of_element = pDesc.m_num_of_element;
		m_stride = pDesc.m_stride;
		m_buffer_size = pDesc.m_buffer_size;
		m_data = pDesc.m_data;
	}

	BufferDesc() : m_num_of_element(0), m_stride(0), m_buffer_size(0), m_data(nullptr)
	{

	}

	~BufferDesc()
	{

	}

	size_t m_num_of_element;
	size_t m_stride;
	size_t m_buffer_size;
	void *m_data;
};

class Buffer
{
public:
	friend class Context3D;
	Buffer(const BufferDesc &pDesc) : m_desc(pDesc)
	{
		m_data = new unsigned char[pDesc.m_buffer_size];
		std::memcpy(m_data, pDesc.m_data, pDesc.m_buffer_size);
	}

	~Buffer()
	{
		if (m_data != nullptr)
		{
			delete m_data;
		}
	}

	void SetRawData(const void *pData, const size_t &pSize)
	{
		if (pSize != m_desc.m_buffer_size)
		{
			throw std::exception("Error: Data size does not match");
		}

		if (pData == nullptr)
		{
			throw std::exception("Error: pData is nullptr");
		}

		std::memcpy(m_data, pData, m_desc.m_buffer_size);
	}

	void GetRawData(void *pData, size_t &pSize) const
	{
		pSize = m_desc.m_buffer_size;
		std::memcpy(pData, m_data, pSize);
	}

	const void *GetElementData(size_t &pIndex) const
	{
		return static_cast<unsigned char*>(m_data) + pIndex * m_desc.m_stride;
	}

	size_t GetElementNum()
	{
		return m_desc.m_num_of_element;
	}


	void *GetData()
	{
		return m_data;
	}
private:
	void *m_data;
	BufferDesc m_desc;
};
#endif // !BUFFER_H
