#include "Clipper.h"
#include "Rasterizer.h"
#include "Camera.h"
#include "RenderInterface.h"
#include "Window.h"
#include "App.h"
#include "ImageHelper.h"
#include "Light.h"

struct KeyFrame
{
	float m_time;
	Vec3f m_trans;
	Vec3f m_scale;
	Vec4f m_rotation_qua;
};

struct Animation
{
	float GetStartTime() const
	{
		return m_frames.front().m_time;
	}

	float GetEndTime() const
	{
		return m_frames.back().m_time;
	}

	void Interpolate(float pTime, Matrix4x4f &pMat) const
	{
		if (pTime <= m_frames.front().m_time)
		{
			Vec4f scale = LoadVector3(m_frames.front().m_scale);
			Vec4f rotation = m_frames.front().m_rotation_qua;
			Vec4f translation = LoadVector3(m_frames.front().m_trans);

			pMat = MatrixTransformation(scale, rotation, translation);
		}
		else if (pTime >= m_frames.back().m_time)
		{
			Vec4f scale = LoadVector3(m_frames.back().m_scale);
			Vec4f rotation = m_frames.back().m_rotation_qua;
			Vec4f translation = LoadVector3(m_frames.back().m_trans);

			pMat = MatrixTransformation(scale, rotation, translation);
		}
		else
		{
			for (size_t i = 0; i < m_frames.size() - 1; i++)
			{
				if (pTime >= m_frames[i].m_time && pTime <= m_frames[i + 1].m_time)
				{
					float lerp_percent = (pTime - m_frames[i].m_time) / (m_frames[i + 1].m_time - m_frames[i].m_time);

					Vec4f scale0 = LoadVector3(m_frames[i].m_scale);
					Vec4f rotation0 = m_frames[i].m_rotation_qua;
					Vec4f translation0 = LoadVector3(m_frames[i].m_trans);

					Vec4f scale1 = LoadVector3(m_frames[i + 1].m_scale);
					Vec4f rotation1 = m_frames[i + 1].m_rotation_qua;
					Vec4f translation1 = LoadVector3(m_frames[i + 1].m_trans);

					Vec4f scale = VectorLerp(scale0, scale1, lerp_percent);
					Vec4f rotation = QuaternionSlerp(rotation0, rotation1, lerp_percent);
					Vec4f translation = VectorLerp(translation0, translation1, lerp_percent);

					pMat = MatrixTransformation(scale, rotation, translation);
				}
			}
		}
	}

	std::vector<KeyFrame> m_frames;
};

struct ShaderStruct
{
	struct ConstBuffer
	{
		Matrix4x4f world;
		Matrix4x4f view_proj;

		Matrix4x4f world_inv_trans;

		Vec3f eye_posw;
		DirectionalLight light;
		Material mat;
	};

	static ConstBuffer buffer;

	inline static void VS(const Vertex &pVertexIn, Fragment &pVertexOut)
	{
		Vec4f posL(pVertexIn.m_pos.x, pVertexIn.m_pos.y, pVertexIn.m_pos.z, 1);
		Vec4f posW = Transform(posL, buffer.world);
		pVertexOut.m_pos = Transform(posW, buffer.view_proj);
		StoreVector3(pVertexOut.m_normal, Transform(LoadVector3(pVertexIn.m_normal), buffer.world_inv_trans));
		pVertexOut.m_uv = pVertexIn.m_uv;
		pVertexOut.pack0 = posW;
	}

	inline static void PS(const Fragment &pFragmentIn, Vec4f **pFragmentOut)
	{
		Vec4f tex_diff = LoadVector3(SampleTexturePoint<Vec3f>(App::GetContext()->GetShaderResource(0), pFragmentIn.m_uv));

		Vec4f ambient = Vec4f(0.0f);
		Vec4f diffuse = Vec4f(0.0f);
		Vec4f spec = Vec4f(0.0f);

		Vec3f toeye = Normalize(buffer.eye_posw - Vec3f(pFragmentIn.pack0.x, pFragmentIn.pack0.y, pFragmentIn.pack0.z));

		ComputeDirectionalLight(buffer.mat, buffer.light, Normalize(pFragmentIn.m_normal), toeye, ambient, diffuse, spec);

		Vec4f color = Mul(ambient + diffuse, tex_diff) + spec;
		(*pFragmentOut)[0] = color ;
	}
};

ShaderStruct::ConstBuffer ShaderStruct::buffer;

class SimpleApp : public App
{
public:
	SimpleApp(const std::string &pName = " ", HINSTANCE pHinstance = NULL, const size_t &pWidth = 512, const size_t &pHeight = 512) : App(pName, pHinstance, pWidth, pHeight)
	{
		m_vertex_buffer = nullptr;
		m_index_buffer = nullptr;
		m_depth_image = nullptr;
	}

	~SimpleApp()
	{
		m_vertex_buffer = nullptr;
		m_index_buffer = nullptr;
		m_depth_image = nullptr;
		SavePPMImage(m_swap_chain->GetBackBuffer(), "render_target.ppm");
	}

protected:
	virtual void Initialize() override
	{
		m_cam.SetPosition(0, 0, 0);
		float aspect = static_cast<float>(m_swap_chain->GetBackBufferWidth()) / static_cast<float>(m_swap_chain->GetBackBufferHeight());
		m_cam.SetLens(AngleToRadian(90.0f), aspect, 1.0f, 1000.0f);
		m_cam.UpdateViewMatrix();

		float width = 5;
		float height = 5;
		float depth = 5;

		Vertex v[24];

		float w2 = 0.5f*width;
		float h2 = 0.5f*height;
		float d2 = 0.5f*depth;

		//Vertex
		v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

		v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

		v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

		v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
		v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
		v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
		v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

		v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);


		size_t i[36];

		// Index
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		i[6] = 4; i[7] = 5; i[8] = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		i[12] = 8; i[13] = 9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		BufferDesc vertex_buffer_desc;
		vertex_buffer_desc.m_stride = sizeof(Vertex);
		vertex_buffer_desc.m_num_of_element = 24;
		vertex_buffer_desc.m_data = v;
		vertex_buffer_desc.m_buffer_size = sizeof(Vertex) * 24;

		m_vertex_buffer = m_device->CreateBuffer(vertex_buffer_desc);

		BufferDesc index_buffer_desc;
		index_buffer_desc.m_stride = sizeof(size_t);
		index_buffer_desc.m_num_of_element = 36;
		index_buffer_desc.m_data = i;
		index_buffer_desc.m_buffer_size = sizeof(size_t) * 36;

		m_index_buffer = m_device->CreateBuffer(index_buffer_desc);

		ImageDesc depth_image_desc;
		depth_image_desc.m_format = IMAGE_FORMAT::R32_FLOAT;
		depth_image_desc.m_width = m_swap_chain->GetBackBufferWidth();
		depth_image_desc.m_height = m_swap_chain->GetBackBufferHeight();

		m_depth_image = m_device->CreateImage(depth_image_desc);
		m_color_image = ReadPPMImage("RenderTest\\kugga.ppm");

		Viewport port;
		port.m_top_leftx = 0;
		port.m_top_lefty = 0;
		port.m_width = static_cast<float>(m_swap_chain->GetBackBufferWidth());
		port.m_height = static_cast<float>(m_swap_chain->GetBackBufferHeight());

		m_context->SetViewport(port);
		
		m_anima.m_frames.reserve(5);

		KeyFrame frame0;
		frame0.m_time = 0;
		frame0.m_scale = Vec3f(1,1,1);
		frame0.m_rotation_qua = QuaternionRotationAxis(Vec4f(0,1,0,0), AngleToRadian(30.0f));
		frame0.m_trans = Vec3f(5,0,10);

		KeyFrame frame1;
		frame1.m_time = 2;
		frame1.m_scale = Vec3f(0.5, 0.5, 0.5);
		frame1.m_rotation_qua = QuaternionRotationAxis(Vec4f(0, 1, 0, 0), AngleToRadian(45.0f));
		frame1.m_trans = Vec3f(0, 5, 5);

		KeyFrame frame2;
		frame2.m_time = 4;
		frame2.m_scale = Vec3f(1.5, 1.5, 1.5);
		frame2.m_rotation_qua = QuaternionRotationAxis(Vec4f(0, 1, 0, 0), AngleToRadian(-30.0f));
		frame2.m_trans = Vec3f(3, 3, 7);

		KeyFrame frame3;
		frame3.m_time = 6;
		frame3.m_scale = Vec3f(0.25, 0.25, 0.25);
		frame3.m_rotation_qua = QuaternionRotationAxis(Vec4f(0, 1, 0, 0), AngleToRadian(70.0f));
		frame3.m_trans = Vec3f(-5, -3, 4);

		KeyFrame frame4;
		frame4.m_time = 8;
		frame4.m_scale = Vec3f(1, 1, 1);
		frame4.m_rotation_qua = QuaternionRotationAxis(Vec4f(0, 1, 0, 0), AngleToRadian(30.0f));
		frame4.m_trans = Vec3f(5, 0, 10);

		m_anima.m_frames.push_back(frame0);
		m_anima.m_frames.push_back(frame1);
		m_anima.m_frames.push_back(frame2);
		m_anima.m_frames.push_back(frame3);
		m_anima.m_frames.push_back(frame4);

		m_anima_time = 0;

		DirectionalLight light;
		light.m_ambient = Vec4f(0.1f, 0.1f, 0.1f, 1.0f);
		light.m_diffuse = Vec4f(0.8f, 0.8f, 0.8f, 1.0f);
		light.m_specular = Vec4f(0.5f, 0.5, 0.5, 1.0f);
		light.m_direction = Vec3f(0.57735f, -0.57735f, 0.57735f);

		Material mat;
		mat.m_ambient = Vec4f(0.5f, 0.5f, 0.5f, 1.0f);
		mat.m_diffuse = Vec4f(0.8f, 0.2f, 0.4f, 1.0f);
		mat.m_specular = Vec4f(0.2f, 0.2f, 0.2f, 16.0f);

		ShaderStruct::buffer.light = light;
		ShaderStruct::buffer.mat = mat;
		ShaderStruct::buffer.eye_posw = m_cam.GetPosition();
	}

	virtual void Update(const float &pDelta) override
	{
		m_anima_time += pDelta;

		if (m_anima_time >= m_anima.GetEndTime())
		{
			m_anima_time -= m_anima.GetEndTime();
		}
		
		m_anima.Interpolate(m_anima_time, ShaderStruct::buffer.world);

		ShaderStruct::buffer.world_inv_trans = Matrix4x4Transpose(Matrix4x4Inverse(ShaderStruct::buffer.world));
		ShaderStruct::buffer.view_proj = m_cam.GetViewProjMatrix();
	}

	virtual void Render(const float &pDelta) override
	{
		m_context->SetFragmentLayout(FragmentLayout::EXTENSION0);
		m_context->SeteDepthBuffer(m_depth_image);
		m_context->SetIndexBuffer(m_index_buffer);
		m_context->SetVertexBuffer(m_vertex_buffer);

		std::shared_ptr<Image> targets[1];
		targets[0] = m_swap_chain->GetBackBuffer();
		m_context->SetRenderTargets(targets, 1);

		std::shared_ptr<Image> resource[1];
		resource[0] = m_color_image;
		m_context->SetShaderResources(resource, 1);

		m_context->SetVertexShader(ShaderStruct::VS);
		m_context->SetFragmentShader(ShaderStruct::PS);

		m_context->Draw();
	}

private:
	Camera m_cam;
	std::shared_ptr<Buffer> m_vertex_buffer;
	std::shared_ptr<Buffer> m_index_buffer;

	std::shared_ptr<Image> m_depth_image;
	std::shared_ptr<Image> m_color_image;

	Animation m_anima;
	float m_anima_time;
};

int APIENTRY wWinMain(HINSTANCE pHinstance, HINSTANCE, LPWSTR, int pShow)
{
	SimpleApp app("SimpeRasterizer", pHinstance, 600, 600);
	app.Run();
	return 0;
}

