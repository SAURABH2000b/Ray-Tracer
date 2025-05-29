#ifndef CAMERA_H
#define CAMERA_H

#include <windows.h>
#include "OutputSettings.h"
#include "Hittable.h"
#include "Vec3.h"
#include "Material.h"

class Camera
{
public:
	Camera();
	Camera(double focalLength, double defocusAngle, double vfov, const Point3& cameraCenter, const Vec3& cameraDirection,
		const OutputSettings& outputSettings);

	double m_getFocalDistance() const;
	const Point3& m_getCameraCenter() const;
	const Vec3& m_getCameraDirection() const;
	const double m_getDefocusAngle() const;
	const OutputSettings& m_getOutputSettings() const;

	const Vec3& m_getViewportU() const;
	const Vec3& m_getViewportV() const;

	const Vec3& m_getPixelDeltaU() const;
	const Vec3& m_getPixelDeltaV() const;

	const Point3& m_getViewportUpperLeftCornerLocation() const;
	const Point3& m_getPixel00Location() const;

	double m_getVerticalFieldOfView() const;
	double m_getViewportHeight() const;
	double m_getViewportWidth() const;

	void m_updateCameraFocalDistance(double focalDistance);
	void m_updateVerticalFieldOfView(double vfov);
	void m_updateCameraCenter(const Point3& cameraCenter);
	void m_updateCameraDirection(const Vec3& cameraDirection);
	void m_updateDefocusAngle(double defocusAngle);
	void m_updateOutputSettings(const OutputSettings& outputSettings);

	void m_render(const Hittable& world);

private:
	void m_initialize();
	Ray m_getRay(int i, int j) const;

	//The object can be Hittable object eg. sphere, cube, etc. or HittableList object,
	//as HittableList is child class of Hittable.
	Color m_rayColor(const Ray& r, int bouncesLeft, const Hittable& object) const;

	Vec3 m_sampleSquare() const;
	Vec3 m_defocusDiskSample() const;

	bool m_isInitialized = false;
	double m_focusDistance; //Distance from camera center to the plane of perfect focus.
							//In our case, this is same as the focal length,
							//i.e. length from camera where viewwport is placed.
	double m_verticalFieldOfView;
	Point3 m_cameraCenter;
	Vec3 m_cameraDirection;
	double m_defocusAngle; //Variation angle of rays through each pixel.
	OutputSettings m_outputSettings;
	double m_pixelSampleScale;

	Vec3 m_upDirection = Vec3(0, 1, 0);
	Vec3 m_u, m_v, m_w; //Camera frame basis vectors.

	double m_viewportHeight;
	double m_viewportWidth;

	Vec3 m_viewportU;
	Vec3 m_viewportV;

	Vec3 m_pixelDeltaU;
	Vec3 m_pixelDeltaV;

	Point3 m_viewportUpperLeftCornerLocation;
	Point3 m_pixel00Location;

	Vec3 m_defocusDiskU; //Defocus disk horizontal radius.
	Vec3 m_defocusDiskV; //Defocus disk vertical radius.
};

#endif