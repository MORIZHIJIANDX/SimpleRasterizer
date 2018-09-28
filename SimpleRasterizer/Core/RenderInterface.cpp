#include "RenderInterface.h"

SwapChain::SwapChain(HWND pHwnd, size_t pWidth, size_t pHeight) : m_hwnd(pHwnd)
{
	HDC hdc = GetDC(m_hwnd);
	m_hdc = CreateCompatibleDC(hdc);
	ReleaseDC(m_hwnd, hdc);

	BITMAPINFO bmp_info =
	{
		sizeof(BITMAPINFOHEADER),
		pWidth,
		pHeight,
		1,
		32,
		BI_RGB,
		pWidth * pHeight * 4,
		0,
		0,
		0,
		0
	};

	LPVOID ptr = nullptr;
	m_bit_map = CreateDIBSection(m_hdc, &bmp_info, DIB_RGB_COLORS, &ptr, nullptr, 0);
	if (!m_bit_map)
	{
		MessageBox(0, "CreateDIBSection Failed.", 0, 0);
		throw std::exception("Error: CreateDIBSection Failed");
	}
	SelectObject(m_hdc, m_bit_map);

	ImageDesc back_buffer_desc;
	back_buffer_desc.m_width = pWidth;
	back_buffer_desc.m_height = pHeight;
	back_buffer_desc.m_format = IMAGE_FORMAT::R8G8B8A8_UINT;

	m_back_buffer = std::make_shared<ExtensionImage<Vec4<uint8_t>>>(back_buffer_desc, ptr, "back_buffer");
}

SwapChain::~SwapChain()
{
	m_back_buffer = nullptr;

	if (m_hdc)
		DeleteDC(m_hdc);
	m_hdc = nullptr;

	if (m_bit_map)
		DeleteObject(m_bit_map);
	m_bit_map = nullptr;
}

void SwapChain::Present()
{
	HDC hdc = GetDC(m_hwnd);
	BitBlt(hdc, 0, 0, static_cast<int>(m_back_buffer->GetWidth()), static_cast<int>(m_back_buffer->GetHeight()), m_hdc, 0, 0, SRCCOPY);
	ReleaseDC(m_hwnd, hdc);
}

std::shared_ptr<Image> SwapChain::GetBackBuffer()
{
	return m_back_buffer;
}

void SwapChain::ClearBackBuffer(const Vec4<uint8_t> &pColor)
{
	m_back_buffer->Clear(pColor);
}

size_t SwapChain::GetBackBufferWidth()
{
	return m_back_buffer->GetWidth();
}

size_t SwapChain::GetBackBufferHeight()
{
	return m_back_buffer->GetHeight();
}

Device3D::Device3D()
{
}

Device3D::~Device3D()
{
}

std::shared_ptr<Image> Device3D::CreateImage(const ImageDesc &pDesc, const std::string &pName)
{
	std::shared_ptr<Image> image = nullptr;
	switch (pDesc.m_format)
	{
	case IMAGE_FORMAT::R8G8B8A8_UINT:
		image = std::make_shared<ExtensionImage<Vec4<uint8_t>>>(pDesc, pName);
		break;
	case IMAGE_FORMAT::R32_FLOAT:
		image = std::make_shared<ExtensionImage<float>>(pDesc, pName);
		break;
	case IMAGE_FORMAT::R32G32_FLOAT:
		image = std::make_shared<ExtensionImage<Vec2f>>(pDesc, pName);
		break;
	case IMAGE_FORMAT::R32G32B32_FLOAT:
		image = std::make_shared<ExtensionImage<Vec3f>>(pDesc, pName);
		break;
	case IMAGE_FORMAT::R32G32B32A32_FLOAT:
		image = std::make_shared<ExtensionImage<Vec4f>>(pDesc, pName);
		break;
	default:
		break;
	}
	return image;
}

std::shared_ptr<Buffer> Device3D::CreateBuffer(const BufferDesc &pDesc)
{
	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(pDesc);
	return buffer;
}


Context3D::Context3D()
{
	m_clipper = std::make_shared<Clipper>();
	m_rasterizer = std::make_shared<Rasterizer>();
}

Context3D::~Context3D()
{
	m_clipper = nullptr;
	m_rasterizer = nullptr;
}

void Context3D::SetViewport(const Viewport &pViewport)
{
	m_viewport = pViewport;
	m_rasterizer->SetViewport(m_viewport);
}

Viewport Context3D::GetViewport()
{
	return m_viewport;
}

void Context3D::SetFragmentLayout(const FragmentLayout &pLayout)
{
	m_layout = pLayout;
	m_clipper->SetFragmentLayout(pLayout);
	m_rasterizer->SetFragmentLayout(pLayout);
}

void Context3D::SetVertexBuffer(std::shared_ptr<Buffer> pVertexBuffer)
{
	m_vertex_buffer = pVertexBuffer;
}

void Context3D::SetIndexBuffer(std::shared_ptr<Buffer> pIndexBuffer)
{
	m_index_buffer = pIndexBuffer;
}

void Context3D::SetRenderTargets(std::shared_ptr<Image> pTargets[], const size_t &pNum)
{
	m_rtv_num = pNum;
	for (size_t i = 0; i < pNum; i++)
	{
		m_render_targets[i] = pTargets[i];
		m_render_targets[i]->BindRenderTarget();
	}
}

void Context3D::SetShaderResources(std::shared_ptr<Image> pResources[], const size_t &pNum)
{
	m_srv_num = pNum;
	for (size_t i = 0; i < pNum; i++)
	{
		m_shader_resources[i] = pResources[i];
		m_shader_resources[i]->BindShaderResource();
	}
}

void Context3D::SeteDepthBuffer(std::shared_ptr<Image> pDepth)
{
	if (pDepth->GetFormat() != IMAGE_FORMAT::R32_FLOAT)
	{
		throw std::exception("Error: Depth buffer type error");
	}

	m_depth_buffer = std::dynamic_pointer_cast<ExtensionImage<float>>(pDepth);
	m_depth_buffer->Clear<float>(255.0f);
	m_depth_buffer->BindRenderTarget();
}

void Context3D::SetVertexShader(VertexShader pVertexShader)
{
	m_vertex_shader = pVertexShader;
}

void Context3D::SetFragmentShader(FragmentShader pFragmentShader)
{
	m_fragment_shader = pFragmentShader;
}

std::shared_ptr<Image> Context3D::GetShaderResource(const size_t &pIndex)
{
	return m_shader_resources[pIndex];
}

void Context3D::UnbindShaderResources()
{
	for (size_t i = 0; i < m_srv_num; i++)
	{
		m_shader_resources[i]->Unbind();
		m_shader_resources[i] = nullptr;
	}
	m_srv_num = 0;
}

void Context3D::UnbindRenderTargets()
{
	for (size_t i = 0; i < m_rtv_num; i++)
	{
		m_render_targets[i]->Unbind();
		m_render_targets[i] = nullptr;
	}
	m_rtv_num = 0;
}

void Context3D::UnbindDepthBuffer()
{
	m_depth_buffer->Unbind();
	m_depth_buffer = nullptr;
}

void Context3D::ClearDepthBuffer()
{
	if (m_depth_buffer != nullptr)
	{
		m_depth_buffer->Clear<float>(255.0f);
	}
}

void Context3D::Draw()
{
	if ((m_vertex_buffer == nullptr) || (m_index_buffer == nullptr))
	{
		throw std::exception("Error: vertex buffer or index buffer is nullptr");
	}

	size_t vertex_num = m_vertex_buffer->GetElementNum();
	Vertex *vertex_data = static_cast<Vertex*>(m_vertex_buffer->GetData());
	Fragment *processed_vertexs = new Fragment[vertex_num];

#ifdef PARALL
	concurrency::parallel_for(size_t(0), vertex_num, [&](const size_t &i) {
		m_vertex_shader(vertex_data[i], processed_vertexs[i]);
	});
#else
	for (size_t i = 0; i < vertex_num; i++)
	{
		m_vertex_shader(vertex_data[i], processed_vertexs[i]);
	}
#endif // PARALL

	size_t triangle_num = m_index_buffer->GetElementNum() / 3;
	size_t *index_data = static_cast<size_t*>(m_index_buffer->GetData());
	Triangle *triangles = new Triangle[triangle_num];
	for (size_t i = 0; i < triangle_num; i++)
	{
		size_t index_begin = i * 3;
		triangles[i].m_vertex[0] = processed_vertexs[index_data[index_begin]];
		triangles[i].m_vertex[1] = processed_vertexs[index_data[index_begin + 1]];
		triangles[i].m_vertex[2] = processed_vertexs[index_data[index_begin + 2]];
	}

	delete[] processed_vertexs;

	m_clipper->Clip(&triangles, triangle_num);

	std::vector<Fragment> fragments;
	std::vector<Vec2I> fragmentIndexes;

	for (size_t i = 0; i < triangle_num; i++)
	{
		m_rasterizer->Rasterize(triangles[i], fragments, fragmentIndexes, m_depth_buffer);

		size_t fragment_size = fragments.size();
		Vec4f *fragment_out = new Vec4f[fragment_size * m_rtv_num];
		Vec4f *curr_fragment_out = fragment_out;

#ifdef PARALL
		concurrency::parallel_for(size_t(0), fragment_size, [&](const size_t &j) {
			Vec4f *curr_fragment_out = fragment_out + j * m_rtv_num;
			m_fragment_shader(fragments[j], &curr_fragment_out);
			WriteOutputToRenderTarget(curr_fragment_out, fragmentIndexes[j]);
		});
#else
		for (size_t j = 0; j < fragment_size; j++)
		{
			m_fragment_shader(fragments[j], &curr_fragment_out);
			WriteOutputToRenderTarget(curr_fragment_out, fragmentIndexes[j]);
			curr_fragment_out += m_rtv_num;
		}
#endif // PARALL 

		fragments.clear();
		fragmentIndexes.clear();

		delete[] fragment_out;
	}

	delete[] triangles;
}