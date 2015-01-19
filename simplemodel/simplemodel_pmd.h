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

#ifndef SIMPLEMODEL_PMD_H
#define SIMPLEMODEL_PMD_H

#include "simplemodel_pmx.h"

class SimpleModel_PMD : public SimpleModel_PMX {
 public:
  SimpleModel_PMD(const string &filenm, const string &defskin = "default");
  virtual ~SimpleModel_PMD();

  virtual bool Load(const string &filenm, const string &defskin = "default");

 protected:
  SimpleModel_PMD();
};

#endif  // SIMPLEMODEL_PMD_H
