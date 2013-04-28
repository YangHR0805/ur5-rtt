#include "UniversalRobotsUR5-component.hpp"
#include <rtt/Component.hpp>
#include <kdl/frames_io.hpp>
#include <iostream>
#include <cmath>

UniversalRobotsUR5::UniversalRobotsUR5(std::string const& name) :
    TaskContext(name){
    // setup output ports
    this->ports()->addPort("UR5DesiredJointPosition", q_to_robot);
    q_to_robot.setDataSample(q_cmd_std);
    // setup input ports
    this->ports()->addPort("UR5JointPosition", q_from_robot);
    this->ports()->addPort("UR5JointVelocity", qdot_from_robot);
    // setup properties
    this->properties()->addProperty("tool_frame", tool_frame).doc(
            "pose from mounting-plate to tool center point");

  std::cout << "UniversalRobotsUR5 constructed !" <<std::endl;
}

bool UniversalRobotsUR5::configureHook(){

    //DH Params
    double DH_A[6] = {0, -0.425, -0.39243, 0, 0, 0};
    double DH_D[6] = {0.0892, 0, 0, 0.109, 0.093, 0.082};
    double DH_ALPHA[6] = {M_PI_2, 0, 0, M_PI_2, -M_PI_2, 0};
    double DH_Q_HOME_OFFSET[6] = {0, -M_PI_2, -M_PI_2 ,-M_PI_2, M_PI_2, 0};

    frame_chain = KDL::Chain();

    for (unsigned int i = 0; i < 6; i++) {
        frame_chain.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotZ),
                KDL::Frame::DH(DH_A[i], DH_ALPHA[i], DH_D[i], 0)));
    }
    q_kdl = KDL::JntArray(6);
    // initialize position vector to home offset
    for (unsigned int i; i < 6; i++) {
        q_kdl(i) = DH_Q_HOME_OFFSET[i];
    }

    // create solvers for the chain: pose and jacobian solver
    fk = new KDL::ChainFkSolverPos_recursive(frame_chain);
    jnt2jac = new KDL::ChainJntToJacSolver(frame_chain);

    //fk->JntToCart(q_kdl, tool_frame);

    return true;
}

bool UniversalRobotsUR5::startHook(){
  std::cout << "UniversalRobotsUR5 started !" <<std::endl;
  return true;
}

void UniversalRobotsUR5::updateHook(){
  std::cout << "UniversalRobotsUR5 executes updateHook !" <<std::endl;
}

void UniversalRobotsUR5::stopHook() {
  std::cout << "UniversalRobotsUR5 executes stopping !" <<std::endl;
}

void UniversalRobotsUR5::cleanupHook() {
  std::cout << "UniversalRobotsUR5 cleaning up !" <<std::endl;
}

/*
 * Using this macro, only one component may live
 * in one library *and* you may *not* link this library
 * with another component library. Use
 * ORO_CREATE_COMPONENT_TYPE()
 * ORO_LIST_COMPONENT_TYPE(UniversalRobotsUR5)
 * In case you want to link with another library that
 * already contains components.
 *
 * If you have put your component class
 * in a namespace, don't forget to add it here too:
 */
ORO_CREATE_COMPONENT(UniversalRobotsUR5)
