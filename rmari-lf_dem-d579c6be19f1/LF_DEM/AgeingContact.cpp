//
//
//  Created by Romain Mari on 9/06/20.
//  Copyright (c) 2020 Romain Mari. All rights reserved.
//
#include "AgeingContact.h"

namespace Interactions 
{

AgeingContact::AgeingContact(PairwiseInteraction* interaction_, 
							 const ContactParams &p, 
							 const AgeingContactParams &ageing_params, 
							 double norm_dashpot_coeff, 
							 double tan_dashpot_coeff) :
Contact(interaction_, p, norm_dashpot_coeff, tan_dashpot_coeff),
ageing_p(ageing_params),
prev_age({0})
{
	setAge(0);
}

void AgeingContact::incrementDisplacements(double dt, const struct PairVelocity &vel)
{
	incrementAge(dt, vel);
	Contact::incrementDisplacements(dt, vel);
}

void AgeingContact::incrementAge(double dt, const struct PairVelocity &vel)
{
	switch(sliding_state) {
		case SlidingState::nonsliding:
			setAge(age.age + dt);
			break;
		case SlidingState::sliding:
			{
				auto decay_time = ageing_p.memory_length/getSlidingVelocity(vel).norm();
				setAge((age.age - decay_time)*std::exp(-dt/decay_time) + decay_time);  // this integrated form ensures the age is always positive
			}
			break;
		default:
			throw std::runtime_error(" AgeingContact: contact state must be nonsliding or sliding");
	}
}

void AgeingContact::setAge(double a)
{	
	age.age = a;
	mu_sliding = ageing_p.mu_0 + ageing_p.m*std::log(1 + age.age*ageing_p.tau_inv);	

	// we also set mu_static and mu_dynamic to mu_sliding so that the base Contact class 
	// does not override mu_sliding when switching static <-> dynamic sliding friction.
	mu_static = mu_sliding;
	mu_dynamic = mu_sliding;
}

struct AgeingState AgeingContact::getAgeingState() const
{
	return age;
}

void AgeingContact::setAgeingState(const struct AgeingState& as)
{
	setAge(as.age);
}

void AgeingContact::saveState()
{
	Contact::saveState();
	prev_age = age;	
}

void AgeingContact::restoreState()
{
	Contact::restoreState();
	setAge(prev_age.age);
}

} // namespace Interactions