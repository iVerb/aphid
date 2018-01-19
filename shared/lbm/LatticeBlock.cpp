/*
 *  LatticeBlock.cpp
 *
 *  Created by jian zhang on 1/16/18.
 *  Copyright 2018 __MyCompanyName__. All rights reserved.
 *
 */

#include "LatticeBlock.h"
#include "D3Q19DF.h"
#include <math/linearMath.h>
#include <math/miscfuncs.h>

namespace aphid {

namespace lbm {

LatticeBlock::LatticeBlock(sdb::Entity * parent) : sdb::Entity(parent)
{}

void LatticeBlock::resetFlag(char* fg, const int& iblock)
{ 
	m_flag = &fg[iblock * BlockLength]; 
	memset(m_flag, 0, BlockLength);
}

void LatticeBlock::resetQi(float* q, const int& iblock, const int& i)
{
	m_q[i] = &q[iblock * BlockLength];
	
	if(i > 18)
		return;
		
	D3Q19DF::SetWi(m_q[i], BlockLength, i);
	
}

void LatticeBlock::simulationStep()
{
/// no stream on q_0
	for(int i=1;i<19;++i) {
		streaming(i);
	}
/// boundary conditions here
	collision();
}

void LatticeBlock::streaming(const int& i)
{
	float* q_i = m_q[i];
	float* tmp = m_q[19];
	memcpy(tmp, q_i, BlockLength * 4);
	
	int c_i[3];
	D3Q19DF::GetStreamDirection(c_i, i);
	
	for(int k=0; k<BlockDim[2];++k) {
		for(int j=0;j<BlockDim[1];++j) {
			for(int i=0;i<BlockDim[0];++i) {
				if(IsCellCoordValid(i + c_i[0], j + c_i[1], k + c_i[2] ) ) 
					q_i[CellInd(i, j, k)] = tmp[CellInd(i + c_i[0], j + c_i[1], k + c_i[2])];
			}
		}
	}
}

void LatticeBlock::collision()
{
	float u[3];
	float rho, uu, omega = 1.9f;
	int ii, j, k;
	for(int i=0;i<BlockLength;++i) {
		
		D3Q19DF::IncompressibleVelocity(u, rho, m_q, i);
		
		uu = u[0] * u[0] + u[1] * u[1] + u[2] * u[2];
		
		D3Q19DF::Relaxing(m_q, u, uu, rho, omega, i);
	}
}

void LatticeBlock::initialCondition()
{
	float u[3], uu;
	for(int k=0; k<BlockDim[2];++k) {
		for(int j=0;j<BlockDim[1];++j) {
			for(int i=0;i<BlockDim[0];++i) {
				evaluateCellCenterVelocity(u, i, j, k);
				uu = u[0] * u[0] + u[1] * u[1] + u[2] * u[2];
				
				if(uu < 1e-9f)
					continue;
					
				m_flag[CellInd(i, j, k)] = 1;
				D3Q19DF::DiscretizeVelocity(m_q, 1.f, u, CellInd(i, j, k) );

			}
		}
	}
}

void LatticeBlock::boundaryCondition(const float& bcRho)
{
	float u[3];
	int ii, j, k;
	for(int i=0;i<BlockLength;++i) {
		if(m_flag[i] > 0) {
			CellCoord(ii, j, k, i);
			evaluateCellCenterVelocity(u, ii, j, k);
			D3Q19DF::IncomingBC(m_q, u, bcRho, i);
		}
	}
}

void LatticeBlock::updateVelocity()
{
	clearVelocities();

	float u[3];
	float rho, uu;
	for(int k=0; k<BlockDim[2];++k) {
		for(int j=0;j<BlockDim[1];++j) {
			for(int i=0;i<BlockDim[0];++i) {
				
				D3Q19DF::IncompressibleVelocity(u, rho, m_q, CellInd(i, j, k) );
				uu = u[0] * u[0] + u[1] * u[1] + u[2] * u[2];
				
				if(uu < 1e-9f) 
					continue;
					
				depositeCellCenterVelocity(i, j, k, u);

			}
		}
	}
}

void LatticeBlock::limitSpeed(float& x) const
{ ClampInPlace<float>(x, -.3f, .3f); }

void LatticeBlock::extractCellDensities(float* dst)
{ 
	for(int i=0;i<BlockLength;++i) {
		D3Q19DF::Density(dst[i], m_q, i );
	}
}

void LatticeBlock::evaluateVelocityDensityAtPosition(float* u, float& rho, const float* p)
{
	evaluateVelocityAtPosition(u, p);
	D3Q19DF::Density(rho, m_q, getCellInd(p) );
}

}

}
