#include "iostream"
#include "memory"
#include "core/Vector.h"
#include "core/RigidBody.h"
#include "core/Environment.h"
#include "core/Target.h"

int main()
{
	Environment env;
	RigidBody rocket(Vector(0, 0, 1), Vector(0, 0, 0), 50, 60);
	Target target(Vector(100, 0, 200), Vector(20, 0, 10));

	const double PI = 3.14159265358979323846;
	double dt = 0.1;
	double time = 0.0;
	double logTime = 0.0;
	double maxAngle = 45.0;

	while (rocket.getPosition().getZ() > 0 && (target.getPosition() - rocket.getPosition()).findMagnitude() > 5.0) {
		time += dt;
		logTime += dt;

		Vector toTarget = target.getPosition() - rocket.getPosition();
		double desiredAngle = std::atan2(toTarget.getZ(), toTarget.getX()) * 180.0 / PI;
		double angleDiff = desiredAngle - rocket.getPitchAngleDeg();
		double maxTurnThisStep = maxAngle * dt;
		double currentAngle = rocket.getPitchAngleDeg();

		if (angleDiff > maxTurnThisStep) {
			angleDiff = maxTurnThisStep;
		}
		else if (angleDiff < -maxTurnThisStep){
			angleDiff = -maxTurnThisStep;
		}

		rocket.setPitchAngleDeg(currentAngle + angleDiff);

		if (time < 20.0) {
			rocket.applyThrust(1000);
		}
		rocket.updatePhysics(dt);
		target.updatePosition(dt);
		
		if (logTime >= 1.0) {
			std::cout << "Time: " << time << "s, Rocket Pos: (" << rocket.getPosition().getX() << ", " << rocket.getPosition().getY() << ", " << rocket.getPosition().getZ() << "), Target Pos: (" << target.getPosition().getX() << ", " << target.getPosition().getY() << ", " << target.getPosition().getZ() << ")\n";
			logTime = 0.0;
		}
	}

	if (rocket.getPosition().getZ() <= 0) {
		std::cout << "Rocket crashed at time: " << time << " seconds\n";
	}
	else {
		std::cout << "Rocket hit the target at time: " << time << " seconds\n";
	}

	std::cin.get();
	return 0;
}
