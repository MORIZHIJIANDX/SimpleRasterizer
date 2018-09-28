#pragma once
#ifndef IMAGEHELER_H
#define IMAGEHELER_H
#include "Image.h"

enum class TEXURE_SAMPLE_STATE
{
	POINT,
	LINEAR
};

template<typename T>
inline T SampleTexturePoint(std::shared_ptr<Image> pImage, const Vec2f &pIndex)
{
	Vec2I index(min(std::round(pIndex.x * pImage->GetWidth()), pImage->GetWidth() - 1), min(std::round(pIndex.y * pImage->GetHeight()), pImage->GetHeight() - 1));
	return (std::dynamic_pointer_cast<ExtensionImage<T>>(pImage))->GetPixel(index);
}

template<typename T>
inline T SampleTextureLinear(std::shared_ptr<Image> pImage, const Vec2f &pIndex)
{
	int image_width = pImage->GetWidth();
	int image_height = pImage->GetHeight();

	Vec2f float_index(pIndex.x * (image_width - 1), pIndex.y * (image_height - 1));

	Vec2I index(static_cast<int>(std::floor(float_index.x)), static_cast<int>(std::floor(float_index.y)));
	std::shared_ptr<ExtensionImage<T>> ex_image = std::dynamic_pointer_cast<ExtensionImage<T>>(pImage);

	T c00 = ex_image->GetPixel(index.x, index.y);
	T c01 = ex_image->GetPixel(index.x, min(index.y + 1, image_height -1));
	T c10 = ex_image->GetPixel(min(index.x + 1, image_width - 1), index.y);
	T c11 = ex_image->GetPixel(min(index.x + 1, image_width - 1), min(index.y + 1, image_height -1));

	float tx = float_index.x - index.x;
	float ty = float_index.y - index.y;

	T a = c00 * (1 - tx) + c10 * tx;
	T b = c01 * (1 - tx) + c11 * tx;

	return a * (1 - ty) + b * ty;
}

template<typename T>
T SampleTexture(std::shared_ptr<Image> pImage, const Vec2f &pIndex, const TEXURE_SAMPLE_STATE &pState = TEXURE_SAMPLE_STATE::POINT)
{
	if (!TypeCheck<T>(pImage->GetFormat()))
	{
		throw std::exception("Error: Texture sample type error");
	}

	switch (pState)
	{
	case TEXURE_SAMPLE_STATE::POINT:
		return SampleTexturePoint<T>(pImage, pIndex);
		break;
	case TEXURE_SAMPLE_STATE::LINEAR:
		return SampleTextureLinear<T>(pImage, pIndex);
		break;
	default:
		break;
	}

	return T(0);
}

void GetImageColor(Vec3f &pColor, const Vec2I &pIndex, std::shared_ptr<Image> pImage, bool pRepeat = false)
{
	switch (pImage->GetFormat())
	{
	case R32_FLOAT:
	{
		float temp = std::dynamic_pointer_cast<ExtensionImage<float>>(pImage)->GetPixel(pIndex);
		if (pRepeat)
		{
			pColor.x = pColor.y = pColor.z = temp * 255;
		}
		else
		{
			pColor.x = temp * 255;
		}
		return;
	}
		break;
	case R32G32_FLOAT:
	{
		Vec2f temp = std::dynamic_pointer_cast<ExtensionImage<Vec2f>>(pImage)->GetPixel(pIndex);
		if (pRepeat)
		{
			pColor.x = temp.x * 255;
			pColor.y = temp.y * 255;
			pColor.z = temp.y * 255;
		}
		else
		{
			pColor.x = temp.x * 255;
			pColor.y = temp.y * 255;
		}
		return;
	}
		break;
	case R32G32B32_FLOAT:
	{
		Vec3f temp = std::dynamic_pointer_cast<ExtensionImage<Vec3f>>(pImage)->GetPixel(pIndex);

		pColor.x = temp.x * 255;
		pColor.y = temp.y * 255;
		pColor.z = temp.z * 255;

		return;
	}
		break;
	case R32G32B32A32_FLOAT:
	{
		Vec4f temp = std::dynamic_pointer_cast<ExtensionImage<Vec4f>>(pImage)->GetPixel(pIndex);

		pColor.x = temp.x * 255;
		pColor.y = temp.y * 255;
		pColor.z = temp.z * 255;

		return;
	}
		break;
	case R8G8B8A8_UINT:
	{
		Vec4<uint8_t> temp = std::dynamic_pointer_cast<ExtensionImage<Vec4<uint8_t>>>(pImage)->GetPixel(pIndex.x, pImage->GetHeight() - 1 - pIndex.y);

		pColor.x = temp.z;
		pColor.y = temp.y;
		pColor.z = temp.x;

		return;
	}
		break;
	default:
		break;
	}
}

void SavePPMImage(std::shared_ptr<Image> pImage, const std::string &name)
{
	std::ofstream output;
	output.open(name, std::ios::binary);
	try
	{
		if (output.fail())
		{
			throw("open write image failed");
		}
		output << "P6\n" << pImage->GetWidth() << " " << pImage->GetHeight() << "\n";
		float maxval = 255.0f;
		output << maxval << "\n";

		size_t width = pImage->GetWidth();
		size_t height = pImage->GetHeight();

		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				Vec3f color;

				GetImageColor(color, Vec2I(static_cast<int>(j), static_cast<int>(i)), pImage);

				unsigned char r = static_cast<unsigned char>(max(0.0f, min(maxval, color.x + 0.5f)));
				unsigned char g = static_cast<unsigned char>(max(0.0f, min(maxval, color.y + 0.5f)));
				unsigned char b = static_cast<unsigned char>(max(0.0f, min(maxval, color.z + 0.5f)));

				output << r << g << b;
			}
		}

		output.close();
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
		output.close();
	}
}

std::shared_ptr<Image> ReadPPMImage(const std::string &name)
{
	std::ifstream input;
	input.open(name, std::ios::binary);
	std::shared_ptr<ExtensionImage<Vec3f>> image = nullptr;
	try
	{
		if (input.fail())
		{
			std::cout << "open image:" << name << "failed" << std::endl;
			throw("read image failed");
		}

		std::string fileType;
		input >> fileType;
		if (fileType != "P6")
		{
			throw("image type error P6");
		}

		size_t width, height;
		float maxval;
		input >> width >> height;
		input >> maxval;

		ImageDesc color_image_desc;
		color_image_desc.m_format = IMAGE_FORMAT::R32G32B32_FLOAT;
		color_image_desc.m_height = height;
		color_image_desc.m_width = width;
		
		image = std::make_shared<ExtensionImage<Vec3f>>(color_image_desc);

		input.ignore(256, '\n');

		unsigned char currpixel[3];
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				input.read(reinterpret_cast<char *>(currpixel), 3);

				Vec3f color(currpixel[0] / maxval,
					currpixel[1] / maxval,
					currpixel[2] / maxval);

				image->SetPixel(color, j, i);
			}
		}

		input.close();
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
		input.close();
	}

	return image;
}

#endif // !IMAGEHELER_H
