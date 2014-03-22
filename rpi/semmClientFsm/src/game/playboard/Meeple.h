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

//forward declarations
class Field;

class Meeple {
typedef boost::shared_ptr<Field> FieldPtr;

public:
	Meeple();
	virtual ~Meeple();

	uint8_t color() const { return _color; }
	FieldPtr field() const { return _field; }
	uint16_t tagId() const { return _tagId; }
	void color(uint8_t c) { _color = c; }
	void field(FieldPtr f) { _field = f; }
	void tagId(uint16_t t) { _tagId = t; }

private:
	uint8_t _color;
	FieldPtr _field;
	uint16_t _tagId;
};

#endif /* MEEPLE_H_ */
