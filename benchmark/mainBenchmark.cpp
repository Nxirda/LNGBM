#include "BaggingModel.hpp"
#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"
#include "TrainingElement.hpp"

int main(int argc, char **argv) {
  if (argc < 0) {
    std::cerr << "Usage is ./" << argv[0] << "\n ";
  }
  std::cout << "=== Types of elements ===\n";
  std::cout << "Dataset      Size in bytes " << sizeof(DataSet) << "\n";
  std::cout << "TreeNode     Size in bytes " << sizeof(TreeNode) << "\n";
  std::cout << "DecisionTree Size in bytes " << sizeof(DecisionTree) << "\n";
  std::cout << "Percentiles  Size in bytes " << sizeof(Percentiles) << "\n";
  std::cout << "Quartiles    Size in bytes " << sizeof(Quartiles) << "\n";
  std::cout << "RandomValues Size in bytes " << sizeof(RandomValues) << "\n";
  std::cout << "UniqueValues Size in bytes " << sizeof(UniqueValues) << "\n";
  std::cout << "MAE          Size in bytes " << sizeof(MAE) << "\n";
  std::cout << "MAPE         Size in bytes " << sizeof(MAPE) << "\n";
  std::cout << "RIV          Size in bytes " << sizeof(RIV) << "\n";
  std::cout << "RMSE         Size in bytes " << sizeof(RMSE) << "\n";

  std::cout << "TrainingElement Size in bytes " << sizeof(TrainingElement) << "\n";
}