#ifndef __LF_DEM__AgeingContact_IO__
#define __LF_DEM__AgeingContact_IO__

#include <iostream>
#include "AgeingContact.h"

namespace Interactions
{
class StdInteractionManager;
namespace Contact_ios {
namespace AgeingContact {
	std::vector < std::pair<struct contact_state, struct AgeingState> > readStatesBStream(std::istream &input, unsigned int np);
	void writeStatesBStream(std::ostream &output,
							const Interactions::StdInteractionManager &interactions);
} // namespace AgeingContact
} // namespace Contact_ios
} // namespace Interactions

#endif