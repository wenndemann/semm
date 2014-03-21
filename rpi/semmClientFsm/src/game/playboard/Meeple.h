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

//forward declarations
class Field;

class Meeple {
public:
	Meeple();
	virtual ~Meeple();

	uint8_t color() const { return _color; }
	Field* field() const { return _field; }
	uint16_t tagId() const { return _tagId; }
	void color(uint8_t c) { _color = c; }
	void field(Field* f) { _field = f; }
	void tagId(uint16_t t) { _tagId = t; }

private:
	uint8_t _color;
	Field* _field;
	uint16_t _tagId;
};

#endif /* MEEPLE_H_ */
