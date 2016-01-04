#include "AffinityMatrix.h"

/**
* @brief isAdjacent
*
* 判断区域是否邻接
* @param[in] last  预测区域
* @param[in] cur  当前帧区域
* @return bool  如果两个区域邻接则返回true，否则false
*/
bool isAdjacent(const cv::Rect& last, const cv::Rect& cur)
{
	return (last & cur).area() != 0;
}

/**
* @brief AffinityMatrix::AffinityMatrix.
*
* 构造函数
*/
AffinityMatrix::AffinityMatrix(void)
{
	predictedRects.reserve(gDefaultVectorLength);
	curRects.reserve(gDefaultVectorLength);
}

/**
* @brief AffinityMatrix::clear
*
* 重置相似度矩阵状态
* @param[in] void
* @return void
*/
void AffinityMatrix::clear(void)
{
	row = 0;
	col = 0;

	predictedRects.clear();
	curRects.clear();

	rowResults.clear();
	colResults.clear();
}

/**
* @brief AffinityMatrix::build
*
* 构建相似度矩阵
* @param[in] pRects  预测区域
* @param[in] cRects  当前帧区域
* @return void
*/
void AffinityMatrix::build(const vector< pair<ID, cv::Rect> >& pRects,
										const vector<cv::Rect>& cRects)
{
	clear();

	predictedRects = pRects;
	col = predictedRects.size();

	curRects = cRects;
	row = curRects.size();

	build();

}

/**
* @brief AffinityMatrix::build
*
* 构建相似度矩阵
* @param[in] void
* @return void
*/
void AffinityMatrix::build(void)
{
	for (size_t r = 0; row != r; ++r)
	{
		for (size_t c = 0; col != c; ++c)
		{
			if (true == isAdjacent(predictedRects[c].second, curRects[r]))
			{
				rowResults[r].push_back(c);
				colResults[c].push_back(r);
			}
		}
	}
}

/**
* @brief AffinityMatrix::newTarget
*
* 获取新的目标
* @param[in] targets  新出现的目标会被加入该向量
* @return void
*/
void AffinityMatrix::newTarget(vector<cv::Rect>& targets)
{
	const auto endIter = rowResults.cend();
	for (size_t r = 0; row != r; ++r)
	{
		const auto iter = rowResults.find(r);
		if (endIter == iter)
		{
			targets.push_back(curRects[r]);
		}
	}
}

/**
* @brief AffinityMatrix::lostTarget
*
* 获取丢失的目标
* @param[in] targets  丢失的目标会被加入该向量
* @return void
*/
void AffinityMatrix::lostTarget(vector<ID>& targets)
{
	const auto endIter = colResults.cend();
	for (size_t c = 0; col != c; ++c)
	{
		const auto iter = colResults.find(c);
		if (endIter == iter)
		{
			targets.push_back(predictedRects[c].first);
		}
	}
}

/**
* @brief AffinityMatrix::matchedTarget
*
* 获取匹配的目标
* @param[in] targets  匹配的目标会被加入该向量
* @return void
*/
void AffinityMatrix::matchedTarget(vector< pair<ID, cv::Rect> >& targets)
{
	auto endRowIter = rowResults.cend();
	auto endColIter = colResults.cend();
	for (auto rowIter = rowResults.cbegin()
		; endRowIter != rowIter
		; nullptr)
	{
		if (1 == rowIter->second.size())
		{
			const size_t c = rowIter->second.front();
			const auto colIter = colResults.find(c);
			if (endColIter != colIter
				&& 1 == colIter->second.size())
			{
				targets.push_back(
					std::make_pair(predictedRects[c].first, curRects[rowIter->first]));

				rowIter = rowResults.erase(rowIter);
				endRowIter = rowResults.cend();

				colResults.erase(colIter);
				endColIter = colResults.cend();
				continue;
			}
		}
		++rowIter;
	}
}

/**
* @brief AffinityMatrix::splitTarget
*
* 获取分裂的目标
* @param[in] targets  分裂的目标会被加入该向量
* @return void
*/
void AffinityMatrix::splitTarget(vector< pair<ID, vector<cv::Rect> > >& targets)
{
	vector<cv::Rect> rects;
	rects.reserve(gDefaultVectorLength);

	auto endIter = colResults.cend();

	for (auto colIter = colResults.cbegin()
		; endIter != colIter
		; nullptr)
	{
		if (1 < colIter->second.size())
		{
			const auto& indexes = colIter->second;
			const size_t size = indexes.size();
			for (size_t i = 0; size != i; ++i)
			{
				rects.push_back(curRects[indexes[i]]);
			}
			targets.push_back(make_pair(predictedRects[colIter->first].first, rects));
			rects.clear();

			colIter = colResults.erase(colIter);
			endIter = colResults.cend();
			continue;
		}
		++colIter;
	}
}

/**
* @brief AffinityMatrix::occludedTarget
*
* 获取遮挡的目标
* @param[in] targets  遮挡的目标会被加入该向量
* @return void
*/
void AffinityMatrix::occludedTarget(vector< pair<cv::Rect, vector<ID> > >& targets)
{
	vector<ID> ids;
	ids.reserve(gDefaultVectorLength);

	auto endIter = rowResults.cend();
	for (auto iter = rowResults.cbegin()
		; endIter != iter
		; nullptr)
	{
		if (1 < iter->second.size())
		{
			const auto& indexes = rowResults[iter->first];
			const size_t size = indexes.size();
			for (size_t i = 0; size != i; ++i)
			{
				ids.push_back(predictedRects[indexes[i]].first);
			}
			targets.push_back(make_pair(curRects[iter->first], ids));
			ids.clear();

			iter = rowResults.erase(iter);
			endIter = rowResults.cend();
			continue;
		}
		++iter;
	}
}

/**
* @brief AffinityMatrix::isEmpty
*
* 判断预测结果与检测结构是否为空
* @return bool  若预测结果与检测结构都为空则返回true，否则返回false
*/
bool AffinityMatrix::isEmpty() const
{
	return rowResults.empty() && colResults.empty();
}



