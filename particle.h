#pragma once
#include <Eigen/Dense>
#include <vector>

class Particle {
public:
  Eigen::Vector3d position, 
	velocity, 
	restPosition, 
	oldPosition, 
	goalPosition, 
	goalVelocity;
  double mass; 
  double totalweight; // sum of weights of this particle in all clusters
  size_t numClusters; // only used to determine singelton particles
  bool outsideSomeMovingPlane;
  std::vector<int> clusters; 
  Particle() {};
  Particle(const Particle &p)  = default; //c++11 magic :)
  short flags;
  static const short SPLIT = 1;
  //: position(p.position), velocity(p.velocity), restPosition(p.restPosition),
  //	oldPosition(p.oldPosition), goalPosition(p.goalPosition), goalVelocity(p.goalVelocity), mass(p.mass),
  //	totalweight(p.totalweight), numClusters(p.numClusters), outsideSomeMovingPlane(p.outsideSomeMovingPlane),
  //	clusters(p.clusters){};
};

class CollisionGeometry {
 private:
  Eigen::Vector3d c;
  double r;
 public:
  CollisionGeometry(){};
  CollisionGeometry & operator= (const CollisionGeometry &that);
  CollisionGeometry(const CollisionGeometry &that) = default;
  bool project(Eigen::Vector3d &x);
  void init (const Eigen::Vector3d &c, double r) { this->c = c; this->r = r;};
  void addPlane(const Eigen::Vector3d &n, double offset) { planes.emplace_back(n, offset); };
  
  std::vector<std::pair<Eigen::Vector3d, double> > planes;
};

class Cluster {
 public:
  Eigen::Vector3d restCom;
  Eigen::Vector3d worldCom;
  Eigen::Matrix3d aInv, Fp, FpNew;
  std::vector<std::pair<int, double> > neighbors;
  double mass, width, renderWidth;
  double toughness;
  double cstrain; // cumulative strain (for work hardening)
  Cluster() {Fp.setIdentity(); cstrain = 0.0;}
  Cluster(const Cluster &c) = default;
  void updatePlasticity(const Eigen::Vector3d &sigma, const Eigen::Matrix3d &U, const Eigen::Matrix3d &V, 
	  double yield, double nu, double hardening);

  CollisionGeometry cg;
  Eigen::Matrix3d worldToRestTransform, restToWorldTransform;
};

