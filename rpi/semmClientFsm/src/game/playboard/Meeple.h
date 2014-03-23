/*
 * Meeple.h
 *
 *  Created on: Mar 21, 2014
 *      Author: reinhard
 */

#ifndef MEEPLE_H_
#define MEEPLE_H_

// global includes
#include <inttypes.h>
#include <boost/shared_ptr.hpp>

class Meeple {

public:
	Meeple( uint16_t, int32_t );
	virtual ~Meeple();

	uint16_t tag() const { return _tag; }
	void tag(uint16_t t) { _tag = t; }
	int32_t fieldId() const { return _fieldId; }
	void fieldId(int32_t f) { _fieldId = f; }

private:
	uint16_t _tag;
	int32_t _fieldId;
};

#endif /* MEEPLE_H_ */
