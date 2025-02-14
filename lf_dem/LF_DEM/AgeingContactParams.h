#ifndef __LF_DEM__AgeingContactParams__
#define __LF_DEM__AgeingContactParams__

namespace Interactions {

struct AgeingContactParams {
	double memory_length;
	double m;
	double tau_inv;
	double mu_0;
};

inline bool has_ageing_contacts(const ContactParams &p)
{
	return p.friction_model == FrictionModel::ageing_Coulomb;
}

} // namespace Interaction

#endif