/**
 * MEX wrappers for Ripser functions.
 */

#include "MatlabDataArray/StructArray.hpp"
#include "cppmex/mexMatlabEngine.hpp"
#include <algorithm>
#include <string>
#include <utility>

#include "mexAdapter.hpp"

#include "ripser.hpp"

using namespace matlab::data;
using matlab::mex::ArgumentList;

/**
 * @brief
 */
class MexFunction : public matlab::mex::Function {
	ArrayFactory fac;

    public:
	/**
	 * @brief Wraps rips_dm.
	 *
	 * Invoke rips_dm, computing the persistant homology of the 
	 * Vietoris-Rips complex
	 *
	 * @param[out] outputs List of data objects returned to MATLAB.
	 * @param[out] outputs[0] Cell array of barcodes.
	 * @param[out] outputs[1] Cell array of representative cocycles.
	 * @param[out] outputs[2] Scalar number of edges removed.
	 * @param[in]  inputs List of arrays holding input arguments from 
	 * @param[in]  inputs[0] "dist_mat": Flat array containing the entries 
	 * of the (triangular) distance matrix.
	 * @param[in]  inputs[1] "opts": Struct holding optional parameters 
	 * passed to Ripser.
	 */
	void operator()(ArgumentList outputs, ArgumentList inputs)
	{
		TypedArray<value_t> dist_mat = std::move(inputs[0]);

		/**
		 * Set default values for Ripser parameters.
		 *
		 * @todo Why does do_cocycles = true break Ripser?
		 */
		int dim_max = 0;
		int modulus = 2; // Should be prime?
		float threshold = -1.0;
		bool do_cocycles = false;

		/**
		 * Get Vietoris-Rips params from MATLAB options struct.
		 *
		 * @todo Handle nonexistent fields for optional params.
		 *
		 * @todo Figure out why type 'Array' works below rather than, 
		 * say, 'TypedArray<int>'.
		 */
		StructArray opts(inputs[1]);
		Range fld_names = opts.getFieldNames();

		if (std::find(fld_names.begin(), fld_names.end(),
			      MATLABFieldIdentifier("dim_max")) !=
		    fld_names.end()) {
			Array dim_max_mat = opts[0]["dim_max"];
			dim_max = dim_max_mat[0];
		}

		if (std::find(fld_names.begin(), fld_names.end(),
			      MATLABFieldIdentifier("modulus")) !=
		    fld_names.end()) {
			Array modulus_mat = opts[0]["modulus"];
			modulus = modulus_mat[0];
		}

		// Maximal distance used in the Vietoris-Rips filtration.
		if (std::find(fld_names.begin(), fld_names.end(),
			      MATLABFieldIdentifier("threshold")) !=
		    fld_names.end()) {
			Array threshold_mat = opts[0]["threshold"];
			threshold = threshold_mat[0];
		}

		if (std::find(fld_names.begin(), fld_names.end(),
			      MATLABFieldIdentifier("do_cocycles")) !=
		    fld_names.end()) {
			Array do_cocycles_mat = opts[0]["do_cocycles"];
			do_cocycles = do_cocycles_mat[0];
		}

		ripserResults res = rips_dm(dist_mat.release().get(),
					    dist_mat.getNumberOfElements(),
					    modulus, dim_max, threshold,
					    do_cocycles);

		outputs[0] = fac.createArray<Array>(
			{ 1, res.births_and_deaths_by_dim.size() });
		for (int k = 0; k < res.births_and_deaths_by_dim.size(); k++) {
			outputs[0][k] = fac.createArray(
				{ 1, res.births_and_deaths_by_dim[k].size() },
				res.births_and_deaths_by_dim[k].begin(),
				res.births_and_deaths_by_dim[k].end());
		}

		outputs[1] = fac.createArray<Array>(
			{ 1, res.cocycles_by_dim.size() });
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
	}
};
