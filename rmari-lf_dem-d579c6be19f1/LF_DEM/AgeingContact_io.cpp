#include "AgeingContact_io.h"
#include "Contact_io.h"
#include "StdInteractionManager.h"

namespace Interactions {

namespace Contact_ios {

namespace AgeingContact {

struct AgeingState readAgeBStream(std::istream &input)
{
	double age;
	input.read((char*)&age, sizeof(decltype(age)));
	return {age};
}

std::vector < std::pair<struct contact_state, struct AgeingState> > readStatesBStream(std::istream &input, unsigned np)
{
	unsigned ncont;
	input.read((char*)&ncont, sizeof(decltype(ncont)));

	std::vector < std::pair<struct contact_state, struct AgeingState> > ageing_cont_states;

	for (unsigned i=0; i<ncont; i++) {
		auto cstate = readStateBStream<unsigned>(input);
		auto astate = readAgeBStream(input);
		ageing_cont_states.push_back(std::make_pair(cstate, astate));
	}
	return ageing_cont_states;
}

void writeAgeBStream(std::ostream &conf_export, const struct AgeingState &as)
{
	conf_export.write((char*)&(as.age), sizeof(decltype(as.age)));
}


void writeStatesBStream(std::ostream &output,
						const Interactions::StdInteractionManager &interactions)
{
	std::vector < std::pair<struct contact_state, struct AgeingState> > cs;
	for (const auto &inter: interactions) {
		if (inter->contact) {
			Interactions::AgeingContact *ageing_contact =  dynamic_cast<Interactions::AgeingContact*>(inter->contact.get());
			cs.push_back(std::make_pair(inter->contact->getState(), ageing_contact->getAgeingState()));
		}
	}

	unsigned ncont = cs.size();
	output.write((char*)&ncont, sizeof(unsigned));
	for (auto &state: cs) {
		writeStateBStream(output, state.first);
		writeAgeBStream(output, state.second);	
	}
}

} // namespace AgeingContact

} // namespace Contact_io

} // namespace Interactions
