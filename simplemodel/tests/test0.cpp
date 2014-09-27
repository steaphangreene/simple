// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <stea@cs.binghamton.edu>
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

const float accuracy = (1.0/256.0);

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
  fprintf(stderr, "| %1.2f %1.2f %1.2f %1.2f |\n",
          m.data[0], m.data[1], m.data[2], m.data[3]);
  fprintf(stderr, "| %1.2f %1.2f %1.2f %1.2f |\n",
          m.data[4], m.data[5], m.data[6], m.data[7]);
  fprintf(stderr, "| %1.2f %1.2f %1.2f %1.2f |\n",
          m.data[8], m.data[9], m.data[10], m.data[11]);
  fprintf(stderr, "| %1.2f %1.2f %1.2f %1.2f |\n",
          m.data[12], m.data[13], m.data[14], m.data[15]);
  }

bool close_enough(const SimpleModel::Vector3 a,
                  const SimpleModel::Vector3 b) {
  if(close_enough(a.data[0], b.data[0]) &&
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
  if(close_enough(a.w, b.w) &&
     close_enough(a.x, b.x) &&
     close_enough(a.y, b.y) &&
     close_enough(a.z, b.z)) {
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
  if(close_enough(a.data[0], b.data[0]) &&
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

  SimpleModel::Quaternion qident = { 1.0, 0.0, 0.0, 0.0 };
  SimpleModel::Quaternion qx180 = { 0.0, 1.0, 0.0, 0.0 };
  SimpleModel::Quaternion qy180 = { 0.0, 0.0, 1.0, 0.0 };
  SimpleModel::Quaternion qz180 = { 0.0, 0.0, 0.0, 1.0 };
  SimpleModel::Quaternion qx90 = { 0.70710678118, 0.70710678118, 0.0, 0.0 };
  SimpleModel::Quaternion qy90 = { 0.70710678118, 0.0, 0.70710678118, 0.0 };
  SimpleModel::Quaternion qz90 = { 0.70710678118, 0.0, 0.0, 0.70710678118 };
  SimpleModel::Quaternion qxz9090 = { 0.5, 0.5, 0.5, 0.5 };
  print(qident);

  SimpleModel::Matrix4x4 mident = SimpleModel::identity4x4;
  SimpleModel::Matrix4x4 mat = SimpleModel::identity4x4;
  print(mident);

  fprintf(stderr, "\nTesting:\n\n");

  SimpleModel::Vector3 pos = { 1.0, 2.0, 4.0 };
  SimpleModel::Vector3 start = pos;
  SimpleModel::Vector3 res = pos;

  pos = start;
  SimpleModel::QuaternionRotate(pos, qident);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat ident rotate failed!\n\n");
    }

  pos = start;
  SimpleModel::MatrixTransform(pos, mident);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 ident transformation failed!\n\n");
    }

  pos = start;
  res = pos;
  res.data[1] = -pos.data[1];
  res.data[2] = -pos.data[2];
  SimpleModel::QuaternionRotate(pos, qx180);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat x180 rotate failed!\n\n");
    }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qx180);
  SimpleModel::MatrixTransform(pos, mat);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 x180 transformation failed!\n\n");
    }

  pos = start;
  res = pos;
  res.data[0] = -pos.data[0];
  res.data[2] = -pos.data[2];
  SimpleModel::QuaternionRotate(pos, qy180);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat y180 rotate failed!\n\n");
    }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qy180);
  SimpleModel::MatrixTransform(pos, mat);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 y180 transformation failed!\n\n");
    }

  pos = start;
  res = pos;
  res.data[0] = -pos.data[0];
  res.data[1] = -pos.data[1];
  SimpleModel::QuaternionRotate(pos, qz180);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat z180 rotate failed!\n\n");
    }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qz180);
  SimpleModel::MatrixTransform(pos, mat);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 z180 transformation failed!\n\n");
    }

  pos = start;
  res = pos;
  res.data[1] = -pos.data[2];
  res.data[2] = pos.data[1];
  SimpleModel::QuaternionRotate(pos, qx90);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat x90 rotate failed!\n\n");
    }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qx90);
  SimpleModel::MatrixTransform(pos, mat);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 x90 transformation failed!\n\n");
    }

  pos = start;
  res = pos;
  res.data[0] = pos.data[2];
  res.data[2] = -pos.data[0];
  SimpleModel::QuaternionRotate(pos, qy90);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat y90 rotate failed!\n\n");
    }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qy90);
  SimpleModel::MatrixTransform(pos, mat);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 y90 transformation failed!\n\n");
    }

  pos = start;
  res = pos;
  res.data[0] = -pos.data[1];
  res.data[1] = pos.data[0];
  SimpleModel::QuaternionRotate(pos, qz90);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat z90 rotate failed!\n\n");
    }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qz90);
  SimpleModel::MatrixTransform(pos, mat);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 z90 transformation failed!\n\n");
    }

  pos = start;
  res.data[0] = pos.data[2];
  res.data[1] = pos.data[0];
  res.data[2] = pos.data[1];
  SimpleModel::QuaternionRotate(pos, qxz9090);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: Quat xz9090 rotate failed!\n\n");
    }

  pos = start;
  SimpleModel::QuaternionToMatrix4x4(mat, qxz9090);
  SimpleModel::MatrixTransform(pos, mat);
  if(!close_enough(pos, res)) {
    fprintf(stderr, "ERROR: 4x4 xz9090 transformation failed!\n\n");
    }

  fprintf(stderr, "\nTesting Complete.\n");
  return 0;
  }
