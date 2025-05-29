#include "OutputSettings.h"

OutputSettings::OutputSettings()
{
	m_aspectRatio = 16.0 / 9.0;
	m_imageWidth = 400;
	
	m_imageHeight = int(m_imageWidth / m_aspectRatio);
	m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

	m_samplesPerPixel = 1;
	m_maximumRayBounces = 10;
}

OutputSettings::OutputSettings(double aspectRatio, int imageWidth, int samplesPerPixel, int maximumRayBounces)
{
	m_aspectRatio = aspectRatio;
	m_imageWidth = imageWidth;

	m_imageHeight = int(m_imageWidth / m_aspectRatio);
	m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

	m_samplesPerPixel = samplesPerPixel;
	m_maximumRayBounces = maximumRayBounces;
}

double OutputSettings::m_getAspectRatio() const
{
	return m_aspectRatio;
}

int OutputSettings::m_getImageWidth() const
{
	return m_imageWidth;
}

int OutputSettings::m_getImageHeight() const
{
	return m_imageHeight;
}

int OutputSettings::m_getSamplesPerPixel() const
{
	return m_samplesPerPixel;
}

int OutputSettings::m_getMaximumRayBounces() const
{
	return m_maximumRayBounces;
}

void OutputSettings::m_updateAspectRatio(double aspectRatio)
{
	m_aspectRatio = aspectRatio;

	m_imageHeight = int(m_imageWidth / m_aspectRatio);
	m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;
}

void OutputSettings::m_updateImageWidth(int imageWidth)
{
	m_imageWidth = imageWidth;

	m_imageHeight = int(m_imageWidth / m_aspectRatio);
	m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;
}

void OutputSettings::m_updateSamplesPerPixel(int samplesPerPixel)
{
	m_samplesPerPixel = samplesPerPixel;
}

void OutputSettings::m_updateMaximumRayBounces(int maximumRayBounces)
{
	m_maximumRayBounces = maximumRayBounces;
}
