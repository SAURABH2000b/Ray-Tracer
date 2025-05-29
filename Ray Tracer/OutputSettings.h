#ifndef OUTPUT_SETTINGS_H
#define OUTPUT_SETTINGS_H
class OutputSettings {
public:
	OutputSettings();
	OutputSettings(double aspectRatio, int imageWidth, int samplesPerPixel, int maximumRayBounces);

	double m_getAspectRatio() const;
	int m_getImageWidth() const;
	int m_getImageHeight() const;
	
	int m_getSamplesPerPixel() const;
	int m_getMaximumRayBounces() const;

	void m_updateAspectRatio(double aspectRatio);
	void m_updateImageWidth(int imageWidth);
	void m_updateSamplesPerPixel(int samplesPerPixel);
	void m_updateMaximumRayBounces(int maximumRayBounces);

private:
	double m_aspectRatio;
	int m_imageWidth;
	int m_imageHeight;
	
	int m_samplesPerPixel;
	int m_maximumRayBounces;
};

#endif