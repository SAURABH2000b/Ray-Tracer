//Disable strict warnings for this header from Microsoft Visual C++ compiler.
#ifdef _MSC_VER
#pragma warning (push, 0)
#define _CRT_SECURE_NO_WARNINGS
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include "NativeImage.h"

NativeImage::NativeImage(const char* imageFileName)
{
	//Loads image data from the specified file. If the RAY_TRACER_IMAGES environment variable is defined,
	//looks only in that directory for the image file. If the image was not found, searches for the 
	//specified image file first in the current directory, then in the images/ subdirectory, then the parent's
	//image/ subdirectory, and then parent's parent's image/ subdirectory, so on, for six levels up. If the
	//image was not loaded successfully, m_width() and m_height() will return 0.

	auto l_filename = std::string(imageFileName);
	auto l_imageDirectory = getenv("RAY_TRACER_IMAGES");

	//Hunt for the image file in some likely locations.
	if (l_imageDirectory && m_load(std::string(l_imageDirectory) + "/" + l_filename)) return;
	if (m_load(l_filename)) return;
	if (m_load("images/" + l_filename)) return;
	if (m_load("../images/" + l_filename)) return;
	if (m_load("../../images/" + l_filename)) return;
	if (m_load("../../../images/" + l_filename)) return;
	if (m_load("../../../../images/" + l_filename)) return;
	if (m_load("../../../../../images/" + l_filename)) return;
	if (m_load("../../../../../../images/" + l_filename)) return;

	std::cerr << "ERROR: Could not load image file '" << imageFileName << "'.\n";
}

NativeImage::~NativeImage()
{
	delete[] m_bdata;
	STBI_FREE(m_fdata);
}

bool NativeImage::m_load(const std::string& filename)
{
	//Loads the linear (gamma = 1) image data from the given file name. Returns true if the
	//load succeeded. The resulting data buffer contains the three [0.0, 1.0]
	//floating-point values for the first pixel (red, then green, then blue). Pixels are
	//contiguous, going left to right for the width of the image, followed by the next row
	//below, for the full height of the image.

	auto n = m_bytesPerPixel; //to get the number of original components per pixel.
	m_fdata = stbi_loadf(filename.c_str(), &m_imageWidth, &m_imageHeight, &n, m_bytesPerPixel);
	if (m_fdata == nullptr) return false;

	m_bytesPerScanline = m_imageWidth * m_bytesPerPixel;
	m_convertToBytes();
	return true;
}

int NativeImage::m_getWidth() const
{
	return (m_fdata == nullptr) ? 0 : m_imageWidth;
}

int NativeImage::m_getHeight() const
{
	return (m_fdata == nullptr) ? 0 : m_imageHeight;
}

const unsigned char* NativeImage::m_getPixelData(int x, int y) const
{
	//returns the address of the three RGB bytes of the pixel at x, y. If there is no image data
	//, it returns magenta.
	static unsigned char magenta[] = { 255, 0, 255 }; //Static variable, so that it persists till the life time of the program.
	if (m_bdata == nullptr) return magenta;

	x = m_clamp(x, 0, m_imageWidth);
	y = m_clamp(y, 0, m_imageHeight);

	return m_bdata + y * m_bytesPerScanline + x * m_bytesPerPixel;
}

int NativeImage::m_clamp(int x, int low, int high)
{
	//Return the value clamped to the range [low, high).
	if (x < low)return low;
	if (x < high)return x;
	return high - 1;
}

unsigned char NativeImage::m_floatToByte(float value)
{
	if (value <= 0.0)
		return 0;
	if (1.0 <= value)
		return 255;
	return static_cast<unsigned char>(256.0 * value);
}

void NativeImage::m_convertToBytes()
{
	//Convert the linear floating point pixel data to bytes, storing the resulting byte
	//data in the 'm_bdata' member.

	int totalBytes = m_imageWidth * m_imageHeight * m_bytesPerPixel;
	m_bdata = new unsigned char[totalBytes];

	//Iterate through all pixel components, converting from [0.0, 1.0] float values to
	//unsigned [0, 255] byte values;

	auto* bptr = m_bdata;
	auto* fptr = m_fdata;
	for (auto i = 0; i < totalBytes; i++, fptr++, bptr++)
		*bptr = m_floatToByte(*fptr);
}

//Restore MSVC compiler warnings
#ifdef _MSC_VER
#pragma warning (pop)
#endif