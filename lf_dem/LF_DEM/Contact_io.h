#ifndef __LF_DEM__Contact_IO__
#define __LF_DEM__Contact_IO__

#include <iostream>
#include "Contact.h"

namespace Interactions
{

namespace Contact_ios {
	template<typename T> 
	struct contact_state readStateBStream(std::istream &input)
	{
		T p0, p1;
		double dt_x, dt_y, dt_z, dr_x, dr_y, dr_z;
		input.read((char*)&p0, sizeof(T));
		input.read((char*)&p1, sizeof(T));
		input.read((char*)&dt_x, sizeof(decltype(dt_x)));
		input.read((char*)&dt_y, sizeof(decltype(dt_y)));
		input.read((char*)&dt_z, sizeof(decltype(dt_z)));
		input.read((char*)&dr_x, sizeof(decltype(dr_x)));
		input.read((char*)&dr_y, sizeof(decltype(dr_y)));
		input.read((char*)&dr_z, sizeof(decltype(dr_z)));
		struct contact_state cs;
		cs.p0 = (int)p0;
		cs.p1 = (int)p1;
		cs.disp_tan = vec3d(dt_x, dt_y, dt_z);
		cs.disp_rolling = vec3d(dr_x, dr_y, dr_z);
		return cs;
	}

	std::vector <struct contact_state> readStatesBStream(std::istream &input, unsigned int np);
	void writeStateBStream(std::ostream &conf_export, const struct contact_state &cs);
	void writeStatesBStream(std::ostream &conf_export, const std::vector <struct contact_state> &cs);
} // namespace Contact_ios

} // namespace Interactions

#endif