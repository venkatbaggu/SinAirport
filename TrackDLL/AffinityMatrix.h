#ifndef AFFINITY_MATRIX_H
#define AFFINITY_MATRIX_H

#include <map>
#include <utility>
using std::pair;
using std::map;

#include "Global.h"
#include "Singleton.h"

/**
* @class AffinityMatrix
* @brief
*
* 负责利用预测结果与当前检测结果构造相似度矩阵，并对结果进行统计
*/
class AffinityMatrix : public Singleton<AffinityMatrix>
{
	friend class Singleton<AffinityMatrix>;

public:
	void build(const vector< pair<ID, cv::Rect> >& pRects, 
										const vector<cv::Rect>& cRects);

	void newTarget(vector<cv::Rect>& targets);
	void lostTarget(vector<ID>& targets);
	void matchedTarget(vector< pair<ID, cv::Rect> >& targets);
	void splitTarget(vector< pair<ID, vector<cv::Rect> > >& targets);
	void occludedTarget(vector< pair<cv::Rect, vector<ID> > >& targets);
	bool isEmpty() const;

private:
	AffinityMatrix(void);
	void clear(void);
	void build(void);

private:
	size_t row;		/**< 行数：新的区域数 */
	size_t col;		/**< 列数：预测区域数 */

	vector< pair<ID, cv::Rect> > predictedRects;		/**< 预测区域 */
	vector< cv::Rect > curRects;					/**< 当前帧区域 */

	map< size_t, vector<size_t> > rowResults;	/**< 相似度矩阵行统计结果 */
	map< size_t, vector<size_t> > colResults;	/**< 相似度矩阵列统计结果 */
};

#endif // AFFINITY_MATRIX_H

