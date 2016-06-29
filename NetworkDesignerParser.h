#ifndef NETWORKDESIGNERPARSER_H_
#define NETWORKDESIGNERPARSER_H_

#include <QObject>
#include "Network.h"
#include "UpdateSchedulingPlan.h"

class NetworkDesignerParser : public QObject
{
	Q_OBJECT
public:
	NetworkDesignerParser();
	NetworkDesignerParser(Network * network, UpdateSchedulingPlan * updateSchedulingPlan);
	virtual ~NetworkDesignerParser();

	void setNetwork(Network * network);
	void setUpdateSchedulingPlan(UpdateSchedulingPlan * updateSchedulingPlan);
	Network * getNetwork() const;
	UpdateSchedulingPlan * getUpdateSchedulingPlan() const;

	void save(QString fileName);
	void load(QString fileName);

	void exportToGNBoxPremodel(QString fileName);


private:
	Network * network;
	UpdateSchedulingPlan * updateSchedulingPlan;

};

#endif /*NETWORKDESIGNERPARSER_H_*/
