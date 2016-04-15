#include <string>

#ifndef FSM_H
#define FSM_H

class FSM {
	class State* start;
	class State* final;
	
	public:
		void set_start(class State* start_in);
		void set_final(class State* final_in);
		bool recognize(std::string str);
		FSM();
		~FSM();
};
#endif
