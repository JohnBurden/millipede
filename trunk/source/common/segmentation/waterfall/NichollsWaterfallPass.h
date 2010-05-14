/***
 * millipede: NichollsWaterfallPass.h
 * Copyright Stuart Golodetz, 2010. All rights reserved.
 ***/

#ifndef H_MILLIPEDE_NICHOLLSWATERFALLPASS
#define H_MILLIPEDE_NICHOLLSWATERFALLPASS

#include <algorithm>
#include <list>
#include <utility>

#include "WaterfallPass.h"

namespace mp {

template <typename EdgeWeight>
class NichollsWaterfallPass : public WaterfallPass<EdgeWeight>
{
	//#################### CONSTANTS ####################
private:
	enum Flag
	{
		GUARD = 0,
		NON_GUARD = 1
	};

	//#################### NESTED CLASSES ####################
private:
	struct Result
	{
		int node;
		EdgeWeight weight;
		Flag flag;

		Result(int node_, EdgeWeight weight_, Flag flag_)
		:	node(node_), weight(weight_), flag(flag_)
		{}

		bool operator<(const Result& rhs) const
		{
			return	weight < rhs.weight ||
					(weight == rhs.weight && flag < rhs.flag);
		}
	};

	//#################### PUBLIC METHODS ####################
public:
	RootedMST<EdgeWeight>& run(RootedMST<EdgeWeight>& mst)
	{
		run_sub(mst, mst.tree_root(), INT_MAX);
		return mst;
	}

	//#################### PRIVATE METHODS ####################
private:
	void merge_nodes(RootedMST<EdgeWeight>& mst, int u, int v)
	{
		mst.merge_nodes(u, v);
		m_listeners.merge_nodes(u, v);
	}

	Flag run_sub(RootedMST<EdgeWeight>& mst, int parent, EdgeWeight parentWeight)
	{
		std::set<int> children = mst.tree_children(parent);
		if(children.size() > 0)
		{
			// Recurse on all the children.
			std::list<Result> results;
			for(std::set<int>::const_iterator it=children.begin(), iend=children.end(); it!=iend; ++it)
			{
				EdgeWeight childWeight = mst.edge_weight(parent, *it);
				Flag childFlag = run_sub(mst, *it, childWeight);
				results.push_back(Result(*it, childWeight, childFlag));
			}

			// Find the 'minimum' element when sorting first by ascending weight and then by flag (GUARD before NON_GUARD).
			std::list<Result>::iterator lowestIt = std::min_element(results.begin(), results.end());
			Result& lowest = *lowestIt;

			// Calculate the parent flag.
			Flag parentFlag = (parentWeight < lowest.weight) ? NON_GUARD : GUARD;

			// If the parent is a guard edge, merge the 'minimum' edge regardless of its own flag.
			if(parentFlag == GUARD)
			{
				merge_nodes(mst, parent, lowest.node);
				results.erase(lowestIt);
			}

			// Merge all remaining non-guard edges.
			for(std::list<Result>::const_iterator it=results.begin(), iend=results.end(); it!=iend; ++it)
			{
				if(it->flag == NON_GUARD)
				{
					merge_nodes(mst, parent, it->node);
				}
			}

			return parentFlag;
		}
		else return NON_GUARD;
	}
};

}

#endif