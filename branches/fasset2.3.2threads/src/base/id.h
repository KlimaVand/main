/*
 * id.h
 *
 *  Created on: 10/11/2011
 *      Author: beer
 */

#ifndef ID_H_
#define ID_H_
#include <vector>
#include <thread>


class ThreadID
{

private:
	vector<thread::id > ids;
public:
	std::mutex locks;
	std::mutex fileLocks;
	ThreadID()
	{


	}
	void setID(thread::id ting)
	{
		while(locks.try_lock()==false)
				std::this_thread::sleep_for(std::chrono::seconds(1));
		ids.push_back(ting);
		locks.unlock();
	}
	int getID(thread::id ting)
	{
		for(unsigned int i=0;i<ids.size();i++)
		{
			if(ids[i]==ting)
			{
				return i;
			}
		}
		//exit(99);
	}

};
extern ThreadID * threadID;

#endif /* ID_H_ */
