#include "AffinityMatrix.h"

/**
* @brief isAdjacent
*
* �ж������Ƿ��ڽ�
* @param[in] last  Ԥ������
* @param[in] cur  ��ǰ֡����
* @return bool  ������������ڽ��򷵻�true������false
*/
bool isAdjacent(const cv::Rect& last, const cv::Rect& cur)
{
	return (last & cur).area() != 0;
}

/**
* @brief AffinityMatrix::AffinityMatrix.
*
* ���캯��
*/
AffinityMatrix::AffinityMatrix(void)
{
	predictedRects.reserve(gDefaultVectorLength);
	curRects.reserve(gDefaultVectorLength);
}

/**
* @brief AffinityMatrix::clear
*
* �������ƶȾ���״̬
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
* �������ƶȾ���
* @param[in] pRects  Ԥ������
* @param[in] cRects  ��ǰ֡����
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
* �������ƶȾ���
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
* ��ȡ�µ�Ŀ��
* @param[in] targets  �³��ֵ�Ŀ��ᱻ���������
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
* ��ȡ��ʧ��Ŀ��
* @param[in] targets  ��ʧ��Ŀ��ᱻ���������
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
* ��ȡƥ���Ŀ��
* @param[in] targets  ƥ���Ŀ��ᱻ���������
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
* ��ȡ���ѵ�Ŀ��
* @param[in] targets  ���ѵ�Ŀ��ᱻ���������
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
* ��ȡ�ڵ���Ŀ��
* @param[in] targets  �ڵ���Ŀ��ᱻ���������
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
* �ж�Ԥ��������ṹ�Ƿ�Ϊ��
* @return bool  ��Ԥ��������ṹ��Ϊ���򷵻�true�����򷵻�false
*/
bool AffinityMatrix::isEmpty() const
{
	return rowResults.empty() && colResults.empty();
}



