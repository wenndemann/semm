/*
 * Field.h
 *
 *  Created on: Mar 21, 2014
 *      Author: reinhard
 */

#ifndef FIELD_H_
#define FIELD_H_

// global includes
#include <inttypes.h>
#include <boost/shared_ptr.hpp>

//forward declarations
class Meeple;

class Field {
typedef boost::shared_ptr<Meeple> MeeplePtr;

public:
	Field();
	Field(uint8_t x, uint8_t y, uint8_t no);
	virtual ~Field();

	uint8_t x() const { return _x; }
	uint8_t y() const { return _y; }
	uint8_t no() const { return _no; }
	MeeplePtr meeple() const { return _meeple; }
	void x(uint8_t x) { _x = x; }
	void y(uint8_t y) { _y = y; }
	void no(uint8_t no) { _no = no; }
	void meeple(MeeplePtr m) { _meeple = m; }

private:
	uint8_t _x, _y, _no;
	MeeplePtr _meeple;

	void set(uint8_t x, uint8_t y) {
		_x = x;
		_y = y;
	}

	Field &operator=(const Field& cmp) {
		return *this;
	}

	bool operator!=(Field* cmp) {
		return !(this->no() == cmp->no());
	}
};

#endif /* FIELD_H_ */
