/*
 *  MlEngine.cpp
 *  mallard
 *
 *  Created by jian zhang on 12/31/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "MlEngine.h"
#include <boost/asio.hpp>
#include <boost/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;
using boost::asio::ip::tcp;

MlEngine::MlEngine() 
{
	std::cout<<" renderEngine ";
	//std::cout<<" working thread"<<m_workingThread.get_id();
}

MlEngine::~MlEngine() 
{
	interruptRender();
}

void MlEngine::render() 
{
	interruptRender();
	m_workingThread = boost::thread(boost::bind(&MlEngine::testOutput, this));
	std::cout<<"treadId"<<m_workingThread.get_id();
}

void MlEngine::interruptRender()
{
	std::cout<<"treadId"<<m_workingThread.get_id();
	std::cout<<"render cancelled";
	m_workingThread.interrupt();
}

void MlEngine::testOutput()
{
	ptime tt(second_clock::local_time());
	std::cout<<"test output at "<<to_simple_string(tt)<<"\n";
	
    std::string ts("2002-01-20 23:59:59.000");
    ptime tref(time_from_string(ts));
    time_duration td = tt - tref;
    std::cout<<"time elapse "<<td.total_seconds()<<"\n";
	
	try
	{
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(tcp::v4(), "localhost", "7879");
		tcp::resolver::iterator iterator = resolver.resolve(query);
		
		boost::asio::deadline_timer t(io_service);
	
		const int bucketSize = 64;
		const int imageSizeX = resolutionX();
		const int imageSizeY = resolutionY();
		int rect[4];
				
		for(int by = 0; by <= imageSizeY/bucketSize; by++) {
			rect[2] = by * bucketSize;
			rect[3] = rect[2] + bucketSize - 1;
			if(rect[3] > imageSizeY - 1) rect[3] = imageSizeY - 1;
			for(int bx = 0; bx <= imageSizeX/bucketSize; bx++) {
				rect[0] = bx * bucketSize;
				rect[1] = rect[0] + bucketSize - 1;
				if(rect[1] > imageSizeX - 1) rect[1] = imageSizeX - 1;
				
				const float grey = (float)((rand() + td.seconds() * 391) % 457) / 457.f;
				std::cout<<"grey"<<grey<<"\n";

				const unsigned npix = (rect[1] - rect[0] + 1) * (rect[3] - rect[2] + 1);
				std::cout<<"n pixels "<<npix<<"\n";
				int npackage = npix * 16 / 4096;
				if((npix * 16) % 4096 > 0) npackage++;
				std::cout<<"n packages "<<npackage<<"\n";
				
				tcp::socket s(io_service);
				s.connect(*iterator);
				boost::asio::write(s, boost::asio::buffer((char *)rect, 16));
				boost::array<char, 128> buf;
				boost::system::error_code error;
				size_t reply_length = s.read_some(boost::asio::buffer(buf), error);

				std::cout<<" bucket("<<rect[0]<<","<<rect[1]<<","<<rect[2]<<","<<rect[3]<<")\n";
				
				float *color = new float[npackage * 256 * 4];
				for(int i = 0; i < npix; i++) {
					color[i * 4] = color[i * 4 + 1] = color[i * 4 + 2] = grey;
					color[i * 4 + 3] = 1.f;
				}

				for(int i=0; i < npackage; i++) {
					boost::asio::write(s, boost::asio::buffer((char *)&(color[i * 256 * 4]), 4096));
					reply_length = s.read_some(boost::asio::buffer(buf), error);
				}
				
				boost::asio::write(s, boost::asio::buffer("transferEnd", 11));
				reply_length = s.read_some(boost::asio::buffer(buf), error);
				
				s.close();
				
				boost::this_thread::interruption_point();
				
				t.expires_from_now(boost::posix_time::seconds(1));
				t.wait();
				
				
			}
		}

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	
}