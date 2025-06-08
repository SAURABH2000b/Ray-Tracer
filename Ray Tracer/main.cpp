#include "Utility.h"

#include "BoundingVolumeHierarchyNode.h"

#include "OutputSettings.h"
#include "Camera.h"

#include "HittableList.h"
#include "Sphere.h"
#include <string>

void bouncingSpheres() {

	//Output Settings:
	OutputSettings setting1(16.0 / 9.0, 400, 100, 50);

	//Camera:
	Point3 lookAt(0.0, 0.0, 0.0);
	Point3 cameraCenter(13.0, 2.0, 3.0);
	Camera camera1(10, 0.5, 20.0, cameraCenter, (lookAt - cameraCenter), setting1);

	//World
	HittableList world;

	shared_ptr<Texture> checkeredTexture = make_shared<CheckerTexture>(0.32, 0.32, 0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	auto groundMaterialChecker = make_shared<Lambertian>(checkeredTexture, 0.98);
	world.m_add(make_shared<Sphere>(Point3(0, -1000, 0), 1000.0, groundMaterialChecker));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto chooseMat = g_randomDouble();
			Point3 center(a + 0.5 * g_randomDouble(), 0.2, b + 0.5 * g_randomDouble());

			if ((center - Point3(4, 0.2, 0)).m_length() > 0.9) {
				shared_ptr<Material> sphereMaterial;
				if (chooseMat < 0.4) {
					//Diffuse
					auto albedo = Color::s_random() * Color::s_random();
					sphereMaterial = make_shared<Lambertian>(albedo, 0.98);
				}
				else if (chooseMat < 0.65) {
					//Metal
					auto albedo = Color::s_random();
					auto fuzz = g_randomDouble(0, 0.3);
					sphereMaterial = make_shared<Metal>(albedo, fuzz);
				}
				else {
					//Glass
					auto albedo = Color::s_random();
					auto fuzz = g_randomDouble(0, 0.5);
					sphereMaterial = make_shared<Dielectric>(1.5, albedo, fuzz, 1.0);
				}
				auto center2 = center + Vec3(0, g_randomDouble(0, 0.5), 0);
				world.m_add(make_shared<Sphere>(center, center2, 0.2, sphereMaterial));
			}
		}
	}

	auto material1 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1), 0.999);
	world.m_add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.m_add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<Dielectric>(1.5, Color(0.8, 0.1, 0.1), 0.3, 1.0);
	world.m_add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

	world = HittableList(make_shared<BoundingVolumeHierarchyNode>(world)); //Wrapping all the hittables into bounded volume hierarchy.

	camera1.m_render(world);
	
}

void checkeredSphere() {
	//Output Settings:
	OutputSettings setting1(16.0 / 9.0, 400, 100, 50);

	//Camera:
	Point3 lookAt(0.0, 0.0, 0.0);
	Point3 cameraCenter(13.0, 2.0, 3.0);
	Camera camera1(10, 0, 20.0, cameraCenter, (lookAt - cameraCenter), setting1);

	//World
	HittableList world;

	shared_ptr<Texture> checkeredTexture = make_shared<CheckerTexture>(0.32, 0.32, 0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	auto checkeredMaterial = make_shared<Metal>(checkeredTexture, 0.1);
	world.m_add(make_shared<Sphere>(Point3(0, -10, 0), 10, checkeredMaterial));
	world.m_add(make_shared<Sphere>(Point3(0, 10, 0), 10, checkeredMaterial));

	camera1.m_render(world);

}

void solarSystem() {
	//Output Settings:
	OutputSettings setting1(16.0 / 9.0, 400, 100, 70);

	//Camera:
	Point3 lookAt(0.0, 1.0, -1.0);
	Point3 cameraCenter(0.0, 1.0, 9.0);
	Camera camera1(10, 0.5, 20.0, cameraCenter, (lookAt - cameraCenter), setting1);

	//World
	HittableList world;

	auto earthTexture = make_shared<ImageTexture>("earth_color_map.png");
	auto marsTexture = make_shared<ImageTexture>("mars_color_map.jpg");
	auto venusTexture = make_shared<ImageTexture>("venus_color_map.jpg");


	shared_ptr<Texture> checkeredTexture = make_shared<CheckerTexture>(0.32, 0.32, 0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	auto groundMaterialChecker = make_shared<Lambertian>(checkeredTexture, 0.98);
	world.m_add(make_shared<Sphere>(Point3(0, -1000, 0), 1000.0, groundMaterialChecker));

	auto earthMaterial = make_shared<Dielectric>(1.5, earthTexture, 0.3, 0.5);
	world.m_add(make_shared<Sphere>(Point3(-0.3, 1, 0), 1.0, earthMaterial));
	//world.m_add(make_shared<Sphere>(Point3(-0.3, 1, 0), 0.8, make_shared<Dielectric>(1.0, Color(1.0, 1.0, 1.0), 0, 1.0)));

	auto marsMaterial = make_shared<Dielectric>(1.5, marsTexture, 0.3, 0.5);
	world.m_add(make_shared<Sphere>(Point3(-2.0, 0.5, 0), 0.5, marsMaterial));
	//world.m_add(make_shared<Sphere>(Point3(-0.3, 0.5, 0), 0.35, make_shared<Dielectric>(1.0, Color(1.0, 1.0, 1.0), 0, 1.0)));

	auto venusMaterial = make_shared<Dielectric>(1.5, venusTexture, 0.3, 0.5);
	world.m_add(make_shared<Sphere>(Point3(1.7, 0.95, 0), 0.95, venusMaterial));
	//world.m_add(make_shared<Sphere>(Point3(-0.3, 00.95, 0), 0.78, make_shared<Dielectric>(1.0, Color(1.0, 1.0, 1.0), 0, 1.0)));

	world = HittableList(make_shared<BoundingVolumeHierarchyNode>(world)); //Wrapping all the hittables into bounded volume hierarchy.

	camera1.m_render(world);
}

int main(int argc, char* argv[]) {
	std::string arg1 = argv[1];
	if (arg1 == "scene" && argc == 3) {
		std::string argValue = argv[2];
		if (argValue == "Bouncy-balls") {
			std::clog << "Selected scene: " << argValue << "\n";
			bouncingSpheres();
		}
		else if (argValue == "Checkered-balls") {
			std::clog << "Selected scene: " << argValue << "\n";
			checkeredSphere();
		}
		else if (argValue == "Solar-system") {
			std::clog << "Selected scene: " << argValue << "\n";
			solarSystem();
		}
	}

	//Sample run command: "Ray Tracer.exe" scene Checkered-balls >image30.ppm
	//"Ray Tracer.exe" scene Solar-system > image31.ppm
	return 0;
}