#pragma once
namespace CollisionChecks
{
	bool CheckCollisionBetween(const CollisionShape* shape1, const CollisionShape* shape2);
	bool CheckCollisionBetween(const CollisionShape_Cuboid* cuboid1, const CollisionShape_Cuboid* cuboid2);
	bool CheckCollisionBetween(const CollisionShape_Sphere* sphere1, const CollisionShape_Sphere* sphere2);
	bool CheckCollisionBetween(const CollisionShape_Cuboid* cuboid, const CollisionShape_Sphere* sphere);
	bool CheckCollisionBetween(const CollisionShape_Sphere* sphere, const CollisionShape_Cuboid* cuboid);
}