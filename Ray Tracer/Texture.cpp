#include "Texture.h"

Color SolidColor::m_value(double u, double v, const Point3& p) const
{
    return m_albedo;
}

Color CheckerTexture::m_value(double u, double v, const Point3& p) const
{
    auto xInteger = int(std::floor(m_xInverseScale * p.m_x()));
    auto yInteger = int(std::floor(m_yInverseScale * p.m_y()));
    auto zInteger = int(std::floor(m_zInverseScale * p.m_z()));

    bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
    return isEven ? m_evenTexture->m_value(u, v, p) : m_oddTexture->m_value(u, v, p);
}

Color ImageTexture::m_value(double u, double v, const Point3& p) const
{
    //If we have no texture data, then return solid cyan as a debugging aid.
    if (m_image.m_getHeight() <= 0)return Color(0.0, 1.0, 1.0);

    //Clamp input texture coordinates to [0,1] x [0,1].
    u = Interval(0, 1).m_clamp(u);
    v = 1.0 - Interval(0, 1).m_clamp(v); //Flip v to image coordinates.

    auto i = int(u * m_image.m_getWidth());
    auto j = int(v * m_image.m_getHeight());
    auto pixel = m_image.m_getPixelData(i, j);

    auto colorScale = 1.0 / 255.0;
    return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
}
