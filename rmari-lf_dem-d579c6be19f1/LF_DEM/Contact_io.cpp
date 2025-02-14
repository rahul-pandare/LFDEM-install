#include "Contact_io.h"

namespace Interactions {

namespace Contact_ios {

std::vector <struct contact_state> readStatesBStream(std::istream &input, unsigned np)
{
	unsigned ncont;
	input.read((char*)&ncont, sizeof(decltype(ncont)));

	// hacky thing to guess if this is an old format with particle numbers as unsigned short
	bool ushort_format = false;
	unsigned p0;
	std::iostream::pos_type file_pos = input.tellg();
	input.read((char*)&p0, sizeof(decltype(p0)));
	if(p0>np){
		ushort_format = true;
	}
	input.seekg(file_pos);

	std::vector <struct contact_state> cont_states;
	if (ushort_format) {
		for (unsigned i=0; i<ncont; i++) {
			cont_states.push_back(readStateBStream<unsigned short>(input));
		}
	} else {
		for (unsigned i=0; i<ncont; i++) {
			cont_states.push_back(readStateBStream<unsigned>(input));
		}
	}
	return cont_states;
}

void writeStateBStream(std::ostream &conf_export, const struct contact_state &cs)
{
	conf_export.write((char*)&(cs.p0), sizeof(unsigned int));
	conf_export.write((char*)&(cs.p1), sizeof(unsigned int));
	conf_export.write((char*)&(cs.disp_tan.x), sizeof(double));
	conf_export.write((char*)&(cs.disp_tan.y), sizeof(double));
	conf_export.write((char*)&(cs.disp_tan.z), sizeof(double));
	conf_export.write((char*)&(cs.disp_rolling.x), sizeof(double));
	conf_export.write((char*)&(cs.disp_rolling.y), sizeof(double));
	conf_export.write((char*)&(cs.disp_rolling.z), sizeof(double));
}

void writeStatesBStream(std::ostream &conf_export, const std::vector <struct contact_state> &cs)
{
	unsigned ncont = cs.size();
	conf_export.write((char*)&ncont, sizeof(unsigned int));
	for (const auto &c: cs) {
		writeStateBStream(conf_export, c);
	}
}

}

} // namespace Contact_io
