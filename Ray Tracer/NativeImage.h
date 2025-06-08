#ifndef NATIVE_IMAGE_H
#define NATIVE_IMAGE_H

#include <cstdlib>
#include <iostream>

class NativeImage
{
public:
	NativeImage() {}
	NativeImage(const char* imageFileName);

	~NativeImage();

	bool m_load(const std::string& filename);

	int m_getWidth() const;
	int m_getHeight() const;

	const unsigned char* m_getPixelData(int x, int y) const;
private:
	const int m_bytesPerPixel = 3;
	float* m_fdata = nullptr; //Linear floating point pixel data.
	unsigned char* m_bdata = nullptr; //Linear 8-bit pixel data.
	int m_imageWidth = 0; //Loaded image width.
	int m_imageHeight = 0; //Loaded image height.
	int m_bytesPerScanline = 0;

	static int m_clamp(int x, int low, int high);
	static unsigned char m_floatToByte(float value);

	void m_convertToBytes();
};

#endif