/*
 * QAT_FIFO.hpp
 *
 *  Created on: 1 Feb. 2023
 *      Author: ryan
 */

#ifndef QAT_FIFO_HPP_
#define QAT_FIFO_HPP_

#include "setup.hpp"

#include <memory>

class QAT_FIFOBuffer {
public:
	enum FIFOState : uint8_t {FS_NotEmpty = 0, FSEmpty};

	// unique pointer type of smart pointer, i.e. will delete itself out of scope
	std::unique_ptr<uint8_t[]> buffer;
	uint16_t size;

	// will treat as circular
	// if these indexes don't match, then there is data waiting?
	uint16_t read_index;
	uint16_t write_index;

	QAT_FIFOBuffer() = delete;
  QAT_FIFOBuffer(uint16_t size) :
  	buffer(std::make_unique<uint8_t[]>(size)),
		size(size),
		read_index(0),
		write_index(0) {}

  void clear(void);

  FIFOState empty(void);

  uint16_t pending(void);

  void push(uint8_t data);

  uint8_t pop(void);
};

#endif /* QAT_FIFO_HPP_ */
