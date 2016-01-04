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
* ��������Ԥ�����뵱ǰ������������ƶȾ��󣬲��Խ������ͳ��
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
	size_t row;		/**< �������µ������� */
	size_t col;		/**< ������Ԥ�������� */

	vector< pair<ID, cv::Rect> > predictedRects;		/**< Ԥ������ */
	vector< cv::Rect > curRects;					/**< ��ǰ֡���� */

	map< size_t, vector<size_t> > rowResults;	/**< ���ƶȾ�����ͳ�ƽ�� */
	map< size_t, vector<size_t> > colResults;	/**< ���ƶȾ�����ͳ�ƽ�� */
};

#endif // AFFINITY_MATRIX_H

