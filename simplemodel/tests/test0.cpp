// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleModel is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleModel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleModel (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#include <cmath>
#include <cstdio>
#include <string>

using namespace std;

#include "../simplemodel.h"

const float accuracy = (1.0 / 256.0);

bool close_enough(float a, float b) {
  return (a > (b - accuracy)) && (a < (b + accuracy));
}

void print(const SimpleModel::Vector3 &v) {
  fprintf(stderr, " (%1.2f, %1.2f, %1.2f)\n", v.data[0], v.data[1], v.data[2]);
}

void print(const SimpleModel::Quaternion &q) {
  fprintf(stderr, " [%1.2f, %1.2f, %1.2f, %1.2f]\n", q.w, q.x, q.y, q.z);
}

void print(const SimpleModel::Matrix4x4 &m) {
  fprintf(stderr, "\n");
  fprintf(stderr, "| %1.2f %1.2f %1.2f %1.2f |\n", m.data[0], m.data[1],
          m.data[2], m.data[3]);
  fprintf(stderr, "| %1.2f %1.2f %1.2f %1.2f |\n", m.data[4], m.data[5],
          m.data[6], m.data[7]);
  fprintf(stderr, "| %1.2f %1.2f %1.2f %1.2f |\n", m.data[8], m.data[9],
          m.data[10], m.data[11]);
  fprintf(stderr, "| %1.2f %1.2f %1.2f %1.2f |\n", m.data[12], m.data[13],
          m.data[14], m.data[15]);
}

bool close_enough(const SimpleModel::Vector3 a, const SimpleModel::Vector3 b) {
  if (close_enough(a.data[0], b.data[0]) &&
      close_enough(a.data[1], b.data[1]) &&
      close_enough(a.data[2], b.data[2])) {
    return true;
  }
  fprintf(stderr, "ERROR: Vector3 is off:\n");
  fprintf(stderr, "A:");
  print(a);
  fprintf(stderr, "B:");
  print(b);
  return false;
}

bool close_enough(const SimpleModel::Quaternion a,
                  const SimpleModel::Quaternion b) {
  if (close_enough(a.w, b.w) && close_enough(a.x, b.x) &&
      close_enough(a.y, b.y) && close_enough(a.z, b.z)) {
    return true;
  }
  fprintf(stderr, "ERROR: Quaternion is off:\n");
  fprintf(stderr, "A:");
  print(a);
  fprintf(stderr, "B:");
  print(b);
  return false;
}

bool close_enough(const SimpleModel::Matrix4x4 a,
                  const SimpleModel::Matrix4x4 b) {
  if (close_enough(a.data[0], b.data[0]) &&
      close_enough(a.data[1], b.data[1]) &&
      close_enough(a.data[2], b.data[2]) &&
      close_enough(a.data[3], b.data[3]) &&
      close_enough(a.data[4], b.data[4]) &&
      close_enough(a.data[5], b.data[5]) &&
      close_enough(a.data[6], b.data[6]) &&
      close_enough(a.data[7], b.data[7]) &&
      close_enough(a.data[8], b.data[8]) &&
      close_enough(a.data[9], b.data[9]) &&
      close_enough(a.data[10], b.data[10]) &&
      close_enough(a.data[11], b.data[11]) &&
      close_enough(a.data[12], b.data[12]) &&
      close_enough(a.data[13], b.data[13]) &&
      close_enough(a.data[14], b.data[14]) &&
      close_enough(a.data[15], b.data[15])) {
    return true;
  }
  fprintf(stderr, "ERROR: 4x4 Matrix is off:\n");
  fprintf(stderr, "A:");
  print(a);
  fprintf(stderr, "B:");
  print(b);
  return false;
}

int main(int argc, char **argv) {
  fprintf(stderr, "\nIdentities:\n\n");

  SimpleModel::Quaternion qident = {1.0, 0.0, 0.0, 0.0};
  SimpleModel::Quaternion qx180 = {0.0, 1.0, 0.0, 0.0};
  SimpleModel::Quaternion qy180 = {0.0, 0.0, 1.0, 0.0};
  SimpleModel::Quaternion qz180 = {0.0, 0.0, 0.0, 1.0};
  SimpleModel::Quaternion qx90 = {0.70710678118, 0.70710678118, 0.0, 0.0};
  SimpleModel::Quaternion qy90 = {0.70710678118, 0.0, 0.70710678118, 0.0};
  SimpleModel::Quaternion qz90 = {0.70710678118, 0.0, 0.0, 0.70710678118};
  SimpleModel::Quaternion qxz9090 = {0.5, 0.5, 0.5, 0.5};
  SimpleModel::Quaternion quat = {1.0, 0.0, 0.0, 0.0};
  print(qident);

  SimpleModel::Matrix4x4 mident = SimpleModel::identity4x4;
  SimpleModel::Matrix4x4 mat = SimpleModel::identity4x4;
  print(mident);

  fprintf(stderr, "\nTesting:\n\n");

  SimpleModel::Vector3 pos = {{1.0, 2.0, 4.0}};
  SimpleModel::Vector3 start = pos;
  SimpleModel::Vector3 res = pos;

  pos = start;
  SimpleModel::QuaternionRotate(pos, qident);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat ident rotate failed!\n\n");
  }

  pos = start;
  SimpleModel::MatrixTransform(pos, mident);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 ident transformation failed!\n\n");
  }

  pos = start;
  res = pos;
  res.data[1] = -pos.data[1];
  res.data[2] = -pos.data[2];
  SimpleModel::QuaternionRotate(pos, qx180);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat x180 rotate failed!\n\n");
  }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qx180);
  SimpleModel::MatrixTransform(pos, mat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 x180 transformation failed!\n\n");
  }

  pos = start;
  SimpleModel::Matrix4x4ToQuaternion(quat, mat);
  if (!close_enough(quat, qx180)) {
    fprintf(stderr, "ERROR: 4x4 x180 mangle failed!\n\n");
  }
  SimpleModel::QuaternionRotate(pos, quat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 x180 mangled rotate failed!\n\n");
  }

  pos = start;
  res = pos;
  res.data[0] = -pos.data[0];
  res.data[2] = -pos.data[2];
  SimpleModel::QuaternionRotate(pos, qy180);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat y180 rotate failed!\n\n");
  }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qy180);
  SimpleModel::MatrixTransform(pos, mat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 y180 transformation failed!\n\n");
  }

  pos = start;
  SimpleModel::Matrix4x4ToQuaternion(quat, mat);
  if (!close_enough(quat, qy180)) {
    fprintf(stderr, "ERROR: 4x4 y180 mangle failed!\n\n");
  }
  SimpleModel::QuaternionRotate(pos, quat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 y180 mangled rotate failed!\n\n");
  }

  pos = start;
  res = pos;
  res.data[0] = -pos.data[0];
  res.data[1] = -pos.data[1];
  SimpleModel::QuaternionRotate(pos, qz180);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat z180 rotate failed!\n\n");
  }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qz180);
  SimpleModel::MatrixTransform(pos, mat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 z180 transformation failed!\n\n");
  }

  pos = start;
  SimpleModel::Matrix4x4ToQuaternion(quat, mat);
  if (!close_enough(quat, qz180)) {
    fprintf(stderr, "ERROR: 4x4 z180 mangle failed!\n\n");
  }
  SimpleModel::QuaternionRotate(pos, quat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 z180 mangled rotate failed!\n\n");
  }

  pos = start;
  res = pos;
  res.data[1] = -pos.data[2];
  res.data[2] = pos.data[1];
  SimpleModel::QuaternionRotate(pos, qx90);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat x90 rotate failed!\n\n");
  }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qx90);
  SimpleModel::MatrixTransform(pos, mat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 x90 transformation failed!\n\n");
  }

  pos = start;
  SimpleModel::Matrix4x4ToQuaternion(quat, mat);
  if (!close_enough(quat, qx90)) {
    fprintf(stderr, "ERROR: 4x4 x90 mangle failed!\n\n");
  }
  SimpleModel::QuaternionRotate(pos, quat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 x90 mangled rotate failed!\n\n");
  }

  pos = start;
  res = pos;
  res.data[0] = pos.data[2];
  res.data[2] = -pos.data[0];
  SimpleModel::QuaternionRotate(pos, qy90);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat y90 rotate failed!\n\n");
  }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qy90);
  SimpleModel::MatrixTransform(pos, mat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 y90 transformation failed!\n\n");
  }

  pos = start;
  SimpleModel::Matrix4x4ToQuaternion(quat, mat);
  if (!close_enough(quat, qy90)) {
    fprintf(stderr, "ERROR: 4x4 y90 mangle failed!\n\n");
  }
  SimpleModel::QuaternionRotate(pos, quat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 y90 mangled rotate failed!\n\n");
  }

  pos = start;
  res = pos;
  res.data[0] = -pos.data[1];
  res.data[1] = pos.data[0];
  SimpleModel::QuaternionRotate(pos, qz90);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat z90 rotate failed!\n\n");
  }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qz90);
  SimpleModel::MatrixTransform(pos, mat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 z90 transformation failed!\n\n");
  }

  pos = start;
  SimpleModel::Matrix4x4ToQuaternion(quat, mat);
  if (!close_enough(quat, qz90)) {
    fprintf(stderr, "ERROR: 4x4 z90 mangle failed!\n\n");
  }
  SimpleModel::QuaternionRotate(pos, quat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 z90 mangled rotate failed!\n\n");
  }

  pos = start;
  res.data[0] = pos.data[2];
  res.data[1] = pos.data[0];
  res.data[2] = pos.data[1];
  SimpleModel::QuaternionRotate(pos, qxz9090);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat xz9090 rotate failed!\n\n");
  }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qxz9090);
  SimpleModel::MatrixTransform(pos, mat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 xz9090 transformation failed!\n\n");
  }

  pos = start;
  SimpleModel::Matrix4x4ToQuaternion(quat, mat);
  if (!close_enough(quat, qxz9090)) {
    fprintf(stderr, "ERROR: 4x4 xz9090 mangle failed!\n\n");
  }
  SimpleModel::QuaternionRotate(pos, quat);
  if (!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 xz9090 mangled rotate failed!\n\n");
  }

  SimpleModel::Matrix4x4 m1, m2, mres;
  m1.data[0] = 2.0;
  m1.data[1] = 3.0;
  m1.data[2] = 4.0;
  m1.data[3] = 5.0;
  m1.data[4] = 6.0;
  m1.data[5] = 7.0;
  m1.data[6] = 8.0;
  m1.data[7] = 9.0;
  m1.data[8] = 10.0;
  m1.data[9] = 11.0;
  m1.data[10] = 12.0;
  m1.data[11] = 13.0;
  m1.data[12] = 14.0;
  m1.data[13] = 15.0;
  m1.data[14] = 16.0;
  m1.data[15] = 17.0;
  m2.data[0] = 18.0;
  m2.data[1] = 19.0;
  m2.data[2] = 20.0;
  m2.data[3] = 21.0;
  m2.data[4] = 22.0;
  m2.data[5] = 23.0;
  m2.data[6] = 24.0;
  m2.data[7] = 25.0;
  m2.data[8] = 26.0;
  m2.data[9] = 27.0;
  m2.data[10] = 28.0;
  m2.data[11] = 29.0;
  m2.data[12] = 30.0;
  m2.data[13] = 31.0;
  m2.data[14] = 32.0;
  m2.data[15] = 33.0;
  mres.data[0] = 356.0;
  mres.data[1] = 370.0;
  mres.data[2] = 384.0;
  mres.data[3] = 398.0;
  mres.data[4] = 740.0;
  mres.data[5] = 770.0;
  mres.data[6] = 800.0;
  mres.data[7] = 830.0;
  mres.data[8] = 1124.0;
  mres.data[9] = 1170.0;
  mres.data[10] = 1216.0;
  mres.data[11] = 1262.0;
  mres.data[12] = 1508.0;
  mres.data[13] = 1570.0;
  mres.data[14] = 1632.0;
  mres.data[15] = 1694.0;
  SimpleModel::Multiply(mat, m2, m1);
  if (!close_enough(mat, mres)) {
    fprintf(stderr, "ERROR: 4x4 multiplication(2) failed!\n\n");
  }
  SimpleModel::Multiply(mat, mident, m2, m1);
  if (!close_enough(mat, mres)) {
    fprintf(stderr, "ERROR: 4x4 multiplication(3) failed!\n\n");
  }

  SimpleModel::Multiply(mat, mident, m2, mident, m1);
  if (!close_enough(mat, mres)) {
    fprintf(stderr, "ERROR: 4x4 multiplication(4) failed!\n\n");
  }

  SimpleModel::Multiply(mat, mident, m2, mident, m1, mident);
  if (!close_enough(mat, mres)) {
    fprintf(stderr, "ERROR: 4x4 multiplication(5) failed!\n\n");
  }

  fprintf(stderr, "\nTesting Complete.\n");
  return 0;
}
