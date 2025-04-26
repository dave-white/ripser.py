/**
 * MEX wrappers for Ripser functions.
 */

#include "cppmex/mexMatlabEngine.hpp"
#include <memory>
#include <sstream>
#include <utility>

#include "mex.hpp"
#include "mexAdapter.hpp"

#include "ripser.hpp"

using namespace matlab::data;
using matlab::mex::ArgumentList;

class MexFunction : public matlab::mex::Function {
	ArrayFactory fac;
	std::shared_ptr<matlab::engine::MATLABEngine> matlab_p = getEngine();
	std::ostringstream stream;

public:
	void operator()(ArgumentList outputs, ArgumentList inputs)
	{
		TypedArray<value_t> dist_mat = std::move(inputs[0]);
		int N = inputs[1][0];
		int dim_max = inputs[2][0];
		int modulus = 2;
		float threshold = -1.0;
		int do_cocycles = 0;

		stream << "Break 1" << std::endl;
		printmex(stream);

		switch (inputs.size()) {
		case 6:
			do_cocycles = inputs[5][0];

		case 5:
			threshold = inputs[4][0];

		case 4:
			modulus = inputs[3][0];

		default:
			break;
		}

		stream << "Break 2" << std::endl;
		printmex(stream);

		ripserResults res = rips_dm(dist_mat.release().get(), N,
					    modulus, dim_max, threshold,
					    do_cocycles);

		stream << "Break 3" << std::endl;
		printmex(stream);

		outputs[0] = fac.createArray<Array>({1, 
			res.births_and_deaths_by_dim.size()});
		for (int k = 0; k < res.births_and_deaths_by_dim.size(); k++) {
		stream << "Break 5.1" << std::endl;
		printmex(stream);

			outputs[0][k] = fac.createArray(
				{ 1, res.births_and_deaths_by_dim[k].size() },
				res.births_and_deaths_by_dim[k].begin(),
				res.births_and_deaths_by_dim[k].end());
		}

		outputs[1] = fac.createArray<Array>({1, 
			res.cocycles_by_dim.size()});
		for (int k = 0; k < res.cocycles_by_dim.size(); k++) {
			outputs[1][k] = fac.createArray<Array>(
				{ 1, res.cocycles_by_dim[k].size() });
			for (int p = 0; p < res.cocycles_by_dim[k].size();
			     p++) {
				outputs[1][k][p] = fac.createArray(
					{ 1, res.cocycles_by_dim[k][p].size() },
					res.cocycles_by_dim[k][p].begin(),
					res.cocycles_by_dim[k][p].end());
			}
		}

		outputs[2] = fac.createScalar(res.num_edges);
		return;

		StructArray out = fac.createStructArray(
			{ 1, 1 }, { "intervals", "cocycles", "edges" });

		stream << "Break 4" << std::endl;
		printmex(stream);

		out[0]["intervals"] = fac.createArray<Array>(
			{ 1, res.births_and_deaths_by_dim.size() });

		stream << "Break 5" << std::endl;
		printmex(stream);

		stream << "Break 6" << std::endl;
		printmex(stream);

		out[0]["cocycles"] = fac.createArray<Array>(
			{ 1, res.cocycles_by_dim.size() });
		stream << "Break 7" << std::endl;
		printmex(stream);

		out[0]["edges"] = fac.createScalar(res.num_edges);

		outputs[0] = out;
	}

	void printmex(std::ostringstream& stream) {
		matlab_p->feval(u"fprintf", 0, std::vector<Array>({ 
			fac.createScalar(stream.str())}));
		stream.str("");
	}
};
