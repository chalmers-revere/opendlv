#include <cmath>

#include "gcdc2016/vehicle.hpp"

namespace opendlv {
namespace tools {
namespace simulation {
namespace gcdc2016 {

Vehicle::Vehicle():
    m_cmToFront(1.0565f),
    m_cmToRear(1.4935f),
    m_corneringStiffnessFront(88765.0),
    m_corneringStiffnessRear(102371.0),
    m_inertia(1865.0f),
    m_mass(1387.0f),
    m_heading(0.0f),
    m_lateralPosition(0.0f),
    m_lateralVelocity(0.0f),
    m_longitudinalPosition(0.0f),
    m_speed(0.0f),
    m_yawRate(0.0f),
    m_roadWheelAngle(0.0),
    m_throttlePedalPosition(0.0),
    m_deceleraionRequest(0.0)
{
}

Vehicle::~Vehicle()
{
}

double Vehicle::GetHeading() const
{
  return m_heading;
}

double Vehicle::GetLateralPosition() const
{
  return m_lateralPosition;
}

double Vehicle::GetYawRate() const
{
  return m_yawRate;
}

double Vehicle::GetSpeed() const
{
  return m_speed;
}

void Vehicle::SetHeading(double a_heading)
{
  m_heading = a_heading;
}

void Vehicle::SetLateralPosition(double a_lateralPosition)
{
  m_lateralPosition = a_lateralPosition;
}

void Vehicle::SetRoadVheelAngle(double a_roadWheelAngle)
{
m_roadWheelAngle = a_roadWheelAngle;

}

void Vehicle::SetThrottlePedalPosition(double a_throttlePedalPosition)
{
m_throttlePedalPosition = a_throttlePedalPosition;

}

void Vehicle::SetDeceleraionRequest(double a_deceleraionRequest)
{
m_deceleraionRequest = a_deceleraionRequest;
}

void Vehicle::SetSpeed(double a_throttlePedalPosition, double a_deceleraionRequest, double a_dt)
{
m_speed = m_speed;
   if (a_throttlePedalPosition > 0)
   {
        m_speed = m_speed + a_throttlePedalPosition*0.1*a_dt - std::pow(M_E, -20.0/m_speed )*std::atan(0.1*m_speed);
   }
    else
   {
        m_speed = m_speed - a_deceleraionRequest*0.1*a_dt;
   }

}

void Vehicle::Update(double a_dt)
{
  double const a = m_cmToFront;
  double const b = m_cmToRear;
  double const cF = m_corneringStiffnessFront;
  double const cR = m_corneringStiffnessRear;


  this->SetSpeed(m_throttlePedalPosition, m_deceleraionRequest, a_dt);


  double psi = m_heading;
  double y = m_lateralPosition;
  double v = m_lateralVelocity;
  double x = m_longitudinalPosition;
  double u = m_speed;
  double r = m_yawRate;

  double motionAngleFront = (-a * r - v) / u;
  double motionAngleRear = (b * r - v) / u;

  double vdot = (
      cR * motionAngleRear +
      cF * motionAngleFront +
      cF * m_roadWheelAngle
      ) / m_mass - u * r;

  double rdot = (
      -b * cR * motionAngleRear +
      a * cF * motionAngleFront +
      a * cF * m_roadWheelAngle
      ) / m_inertia;

  v = v + vdot * a_dt;
  r = r + rdot * a_dt;
  
  psi = psi + r * a_dt;

  double di = u * a_dt;
  double dj = v * a_dt;

  x = x + cos(psi) * di - sin(psi) * dj;
  y = y + sin(psi) * di + cos(psi) * dj;

  m_heading = psi;
  m_lateralPosition = y;
  m_lateralVelocity = v;
  m_longitudinalPosition = x;
  m_speed = u;
  m_yawRate = r;
}

} // gcdc2016
} // simulation
} // tools
} // opendlv
