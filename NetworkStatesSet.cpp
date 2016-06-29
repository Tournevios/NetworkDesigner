#include "NetworkStatesSet.h"
using namespace std;

/*
 * A network NetworkStatesSet is a set of NetworkStates
 */

NetworkStatesSet::NetworkStatesSet()
{
	cardinal = 0;
	maxCardinal = -1;
	set.clear();
	filled = false;
}

NetworkStatesSet::NetworkStatesSet(const NetworkStatesSet& cp){
	maxCardinal = -1;//cp.getMaxCardinal();
	for(int i=0; i<cp.getCardinal(); i++){
		set.push_back(new NetworkState(*(cp.getNetworkState(i))));
	}
	cardinal = cp.getCardinal();
	filled = cp.getFilled();
}


NetworkStatesSet::NetworkStatesSet(int maxCardinal){
	this->maxCardinal = maxCardinal;
	cardinal = 0;
	set.clear();
	filled = false;
}

NetworkStatesSet::~NetworkStatesSet()
{	
	while(cardinal > 0)	removeNetworkState(0);

}

void NetworkStatesSet::deleteOneElement(int index){
	
	if(index < cardinal){
		delete set[index];
		set.erase(set.begin() + index);
		cardinal--;
	}
}

void NetworkStatesSet::addNetworkStatesSet(const NetworkStatesSet& networkStatesSet){
	for(int i = 0; i < networkStatesSet.getCardinal(); i++){
		this->addNetworkState(*(networkStatesSet.getNetworkState(i)));
	}
}

/*
 * Add the new state only if it doesn't hava an equivalent
 */
void NetworkStatesSet::addNetworkState(const NetworkState& networkState){

	if((cardinal >= maxCardinal) and (maxCardinal>0)){
		deleteOneElement(0);
	}

	set.push_back(new NetworkState(networkState));
	cardinal++;
}

void NetworkStatesSet::removeNetworkState(int index){
	if(index<cardinal){
		deleteOneElement(index);
	}
}


NetworkState* NetworkStatesSet::getNetworkState(int index) const{
	if(index < cardinal){
		return set[index];
	}
	return NULL;
}

int NetworkStatesSet::getCardinal() const{
	return cardinal;
}

void NetworkStatesSet::setFilled(bool filled){
	this->filled = filled;
}

bool NetworkStatesSet::getFilled() const{
	return filled;
}

int NetworkStatesSet::getMaxCardinal() const{
	return maxCardinal;
}

void NetworkStatesSet::printMe() const{
	std::cout<< "My states are: " << std::endl;
	for(int i=0; i<cardinal; i++){
		set[i]->printMe();
	}
	std::cout << std::endl; 
}

void NetworkStatesSet::setMaxCardinal(int maxCardinal){
	if((maxCardinal < cardinal) and (maxCardinal>=0)){
		while(cardinal > maxCardinal) deleteOneElement(cardinal-1);		
		cardinal = maxCardinal; 
	}
	this->maxCardinal = maxCardinal;
}


void NetworkStatesSet::addAndCompress(const NetworkState& networkState){
	int i=0;
	bool added = false;
	// = new NetworkState(networkState->getSize(), networkState->getNbStatesNS());

	
	// Add a the networkState to the set
	while((i < this->getCardinal()) and (!added)){
		NetworkState tmpResult = *(this->getNetworkState(i)) + networkState;
		if(tmpResult.coherent()){
			added = true;
			//delete this->getNetworkState(i);
			this->removeNetworkState(i);
			this->addNetworkState(tmpResult);
		}
		else {
			i++;
		}
	}
	
	if(!added) {
		this->addNetworkState(networkState);
	}
	
	this->compress();
	
}

/*
 * Compress the set
 */
void NetworkStatesSet::compress(){
	
	int i=0;
	int j=0;

	while(i < this->getCardinal()){
		j = 0;
		while(j < this->getCardinal()){
			if(i!=j){
				NetworkState tmpResult2 = *(this->getNetworkState(j)) + *(this->getNetworkState(i));
				if(tmpResult2.coherent()){
					this->removeNetworkState(i);
					if(i > j)	this->removeNetworkState(j);
					else this->removeNetworkState(j-1);
					this->addNetworkState(tmpResult2);
					i = 0;
					break;
				}
			}
			j++;
		}
		i++;
	}
}

/*
 * Get all the number of all possbile states.
 */
int NetworkStatesSet::getNbOfAllPossibleStates() const{
	int sum = 0;
	for(int i=0; i < cardinal; i++){
		sum += set[i]->getNbOfAllPossibleStates();
	}
	return sum;
}


int NetworkStatesSet::coherent() const{
	for(int i=0; i < this->getCardinal(); i++){
		if(!(this->getNetworkState(i)->coherent())) return false; 
	}
	return true;
}

void NetworkStatesSet::removeDuplications(){
	int i=0;
	int j=0;
	bool found;
	while(i < this->getCardinal()){
		j=i+1;
		found = false;
		while(j < this->getCardinal()){
			if(*(this->getNetworkState(i)) < *(this->getNetworkState(j))){
				this->removeNetworkState(i);
				found = true;
				break;
			}
			else if(*(this->getNetworkState(i)) > *(this->getNetworkState(j))){
				this->removeNetworkState(j);
			}
			else j++;
		}
		if(!found) i++; 
	}
}

/*
 * Includsion operator
 *
 */
int operator==(const NetworkState& lv, const NetworkStatesSet& rv) {

	for(int i=0; i < rv.getCardinal(); i++){
		if(lv < *rv.getNetworkState(i)) return true;
	}
	return false;
}

/*
 * Union operator without pseudo states
 */
const NetworkStatesSet operator+(const NetworkStatesSet& lv, const NetworkStatesSet& rv){
	
	NetworkStatesSet result(-1);

	if(lv.getCardinal() <=0) {
		result.addNetworkStatesSet(rv);
		return result;
	}
	
	result.addNetworkStatesSet(lv);
	
	for(int i=0; i<rv.getCardinal(); i++){
		result+=*rv.getNetworkState(i); 
	}
	return result;
}

void NetworkStatesSet::operator+=(const NetworkState& rv){
	
	NetworkStatesSet tmpNetworkStates(-1);
	tmpNetworkStates.addNetworkState(rv);
	tmpNetworkStates -= *this;

	// Adding result;

	this->addNetworkStatesSet(tmpNetworkStates);
}


void NetworkStatesSet::operator-=(const NetworkState& rv){
	//NetworkStatesSet * tmpNetworkStates;
	int mYCardinal = this->getCardinal();
	for(int i=0; i < mYCardinal; i++){
		NetworkStatesSet tmpNetworkStates = *(this->getNetworkState(i)) - rv;
		// Adding result
		if(tmpNetworkStates.getCardinal() > 0)	this->addNetworkStatesSet(tmpNetworkStates);
		//delete tmpNetworkStates;
	}
	for(int i=0; i < mYCardinal; i++){
		this->removeNetworkState(0);
	}
}

void NetworkStatesSet::operator*=(const NetworkState& rv){
	//NetworkStatesSet * tmpNetworkStates;
	int mYCardinal = this->getCardinal();
	for(int i=0; i < mYCardinal; i++){
		NetworkState tmpNetworkState = *(this->getNetworkState(i)) * rv;
		// Adding result
		if(tmpNetworkState.coherent() > 0)	this->addNetworkState(tmpNetworkState);
		//delete tmpNetworkStates;
	}
	for(int i=0; i < mYCardinal; i++){
		this->removeNetworkState(0);
	}
}


const NetworkStatesSet operator-(const NetworkStatesSet& lv, const NetworkState& rv){
	NetworkStatesSet result(-1);
	result.addNetworkStatesSet(lv);
	result -= rv;
	return result;
}

/*
 * Intersection operator
 */
const NetworkStatesSet operator*(const NetworkStatesSet& lv, const NetworkStatesSet& rv){
	NetworkStatesSet result(-1);

	if((lv.getCardinal() <=0) or (rv.getCardinal() <=0)) {
		return result;
	}
	/*result = lv - (lv -rv);

	int i=0;
	while(i < result.getCardinal()){
		if(!(result.getNetworkState(i)->coherent())) result.removeNetworkState(i);
		else i++;
	}
	return result;*/
	result = lv;
	result *= rv;
	return result;
}

/*
 * When using pseudo state: a state minus a state is the deleting the possibilitier that the left
 * operand is equivalent to the right one if !!!!!!the operation is permitted!!!!!!! .ie
 * if there is a relation of equivalence or order between the two NetworkState s
 * 
 */
 
 const NetworkStatesSet operator-(const NetworkState& lv, const NetworkState& rv){
	NetworkStatesSet result(-1);

	bool modified;
	bool globalModified = false;
	long int leftValue=0;
	long int rightValue=0;
	long int op;
	int index=0;
	int count = 0;
	
	if(!(lv.coherent() or rv.coherent())) return result;	
	modified = true;
	bool incoherent;
	index = -1;
	while(modified){
		NetworkState tmpNetworkState(lv.getSize(), lv.getNbStatesNS());
		modified = false;
		incoherent = false;	
		for(int i=0; i < lv.getSize(); i++){
			if(lv.getState(i) != rv.getState(i) and lv.getState(i) >=0 and rv.getState(i) >=0){
					if((lv.getState(i) >= 0) and (rv.getState(i) >= 0)){
						while(result.getCardinal() > 0) result.removeNetworkState(0);
						result.addNetworkState(lv);
						return result;
					}
			} 
			else if((i > index) and not modified){
				modified = true;
				index = i;
				if(lv.getState(i)<-1)  leftValue = -lv.getState(i);
				else if(lv.getState(i)>-1) leftValue = (long int)pow(2.0, (double) lv.getState(i));
				
				if(rv.getState(i)<-1)  rightValue = -rv.getState(i);
				else if(rv.getState(i)>-1)  rightValue = (long int)pow(2.0, (double) rv.getState(i));
				
				op = (long int)((unsigned int)leftValue & ~(unsigned int)rightValue);
				
				if(op == 0) {
					count--; 
					incoherent = true; 
					break;
					} 
				else{ 				
					if(log(op)/log(2.0) == floor(log(op)/log(2))) tmpNetworkState.setState(i, (long int)(floor(log(op)/log(2))));
					else tmpNetworkState.setState(i, -op);
					globalModified = true;
				}
			}
			else if(not modified and i <= index){
				if(lv.getState(i)<-1)  leftValue = -lv.getState(i);
				else if(lv.getState(i)>-1) leftValue = (long int)pow(2.0, (double) lv.getState(i));
				
				if(rv.getState(i)<-1)  rightValue = -rv.getState(i);
				else if(rv.getState(i)>-1)  rightValue = (long int)pow(2.0, (double) rv.getState(i));
				
				op = (long int)((unsigned int)leftValue & ~(unsigned int)rightValue);
				
				if(op == 0) tmpNetworkState.setState(i, lv.getState(i)); 
				else{ 				
					tmpNetworkState.setState(i, rv.getState(i));
				}
			}
			else if(i > index){
				tmpNetworkState.setState(i, lv.getState(i));
			}	
             
		}
		if(modified and not incoherent) result.addNetworkState(tmpNetworkState); 
	}
	return result;
}
 
const NetworkStatesSet operator-(const NetworkStatesSet& lv, const NetworkStatesSet& rv){

	NetworkStatesSet result(lv);// = new NetworkStatesSet(-1);
	//*result += *this;

	for(int i=0; i < rv.getCardinal(); i++){
	 	result-=*rv.getNetworkState(i);
	}
	return result;
}

int operator==(const NetworkStatesSet& lv, const NetworkStatesSet& rv){
	return (lv.getCardinal() == rv.getCardinal());
}

void NetworkStatesSet::operator+=(const NetworkStatesSet& rv){
	if(this == &rv) return;
	NetworkStatesSet tmp = (*this + rv);
	while(this->getCardinal() > 0) this->removeNetworkState(0);
	this->addNetworkStatesSet(tmp);
}

void NetworkStatesSet::operator*=(const NetworkStatesSet& rv){
	if(this == &rv) return;
/*	NetworkStatesSet tmp = (*this * rv);
	while(this->getCardinal() > 0) this->removeNetworkState(0);
	this->addNetworkStatesSet(tmp);
	*/
	NetworkStatesSet result(-1);
	NetworkStatesSet tmp = *this;
	for(int i=0; i < rv.getCardinal(); i++){
		tmp = *this;
		tmp *= *(rv.getNetworkState(i));	
		result+=tmp;
	}
	*this = result;
}

void NetworkStatesSet::operator-=(const NetworkStatesSet& rv){
	if(this == &rv){
		while(this->getCardinal() > 0) this->removeNetworkState(0);
	}
	else{
		NetworkStatesSet tmp = (*this - rv);
		while(this->getCardinal() > 0) this->removeNetworkState(0);
		this->addNetworkStatesSet(tmp);
	}	
}

NetworkStatesSet& NetworkStatesSet::operator=(const NetworkStatesSet& rv){
	if(this != &rv){
		while(cardinal > 0) this->removeNetworkState(0);
		maxCardinal = rv.getMaxCardinal();
		filled = rv.getFilled();
		this->addNetworkStatesSet(rv);
	}
	return *this;
}
