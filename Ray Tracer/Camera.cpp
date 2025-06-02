#include "Utility.h"
#include "Camera.h"
#include "LoadingBar.h"

Camera::Camera()
{
    m_focusDistance = 1.0;
    m_defocusAngle = 0.0;
    m_verticalFieldOfView = 90.0;
    m_cameraCenter = Point3(0.0, 0.0, 0.0);
    m_cameraDirection = Vec3(0.0, 0.0, -1.0);
    m_outputSettings = OutputSettings();
    m_pixelSampleScale = 1.0 / m_outputSettings.m_getSamplesPerPixel();

    auto theta = g_degreesToRadians(m_verticalFieldOfView);
    auto h = std::tan(theta/2);
    m_viewportHeight = 2 * h * m_focusDistance;
    m_viewportWidth = m_viewportHeight * (double(m_outputSettings.m_getImageWidth()) / m_outputSettings.m_getImageHeight());

    //Calculate the m_u, m_v, m_w unit basis vectors for the camera coordinate frame.
    m_w = g_unitVector(-m_cameraDirection);
    m_u = g_unitVector(g_cross(m_upDirection, m_w));
    m_v = g_cross(m_w, m_u);

    //Calculate the vectors across the horizontal and down the vertical viewport edges.
    m_viewportU = m_viewportWidth * m_u; //Vector across viewport horizontal edge.
    m_viewportV = m_viewportHeight * (- m_v); // Vector down viewport vertical edge.

    //Calculate the horizontal and vertical delta vectors from pixel to pixel.
    m_pixelDeltaU = m_viewportU / m_outputSettings.m_getImageWidth();
    m_pixelDeltaV = m_viewportV / m_outputSettings.m_getImageHeight();

    //Calculate the location of the uppper left corner of the viewport.
    m_viewportUpperLeftCornerLocation = m_cameraCenter - (m_focusDistance * m_w)
        - m_viewportU / 2 - m_viewportV / 2;

    //Calculate the location of the uppper left pixel.
    m_pixel00Location = m_viewportUpperLeftCornerLocation + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    //Calculate the camera defocus disk basis vectors.
    auto defocusRadius = m_focusDistance * std::tan(g_degreesToRadians(m_defocusAngle / 2));
    m_defocusDiskU = m_u * defocusRadius;
    m_defocusDiskV = m_v * defocusRadius;

    m_isInitialized = true;
}

Camera::Camera(double focalLength, double defocusAngle, double vfov, const Point3& cameraCenter, const Vec3& cameraDirection,
    const OutputSettings& outputSettings)
{
    m_focusDistance = focalLength;
    m_defocusAngle = defocusAngle;
    m_verticalFieldOfView = vfov;
    m_cameraCenter = cameraCenter;
    m_cameraDirection = g_unitVector(cameraDirection);
    m_outputSettings = outputSettings;
    m_pixelSampleScale = 1.0 / m_outputSettings.m_getSamplesPerPixel();

    auto theta = g_degreesToRadians(m_verticalFieldOfView);
    auto h = std::tan(theta / 2);
    m_viewportHeight = 2 * h * m_focusDistance;
    m_viewportWidth = m_viewportHeight * (double(m_outputSettings.m_getImageWidth()) / m_outputSettings.m_getImageHeight());

    //Calculate the m_u, m_v, m_w unit basis vectors for the camera coordinate frame.
    m_w = g_unitVector(-m_cameraDirection);
    m_u = g_unitVector(g_cross(m_upDirection, m_w));
    m_v = g_cross(m_w, m_u);

    //Calculate the vectors across the horizontal and down the vertical viewport edges.
    m_viewportU = m_viewportWidth * m_u; //Vector across viewport horizontal edge.
    m_viewportV = m_viewportHeight * (-m_v); // Vector down viewport vertical edge.

    //Calculate the horizontal and vertical delta vectors from pixel to pixel.
    m_pixelDeltaU = m_viewportU / m_outputSettings.m_getImageWidth();
    m_pixelDeltaV = m_viewportV / m_outputSettings.m_getImageHeight();

    //Calculate the location of the uppper left corner of the viewport.
    m_viewportUpperLeftCornerLocation = m_cameraCenter - (m_focusDistance * m_w)
        - m_viewportU / 2 - m_viewportV / 2;

    //Calculate the location of the uppper left pixel.
    m_pixel00Location = m_viewportUpperLeftCornerLocation + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    //Calculate the camera defocus disk basis vectors.
    auto defocusRadius = m_focusDistance * std::tan(g_degreesToRadians(m_defocusAngle / 2));
    m_defocusDiskU = m_u * defocusRadius;
    m_defocusDiskV = m_v * defocusRadius;

    m_isInitialized = true;
}

double Camera::m_getFocalDistance() const
{
    return m_focusDistance;
}

const Point3& Camera::m_getCameraCenter() const
{
    return m_cameraCenter;
}

const Vec3& Camera::m_getCameraDirection() const
{
    return m_cameraDirection;
}

const double Camera::m_getDefocusAngle() const
{
    return m_defocusAngle;
}

const OutputSettings& Camera::m_getOutputSettings() const
{
    return m_outputSettings;
}

const Vec3& Camera::m_getViewportU() const
{
    return m_viewportU;
}

const Vec3& Camera::m_getViewportV() const
{
    return m_viewportV;
}

const Vec3& Camera::m_getPixelDeltaU() const
{
    return m_pixelDeltaU;
}

const Vec3& Camera::m_getPixelDeltaV() const
{
    return m_pixelDeltaV;
}

const Point3& Camera::m_getViewportUpperLeftCornerLocation() const
{
    return m_viewportUpperLeftCornerLocation;
}

const Point3& Camera::m_getPixel00Location() const
{
    return m_pixel00Location;
}

double Camera::m_getVerticalFieldOfView() const
{
    return m_verticalFieldOfView;
}

double Camera::m_getViewportHeight() const
{
    return m_viewportHeight;
}

double Camera::m_getViewportWidth() const
{
    return m_viewportWidth;
}

void Camera::m_updateCameraFocalDistance(double focalDistance)
{
    m_focusDistance = focalDistance;

    auto theta = g_degreesToRadians(m_verticalFieldOfView);
    auto h = std::tan(theta / 2);
    m_viewportHeight = 2 * h * m_focusDistance;
    m_viewportWidth = m_viewportHeight * (double(m_outputSettings.m_getImageWidth()) / m_outputSettings.m_getImageHeight());

    //Calculate the vectors across the horizontal and down the vertical viewport edges.
    m_viewportU = m_viewportWidth * m_u; //Vector across viewport horizontal edge.
    m_viewportV = m_viewportHeight * (-m_v); // Vector down viewport vertical edge.

    //Calculate the horizontal and vertical delta vectors from pixel to pixel.
    m_pixelDeltaU = m_viewportU / m_outputSettings.m_getImageWidth();
    m_pixelDeltaV = m_viewportV / m_outputSettings.m_getImageHeight();

    //Calculate the location of the uppper left corner of the viewport.
    m_viewportUpperLeftCornerLocation = m_cameraCenter - (m_focusDistance * m_w)
        - m_viewportU / 2 - m_viewportV / 2;

    //Calculate the location of the uppper left pixel.
    m_pixel00Location = m_viewportUpperLeftCornerLocation + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    //Calculate the camera defocus disk basis vectors.
    auto defocusRadius = m_focusDistance * std::tan(g_degreesToRadians(m_defocusAngle / 2));
    m_defocusDiskU = m_u * defocusRadius;
    m_defocusDiskV = m_v * defocusRadius;
}

void Camera::m_updateVerticalFieldOfView(double vfov)
{
    m_verticalFieldOfView = vfov;

    auto theta = g_degreesToRadians(m_verticalFieldOfView);
    auto h = std::tan(theta / 2);
    m_viewportHeight = 2 * h * m_focusDistance;
    m_viewportWidth = m_viewportHeight * (double(m_outputSettings.m_getImageWidth()) / m_outputSettings.m_getImageHeight());

    //Calculate the vectors across the horizontal and down the vertical viewport edges.
    m_viewportU = m_viewportWidth * m_u; //Vector across viewport horizontal edge.
    m_viewportV = m_viewportHeight * (-m_v); // Vector down viewport vertical edge.

    //Calculate the horizontal and vertical delta vectors from pixel to pixel.
    m_pixelDeltaU = m_viewportU / m_outputSettings.m_getImageWidth();
    m_pixelDeltaV = m_viewportV / m_outputSettings.m_getImageHeight();

    //Calculate the location of the uppper left corner of the viewport.
    m_viewportUpperLeftCornerLocation = m_cameraCenter - (m_focusDistance * m_w)
        - m_viewportU / 2 - m_viewportV / 2;

    //Calculate the location of the uppper left pixel.
    m_pixel00Location = m_viewportUpperLeftCornerLocation + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);
}

void Camera::m_updateCameraCenter(const Point3& cameraCenter)
{
    m_cameraCenter = cameraCenter;

    //Calculate the location of the uppper left corner of the viewport.
    m_viewportUpperLeftCornerLocation = m_cameraCenter - (m_focusDistance * m_w)
        - m_viewportU / 2 - m_viewportV / 2;

    //Calculate the location of the uppper left pixel.
    m_pixel00Location = m_viewportUpperLeftCornerLocation + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);
}

void Camera::m_updateCameraDirection(const Vec3& cameraDirection)
{
    m_cameraDirection = cameraDirection;

    //Calculate the m_u, m_v, m_w unit basis vectors for the camera coordinate frame.
    m_w = g_unitVector(-m_cameraDirection);
    m_u = g_unitVector(g_cross(m_upDirection, m_w));
    m_v = g_cross(m_w, m_u);

    //Calculate the vectors across the horizontal and down the vertical viewport edges.
    m_viewportU = m_viewportWidth * m_u; //Vector across viewport horizontal edge.
    m_viewportV = m_viewportHeight * (-m_v); // Vector down viewport vertical edge.

    //Calculate the horizontal and vertical delta vectors from pixel to pixel.
    m_pixelDeltaU = m_viewportU / m_outputSettings.m_getImageWidth();
    m_pixelDeltaV = m_viewportV / m_outputSettings.m_getImageHeight();

    //Calculate the location of the uppper left corner of the viewport.
    m_viewportUpperLeftCornerLocation = m_cameraCenter - (m_focusDistance * m_w)
        - m_viewportU / 2 - m_viewportV / 2;

    //Calculate the location of the uppper left pixel.
    m_pixel00Location = m_viewportUpperLeftCornerLocation + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    //Calculate the camera defocus disk basis vectors.
    auto defocusRadius = m_focusDistance * std::tan(g_degreesToRadians(m_defocusAngle / 2));
    m_defocusDiskU = m_u * defocusRadius;
    m_defocusDiskV = m_v * defocusRadius;
}

void Camera::m_updateDefocusAngle(double defocusAngle)
{
    //Calculate the camera defocus disk basis vectors.
    auto defocusRadius = m_focusDistance * std::tan(g_degreesToRadians(m_defocusAngle / 2));
    m_defocusDiskU = m_u * defocusRadius;
    m_defocusDiskV = m_v * defocusRadius;
}

void Camera::m_updateOutputSettings(const OutputSettings& outputSettings)
{
    m_outputSettings = outputSettings;

    m_pixelSampleScale = 1.0 / m_outputSettings.m_getSamplesPerPixel();

    auto theta = g_degreesToRadians(m_verticalFieldOfView);
    auto h = std::tan(theta / 2);
    m_viewportHeight = 2 * h * m_focusDistance;
    m_viewportWidth = m_viewportHeight * (double(m_outputSettings.m_getImageWidth()) / m_outputSettings.m_getImageHeight());

    //Calculate the vectors across the horizontal and down the vertical viewport edges.
    m_viewportU = m_viewportWidth * m_u; //Vector across viewport horizontal edge.
    m_viewportV = m_viewportHeight * (-m_v); // Vector down viewport vertical edge.

    //Calculate the horizontal and vertical delta vectors from pixel to pixel.
    m_pixelDeltaU = m_viewportU / m_outputSettings.m_getImageWidth();
    m_pixelDeltaV = m_viewportV / m_outputSettings.m_getImageHeight();

    //Calculate the location of the uppper left corner of the viewport.
    m_viewportUpperLeftCornerLocation = m_cameraCenter - (m_focusDistance * m_w)
        - m_viewportU / 2 - m_viewportV / 2;

    //Calculate the location of the uppper left pixel.
    m_pixel00Location = m_viewportUpperLeftCornerLocation + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);
}

void Camera::m_render(const Hittable& world)
{
    if (!m_isInitialized) {
        m_initialize();
    }
    LoadingBar progressBar('0', 'A', 25);
    int counter = 0;
    double l_imageWidth = m_outputSettings.m_getImageWidth();
    double l_imageHeight = m_outputSettings.m_getImageHeight();
    int l_samplesPerPixel = m_outputSettings.m_getSamplesPerPixel();
    int l_maximumBounces = m_outputSettings.m_getMaximumRayBounces();
    
    std::cout << "P3\n" << l_imageWidth << ' ' << l_imageHeight << "\n255\n";

    progressBar.m_initialize("Progress:");

    for (int j = 0; j < l_imageHeight; j++) {

        //Scan lines status:
        //std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        //Progress bar:
        float status = (float(j + 1) / float(l_imageHeight)) * 25.0;
        if (int(status) > counter) {
            counter = int(status);
            progressBar.m_update();
        }

        //Scanline processing
        for (int i = 0; i < l_imageWidth; i++) {
            Color pixelColor(0, 0, 0);
            Ray l_ray;
            for (int sample = 0; sample < l_samplesPerPixel; sample++) {
                l_ray = m_getRay(i, j);
                pixelColor += m_rayColor(l_ray, l_maximumBounces, world);
            }
            g_writeColor(std::cout, m_pixelSampleScale * pixelColor);
        }
    }
    //std::clog << "\rDone.                      \n";
    progressBar.m_finish();

}

void Camera::m_initialize()
{
    m_focusDistance = 1.0;
    m_defocusAngle = 0.0;
    m_verticalFieldOfView = 90.0;
    m_cameraCenter = Point3(0.0, 0.0, 0.0);
    m_cameraDirection = Vec3(0.0, 0.0, -1.0);
    m_outputSettings = OutputSettings();
    m_pixelSampleScale = 1.0 / m_outputSettings.m_getSamplesPerPixel();

    auto theta = g_degreesToRadians(m_verticalFieldOfView);
    auto h = std::tan(theta / 2);
    m_viewportHeight = 2 * h * m_focusDistance;
    m_viewportWidth = m_viewportHeight * (double(m_outputSettings.m_getImageWidth()) / m_outputSettings.m_getImageHeight());

    //Calculate the m_u, m_v, m_w unit basis vectors for the camera coordinate frame.
    m_w = g_unitVector(-m_cameraDirection);
    m_u = g_unitVector(g_cross(m_upDirection, m_w));
    m_v = g_cross(m_w, m_u);

    //Calculate the vectors across the horizontal and down the vertical viewport edges.
    m_viewportU = m_viewportWidth * m_u; //Vector across viewport horizontal edge.
    m_viewportV = m_viewportHeight * (-m_v); // Vector down viewport vertical edge.

    //Calculate the horizontal and vertical delta vectors from pixel to pixel.
    m_pixelDeltaU = m_viewportU / m_outputSettings.m_getImageWidth();
    m_pixelDeltaV = m_viewportV / m_outputSettings.m_getImageHeight();

    //Calculate the location of the uppper left corner of the viewport.
    m_viewportUpperLeftCornerLocation = m_cameraCenter - (m_focusDistance * m_w)
        - m_viewportU / 2 - m_viewportV / 2;

    //Calculate the location of the uppper left pixel.
    m_pixel00Location = m_viewportUpperLeftCornerLocation + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);

    //Calculate the camera defocus disk basis vectors.
    auto defocusRadius = m_focusDistance * std::tan(g_degreesToRadians(m_defocusAngle / 2));
    m_defocusDiskU = m_u * defocusRadius;
    m_defocusDiskV = m_v * defocusRadius;

    m_isInitialized = true;
}

Ray Camera::m_getRay(int i, int j) const
{
    //Construct a camera ray originating from the origin and directed at randomly sampled
    //point around the pixel location i, j.

    auto offset = m_sampleSquare();
    auto pixelSample = m_pixel00Location
        + ((i + offset.m_x()) * m_pixelDeltaU)
        + ((j + offset.m_y()) * m_pixelDeltaV);

    auto rayOrigin = (m_defocusAngle <-0) ? m_cameraCenter : m_defocusDiskSample();
    auto rayDirection = pixelSample - rayOrigin;

    auto rayTime = g_randomDouble();

    return Ray(rayOrigin, rayDirection, rayTime);
}

Color Camera::m_rayColor(const Ray& r, int bouncesLeft, const Hittable& object) const
{
    //If bounce limit is reached, then return black color
    if (bouncesLeft <= 0)
        return Color(0, 0, 0);

    HitRecord record;

    if (object.m_hit(r, Interval(0.001, infinity), record)) { //min of interval is 0.001 not 0 because, if t is very less, it can be due to self intersection with surface. It can cause shadow acne.
        //To get normal visualization:
        //return 0.5 * (record.m_normal + Color(1, 1, 1));

        //Actual path tracing logic:
        //Here we are generating a new ray with origin as the point where existing ray
        //hits the hittable. This new ray is actually the reflected/scattered ray.
        //Then we recurse m_rayColor() for this new reflected ray.
        //The recursion stops if ray don't hit any hittables, in that case
        //pixel is colored using default shading logic (outside recursive if statement),
        //or recursive limit is reached (bouncesLeft).
        //Each recursion cycle is called a ray bounce.
        Ray scatteredRay;
        Color attenuation;
        if (record.m_material->m_scatter(r, record, attenuation, scatteredRay))
            return attenuation * m_rayColor(scatteredRay, bouncesLeft - 1, object);
        return Color(0, 0, 0);
    }

    Vec3 unitDirection = g_unitVector(r.m_getDirection());
    auto a = 0.5 * (unitDirection.m_y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

Vec3 Camera::m_sampleSquare() const
{
    //Returns the vector to a random point in the [-0.5, -0.5] to [+0.5, +0.5] unit square.
    return Vec3(g_randomDouble() - 0.5, g_randomDouble() - 0.5, 0);
}

Vec3 Camera::m_defocusDiskSample() const
{
   //Returns a random point in camera defocus disk.
    auto p = g_randomInUnitDisk();
    return m_cameraCenter + (p[0] * m_defocusDiskU) + (p[1] * m_defocusDiskV);
}
