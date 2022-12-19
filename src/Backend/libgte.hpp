#pragma once

#include "Common/Types.hpp"

namespace libgte
{
	struct MATRIX
	{
		short m[3][3];
		long t[3];
	};

	struct SVECTOR
	{
		short vx, vy, vz;
		short pad;
	};

	struct VECTOR
	{
		long vx, vy, vz;
		long pad;
	};

	s16 mult(s16 nb, s16 rate);

	VECTOR* ApplyMatrix(MATRIX* m, SVECTOR* v0, VECTOR* v1);
	SVECTOR* ApplyMatrixSV(MATRIX* m, SVECTOR* v0, SVECTOR* v1);
}