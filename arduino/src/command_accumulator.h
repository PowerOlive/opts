#pragma once

template<size_t T_buf_size>
struct CommandAccumulator
{
	char buffer[T_buf_size];
	size_t next_i;
	bool overflow;
	bool ready;

	CommandAccumulator()
	{
		reset();
	}

	void reset()
	{
		memset(buffer, 0, T_buf_size);
		next_i = 0;
		overflow = false;
		ready = false;
	}

	bool put_char(char c)
	{
		if(ready){
			reset();
		}
		// Support backspace for convenience when testing
		if(c == 127){
			if(next_i != 0){
				next_i--;
				buffer[next_i] = 0;
				overflow = false;
			}
			return false;
		}
		// Check this first to resolve overflows at newline
		// NOTE: \r should be accepted as end-of-command because serial
		//       terminals that's the enter key.
		if(c == '\n' || c == '\r'){
			if(overflow){
				reset();
				return false;
			} else if(next_i == 0){
				// Ignore initial newlines
				return false;
			} else {
				ready = true;
				return true;
			}
		}
		if(next_i == T_buf_size){
			overflow = true;
			return false; // Can't do much
		}
		buffer[next_i++] = c;
		return false;
	}

	template<class T_serial>
	bool read(T_serial &serial)
	{
		while(serial.available()){
			char c = serial.read();
			if(put_char(c))
				return true;
		}
		return false;
	}

	const char* command()
	{
		return buffer;
	}
};
