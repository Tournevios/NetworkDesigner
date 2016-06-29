#ifndef BLOCKSELECTOR_H_
#define BLOCKSELECTOR_H_

#include "Network.h"
#include "UpdateBlock.h"
#include<QtGui/QWidget>

class BlockSelector : public QObject
{
	Q_OBJECT
public:
	BlockSelector(Network * network, int l1_distance, int firstIndex);
	BlockSelector(const BlockSelector& blockSelector);
	virtual ~BlockSelector();
	
	int getL1_distance() const;
	UpdateBlock * getUpdateBlock() const;
	Network * getNetwork() const;
	
	void addNeuronIndex(int index);

	friend int operator<(const BlockSelector& left, const BlockSelector& right);
	BlockSelector& operator=(const BlockSelector& blockSelector);
private:
	Network * network;
	UpdateBlock * updateBlock;
	int l1_distance;
	BlockSelector();

public slots:
	void select(bool checked);

signals:
	void repaintPlease();
};

#endif /*BLOCKSELECTOR_H_*/
