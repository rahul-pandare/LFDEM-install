//
//  AgeingContact.h
//  LF_DEM
//
//  Created by Romain Mari on 9/06/20.
//  Copyright (c) 2020 Ryohei Seto and Romain Mari. All rights reserved.
//

/**
 \class Ageing contact
 \author Romain Mari
 */

#ifndef __LF_DEM__AgeingContact__
#define __LF_DEM__AgeingContact__

#include "Contact.h"
#include "AgeingContactParams.h"

namespace Interactions
{

struct AgeingState {
	double age; // age is just a number
};

class AgeingContact : public Contact {
private:
	AgeingContactParams ageing_p;
	AgeingState age;
	AgeingState prev_age;

	void incrementAge(double dt, const struct PairVelocity &vel);
	void setAge(double a);
public:
	void incrementDisplacements(double dt, const struct PairVelocity &vel);
	AgeingContact(PairwiseInteraction* interaction_, 
				  const ContactParams &params,
				  const AgeingContactParams &ageing_params,
				  double norm_dashpot_coeff, 
				  double tan_dashpot_coeff);

	struct AgeingState getAgeingState() const;
	void setAgeingState(const struct AgeingState& as);
	void saveState();
	void restoreState();
};

} // namespace Interactions

#endif /* defined(__LF_DEM__AgeingContact__) */
