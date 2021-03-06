//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Oct 16 16:08:05 PDT 2016
// Last Modified: Sun Oct 16 16:08:08 PDT 2016
// Filename:      GridStaff.cpp
// URL:           https://github.com/craigsapp/hum2ly/blob/master/src/GridStaff.cpp
// Syntax:        C++11
// vim:           ts=3 noexpandtab
//
// Description:   GridStaff is an intermediate container for converting from
//                MusicXML syntax into Humdrum syntax.
//
//

#include "HumGrid.h"

using namespace std;

namespace hum {


//////////////////////////////
//
// GridStaff::GridStaff -- Constructor.
//

GridStaff::GridStaff(void) : vector<GridVoice*>(0), GridSide() {
	// do nothing;
}



//////////////////////////////
//
// GridStaff::~GridStaff -- Deconstructor.
//

GridStaff::~GridStaff(void) {
	for (int i=0; i<(int)this->size(); i++) {
		if (this->at(i)) {
			delete this->at(i);
			this->at(i) = NULL;
		}
	}
}



//////////////////////////////
//
// GridStaff::setTokenLayer -- Insert a token at the given voice/layer index.
//    If there is another token already there, then delete it.  If there
//    is no slot for the given voice, then create one and fill in all of the
//    other new ones with NULLs.
//

GridVoice* GridStaff::setTokenLayer(int layerindex, HTp token, HumNum duration) {
	if (layerindex < 0) {
		cerr << "Error: layer index is " << layerindex
		     << " for " << token << endl;
		return NULL;
	}
	if (layerindex > (int)this->size()-1) {
		int oldsize = this->size();
		this->resize(layerindex+1);
		for (int i=oldsize; i<(int)this->size(); i++) {
			this->at(i) = NULL;
		}
	}
	if (this->at(layerindex) != NULL) {
		delete this->at(layerindex);
	}
	GridVoice* gv = new GridVoice(token, duration);
	this->at(layerindex) = gv;
	return gv;
}



////////////////////////////
//
// GridStaff::setNullTokenLayer --
//

void GridStaff::setNullTokenLayer(int layerindex, SliceType type,
		HumNum nextdur) {

	string nulltoken;
	if (type < SliceType::_Data) {
		nulltoken = ".";
	} else if (type < SliceType::_Measure) {
		nulltoken = "=";
	} else if (type < SliceType::_Manipulator) {
		nulltoken = "*";
	} else if (type < SliceType::_Spined) {
		nulltoken = "!!";
	} else {
		cerr << "STRANGE ERROR" << endl;
	}

	if (layerindex < (int)this->size()) {
		if (at(layerindex) != NULL) {
			if ((string)*at(layerindex)->getToken() == ".") {
				// there is already a null data token here, so don't 
				// replace it.
				return;
			}
			cerr << "Warning, replacing existing token: "
			     << *this->at(layerindex)->getToken()
			     << " with a null token"
			     << endl;
		}
	}
	HumdrumToken* token = new  HumdrumToken(nulltoken);
	setTokenLayer(layerindex, token, nextdur);

}



//////////////////////////////
//
// GridStaff::appendTokenLayer -- concatenate the string content
//   of a token onto the current token stored in the slot (or just
//   place this one in the slot if none there yet).  This is used for
//   chords normally.
//

void GridStaff::appendTokenLayer(int layerindex, HTp token, HumNum duration,
		const string& spacer) {

	GridVoice* gt;
	if (layerindex > (int)this->size()-1) {
		int oldsize = this->size();
		this->resize(layerindex+1);
		for (int i=oldsize; i<(int)this->size(); i++) {
			this->at(i) = NULL;
		}
	}
	if (this->at(layerindex) != NULL) {
		string newtoken;
		newtoken = (string)*this->at(layerindex)->getToken();
		newtoken += spacer;
		newtoken += (string)*token;
		(string)*(this->at(layerindex)->getToken()) = newtoken;
	} else {
		gt = new GridVoice(token, duration);
		this->at(layerindex) = gt;
	}
}



//////////////////////////////
//
// GridStaff::getMaxVerseCount --
//

int GridStaff::getMaxVerseCount(void) {
	return 5;
// ggg
}



//////////////////////////////
//
// operator<< --
//

ostream& operator<<(ostream& output, GridStaff* staff) {
	if (staff == NULL) {
		output << "{n}";
		return output;
	}
	for (int t=0; t<(int)staff->size(); t++) {
		GridVoice* gt = staff->at(t);
		cout << "(v" << t << ":)";
		if (gt == NULL) {
			cout << "{gt:n}";
			continue;
		} else {
			HTp token = gt->getToken();
			if (token == NULL) {
				cout << "{n}";
			} else {
				cout << " \"" << *token << "\" ";
			}
		}
	}
	return output;
}



} // end namespace hum



