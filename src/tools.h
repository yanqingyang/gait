#ifndef TOOLS_H
#define TOOLS_H

#include <vector>
#include <map>
#include <iostream>
#include <fstream>      // std::ofstream
#include <iomanip>      // std::setprecision
#include <math.h>       // std::sqrt

namespace teo
{

namespace kin
{

class Quaternion
{
public:
    Quaternion();
    bool FromAxisAngle(double ux,double uy,double uz,double angle);
    bool ToAxisAngle(double &ux, double &uy, double &uz, double &angle);
    bool FromProduct(const Quaternion &q1, const Quaternion &q2);
private:
    double qw,qi,qj,qk;
    double sinPart,cosPart; //sin and cosine of the half angle
};

class Pose
{
public:
    Pose();

    /**
     * @brief Pose: Create a Pose object with rotation 0 and translation xyz.
     * @param x0
     * @param y0
     * @param z0
     */
    Pose(double x0, double y0, double z0);

    /**
     * @brief Pose : Create a Pose as a difference from initial to final pose.
     * @param initialPose
     * @param finalPose
     */
    Pose(Pose initialPose, Pose finalPose);

    /**
     * @brief Pose::Pose : Create intermediate pose from initial to final pose
     * @param initialPose
     * @param finalPose
     * @param factor: The ratio defining the new pose. 0 for initial, 1 for final, other for halfways.
     */
    Pose(Pose initialPose, Pose finalPose, double factor);


    /**
     * @brief SetPosition: Sets a new position. Overwrites old position.
     * @param new_x
     * @param new_y
     * @param new_z
     * @return
     */
    bool SetPosition(double new_x, double new_y, double new_z);

    /**
     * @brief ChangePosition: Add an offset to actual position.
     * @param dx
     * @param dy
     * @param dz
     * @return
     */
    bool ChangePosition(double dx, double dy, double dz);

    /**
     * @brief GetPosition: Copies actual pose coordinates on pose_ variables.
     * @param pose_x
     * @param pose_y
     * @param pose_z
     * @return
     */
    bool GetPosition(double &pose_x, double &pose_y, double &pose_z);

    /**
     * @brief GetX: Return the pose x coordinate.
     * @return
     */
    double GetX() const;
    /**
     * @brief GetY: Return the pose y coordinate.
     * @return
     */
    double GetY() const;
    /**
     * @brief GetZ: Return the pose z coordinate.
     * @return
     */
    double GetZ() const;

    /**
     * @brief GetAngle: Return the pose angle from the axis angle rotation.
     * @return
     */
    double GetAngle();

    /**
     * @brief GetRotation: Copies the axis-angle rotation on axis_ and pose_angle variables,
     * @param axis_i
     * @param axis_j
     * @param axis_k
     * @param pose_angle
     * @return
     */
    bool GetRotation(double &axis_i, double &axis_j, double &axis_k, double &pose_angle);

    /**
     * @brief SetRotation: Set a new pose rotation given an axis angle. Overwrites old rotation.
     * @param axis_i
     * @param axis_j
     * @param axis_k
     * @param pose_angle
     * @return
     */
    bool SetRotation(double axis_i, double axis_j, double axis_k, double pose_angle);

    //bool PoseDifference(Pose otherPose, Pose &difference);


    /**
     * @brief PoseInterpolation : Update the pose values with a pose between initial and final poses
     * @param initialPose
     * @param finalPose
     * @param factor : The ratio defining the new pose. 0 for initial, 1 for final, other for halfways.
     * @return
     */
    bool PoseInterpolation(Pose initialPose, Pose finalPose, double factor);

    bool PoseFraction(Pose &fraction, double factor);

    /**
     * @brief ChangeRotation : Change the rotation of the pose. Added rotation is defined from the actual pose.
     * It is based on cuaternion multiplication (0_Q_1 * 1_Q_2).
     * @param u2x : rotation axis x component from actual pose.
     * @param u2y : rotation axis y component from actual pose.
     * @param u2z : rotation axis z component from actual pose.
     * @param angle2 : rotation angle in radians (like std::cos).
     * @return
     */
    bool ChangeRotation(double u2x, double u2y, double u2z, double angle2);
    bool ChangePose(Pose variation);
private:
    double x,y,z; //position
    double ux,uy,uz; //axis
    double angle; //angle
    kin::Quaternion q1,q2,q3; //Not used in addrotation




};

class Link
{
public:
    Link();
    Link(const Pose &initialPose);
    Pose getCOG() const;
    void setCOG(const Pose &value);

protected:
    //The end side of link. Usually the position of the joint with the next link.
    Pose end;
    //Link center of gravity.
    Pose COG;
    /**
     * @brief changePose : virtual function that describes how the link moves. Several standard links are
     * defined in child classes (like LinkRotZ), but you can define your own just deriving from Link class
     * @param dof: The degree of freedom that will change the pose. Angle for rotations, distance for prismatic.
     * @return
     */
    virtual bool changePose(double dof) {return true;}
};

class LinkRotZ : public Link
{
    bool changePose(double dof);

};

class Robot
{
public:

    bool addLink(const Link& newLink);
    Pose getRobotBase() const;
    void setRobotBase(const Pose &value);

private:
    Pose robotBase;
    std::vector<kin::Link> links;
};

}//end namespace kin

namespace tra
{

class SpaceTrajectory
{
public:

    /**
     * @brief SpaceTrajectory : If no first point specified, trajectory will start at (0,0,0)
     */
    SpaceTrajectory();

    /**
     * @brief SpaceTrajectory : Constructor with first waypoint as parameter
     * @param initialWaypoint
     */
    SpaceTrajectory(kin::Pose initialWaypoint);

    /**
     * @brief AddTimedWaypoint: Adds a timed waypoint.
     * @param t: Delta time from last waypoint.
     * @param waypoint : The Pose to add as waypoint.
     * @return
     */
    bool AddTimedWaypoint(double &t, const kin::Pose &waypoint);

    /**
     * @brief AddWaypoint: Adds a timed waypoint. Delta time is based on defaultVelocity.
     * @param waypoint : The Pose to add as waypoint.
     * @return
     */
    double AddWaypoint(const kin::Pose &waypoint);
    bool GetLastWaypoint(kin::Pose & waypoint);
    bool SaveToFile(std::ofstream &csvFile);
    bool GetWaypoint(int index, kin::Pose &getWaypoint);
    bool GetWaypoint(int index, kin::Pose &getWaypoint, double &time_total);

    int Size();
    double getDefaultVelocity() const;
    void setDefaultVelocity(double value);
    bool GetSample(double sampleTime, kin::Pose & samplePose);
    bool GetSampleVelocity(double sampleTime, kin::Pose &samplePoseVelocity);


    bool SetInitialWaypoint( kin::Pose initialWaypoint);
    bool TrajectoryInit();
    int UpdatePointers(double atTime);

    bool ShowData();

    /**
     * @brief move: Change actual position by dx, dy, dz and store a new waypoint
     * @param dx: x coordinate shift.
     * @param dy: y coordinate shift.
     * @param dz: z coordinate shift.
     * @return : estimated time at default velocity
     */
    double move(double dx, double dy, double dz);
private:
    std::vector<kin::Pose> waypoints;
    std::vector<kin::Pose> segments;
    std::vector<kin::Pose> velocities;

    kin::Pose segment; //transformation from last_wp to next_wp
    kin::Pose trajPointer; //
    std::vector<double> time_deltas;
    std::vector<double> time_totals;
    std::vector<double>::iterator time_actual;

    //trajectory tracking
    int segmentIndex; //cant use unsigned due to latter use of lower_bound
    int last_wp; //store the wp index we come from
    double last_wpTime; //time of last waypoint
    int next_wp; //store the wp index we are going to
    double next_wpTime; //time of next waypoint


    //movement parameters
    double defaultVelocity; //default tip velocity (m/s)
    double defaultRotationSpeed; //default tip rotational speed (rad/s)

    /*
    std::map<double,Pose> waypoints;
    std::pair<double,Pose> wp; //Temporary storage. Use as local only. It can change.
    std::map<double,Pose>::iterator it;
    std::pair<std::map<double,Pose>::iterator,bool> error;*/


};

}//end namespace tra




}//end namespace teo


int FindValueIndex(std::vector<double> vector, double value);
int UpdateVectorPointer(const std::vector<double> & vector, const double & actual, double & next, double & last );

#endif // TOOLS_H

