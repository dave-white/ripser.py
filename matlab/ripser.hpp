#include <vector>

typedef float value_t;

typedef struct ripserResults {
    /* The first variable is a vector of unrolled persistence diagrams
       so, for example births_and_deaths_by_dim[0] contains a list of
                [birth0, death0, birth1, death1, ..., birthk, deathk]
       for k points in the 0D persistence diagram
       and likewise for d-dimensional persistence in births_and_deaths_by_dim[d]
    */
    std::vector<std::vector<value_t>> births_and_deaths_by_dim;
    /*
      The second variable is a vector of representative cocycles for each
      dimension. For now, only cocycles above dimension 0 are added, so
      dimension 0 is an empty list For the others, cocycles_by_dim[d] holds an
      array of representative cocycles for dimension d which is parallel with
      the array of births/deaths for dimension d. Each element of the array is
      itself an array of unrolled information about the cocycle For dimension 1,
      for example, the zeroeth element of the array contains [ccl0_simplex0_idx0
      ccl0_simplex0_idx1 ccl0_simplex0_val, ccl0_simplex1_idx0
      ccl0_simplex1_idx1 ccl0_simplex1_val, ... ccl0_simplexk_idx0
      ccl0_simplexk_idx1 ccl0_simplexk_val] for a cocycle representing the first
      persistence point, which has k simplices with nonzero values in the
      representative cocycle
    */
    std::vector<std::vector<std::vector<int>>> cocycles_by_dim;
    /* The third variable is the number of edges that were added during the
     * computation*/
    int num_edges;
} ripserResults;

ripserResults rips_dm(float* D, int N, int modulus, int dim_max,
                      float threshold, int do_cocycles);

