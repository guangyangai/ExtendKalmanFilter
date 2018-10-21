#include "kalman_filter.h"
#include <iostream>

using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;
// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */
  x_ = F_ * x_;
  MatrixXd Ft_ = F_.transpose();
  P_ = F_ * P_ * Ft_ + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */
  VectorXd y_ = z - H_ * x_;
  MatrixXd Ht_ = H_.transpose();
  MatrixXd S_ = H_ * P_ * Ht_ + R_;
  MatrixXd Si_ = S_.inverse();
  MatrixXd K_ =  P_ * Ht_ * Si_;

  x_ = x_ + (K_ * y_);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K_ * H_) * P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
  */
  float px = x_(0);
  float py = x_(1);
  float vx = x_(2);
  float vy = x_(3);
  float c1 = px*px+py*py;
  float c2 = sqrt(c1);
  float c3 = atan2(py, px);
  if(fabs(c1) < 0.0001){
    cout << "UpdateEKF () - Error - Division by Zero" << endl;
    c1 = 0.0001;
  }
  float c4 = (px*vx+py*vy)/c2;
  VectorXd x_transformed(3);
  x_transformed << c2, c3, c4;
  VectorXd y_ = z - x_transformed;
  while(fabs(y_(1)) > M_PI){
    if(y_(1)>0){
      y_(1) -= 2 * M_PI;
    }else{
      y_(1) += 2 * M_PI;
    }  
  }
  //H_ = tools.CalculateJacobian(x_);
  MatrixXd Ht_ = H_.transpose();
  MatrixXd S_ = H_ * P_ * Ht_ + R_;
  MatrixXd Si_ = S_.inverse();
  MatrixXd K_ =  P_ * Ht_ * Si_;

  x_ = x_ + (K_ * y_);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K_ * H_) * P_;

}
