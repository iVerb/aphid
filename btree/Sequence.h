/*
 *  Sequence.h
 *  btree
 *
 *  Created by jian zhang on 5/9/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include <BNode.h>
#include <deque>
namespace sdb {

class MatchFunction {
public:
	enum Condition {
		mExact = 0,
		mLequal = 1
	};
};
	
template<typename T>
class Sequence : public Entity {
public:
	Sequence(Entity * parent = NULL) : Entity(parent) {
		m_root = new BNode<T>();
		m_lastSearchNode = NULL;
	}
	
	virtual ~Sequence() {
		delete m_root;
	}
	
	Pair<T, Entity> * insert(const T & x) { 
		return m_root->insert(x);
	}
	
	void remove(const T & x) {
		m_root->remove(x);
		m_current = NULL;
	}
	
	bool find(const T & x) {
		if(m_root->numKeys() < 1) return false;
		
		Pair<Entity *, Entity> g = m_root->find(x);
		if(!g.index) return false;

		return true;
	}
	
	Pair<Entity *, Entity> findEntity(const T & x, MatchFunction::Condition mf = MatchFunction::mExact, T * extraKey = NULL) {
		Pair<Entity *, Entity> g;
// probably it is the same node
		if(m_lastSearchNode) g = m_lastSearchNode->findInNode(x);
		
		if(!g.index) {
// restart search
			g = m_root->find(x);
// keep the node reached
			m_lastSearchNode = static_cast<BNode<T> *>(g.key);
		}
		
		if(mf == MatchFunction::mLequal) {
			if(g.index) return g;
			SearchResult sr;
			g = m_lastSearchNode->findInNode1(x, &sr);
			if(m_lastSearchNode->key(sr.high) > x) {
				g.key = m_lastSearchNode;
				g.index = m_lastSearchNode->index(sr.high);
				if(extraKey) *extraKey = m_lastSearchNode->key(sr.high);
				return g;
			}
			else {
				BNode<T> * rgt = static_cast<BNode<T> *>(m_lastSearchNode->sibling());
				if(rgt) {
					g.key = rgt;
					g.index = rgt->index(0);
					if(extraKey) *extraKey = rgt->key(0);
					return g;
				}
			}
		}
		
		return g;
	}
	
	void begin() {
		beginLeaf();
		if(leafEnd()) return;
		m_currentData = 0;
	}
	
	/*
	void beginAt(const T & x) {
		Pair<Entity *, Entity> g = m_root->find(x);
		m_current = static_cast<BNode<T> *>(g.key);
		
		SearchResult sr = BNode<T>::LatestSearch;
		m_currentData = sr.found;
	}
	*/
	
	void next() {
		m_currentData++;
		if(m_currentData == leafSize()) {
			nextLeaf();
			if(leafEnd()) return;
			m_currentData = 0;
		} 
	}
	
	const bool end() const {
		return leafEnd();
	}
	
	void setToEnd() {
		m_current = NULL;
	}
	
	int size() {
		int c = 0;
		beginLeaf();
		while(!leafEnd()) {
			c += leafSize();
			nextLeaf();
		}
		return c;
	}
	
	void clear() {
		delete m_root;
		m_root = new BNode<T>();
		m_lastSearchNode = NULL;
	}
	
	bool intersect(Sequence * another)
	{
		begin();
		while(!end()) {
			if(another->find(currentKey())) return true;
			next();
		}
		return false;
	}
	
	void verbose() {
		std::cout<<"\n sequence root node "<<*m_root;
	}

protected:	
	void beginLeaf() {
		m_current = m_root->firstLeaf();
	}
	
	void nextLeaf() {
		m_current = static_cast<BNode<T> *>(m_current->sibling()); 
	}
	
	const bool leafEnd() const {
		return (m_current == NULL || m_root->numKeys() < 1);
	}
	
	const int leafSize() const {
		if(!m_current) return 0;
		return m_current->numKeys();
	}
	
	Entity * currentIndex() const {
		if(!m_current) return NULL;
		return m_current->index(m_currentData);
	}
	
	const T currentKey() const {
		return m_current->key(m_currentData);
	}
	
	std::deque<T> allKeys() {
		typename std::deque<T> r;
		begin();
		while(!end()) {
			r.push_back(currentKey());
			next();
		}
		return r;
	}

private:
	
private:
	BNode<T> * m_root;
	BNode<T> * m_current;
	BNode<T> * m_lastSearchNode;
	int m_currentData;
};
} //end namespace sdb
