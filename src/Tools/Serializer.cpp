#include "Serializer.hpp"

namespace Serializer {

//
void serialize_Node(const TreeNode &node, const std::string &filename) {
  std::ofstream ofs(filename, std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa << node;
}

//
TreeNode deserialize_Node(const std::string &filename) {
  std::ifstream ifs(filename, std::ios::binary);

  if (!ifs.is_open()) {
    std::cerr << "Error: Unable to open file '" << filename << "'" << std::endl;
    return TreeNode();
  }

  boost::archive::binary_iarchive ia(ifs);

  TreeNode node;
  ia >> node;

  return node;
}

//
void serialize_Tree(const DecisionTree &tree, const std::string &filename) {
  std::ofstream ofs(filename, std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa << tree;
}

//
DecisionTree deserialize_Tree(const std::string &filename) {
  std::ifstream ifs(filename, std::ios::binary);

  if (!ifs.is_open()) {
    std::cerr << "Error: Unable to open file '" << filename << "'" << std::endl;
    return DecisionTree();
  }

  boost::archive::binary_iarchive ia(ifs);

  DecisionTree tree;
  ia >> tree;

  return tree;
}

//
std::string serialize_Answers(const Answers &ans) {
  std::ostringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << ans;
  return oss.str();
}

//
Answers deserialize_Answers(const std::string &serialized_Datas) {
  std::istringstream iss(serialized_Datas);
  boost::archive::binary_iarchive ia(iss);

  Answers ans;
  ia >> ans;

  return ans;
}
} // namespace Serializer