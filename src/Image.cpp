#include "Image.h"
using namespace std;
using namespace glm;

Image::Image(int width, int height):
	width(width), height(height), pixels(width * height)
{}

Image::Image(int width, int height, const vec3& fill):
	width(width), height(height), pixels(width * height, fill)
{}

vec3& Image::operator()(int x, int y)
{
	return pixels[x + y * width];
}

const vec3& Image::operator()(int x, int y) const
{
	return pixels[x + y * width];
}

int Image::getWidth() const
{
	return width;
}

int Image::getHeight() const
{
	return height;
}

void Image::savePpm(const string& path) const
{
	ofstream fs(path);
	fs << "P3\n" << width << ' ' << height << "\n255\n";
	for (auto& p : pixels)
	{
		fs << int(255.f * sqrt(p.r)) << ' '
			<< int(255.f * sqrt(p.g)) << ' '
			<< int(255.f * sqrt(p.b)) << '\n';
	}
	fs.close();
}

void Image::saveBmp(const string& path) const
{
	constexpr int bytesPerPixel = 3;
	constexpr int fileHeaderSize = 14;
	constexpr int infoHeaderSize = 40;

	const int widthInBytes = width * bytesPerPixel;
	const int paddingSize = (4 - widthInBytes % 4) % 4;
	const int stride = widthInBytes + paddingSize;
	const int dataSize = stride * height;
	const int fileSize = fileHeaderSize + infoHeaderSize + dataSize;

	// Note: color bytes are ordered BGR in bmp format

	const char fileHeader[] = {
		'B', 'M',																				 /// signature
		(char)(fileSize), (char)(fileSize >> 8), (char)(fileSize >> 16), (char)(fileSize >> 24), /// image file size in bytes
		0, 0, 0, 0,																				 /// reserved
		(char)(fileHeaderSize + infoHeaderSize), 0, 0, 0,										 /// start of pixel array
	};

	const char infoHeader[] = {
		infoHeaderSize, 0, 0, 0,														 /// header size
		(char)(width), (char)(width >> 8), (char)(width >> 16), (char)(width >> 24),	 /// image width
		(char)(height), (char)(height >> 8), (char)(height >> 16), (char)(height >> 24), /// image height
		1, 0,																			 /// number of color planes
		bytesPerPixel * 8, 0,															 /// bits per pixel
		0, 0, 0, 0,																		 /// compression
		0, 0, 0, 0,																		 /// image size
		0, 0, 0, 0,																		 /// horizontal resolution
		0, 0, 0, 0,																		 /// vertical resolution
		0, 0, 0, 0,																		 /// colors in color table
		0, 0, 0, 0,																		 /// important color count
	};

	char* data = new char[dataSize]{0};
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			const int i = y * stride + x * bytesPerPixel;
			const vec3 color = sqrt(glm::clamp((*this)(x, (height - y - 1)), vec3(0), vec3(1))) * 255.f;
			data[i] = (unsigned char)(color.b);
			data[i + 1] = (unsigned char)(color.g);
			data[i + 2] = (unsigned char)(color.r);
		}
		for (int i = 0; i < paddingSize; i++)
		{
			data[(y + 1) * stride - paddingSize + i] = 0;
		}
	}

	ofstream fs(path, ios::binary | ios::out);
	fs.write(fileHeader, fileHeaderSize);
	fs.write(infoHeader, infoHeaderSize);
	fs.write(data, dataSize);
	fs.close();
}
