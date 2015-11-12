#ifndef _HITTABLE_
#define _HITTABLE_

#define noexcept throw ()
#define BLANK_BLOCK {}
class CSphere;

class Hittable
{
public:
	virtual bool hasIntersected(CSphere& ball) const noexcept = 0;
	virtual void hitBy(CSphere& ball) noexcept = 0;

	virtual ~Hittable() BLANK_BLOCK
};

#endif

