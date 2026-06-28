#include "NeuronSynapse.h"
#include <iostream>
#include <cmath>
#include <algorithm>
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
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPainterPath rectPath;

    double x1 = (baseNeuron->getX() * scale) + transX;
    double y1 = (baseNeuron->getY() * scale) + transY;

    if (baseNeuron == finalNeuron) {
        // Self-synapse: small ellipse above the neuron
        rectPath.addEllipse(x1 - 19 * scale, y1 - 24 * scale, 18 * scale, 18 * scale);
    } else {
        double x2 = (finalNeuron->getX() * scale) + transX;
        double y2 = (finalNeuron->getY() * scale) + transY;
        double bcx = (this->cx * scale) + transX;
        double bcy = (this->cy * scale) + transY;

        rectPath.moveTo(x1, y1);
        rectPath.quadTo(bcx, bcy, x2, y2);

        // Arrow head — proportional to scale, minimum 5 px
        const double headLen = std::max(5.0, 7.0 * scale);
        qreal pp, per;
        QPointF tip, root;

        pp  = headLen + 4;
        per = rectPath.percentAtLength(pp);
        root = rectPath.pointAtPercent(per);

        pp  = headLen;
        per = rectPath.percentAtLength(pp);
        tip = rectPath.pointAtPercent(per);

        double alpha = std::atan2(tip.y() - root.y(), tip.x() - root.x());
        const double wing = headLen * 0.45;
        QPointF p1(root.x() + wing * std::sin(alpha), root.y() - wing * std::cos(alpha));
        QPointF p2(root.x() - wing * std::sin(alpha), root.y() + wing * std::cos(alpha));

        rectPath.moveTo(p1);
        rectPath.lineTo(tip);
        rectPath.moveTo(p2);
        rectPath.lineTo(tip);
    }

    gPath = rectPath;

    if (baseNeuron->getSelected() && finalNeuron->getSelected())
        selected = true;

    // Color palette matching dark theme
    QColor lineColor;
    if (selected)        lineColor = QColor(0xcb, 0xa6, 0xf7); // purple accent
    else if (weight >= 0) lineColor = QColor(0x89, 0xb4, 0xfa); // blue (excitatory)
    else                  lineColor = QColor(0xf3, 0x8b, 0xa8); // red (inhibitory)

    Qt::PenStyle myStyle = (delay == 0) ? Qt::SolidLine : Qt::DashLine;
    double lineWidth = selected ? 2.5 * scale : 1.8 * scale;

    painter->setPen(QPen(lineColor, lineWidth, myStyle, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(gPath);

    // Weight label near the midpoint of the path
    if (gPath.length() > 20) {
        QPointF mid = gPath.pointAtPercent(0.5);
        painter->setPen(QColor(0xa6, 0xad, 0xc8));
        QFont f = painter->font();
        f.setPixelSize(std::max(7, static_cast<int>(8 * scale)));
        painter->setFont(f);
        painter->drawText(QRectF(mid.x() + 3, mid.y() - 8, 40, 14),
                          Qt::AlignLeft | Qt::AlignVCenter,
                          QString::number(weight, 'g', 3));
    }
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
