/*
 *  KdTreeBuilder.h
 *  kdtree
 *
 *  Created by jian zhang on 10/21/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include <PartitionBound.h>
#include <PrimitiveArray.h>
#include <SplitEvent.h>
#include <BuildKdTreeContext.h>

class KdTreeBuilder {
public:
	typedef Primitive* PrimitivePtr;

	KdTreeBuilder(BuildKdTreeContext &ctx, const PartitionBound &bound);
	virtual ~KdTreeBuilder();
	
	void calculateSplitEvents(const PartitionBound &bound);
	void calculateSides();
	
	void partitionLeft(BuildKdTreeContext &ctx, PartitionBound & bound);
	void partitionRight(BuildKdTreeContext &ctx, PartitionBound & bound);
	
	const SplitEvent *bestSplit();
		
private:
	unsigned numEvents() const;
	BoundingBox m_bbox;
	BuildKdTreeContext *m_context;
	unsigned m_numPrimitive;
	SplitEvent *m_event;
	PrimitivePtr *m_primitives;
	unsigned *m_indices;
	char *m_primitiveClassification;
	unsigned m_bestEventIdx;
};