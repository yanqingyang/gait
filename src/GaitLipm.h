#ifndef GAITLIPM_H
#define GAITLIPM_H


#include "Gait.h"
#include "tools.h"
#include <stdio.h>

namespace teo {

class GaitLipm : public Gait
{
public:
    GaitLipm(kin::Pose initialRightFoot, kin::Pose initialLeftFoot, double newMass);
    long LipmInitialState(physics::StateVariable mx0, physics::StateVariable my0, physics::StateVariable mz0);
    long LipmInitialState(std::vector<double> &xwp, std::vector<double> &ywp, std::vector<double> &zwp, double dt);

    double GetSwingYInitialSpeed(double initialY, double swingTime);
    double LipZmpTrajectoryWithInit(std::vector<double> & xwp, std::vector<double> & ywp, std::vector<double> & zwp, double dt);

    /**
     * @brief LipmAngularResponse: Angular response of an inverted pendulum. Give an initial trajectory,
     * timestep, and pendulum length, and receive the trajectory.
     * @param tiltwp <io> : Insert two angles for initial trajectory, then receive the following angles for
     * the resulting trajectory of the lipm.
     * @param dt <i> : time step for the trajectory.
     * @param radius <i> : Pendulum length, distance to the mass.
     * @return
     */
    long LipmAngularResponse(std::vector<double> & tiltwp, double dt, double radius);

    double LipZmpTrajectory(std::vector<double> &xwp, std::vector<double> &ywp, std::vector<double> &zwp, double dt);

    long ConvertLipTrajectory(kin::Pose robotOrigin);

private:
    //Step Definitions.
    bool HalfStepForwardRS();
    bool HalfStepForwardLS();

    //long LipForceZMP(const double & xzmp, const double & yzmp, double & x);
    long ChangeMassPosition(double dt, double xzmp, double yzmp);



    //Variables section
    physics::StateVariable mx,my,mz; //currrent inverted pendulum x,y,z mass position from base (foot) variables
    std::vector<double> trax,tray,traz,trat; //x,y,z, time trajectories
    double lipMass;
    double k1,k2,kp,kv;

};

} //namespace teo

#endif // GAITLIPM_H
