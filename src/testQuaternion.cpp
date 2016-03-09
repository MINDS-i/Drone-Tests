#include "testHooks.h"
#include "math/Quaternion.h"
//TESTING "math/Quaternion.cpp"
//TESTING "math/Vec3.cpp"

bool fuzzyQCompare(Quaternion a, Quaternion b, float diff = 0.0000005){
	return	fuzzyCompare(a[0], b[0], diff) &&
			fuzzyCompare(a[1], b[1], diff) &&
			fuzzyCompare(a[2], b[2], diff) &&
			fuzzyCompare(a[3], b[3], diff)   ;
}
#define toRad(x) ((x*3.1415926f)/180.f)
#define toDeg(x) ((x*180.f)/3.1415926f)
bool QtestEulerConstr(){
	Quaternion a(Vec3(toRad(30.f),toRad(30.f),toRad(30.f)));
	return fuzzyQCompare(a, Quaternion(	0.883883,
										0.306186,
										0.306186,
										0.176777 ), 0.00001);
}
bool QtestAxisAngleConstr(){
	Quaternion a(Vec3(4,0,0), toRad(90));
	return fuzzyQCompare(a, Quaternion( 0.7071067,
										0.7071067,
										0,
										0	), 0.00001);
}
bool QtestInverse(){
	Quaternion a(1, 0, 1, 2);
	return fuzzyQCompare(a.inverse(), Quaternion(1,0,-1,-2));
}
bool QtestDot(){
	Quaternion a( 1, 0, 1, 2);
	Quaternion b(-1, 0, 2, 0);
	return fuzzyCompare(a.dot(b), 1);
}
bool QtestLength(){
	Quaternion a;
	Quaternion b(1,2,3,4);
	return	fuzzyCompare(a.length(), 1) &&
			fuzzyCompare(b.length(), 5.477225);
}
bool QtestDistance(){
	Quaternion a(Vec3(1,1,0), toRad(90));
	Quaternion b(Vec3(1,1,0), toRad(45));
	return fuzzyCompare(a.distance(b), toRad(45));
}
bool QtestAxis(){
	Quaternion a(1, 0, 1, 2);
	Vec3 ax = a.axis();
	return	fuzzyCompare(ax[0], 0) &&
			fuzzyCompare(ax[1], 1) &&
			fuzzyCompare(ax[2], 2)   ;
}
bool QtestGetPitch(){
	Quaternion a(Vec3(0,1,0), toRad(37));
	a.normalize();
	Vec3 b(0,0,-1);
	b.rotateBy(a);
	return fuzzyCompare(a.getPitch(), asin(b[0]));
}
bool QtestGetPitch2(){
	Quaternion a(Vec3(1,1,1), toRad(37));
	a.normalize();
	Vec3 b(0,0,-1);
	b.rotateBy(a);
	return fuzzyCompare(a.getPitch(), asin(b[0]));
}
bool QtestGetRoll(){
	Quaternion a(Vec3(1,0,0), toRad(30));
	a.normalize();
	Vec3 b(0,0,-1);
	b.rotateBy(a);
	return fuzzyCompare(a.getRoll(), asin(-b[1]));
}
bool QtestGetRoll2(){
	Quaternion a(Vec3(1,1,1), toRad(30));
	a.normalize();
	Vec3 b(0,0,-1);
	b.rotateBy(a);
	return fuzzyCompare(a.getRoll(), asin(-b[1]));
}
bool QtestGetYaw(){
	//global yaw
	Quaternion a(Vec3(0,0,1), .155);//a(1,-2,3,-2);
	a.normalize();
	Vec3 b(0,1,0);
	b.rotateBy(a);
	return fuzzyCompare(a.getYaw(), atan2(b[0], b[1]));
}
bool QtestNLerp(){
	Quaternion a(1,1,0,0);
	Quaternion b(0,0,1,1);
	Quaternion check(.75,.75,.25,.25);
	check.normalize();
	a.nlerpWith(b, (.25));
	return	fuzzyQCompare(a, check);

}
bool QtestRotateBy(){
	Quaternion a(Vec3(1,0,0), toRad(90));
	Quaternion b(Vec3(0,0,1), toRad(90));
	a.rotateBy(b);
	return fuzzyQCompare(a, Quaternion(.5, .5, -.5, .5));
}
bool QtestNormalize(){
	Quaternion a(1, 0, 1, 2);
	a.normalize();
	return fuzzyCompare(a.length(), 1);
}
bool QtestInvOp(){
	Quaternion a(1, 0, 1, 2);
	return fuzzyQCompare(~a, Quaternion(1,0,-1,-2));
}
bool QtestInvMult(){
	Quaternion a(1,3,-2,4);
	a.normalize();
	Quaternion b = ~a;
	a.rotateBy(b);
	return fuzzyQCompare(a, Quaternion());
}
bool QtestIdentityMult(){
	Quaternion a(1,3,-2,4);
	a.normalize();
	Quaternion b = a;
	b.rotateBy(Quaternion());
	return fuzzyQCompare(a, b);
}
bool QtestMultAssn(){
	Quaternion a(1, 0,-1, 2);
	a *= 2;
	return fuzzyQCompare(a, Quaternion(2,0,-2,4));
}
bool QtestDivAssn(){
	Quaternion a(6, 0, 2, 4);
	a /= 2;
	return fuzzyQCompare(a, Quaternion(3, 0, 1, 2));
}
bool QtestQuatAMult	(){
	Quaternion a(Vec3(1,0,0), toRad(90));
	Quaternion b(Vec3(0,0,1), toRad(90));
	a*=b;
	return fuzzyQCompare(a, Quaternion(.5,.5,-.5,.5));
}
bool QtestPlusAssn(){
	Quaternion a(1, 0, 1, 2);
	Quaternion b(1,-1,-1, 2);
	a += b;
	return fuzzyQCompare(a, Quaternion(2, -1, 0, 4));
}
bool QtestSubAssn(){
	Quaternion a(1, 0, 1, 2);
	Quaternion b(1, 3,-1, 2);
	a -= b;
	return fuzzyQCompare(a, Quaternion(0,-3,2,0));
}
bool QtestFloatMult(){
	Quaternion a(1, 0, 1, 2);
	Quaternion b = a*3;
	return fuzzyQCompare(b, Quaternion(3,0,3,6));
	return false;
}
bool QtestFloatDiv(){
	Quaternion a(-2, 0, 2, 4);
	Quaternion b = a/2;
	return fuzzyQCompare(b, Quaternion(-1,0,1,2));
}
bool QtestPlus(){
	Quaternion a(1, 0, 1, 2);
	Quaternion b(1, 0,-1, 0);
	return fuzzyQCompare(a+b, Quaternion(2,0,0,2));
}
bool QtestSub(){
	Quaternion a(1, 0, 1, 2);
	Quaternion b(1, 0, 1, 2);
	return fuzzyQCompare(a-b, Quaternion(0,0,0,0));
}
bool QtestErrorPositive(){
	Quaternion a(1,1,1,1.f/0.f);
	return a.error();
}
bool QtestErrorNegative(){
	Quaternion a(1,1,1,1);
	return !a.error();
}
bool QtestTransitive(){
	Quaternion hat(Vec3(-1,0,0), toRad(3));
	Quaternion old(Vec3( 0,1,1), toRad(-67));
	Quaternion delta = hat;
	delta.rotateBy(~old);

	Quaternion Hat = ~old;
	Hat.rotateBy(~delta);
	Hat = ~Hat;

	return fuzzyQCompare(hat, Hat);
}
bool QtestTransitive2(){
	Quaternion hat(Vec3(-1,0,0), toRad(3));
	Quaternion old(Vec3( 0,1,1), toRad(-67));
	Quaternion delta = ~old;
	delta.rotateBy(hat);

	Quaternion Hat = old;
	Hat.rotateBy(delta);

	return fuzzyQCompare(hat, Hat);
}
bool QtestDerivative(){
	Quaternion a(Vec3( 2, 3, 4), toRad(35));
	Quaternion b(Vec3( 2,-4, 3), toRad(37));

	Vec3 rate = a.getDerivative(b);
	a.integrate(rate);
	float dist = a.distance(b);

	return fuzzyCompare(dist, 0);
}
bool QtestIntegrate(){
	Quaternion a(Vec3( 2, 3, 4), toRad(35));
	Quaternion b(Vec3( 2,-4, 3), toRad(37));

	float samps = 1000;
	float dist;
	for(int i=0; i<6*samps; i++){
		Vec3 rate = a.getDerivative(b, (1.f/samps));
		a.integrate(rate);
		dist = a.distance(b);
		if(a.error()) break;
		if(dist == 0) break;
	}

	return fuzzyCompare(dist, 0);
}
bool QtestFromAccl(){
	Quaternion orig(Vec3(1,0,0),toRad(45));
	Vec3 accl(0,0,1);
	accl.rotateBy(orig);
	Quaternion res(Vec3(0,0,1), accl);
	return fuzzyQCompare(orig, res);
}
int main(){
	TEST(QtestEulerConstr	 );
	TEST(QtestAxisAngleConstr);
	TEST(QtestInverse		 );
	TEST(QtestDot			 );
	TEST(QtestLength		 );
	TEST(QtestDistance		 );
	TEST(QtestAxis			 );
	TEST(QtestGetPitch		 );
	TEST(QtestGetPitch2      );
	TEST(QtestGetRoll		 );
	TEST(QtestGetRoll2       );
	TEST(QtestGetYaw		 );
	TEST(QtestNLerp			 );
	TEST(QtestRotateBy		 );
	TEST(QtestNormalize		 );
	TEST(QtestInvOp			 );
	TEST(QtestInvMult        );
	TEST(QtestIdentityMult   );
	TEST(QtestMultAssn		 );
	TEST(QtestDivAssn		 );
	TEST(QtestQuatAMult		 );
	TEST(QtestPlusAssn		 );
	TEST(QtestSubAssn		 );
	TEST(QtestFloatMult	 	 );
	TEST(QtestFloatDiv		 );
	TEST(QtestPlus			 );
	TEST(QtestSub			 );
	TEST(QtestErrorPositive  );
	TEST(QtestErrorNegative  );
	TEST(QtestTransitive     );
	TEST(QtestTransitive2    );
	TEST(QtestDerivative     );
	TEST(QtestIntegrate      );
	TEST(QtestFromAccl       );
}
