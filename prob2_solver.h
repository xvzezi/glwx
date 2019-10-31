//
// Created by xvzezi on 2019/4/28.
//

#ifndef OPENGL_WRAP_X_PROB2_SOLVER_H
#define OPENGL_WRAP_X_PROB2_SOLVER_H

#include <xsolver.h>
#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;

class prob2_solver{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    prob2_solver()
    {
        // init
        mass = 1;
        q << 1, 1, 1 + radius, 1;
        q_deriv << 0, 0, 0, 0;
        out_force << 0, 0, 0, 0;
        cons_force << 0, - mass * gravity, 0, -mass * gravity;
        Q << 0, 0, 0, 0;
        cQ << 0, 0, 0, 0;
        float dm = 1 / mass;
        W = Matrix4f::Zero();
        W(0,0) = W(1,1) = W(2,2) = W(3,3) = dm;

    }

    void get_cforce(int index, float& x, float& y)
    {
        if(index != 0 && index != 1) return;
        int base = index * 2;
        x = cQ(base);
        y = cQ(base + 1);
    }

    void apply_force(int index, float& x, float& y)
    {
        if(index != 0 && index != 1) return;
        int base = index * 2;
        out_force(base) = x;
        out_force(base + 1) = y;
    }

    void get_location(int index, float& x, float& y)
    {
        if(index != 0 && index != 1)
        {
            x = y = 0;
        }
        int base = index * 2;
        x = q(base);
        y = q(base + 1);
    }

    void solve_next(float time_step)
    {
        // 1. calculate the forces
        cal_force(Q, q_deriv);
        cal_jacobian(jacobian, q);
        cal_jacobian_derive(jacobian_deriv, q_deriv);
        cal_constraints_damping(q, q_deriv);
        Matrix2f left = jacobian * W * jacobian.transpose();
        Vector2f right = - jacobian_deriv * q_deriv - jacobian * W * Q - ks * ks_m - kd * kd_m;
        lambda = left.householderQr().solve(right);

        // 2. first try the euler method
        cQ = jacobian.transpose() * lambda;
        Vector4f t_acc = (cQ + Q) / mass;
        q += time_step * q_deriv;
        q_deriv += time_step * t_acc;

        // 3. print constraints
//        print_constraint();
    }

private:
    void cal_force(Vector4f& vec, Vector4f& q_d)
    {
        vec = out_force + cons_force - ksys * q_d;
    }
    void cal_jacobian(Matrix<float, 2, 4>& mat, Vector4f& q)
    {
        // \partial C / \partial x
        mat.row(0) << -2 * q(0), 1, 0, 0;
        mat.row(1) << 2 * q(0) - 2 * q(2),
                    2 * q(1) - 2 * q(3),
                    2 * q(2) - 2 * q(0),
                    2 * q(3) - 2 * q(1);
    }
    void cal_jacobian_derive(Matrix<float, 2, 4>& mat, Vector4f& q_d)
    {
        mat.row(0) << -2 * q_d(0), 0, 0, 0;
        Vector2f dif;
        dif << q_d(0) - q_d(2), q_d(1) - q_d(3);
        dif *= 2;
        mat.row(1) << dif(0), dif(1), -dif(0), -dif(1);
    }
    void cal_constraints_damping(Vector4f& q, Vector4f& q_d)
    {
        // ks_m
        Vector2f dif;
        dif << q(0) - q(2), q(1) - q(3);
        float len = dif.squaredNorm();
        float r2 = radius * radius;
        ks_m << q(1) - q(0) * q(0), len - r2;
        // kd_m;
        Vector2f v_dif;
        v_dif << q_d(0) - q_d(2), q_d(1) - q_d(3);
        float con = v_dif.dot(dif) * 2;
        kd_m << -2 * q(0) * q_d(0) + q_d(1), con;
    }

    void print_constraint()
    {
        Vector2f dif;
        dif << q(0) - q(2), q(1) - q(3);
        std::cout << "distance: " << dif.norm() << std::endl;
        std::cout << "para_dif: " << q(1) - q(0) * q(0) << std::endl;
        std::cout << "---------------" << std::endl;

    }

    Vector4f q;             // loc
    Vector4f q_deriv;       // vec
    Vector4f Q;             // force
    Matrix4f W;             // 1 / mass

    Matrix<float, 2, 4> jacobian;
    Matrix<float, 2, 4> jacobian_deriv;

    Vector2f lambda;
    Vector4f cQ;

    float ksys = 2;
    float ks = 100;
    float kd = 100;
    Vector2f ks_m;
    Vector2f kd_m;
    float mass = 1;
    float gravity = 9.8;
    float radius = 1;

    Vector4f out_force;
    Vector4f cons_force;
};

#endif //OPENGL_WRAP_X_PROB2_SOLVER_H
