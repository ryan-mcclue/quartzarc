/*
 * QAT_FIFO.cpp
 *
 *  Created on: 1 Feb. 2023
 *      Author: ryan
 */

#include "QAT_FIFO.hpp"

#include <memory>


class QAT_FIFOBuffer {
public:
	// unique pointer type of smart pointer, i.e. will delete itself out of scope
	std::unique_ptr<uint8_t> buffer;
	uint16_t size;

	QAT_FIFOBuffer() = delete;
  QAT_FIFOBuffer(uint16_t size) : {}
};
