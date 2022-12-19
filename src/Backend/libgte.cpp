#include "libgte.hpp"

namespace libgte
{
	s16 mult(s16 nb, s16 rate)
	{
		s32 nb2{ nb * rate };
		if (nb2 < 0)
		{
			nb2 += 0xFFF;
		}
		return static_cast<s16>(nb2 >> 0xC);
	}

	VECTOR* ApplyMatrix(MATRIX* m, SVECTOR* v0, VECTOR* v1)
	{
		v1->vx = (mult(m->m[0][0], v0->vx) + mult(m->m[0][1], v0->vy) + mult(m->m[0][2], v0->vz));
		v1->vy = (mult(m->m[1][0], v0->vx) + mult(m->m[1][1], v0->vy) + mult(m->m[1][2], v0->vz));
		v1->vz = (mult(m->m[2][0], v0->vx) + mult(m->m[2][1], v0->vy) + mult(m->m[2][2], v0->vz));

		return v1;
	}

	SVECTOR* ApplyMatrixSV(MATRIX* m, SVECTOR* v0, SVECTOR* v1)
	{
		v1->vx = (mult(m->m[0][0], v0->vx) + mult(m->m[0][1], v0->vy) + mult(m->m[0][2], v0->vz));
		v1->vy = (mult(m->m[1][0], v0->vx) + mult(m->m[1][1], v0->vy) + mult(m->m[1][2], v0->vz));
		v1->vz = (mult(m->m[2][0], v0->vx) + mult(m->m[2][1], v0->vy) + mult(m->m[2][2], v0->vz));

		return v1;
	}
}