#pragma once
#ifndef RENDERINTERFACE_H
#define RENDERINTERFACE_H

#include "Buffer.h"
#include "Image.h"
#include "Clipper.h"
#include "Rasterizer.h"

using VertexShader = std::function<void(const Vertex &pVertexIn, Fragment &pVertexOut)>;
using FragmentShader = std::function<void(const Fragment &pFragmentIn, Vec4f **pFragmentOut)>;

class SwapChain
{
public:
	SwapChain(HWND pHwnd, size_t pWidth, size_t pHeight);
	~SwapChain();

	void Present();

	std::shared_ptr<Image> GetBackBuffer();

	void ClearBackBuffer(const Vec4<uint8_t> &pColor);

	size_t GetBackBufferWidth();
	size_t GetBackBufferHeight();

private:
	HWND m_hwnd;
	HDC m_hdc;
	HBITMAP m_bit_map;
	std::shared_ptr<Image> m_back_buffer;
};

class Device3D
{
public:
	Device3D();
	~Device3D();

	std::shared_ptr<Image> CreateImage(const ImageDesc &pDesc, const std::string &pName = " ");
	std::shared_ptr<Buffer> CreateBuffer(const BufferDesc &pDesc);
};

class Context3D
{
public:
	Context3D();
	~Context3D();

	void SetViewport(const Viewport &pViewport);
	Viewport GetViewport();

	void SetFragmentLayout(const FragmentLayout &pLayout);

	void SetVertexBuffer(std::shared_ptr<Buffer> pVertexBuffer);
	void SetIndexBuffer(std::shared_ptr<Buffer> pIndexBuffer);

	void SetRenderTargets(std::shared_ptr<Image> pTargets[], const size_t &pNum);
	void SetShaderResources(std::shared_ptr<Image> pResources[], const size_t &pNum);
	void SeteDepthBuffer(std::shared_ptr<Image> pDepth);

	void SetVertexShader(VertexShader pVertexShader);
	void SetFragmentShader(FragmentShader pFragmentShader);

	std::shared_ptr<Image> GetShaderResource(const size_t &pIndex);

	void UnbindShaderResources();
	void UnbindRenderTargets();
	void UnbindDepthBuffer();

	void ClearDepthBuffer();

	void Draw();

private:
	void WriteOutputToRenderTarget(Vec4f *pOut, const Vec2I &pIndex)
	{
		for (size_t i = 0; i < m_rtv_num; i++)
		{
			IMAGE_FORMAT format = m_render_targets[i]->GetFormat();
			Vec4f out = pOut[i];

			switch (format)
			{
			case R32_FLOAT:
				std::dynamic_pointer_cast<ExtensionImage<float>>(m_render_targets[i])->SetPixel(out.x, pIndex);
				break;
			case R32G32_FLOAT:
				std::dynamic_pointer_cast<ExtensionImage<Vec2f>>(m_render_targets[i])->SetPixel(Vec2f(out.x, out.y), pIndex);
				break;
			case R32G32B32_FLOAT:
				std::dynamic_pointer_cast<ExtensionImage<Vec3f>>(m_render_targets[i])->SetPixel(Vec3f(out.x, out.y, out.z), pIndex);
				break;
			case R32G32B32A32_FLOAT:
				std::dynamic_pointer_cast<ExtensionImage<Vec4f>>(m_render_targets[i])->SetPixel(out, pIndex);
				break;
			case R8G8B8A8_UINT:		
				{
					//std::dynamic_pointer_cast<ExtensionImage<Vec4<uint8_t>>>(m_render_targets[i])->SetPixel(Vec4<uint8_t>(out.y * 255, out.z * 255, out.x * 255, out.w * 255), pIndex.x, m_render_targets[i]->GetHeight() - 1 - pIndex.y);
					std::dynamic_pointer_cast<ExtensionImage<Vec4<uint8_t>>>(m_render_targets[i])->SetPixel(Vec4<uint8_t>(static_cast<uint8_t>(out.b * 255), static_cast<uint8_t>(out.g * 255), static_cast<uint8_t>(out.r * 255), 1), pIndex.x, m_render_targets[i]->GetHeight() - 1 - pIndex.y);
				}
				break;
			default:
				break;
			}
		}
	}

	VertexShader m_vertex_shader;
	FragmentShader m_fragment_shader;

	std::shared_ptr<Image> m_shader_resources[5];
	std::shared_ptr<Image> m_render_targets[5];
	std::shared_ptr<ExtensionImage<float>> m_depth_buffer;

	std::shared_ptr<Buffer> m_vertex_buffer;
	std::shared_ptr<Buffer> m_index_buffer;

	std::shared_ptr<Clipper> m_clipper;
	std::shared_ptr<Rasterizer> m_rasterizer;

	size_t m_srv_num;
	size_t m_rtv_num;

	FragmentLayout m_layout;
	Viewport m_viewport;
};
#endif // !RENDERINTERFACE_H
