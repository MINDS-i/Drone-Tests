#ifndef VEC3UNITTESTS_H
#define VEC3UNITTESTS_H
#include "test.h"
#include "math/Vec3.h"
#include <iostream>
namespace vec3Tests{
	bool fuzzyVecCompare(Vec3 a, Vec3 b){
		return	fuzzyCompare(a[0], b[0]) &&
				fuzzyCompare(a[1], b[1]) &&
				fuzzyCompare(a[2], b[2])    ;
	}
	bool v3testCrossIJ(){
		Vec3 a(1,0,0);
		Vec3 b(0,1,0);
		a.crossWith(b); //a is now aXb
		return fuzzyVecCompare(a, Vec3(0,0,1));
	}
	bool v3testCrossKJ(){
		Vec3 a(0,0,1);
		Vec3 b(0,1,0);
		a.crossWith(b); //a is now aXb
		return fuzzyVecCompare(a, Vec3(-1,0,0));
	}
	bool v3testCrossId(){
		Vec3 a(0,1,0);
		Vec3 b(0,1,0);
		a.crossWith(b); //a is now aXb
		return fuzzyVecCompare(a, Vec3(0,0,0));
	}
	bool v3testCrossSc(){
		Vec3 a(-1,7,4);
		Vec3 b(-5,8,4);
		a.crossWith(b); //a is now aXb
		return fuzzyVecCompare(a, Vec3(-4,-16,27));
	}
	bool v3testLength(){
		Vec3 a(2,3,4);
		return fuzzyCompare(a.length(), 5.38516);
	}
	bool v3testDot(){
		Vec3 a( 1, 1, 1);
		Vec3 b(-2, 3, 4);
		return fuzzyCompare(a.dot(b), 5);
	}
	bool v3testNormalize(){
		Vec3 a(-2, 3, 4);
		a.normalize();
		return fuzzyCompare(a.length(), 1.0f);
	}
	bool v3testLerp(){
		Vec3 a( 1, 1, 1);
		Vec3 b( 0, 0, 0);
		a.lerpWith(b, .75);
		return fuzzyVecCompare(a, Vec3(.25,.25,.25));
	}
	bool v3testRotateBy(){
		Quaternion q(Vec3(-1,-1,0), (3.1415926/2));
		Vec3 a(0,0,1);
		a.rotateBy(q);
		return fuzzyVecCompare(a, Vec3(0.7071, -0.7071, 0));
	}
	bool v3testMultAssn(){
		Vec3 a( 1, 1, 1);
		a *= 3;
		return fuzzyVecCompare(a, Vec3(3,3,3));
	}
	bool v3testDivAssnOp(){
		Vec3 a( 3, 3, 3);
		a /= 3;
		return fuzzyVecCompare(a, Vec3(1,1,1));
	}
	bool v3testAddAssnOp(){
		Vec3 a( 1, 1, 1);
		Vec3 b(-1, 1, 7);
		a += b;
		return fuzzyVecCompare(a, Vec3(0,2,8));
	}
	bool v3testSubAssnOp(){
		Vec3 a( 1, 1, 1);
		Vec3 b( 7,-3,-2);
		a -= b;
		return fuzzyVecCompare(a, Vec3(-6, 4, 3));
	}
	bool v3testMultFloat(){
		Vec3 a( 1, 1, 1);
		Vec3 b = a * 2;
		return fuzzyVecCompare(b, Vec3(2,2,2));
	}
	bool v3testDivFloat(){
		Vec3 a( 3, 6, 9);
		Vec3 b = a / 3;
		return fuzzyVecCompare(b, Vec3(1,2,3));
	}
	bool v3testAddVec(){
		Vec3 a( 1, 1, 1);
		Vec3 b(-1,-2, 2);
		Vec3 c = a + b;
		return fuzzyVecCompare(c, Vec3(0,-1,3));
	}
	bool v3testSubVec(){
		Vec3 a( 1, 1, 1);
		Vec3 b( 1, 2,-3);
		Vec3 c = a - b;
		return fuzzyVecCompare(c, Vec3(0,-1,4));
	}
	bool v3testErrorPositive(){
		Vec3 a(1,1,1.f/0.f);
		return a.error();
	}
	bool v3testErrorNegative(){
		Vec3 a(1,1,1.f/1.f);
		return !a.error();
	}
	void runTests(){
		TEST(v3testCrossIJ		);
		TEST(v3testCrossKJ		);
		TEST(v3testCrossId		);
		TEST(v3testCrossSc		);
		TEST(v3testLength		);
		TEST(v3testDot			);
		TEST(v3testNormalize	);
		TEST(v3testLerp			);
		TEST(v3testRotateBy		);
		TEST(v3testMultAssn		);
		TEST(v3testDivAssnOp	);
		TEST(v3testAddAssnOp	);
		TEST(v3testSubAssnOp	);
		TEST(v3testMultFloat	);
		TEST(v3testDivFloat		);
		TEST(v3testAddVec		);
		TEST(v3testSubVec		);
		TEST(v3testErrorPositive);
		TEST(v3testErrorNegative);
	}
}
#endif
