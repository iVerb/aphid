/*
 *  BlockBlockDrawBuffer.cpp
 *  mallard
 *
 *  Created by jian zhang on 9/26/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "BlockDrawBuffer.h"

#ifdef WIN32
#include <gExtension.h>
#else
#include <gl_heads.h>
#endif
BlockDrawBuffer::BlockDrawBuffer() 
{
    m_blocks.push_back(new PtrTup);
	m_vertexPtr = m_blocks[0]->alignedV;
	m_normalPtr = m_blocks[0]->alignedN;
	m_current = 0;
}

BlockDrawBuffer::~BlockDrawBuffer() 
{
    clearBuffer();
}

void BlockDrawBuffer::clearBuffer()
{
    for (std::vector<PtrTup *>::iterator it = m_blocks.begin(); 
				it != m_blocks.end(); ++it)
		delete *it;
	m_blocks.clear();
	m_current = 0;
}

unsigned BlockDrawBuffer::capacity() const 
{
	return numBlock() * numElementPerBlock();
}

unsigned BlockDrawBuffer::numElementPerBlock() const
{
	return 32768;
}

unsigned BlockDrawBuffer::numBlock() const
{
	return m_blocks.size();
}

void BlockDrawBuffer::expandBy(unsigned size)
{
	if(m_current + size > capacity()) {
		unsigned blockToCreate = (m_current + size) / numElementPerBlock() + 1 - numBlock();
		for(unsigned i = 0; i < blockToCreate; i++) {
			m_blocks.push_back(new PtrTup);
		}
	}
}

void BlockDrawBuffer::begin()
{
	m_current = 0;
	m_vertexPtr = m_blocks[0]->alignedV;
	m_normalPtr = m_blocks[0]->alignedN;
}

void BlockDrawBuffer::next()
{
	m_current++;
	if(m_current % numElementPerBlock() == 0) {
		unsigned blockIdx = m_current / numElementPerBlock();
		m_vertexPtr = m_blocks[blockIdx]->alignedV;
		m_normalPtr = m_blocks[blockIdx]->alignedN;
	}
	else {
		m_vertexPtr += 12;
		m_normalPtr += 12;
	}
}

char BlockDrawBuffer::end() const
{
	return m_current >= capacity();
}

void BlockDrawBuffer::drawBuffer() const
{
	for(unsigned i = 0; i < numBlock(); i++) {
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3, GL_FLOAT, 0, (float *)m_blocks[i]->alignedV);
	
		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer( GL_FLOAT, 0, (float *)m_blocks[i]->alignedN );
	
		glDrawArrays( GL_QUADS, 0, numElementPerBlock());
	
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );		
	}
}

float * BlockDrawBuffer::vertices()
{
	return (float *)m_vertexPtr;
}

float * BlockDrawBuffer::normals()
{
	return (float *)m_normalPtr;
}
