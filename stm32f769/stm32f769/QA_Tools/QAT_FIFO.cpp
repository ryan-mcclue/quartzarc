/*
 * QAT_FIFO.cpp
 *
 *  Created on: 1 Feb. 2023
 *      Author: ryan
 */

#include "QAT_FIFO.hpp"

  void QAT_FIFOBuffer::clear(void) {
    read_index = write_index = 0;
  }

  QAT_FIFOBuffer::FIFOState QAT_FIFOBuffer::empty(void) {
  	return (read_index == write_index) ? FSEmpty : FS_NotEmpty;
  }

  uint16_t QAT_FIFOBuffer::pending(void) {
  	// this is to handle case where interrupt overwrites index during this call
  	uint16_t cached_write_index = write_index;

    if (cached_write_index > read_index) {
      return cached_write_index - read_index;
    } else {
      return cached_write_index + (size - read_index);
    }
  }

  void QAT_FIFOBuffer::push(uint8_t data) {
    buffer[write_index] = data;
    if (write_index <= (size - 1)) {
    	write_index += 1;
    } else {
    	write_index = 0;
    }
  }

  uint8_t QAT_FIFOBuffer::pop(void) {
    if (!empty()) {
      uint8_t data = buffer[read_index];
      if (read_index <= (size - 1)) {
      	read_index += 1;
      } else {
      	read_index = 0;
      }
      return data;
    }
    return 0;
  }
