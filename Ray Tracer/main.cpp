#include "Utility.h"

#include "OutputSettings.h"
#include "Camera.h"

#include "HittableList.h"
#include "Sphere.h"

int main() {

	//Output Settings:
	OutputSettings setting1(16.0 / 9.0, 400, 500, 50);

	//Camera:
	Point3 lookAt(0.0, 0.0, 0.0);
	Point3 cameraCenter(13.0, 2.0, 3.0);
	Camera camera1(10, 0.5, 20.0, cameraCenter, (lookAt - cameraCenter), setting1);

	//World
	HittableList world;

	auto groundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5), 0.98);
	world.m_add(make_shared<Sphere>(Point3(0, -1000, 0), 1000.0, groundMaterial));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto chooseMat = g_randomDouble();
			Point3 center(a + 0.5 * g_randomDouble(), 0.2, b + 0.5 * g_randomDouble());

			if ((center - Point3(4, 0.2, 0)).m_length() > 0.9) {
				shared_ptr<Material> sphereMaterial;
				if (chooseMat < 0.8) {
					//Diffuse
					auto albedo = Color::s_random() * Color::s_random();
					sphereMaterial = make_shared<Lambertian>(albedo, 0.98);
				}
				else if (chooseMat < 0.95) {
					//Metal
					auto albedo = Color::s_random();
					auto fuzz = g_randomDouble(0, 0.5);
					sphereMaterial = make_shared<Metal>(albedo, fuzz);
				}
				else {
					//Glass
					auto albedo = Color::s_random();
					auto fuzz = g_randomDouble(0, 0.5);
					sphereMaterial = make_shared<Dielectric>(1.5, albedo, fuzz, 1.0);
				}
				world.m_add(make_shared<Sphere>(center, 0.2, sphereMaterial));
			}
		}
	}

	auto material1 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1), 0.999);
	world.m_add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.m_add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<Dielectric>(1.5, Color(0.8, 0.1, 0.1), 0.3, 1.0);
	world.m_add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

	camera1.m_render(world);
	return 0;
}