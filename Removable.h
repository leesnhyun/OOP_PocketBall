#ifndef _REMOVABLE_
#define _REMOVABLE_
#include <exception>
#define noexcept throw ()
#define BLANK_BLOCK {}

class Removable
{
public:
	virtual void disable() noexcept = 0;
	virtual int getDisableTurn() const noexcept = 0;
	virtual bool isDisabled() const noexcept = 0;
	virtual ~Removable() BLANK_BLOCK
};
#endif