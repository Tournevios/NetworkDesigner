#include "NeuronSynapse.h"
#include <iostream>
using namespace std;

Synapse::Synapse(){
}

Synapse::~Synapse(){
	while((int)states.size() > 0) states.pop();
}

Synapse::Synapse(Neuron * baseNeuron, Neuron * finalNeuron, double weight, int delay){
	
	this->baseNeuron = baseNeuron;
	this->finalNeuron = finalNeuron;
	this->weight = weight;
	this->delay = delay;
	selected = false;
	
	if(baseNeuron->getIndex()<finalNeuron->getIndex()){
		gExcentricity = 0.06f;
	}
	else{
		gExcentricity = -0.06f;
	}
		
	double x1 = baseNeuron->getX();
	double y1 = baseNeuron->getY();
	
	// body building
	double x2 = finalNeuron->getX();
	double y2 = finalNeuron->getY();
	cx = (x1 + x2) * (0.5f + gExcentricity);
	cy = (y1 + y2) *(0.5f + gExcentricity);
	
}

Neuron * Synapse::getBaseNeuron() const{
	return baseNeuron;
}
void Synapse::setBaseNeuron(Neuron* baseNeuron){
	this->baseNeuron = baseNeuron;
}
	
Neuron * Synapse::getFinalNeuron() const{
	return finalNeuron;	
}

void Synapse::setFinalNeuron(Neuron* finalNeuron){
	this->finalNeuron = finalNeuron;
}

double Synapse::getWeight() const{
	return weight;
}

void Synapse::setWeight(double weight){
	this->weight = weight;
}
	
float Synapse::getGExcentricity() const{
	return gExcentricity;
}

void Synapse::setGExcentricity(float gExcentricity){
	this->gExcentricity = gExcentricity;
}

/*
 * Selected's getter 
 */
bool Synapse::getSelected() const{
	return selected;
}

/*
 * Selected's setter
 */
void Synapse::setSelected(bool selected){
	this->selected = selected;
}

/*
 * Build and draw the graphical representation (QPainterPath) of the arrow
 */
void Synapse::drawMe(QPainter * painter, double scale, double transX, double transY){
	
	QPainterPath rectPath;
	qreal pp;
	qreal per;
	QPointF startPoint, endPoint;
	
	double x1 = (baseNeuron->getX()*scale) + transX;
	double y1 = (baseNeuron->getY()*scale) + transY;
	
	if(baseNeuron == finalNeuron){
			rectPath.addEllipse(x1-19*scale, y1-19*scale, 17*scale, 18*scale);
	} 
	else {
		// body building
		double x2 = (finalNeuron->getX()*scale) + transX;
		double y2 = (finalNeuron->getY()*scale) + transY;
		//double cx = (x1 + x2) * (0.5f + gExcentricity);
		//double cy = (y1 + y2) *(0.5f + gExcentricity);
		double cx = (this->cx * scale) + transX;
		double cy = (this->cy * scale) + transY;
		
		rectPath.moveTo(x1, y1);
		rectPath.quadTo(cx, cy, x2, y2);
		
		// Arrow Head builing
		//pp = rectPath.length()-25;
		double headLength = 2;
		double alpha;
		
		pp = 25;
		per = rectPath.percentAtLength(pp);
		startPoint = rectPath.pointAtPercent(per);
		
		//pp = rectPath.length()-20;
		pp = 20;
		per = rectPath.percentAtLength(pp);
		endPoint = rectPath.pointAtPercent(per);
		
		alpha = atan((endPoint.y()-startPoint.y())/((endPoint.x()-startPoint.x())));
		QPointF p1(startPoint.x()+headLength*sin(alpha), startPoint.y()-headLength*cos(alpha));
		QPointF p2(startPoint.x()-headLength*sin(alpha), startPoint.y()+headLength*cos(alpha));
		//QPointF p1(startPoint.x()+2*sin(alpha), startPoint.x()-2*cos(alpha));
		
		//cout << startPoint.x()<<endl;
		rectPath.moveTo(p1);
		rectPath.lineTo(endPoint);
		rectPath.moveTo(p2);
		rectPath.lineTo(endPoint);		
	}	
	// Updating
    gPath = rectPath;
    
    if(baseNeuron->getSelected() and finalNeuron->getSelected()) selected = true;
    
    Qt::PenStyle myStyle;
    
    if(delay==0) myStyle = Qt::SolidLine;
    else		 myStyle = Qt::DashLine;
     
    if(selected)  painter->setPen(QPen(Qt::blue, 2*scale, myStyle, Qt::RoundCap));
    else if(weight >= 0) painter->setPen(QPen(Qt::black, 2*scale, myStyle, Qt::RoundCap));
    else painter->setPen(QPen(Qt::red, 2*scale, myStyle, Qt::RoundCap));
    painter->drawPath(gPath);	
}

/*
 * if a selection operation could be realized return true
 * else return false
 */
bool Synapse::selectable(double x, double y){
	if(gPath.contains(QPointF(x,y)))	{
		return true;
	}
	return false;
}

/*
 * delay's getter
 */
int Synapse::getDelay() const{
	return delay;
}

/*
 * delay's setter
 */
void Synapse::setDelay(int delay){
	this->delay = delay;
	refreshMe();
}

/*
 * Set empty the content of the synapse
 */
void Synapse::refreshMe(){
	while(!states.empty())	states.pop();
}

/*
 * Give the state of the base neuron after a delay time
 */
int Synapse::getStateOfTheFinalNeuron(){
	int theState = 0;
	states.push(finalNeuron->getState());
	if((int)states.size() > delay) {
		theState = states.front();
		states.pop();
	}
	return theState;
}

/*
 * Bezier's control point x coordinate: setter
 */
void Synapse::setCX(double cx){
	this->cx = cx;
	
}

/*
 * Bezier's control point y coordinate: setter
 */
void Synapse::setCY(double cy){
	this->cy = cy;
}

/*
 * Bezier's control point x coordinate: getter
 */
double Synapse::getCX() const{
	return cx;
}

/*
 * Bezier's control point y coordinate: getter
 */
double Synapse::getCY() const{
	return cy;
}
